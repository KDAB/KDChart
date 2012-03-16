/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "KDChartCartesianAxis.h"
#include "KDChartCartesianAxis_p.h"

#include <cmath>

#include <QtDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QApplication>

#include "KDChartPaintContext.h"
#include "KDChartChart.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractGrid.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartLayoutItems.h"
#include "KDChartBarDiagram.h"
#include "KDChartStockDiagram.h"
#include "KDChartLineDiagram.h"
#include "KDChartPrintingParameters.h"

#include <KDABLibFakes>

#include <limits>

using namespace KDChart;

#define d (d_func())

namespace KDChart {

class XySwitch
{
public:
    explicit XySwitch( bool _isY ) : isY( _isY ) {}

    // for rvalues
    template< class T >
    T operator()( T x, T y ) { return isY ? y : x; }
    int operator()( const QSize &sz ) { return isY ? sz.height() : sz.width(); }

    // lvalues
    qreal &lvalue( qreal& x, qreal& y ) { return isY ? y : x; }
    int &lvalue( int& x, int& y ) { return isY ? y : x; }

    bool isY;
};

class TickIterator
{
public:
    enum TickType {
        NoTick = 0,
        MajorTick,
        MajorTickManualShort,
        MajorTickManualLong,
        MinorTick,
        CustomTick
    };
    TickIterator( CartesianAxis *a, CartesianCoordinatePlane* plane, uint majorThinningFactor );

    qreal position() const { return m_position; }
    QString text() const { return m_text; }
    TickType type() const { return m_type; }
    bool isAtEnd() const { return m_position == std::numeric_limits< qreal >::infinity(); }
    void operator++();

private:
    // these are generally set once in the constructor or by callers
    CartesianAxis* m_axis;
    DataDimension m_dimension; // upper and lower bounds
    bool m_isLogarithmic;
    QMap< qreal, QString > m_customTicks; // custom ticks and "annotations" merged
    QStringList m_manualLabelTexts;
    uint m_majorThinningFactor;
    uint m_majorLabelCount;

    // these generally change in operator++(), i.e. from one label to the next
    int m_manualLabelIndex;
    TickType m_type;
    qreal m_position;
    qreal m_majorTick;
    qreal m_minorTick;
    QString m_text;
};

} // namespace KDChart

static qreal slightlyLessThan( qreal r )
{
    if ( r == 0.0 ) {
        // scale down the epsilon somewhat arbitrarily
        return r - std::numeric_limits< qreal >::epsilon() * 1e-6;
    }
    // scale the epsilon so that it (hopefully) changes at least the least significant bit of r
    qreal diff = qAbs( r ) * std::numeric_limits< qreal >::epsilon() * 2.0;
    return r - diff;
}

TickIterator::TickIterator( CartesianAxis* a, CartesianCoordinatePlane* plane, uint majorThinningFactor )
   : m_axis( a ),
     m_majorThinningFactor( majorThinningFactor ),
     m_majorLabelCount( 0 ),
     m_type( NoTick )
{
    XySwitch xy( m_axis->isOrdinate() );

    m_dimension = xy( plane->gridDimensionsList().first(), plane->gridDimensionsList().last() );
    GridAttributes gridAttributes = plane->gridAttributes( xy( Qt::Horizontal, Qt::Vertical ) );


    m_dimension = AbstractGrid::adjustedLowerUpperRange( m_dimension,
                                                         gridAttributes.adjustLowerBoundToGrid(),
                                                         gridAttributes.adjustUpperBoundToGrid() );

    const bool hasMajorTicks = m_axis->rulerAttributes().showMajorTickMarks() && m_dimension.stepWidth > 0;
    const bool hasMinorTicks = m_axis->rulerAttributes().showMinorTickMarks() && m_dimension.subStepWidth > 0;
    Q_ASSERT( std::numeric_limits< qreal >::has_infinity );
    m_majorTick = hasMajorTicks ? m_dimension.start : std::numeric_limits< qreal >::infinity();
    m_minorTick = hasMinorTicks ? m_dimension.start : std::numeric_limits< qreal >::infinity();
    m_isLogarithmic = m_dimension.calcMode == AbstractCoordinatePlane::Logarithmic;

    KDAB_FOREACH( qreal r, m_axis->d_func()->customTicksPositions ) {
        m_customTicks.insert( r, QString() );
    }
    if ( m_customTicks.isEmpty() ) {
        // this keeps duplicates from annotations - they are considered a user error for now
        m_customTicks = m_axis->d_func()->annotations;
    } else {
        // not using QMap::unite() because we don't want duplicate entries if the same key is in
        // both custom ticks and annotations
        QMap< qreal, QString >::ConstIterator it = m_axis->d_func()->annotations.constBegin();
        QMap< qreal, QString >::ConstIterator end = m_axis->d_func()->annotations.constEnd();
        for ( ; it != end; ++it ) {
            m_customTicks.insert( it.key(), it.value() );
        }
    }

    // TODO compare shortLabels().count() and labels().count() as documented
    m_manualLabelTexts = m_majorThinningFactor > 1 ? m_axis->shortLabels() : m_axis->labels();
    m_manualLabelIndex = m_manualLabelTexts.isEmpty() ? -1 : 0;

    // position the iterator just below the first element so the logic from operator++() can be reused
    // to find the first tick
    // FIXME this is unlikely to work for logarithmic axes starting at 0-ish
    m_position = slightlyLessThan( m_dimension.start );
    ++( *this );
}

void TickIterator::operator++()
{
    //qDebug() << Q_FUNC_INFO << m_majorTick << m_dimension.stepWidth << m_minorTick << m_dimension.subStepWidth;
    // with all three kinds of ticks, make sure to advance from the current position if the tick is
    // exactly on the current position. that means that we had a tick at that position last time.
    if ( isAtEnd() ) {
        return;
    }

    const qreal inf = std::numeric_limits< qreal >::infinity();

    // TODO: a robust way to avoid duplicate ticks due to inaccuracies of floating-point math:
    //       e.g. avoid painting ticks closer than some epsilon (also see the old code)

    if ( !m_customTicks.isEmpty() ) {
        QMap< qreal, QString >::ConstIterator it = m_customTicks.upperBound( m_position );
        if ( it != m_customTicks.constEnd() ) {
            m_position = it.key();
            m_text = it.value();
            m_type = CustomTick;
        } else {
            m_position = inf;
        }
    } else {
        // TODO corresponding logic for logarithmic axes
        while ( m_majorTick <= m_position ) {
            m_majorTick += m_dimension.stepWidth;
        }
        while ( m_minorTick <= m_position ) {
            m_minorTick += m_dimension.subStepWidth;
        }

        if ( m_majorTick != inf && m_majorTick <= m_minorTick ) {
            m_position = m_majorTick;
            if ( m_manualLabelIndex >= 0 ) {
                m_text = m_manualLabelTexts[ m_manualLabelIndex++ ];
                if ( m_manualLabelIndex >= m_manualLabelTexts.count() ) {
                    m_manualLabelIndex = 0;
                }
                m_type = m_majorThinningFactor > 1 ? MajorTickManualShort : MajorTickManualLong;
            } else {
                m_text = QString::number( m_position ); // TODO proper number formatting
                m_type = MajorTick;
                if ( ( m_majorLabelCount++ % m_majorThinningFactor ) != 0 ) {
                    m_text.clear();
                }
            }

            if ( m_minorTick != inf ) {
                // align; also an easy way to avoid duplicate ticks when major and minor almost but not
                // exactly share position due to rounding errors
                m_minorTick = m_majorTick;
            }
        } else if ( m_minorTick != inf ) {
            m_position = m_minorTick;
            m_text.clear();
            m_type = MinorTick;
        } else {
            m_position = inf;
        }
    }

    //qDebug() << m_position << m_majorTick << m_minorTick << (m_minorTick > m_position);

    if ( m_position > m_dimension.end ) {
        // make isAtEnd() return true
        m_position = inf;
        m_text.clear();
        m_type = NoTick;
    }
}

CartesianAxis::CartesianAxis( AbstractCartesianDiagram* diagram )
    : AbstractAxis ( new Private( diagram, this ), diagram )
{
    init();
}

CartesianAxis::~CartesianAxis()
{
    // when we remove the first axis it will unregister itself and
    // propagate the next one to the primary, thus the while loop
    while ( d->mDiagram ) {
        AbstractCartesianDiagram *cd = qobject_cast< AbstractCartesianDiagram* >( d->mDiagram );
        cd->takeAxis( this );
    }
    KDAB_FOREACH( AbstractDiagram *diagram, d->secondaryDiagrams ) {
        AbstractCartesianDiagram *cd = qobject_cast< AbstractCartesianDiagram* >( diagram );
        cd->takeAxis( this );
    }
}

void CartesianAxis::init()
{
    d->customTickLength = 3;
    d->position = Bottom;
    setCachedSizeDirty();
    connect( this, SIGNAL( coordinateSystemChanged() ), SLOT( coordinateSystemChanged() ) );
}


bool CartesianAxis::compare( const CartesianAxis* other ) const
{
    if ( other == this ) {
        return true;
    }
    if ( !other ) {
        return false;
    }
    return  AbstractAxis::compare( other ) && ( position() == other->position() ) &&
            ( titleText() == other->titleText() ) &&
            ( titleTextAttributes() == other->titleTextAttributes() );
}

void CartesianAxis::coordinateSystemChanged()
{
    layoutPlanes();
}


void CartesianAxis::setTitleText( const QString& text )
{
    d->titleText = text;
    layoutPlanes();
}

QString CartesianAxis::titleText() const
{
    return d->titleText;
}

void CartesianAxis::setTitleTextAttributes( const TextAttributes &a )
{
    d->titleTextAttributes = a;
    d->useDefaultTextAttributes = false;
    layoutPlanes();
}

TextAttributes CartesianAxis::titleTextAttributes() const
{
    if ( hasDefaultTitleTextAttributes() ) {
        TextAttributes ta( textAttributes() );
        Measure me( ta.fontSize() );
        me.setValue( me.value() * 1.5 );
        ta.setFontSize( me );
        return ta;
    }
    return d->titleTextAttributes;
}

void CartesianAxis::resetTitleTextAttributes()
{
    d->useDefaultTextAttributes = true;
    layoutPlanes();
}

bool CartesianAxis::hasDefaultTitleTextAttributes() const
{
    return d->useDefaultTextAttributes;
}


void CartesianAxis::setPosition ( Position p )
{
    d->position = p;
    layoutPlanes();
}

#if QT_VERSION < 0x040400 || defined(Q_COMPILER_MANGLES_RETURN_TYPE)
const
#endif
CartesianAxis::Position CartesianAxis::position() const
{
    return d->position;
}

void CartesianAxis::layoutPlanes()
{
    if ( ! d->diagram() || ! d->diagram()->coordinatePlane() ) {
        return;
    }
    AbstractCoordinatePlane* plane = d->diagram()->coordinatePlane();
    if ( plane ) {
        plane->layoutPlanes();
    }
}

static bool referenceDiagramIsBarDiagram( const AbstractDiagram * diagram )
{
    const AbstractCartesianDiagram * dia =
            qobject_cast< const AbstractCartesianDiagram * >( diagram );
    if ( dia && dia->referenceDiagram() )
        dia = dia->referenceDiagram();
    return qobject_cast< const BarDiagram* >( dia ) != 0;
}

static bool referenceDiagramNeedsCenteredAbscissaTicks( const AbstractDiagram *diagram )
{
    const AbstractCartesianDiagram * dia =
            qobject_cast< const AbstractCartesianDiagram * >( diagram );
    if ( dia && dia->referenceDiagram() )
        dia = dia->referenceDiagram();
    if ( qobject_cast< const BarDiagram* >( dia ) )
        return true;
    if ( qobject_cast< const StockDiagram* >( dia ) )
        return true;

    const LineDiagram * lineDiagram = qobject_cast< const LineDiagram* >( dia );
    return lineDiagram && lineDiagram->centerDataPoints();
}

bool CartesianAxis::isAbscissa() const
{
    const Qt::Orientation diagramOrientation = referenceDiagramIsBarDiagram( d->diagram() ) ? ( ( BarDiagram* )( d->diagram() ) )->orientation()
                                                                                            : Qt::Vertical;
    return diagramOrientation == Qt::Vertical ? position() == Bottom || position() == Top
                                              : position() == Left   || position() == Right;
}

bool CartesianAxis::isOrdinate() const
{
    return !isAbscissa();
}

void CartesianAxis::paint( QPainter* painter )
{
    if ( !d->diagram() || !d->diagram()->coordinatePlane() ) {
        return;
    }
    PaintContext ctx;
    ctx.setPainter ( painter );
    ctx.setCoordinatePlane( d->diagram()->coordinatePlane() );

    ctx.setRectangle( QRectF( areaGeometry() ) );
    // enabling clipping so that we're not drawing outside
    QRegion clipRegion( areaGeometry().adjusted( -1, -1, 1, 1 ) );
    PainterSaver painterSaver( painter );
    painter->setClipRegion( clipRegion );
    paintCtx( &ctx );
}

const TextAttributes CartesianAxis::Private::titleTextAttributesWithAdjustedRotation() const
{
    TextAttributes titleTA( titleTextAttributes );
    if ( ( position == Left || position == Right ) ) {
        int rotation = titleTA.rotation() + 270;
        if ( rotation >= 360 )
            rotation -= 360;

        // limit the allowed values to 0, 90, 180, 270:
        if ( rotation  < 90 )
            rotation = 0;
        else if ( rotation  < 180 )
            rotation = 90;
        else if ( rotation  < 270 )
            rotation = 180;
        else if ( rotation  < 360 )
            rotation = 270;
        else
            rotation = 0;

        titleTA.setRotation( rotation );
    }
    return titleTA;
}

QString CartesianAxis::Private::customizedLabelText( const QString& text, Qt::Orientation orientation,
                                                     qreal value ) const
{
    // ### like in the old code, using int( value ) as column number...
    QString withUnits = diagram()->unitPrefix( int( value ), orientation, true ) +
                        text +
                        diagram()->unitSuffix( int( value ), orientation, true );
    return axis()->customizedLabel( withUnits );
}

void CartesianAxis::setTitleSpace( qreal axisTitleSpace )
{
    d->axisTitleSpace = axisTitleSpace;
}

qreal CartesianAxis::titleSpace() const
{
    return d->axisTitleSpace;
}

void CartesianAxis::setTitleSize( qreal value )
{
    d->axisSize = value;
}

qreal CartesianAxis::titleSize() const
{
    return d->axisSize;
}

void CartesianAxis::Private::drawTitleText( QPainter* painter, CartesianCoordinatePlane* plane, const QRect& geoRect ) const
{
    const TextAttributes titleTA( titleTextAttributesWithAdjustedRotation() );
    if ( titleTA.isVisible() ) {
        TextLayoutItem titleItem( titleText, titleTA, plane->parent(), KDChartEnums::MeasureOrientationMinimum,
                                  Qt::AlignHCenter | Qt::AlignVCenter );
        QPointF point;
        QSize size = titleItem.sizeHint();
        //FIXME(khz): We definitely need to provide a way that users can decide
        //            the position of an axis title.
        switch ( position ) {
        case Top:
            point.setX( geoRect.left() + geoRect.width() / 2.0 );
            point.setY( geoRect.top() + ( size.height() / 2 ) / axisTitleSpace );
            size.setWidth( qMin( size.width(), axis()->geometry().width() ) );
            break;
        case Bottom:
            point.setX( geoRect.left() + geoRect.width() / 2.0 );
            point.setY( geoRect.bottom() - ( size.height()/2 ) / axisTitleSpace );
            size.setWidth( qMin( size.width(), axis()->geometry().width() ) );
            break;
        case Left:
            point.setX( geoRect.left() + ( size.width() / 2 ) / axisTitleSpace );
            point.setY( geoRect.top() + geoRect.height() / 2.0 );
            size.setHeight( qMin( size.height(), axis()->geometry().height() ) );
            break;
        case Right:
            point.setX( geoRect.right() - ( size.width() / 2 ) / axisTitleSpace );
            point.setY( geoRect.top() + geoRect.height() / 2.0 );
            size.setHeight( qMin( size.height(), axis()->geometry().height() ) );
            break;
        }
        const PainterSaver painterSaver( painter );
        painter->translate( point );
        titleItem.setGeometry( QRect( QPoint( -size.width() / 2, -size.height() / 2 ), size ) );
        titleItem.paint( painter );
    }
}


static void calculateNextLabel( qreal& labelValue, qreal step, bool isLogarithmic, qreal min )
{
    if ( isLogarithmic ) {
        if ( step > 0.0 ) {
            labelValue *= 10.0;
        } else {
            labelValue /= 10.0;
        }
        if ( labelValue == 0.0 ) {
            labelValue = pow( 10.0, floor( log10( min ) ) );
        }
    } else {
        labelValue += step;
        if ( qAbs( labelValue ) < 1.0e-15 ) {
            labelValue = 0.0;
        }
    }
}

static bool xLessThan( const QPoint& p1, const QPoint& p2 )
{
    return p1.x() < p2.x();
}

static bool yLessThan( const QPoint& p1, const QPoint& p2 )
{
    return p1.y() < p2.y();
}

void CartesianAxis::paintCtx( PaintContext* context )
{
    qDebug() << Q_FUNC_INFO << isAbscissa();
    Q_ASSERT_X ( d->diagram(), "CartesianAxis::paint",
                 "Function call not allowed: The axis is not assigned to any diagram." );

    CartesianCoordinatePlane* plane = dynamic_cast<CartesianCoordinatePlane*>( context->coordinatePlane() );
    Q_ASSERT_X ( plane, "CartesianAxis::paint",
                 "Bad function call: PaintContext::coordinatePlane() NOT a cartesian plane." );

    // note: Not having any data model assigned is no bug
    //       but we can not draw an axis then either.
    if ( !d->diagram()->model() ) {
        return;
    }

    // TODO explain this properly
    // Determine the diagram that specifies the orientation of the diagram we're painting here
    // That diagram is the reference diagram, if it exists, or otherwise the diagram itself.
    // Note: In KDChart 2.3 or earlier, only a bar diagram can be vertical instead of horizontal.
    const AbstractCartesianDiagram* refDiagram = qobject_cast< const AbstractCartesianDiagram * >( d->diagram() );
    if ( refDiagram && refDiagram->referenceDiagram() ) {
        refDiagram = refDiagram->referenceDiagram();
    }
    const BarDiagram* barDiagram = qobject_cast< const BarDiagram* >( refDiagram );
    const Qt::Orientation diagramOrientation = barDiagram ? barDiagram->orientation() : Qt::Vertical;
    const bool diagramIsVertical = diagramOrientation == Qt::Vertical;
    const bool centerTicks = referenceDiagramNeedsCenteredAbscissaTicks( d->diagram() ) && isAbscissa();

    XySwitch dataXy( isOrdinate() ); // take data from x or y axis
    XySwitch geoXy( ( isOrdinate() && diagramIsVertical ) ||
                    ( isAbscissa() && !diagramIsVertical ) ); // paint horizontally or vertically

    QPainter* const painter = context->painter();

    qreal transversePosition;
    {
        DataDimension dimX = plane->gridDimensionsList().first();
        DataDimension dimY = plane->gridDimensionsList().last();
        QPointF start = QPointF( dimX.start, dimY.start );
        QPointF end = QPointF( dimX.end, dimY.end );
        // consider this: you can turn a diagonal line into a horizontal or vertical line on any
        // edge by changing just one of its four coordinates.
        switch ( position() ) {
        case CartesianAxis::Bottom:
            end.setY( dimY.start );
            break;
        case CartesianAxis::Top:
            start.setY( dimY.end );
            break;
        case CartesianAxis::Left:
            end.setX( dimX.start );
            break;
        case CartesianAxis::Right:
            start.setX( dimX.end );
            break;
        }
        if ( rulerAttributes().showRulerLine() ) {
            bool clipSaved = context->painter()->hasClipping();
            painter->setClipping( false );
            painter->drawLine( plane->translate( start ), plane->translate( end ) );
            painter->setClipping( clipSaved );
        }
        transversePosition = geoXy( start.y(), start.x() );
    }

    // TODO: check that we respect all tick attributes

    const TextAttributes labelTA = textAttributes();

    int labelThinningFactor = 1;
    TextLayoutItem *tickLabel = new TextLayoutItem( QString(), labelTA, plane->parent(),
                                                    KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft );
    TextLayoutItem *prevTickLabel = 0;
    QPointF prevTickLabelPos;
    enum {
        Layout = 0,
        Painting,
        Done
    };
    for ( int step = labelTA.isVisible() ? Layout : Painting; step < Done; step++ ) {
        for ( TickIterator it( this, plane, labelThinningFactor ); !it.isAtEnd(); ++it ) {
            if ( it.position() == 0.0 && !rulerAttributes().showZeroLabel() ) { // TODO fuzzy compare?
                continue;
            }
            const qreal drawPos = it.position() + ( centerTicks ? 0.5 : 0. );
            QPointF onAxis = plane->translate( geoXy( QPointF( drawPos, transversePosition ) ,
                                                      QPointF( transversePosition, drawPos ) ) );
            const bool isOutwardsPositive = position() == Bottom || position() == Right;

            // paint the tick mark

            QPointF tickEnd = onAxis;
            qreal tickLen = tickLength( it.type() == TickIterator::MinorTick );
            geoXy.lvalue( tickEnd.ry(), tickEnd.rx() ) += isOutwardsPositive ? tickLen : -tickLen;
            if ( step == Painting ) {
                painter->save();
                if ( rulerAttributes().hasTickMarkPenAt( it.position() ) ) {
                    painter->setPen( rulerAttributes().tickMarkPen( it.position() ) );
                } else {
                    painter->setPen( rulerAttributes().majorTickMarkPen() );
                }
                painter->drawLine( onAxis, tickEnd );
                painter->restore();
            }

            // paint the label

            if ( it.text().isEmpty() || !labelTA.isVisible() ) {
                continue;
            }

            QString text = it.text();
            if ( it.type() == TickIterator::MajorTick ) {
                text = d->customizedLabelText( text, geoXy( Qt::Horizontal, Qt::Vertical ), it.position() );
            }
            tickLabel->setText( text );
            QSizeF size = QSizeF( tickLabel->sizeHint() );
            QPolygon labelPoly = tickLabel->rotatedCorners();
            Q_ASSERT( labelPoly.count() == 4 );
            QPointF labelPos = tickEnd;
            switch ( position() ) {
            case Left:
                labelPos += QPointF( -size.width(), 0. );
                // fall through
            case Right: {
                // find the two points with x-coordinates closest to the axis and adjust height
                // according to their average height (in addition to label height)
                // ### the algorithm fails when at certain angles (that don't look very good anyway)
                //     the two x-closest points are on a long label side, but the fix is somewhat nontrivial
                qSort( labelPoly.begin(), labelPoly.end(), xLessThan );
                QPoint p1 = labelPoly[ position() == Right ? 0 : 3 ];
                QPoint p2 = labelPoly[ position() == Right ? 1 : 2 ];
                // ### the 0.6 is a bit of a fudge factor and depends on the distance between label
                //     and axis. it might become necessary to properly calculate a good value there.
                labelPos += QPointF( 1., -0.5 * size.height() - 0.6 * ( p1.y() + p2.y() ) );
                break; }
            case Top:
                labelPos += QPointF( 0., -size.height() );
                // fall through
            case Bottom: {
                // same as Right:, but with coordinates swapped
                qSort( labelPoly.begin(), labelPoly.end(), yLessThan );
                QPoint p1 = labelPoly[ position() == Bottom ? 0 : 3 ];
                QPoint p2 = labelPoly[ position() == Bottom ? 1 : 2 ];
                labelPos += QPointF( -0.5 * size.width() - 0.6 * ( p1.x() + p2.x() ), 0. );
                break; }
            }

            tickLabel->setGeometry( QRect( labelPos.toPoint(), size.toSize() ) );

            if ( step == Painting ) {
                tickLabel->paint( painter );
            }

            // collision check the current label against the previous one

            // we don't shorten or decimate labels if they are already the manual short type,
            // or if they are the manual long type and on the vertical axis
            if ( step == Layout && ( it.type() == TickIterator::MajorTick ||
                 ( it.type() == TickIterator::MajorTickManualLong && !geoXy.isY ) ) ) {
                if ( !prevTickLabel ) {
                    prevTickLabel = tickLabel;
                    tickLabel = new TextLayoutItem( QString(), labelTA, plane->parent(),
                                                    KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft );
                } else {
                    if ( tickLabel->intersects( *prevTickLabel, labelPos, prevTickLabelPos ) ) {
                        labelThinningFactor++;
                        step--; // relayout
                        break;
                    }
                    qSwap( prevTickLabel, tickLabel );
                }
                prevTickLabelPos = labelPos;
            }
        }
        delete prevTickLabel;
        prevTickLabel = 0;
    }
    delete tickLabel;
    tickLabel = 0;
    delete prevTickLabel;
    prevTickLabel = 0;

    if ( ! titleText().isEmpty() ) {
        d->drawTitleText( painter, plane, d->axis()->geometry() );
    }
}


/* pure virtual in QLayoutItem */
bool CartesianAxis::isEmpty() const
{
    return false; // if the axis exists, it has some (perhaps default) content
}
/* pure virtual in QLayoutItem */
Qt::Orientations CartesianAxis::expandingDirections() const
{
    Qt::Orientations ret;
    switch ( position() )
    {
    case Bottom:
    case Top:
        ret = Qt::Horizontal;
        break;
    case Left:
    case Right:
        ret = Qt::Vertical;
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handeld
        break;
    };
    return ret;
}


static void calculateOverlap( int i, int first, int last,
                              int measure,
                              bool centerAbscissaTicks,
                              int& firstOverlap, int& lastOverlap )
{
    if ( i == first ) {
        if ( centerAbscissaTicks ) {
            //TODO(khz): Calculate the amount of left overlap
            //           for bar diagrams.
        } else {
            firstOverlap = measure / 2;
        }
    }
    // we test both bounds in on go: first and last might be equal
    if ( i == last ) {
        if ( centerAbscissaTicks ) {
            //TODO(khz): Calculate the amount of right overlap
            //           for bar diagrams.
        } else {
            lastOverlap = measure / 2;
        }
    }
}


void CartesianAxis::setCachedSizeDirty() const
{
    d->cachedMaximumSize = QSize();
}

/* pure virtual in QLayoutItem */
QSize CartesianAxis::maximumSize() const
{
    if ( ! d->cachedMaximumSize.isValid() )
        d->cachedMaximumSize = d->calculateMaximumSize();
    return d->cachedMaximumSize;
}

QSize CartesianAxis::Private::calculateMaximumSize() const
{
    QSize result;
    if ( !diagram() )
        return result;

    const AbstractCartesianDiagram * dia = qobject_cast< const AbstractCartesianDiagram * >( diagram() );
    if ( dia && dia->referenceDiagram() )
        dia = dia->referenceDiagram();
    const BarDiagram *barDiagram = qobject_cast< const BarDiagram* >( dia );
    const Qt::Orientation diagramOrientation = barDiagram != 0 ? barDiagram->orientation() : Qt::Vertical;
    const bool diagramIsVertical = diagramOrientation == Qt::Vertical;

    const TextAttributes labelTA = mAxis->textAttributes();
    const bool drawLabels = labelTA.isVisible();

    const TextAttributes titleTA( titleTextAttributesWithAdjustedRotation() );
    const bool drawTitle = titleTA.isVisible() && ! axis()->titleText().isEmpty();

    AbstractCoordinatePlane* plane = diagram()->coordinatePlane();
    //qDebug() << this<<"::maximumSize() uses plane geometry" << plane->geometry();
    QObject* refArea = plane->parent();
    TextLayoutItem labelItem( QString(), labelTA, refArea,
                              KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft );
    TextLayoutItem titleItem( axis()->titleText(), titleTA, refArea,
                              KDChartEnums::MeasureOrientationMinimum, Qt::AlignHCenter | Qt::AlignVCenter );

    const QFontMetrics fm( labelItem.realFont(), GlobalMeasureScaling::paintDevice() );

    const qreal labelGap =
        drawLabels
        ? ( ( diagramIsVertical ? fm.height() : fm.averageCharWidth() ) / 3.0 )
        : 0.0;
    const QFontMetricsF titleFM = QFontMetricsF( titleItem.realFont(), GlobalMeasureScaling::paintDevice() );
    const qreal titleGap =
        drawTitle
        ? ( ( diagramIsVertical ? titleFM.height() : titleFM.averageCharWidth() ) / 3.0 )
        : 0.0;

    if ( axis()->isAbscissa() ) {
        const bool centerAbscissaTicks = referenceDiagramNeedsCenteredAbscissaTicks( diagram() );
        int leftOverlap = 0;
        int rightOverlap = 0;

        qreal w = diagramIsVertical ? 10.0 : 0.0;
        qreal h = diagramIsVertical ? 0.0 : 10.0;
        if ( drawLabels ) {
            // if there're no label strings, we take the biggest needed number as height
            if ( !annotations.isEmpty() ) {
                KDAB_FOREACH( const QString& string, annotations.values() ) {
                    labelItem.setText( string );
                    const QSize siz = labelItem.sizeHint();
                    if ( diagramIsVertical )
                        h = qMax( h, qreal( siz.height() ) );
                    else
                        w = qMax( w, qreal( siz.width() ) );
                }
            }
            else if ( !axis()->labels().isEmpty() )
            {
                // find the longest label text:
                const int first = 0;
                const int last = axis()->labels().count() - 1;
                const QStringList labelsList( axis()->labels() );
                for ( int i = first; i <= last; ++i )
                {
                    labelItem.setText( axis()->customizedLabel( labelsList[ i ] ) );
                    const QSize siz = labelItem.sizeHint();
                    //qDebug()<<siz;
                    if ( diagramIsVertical )
                        h = qMax( h, qreal( siz.height() ) );
                    else
                        w = qMax( w, qreal( siz.width() ) );
                    calculateOverlap( i, first, last, diagramIsVertical ? siz.width() : siz.height(), centerAbscissaTicks,
                                      leftOverlap, rightOverlap );

                }
                if ( position == CartesianAxis::Top || position == CartesianAxis::Bottom )
                    h *= axisSize >=0.0 ? axisSize : -1.0 / axisSize;
                else
                    w *= axisSize >=0.0 ? axisSize : -1.0 / axisSize;
            }
            else
            {
                QStringList headerLabels = diagram()->itemRowLabels();
                const int headerLabelsCount = headerLabels.count();
                if ( headerLabelsCount ) {
                    if ( cachedHeaderLabels == headerLabels && ( diagramIsVertical ? cachedFontHeight == fm.height() : cachedFontWidth == fm.averageCharWidth() ) ) {
                        if ( diagramIsVertical )
                            h = cachedLabelHeight;
                        else
                            w = cachedLabelWidth;
                    } else {
                        cachedHeaderLabels = headerLabels;
                        if ( diagramIsVertical )
                            cachedFontWidth = fm.averageCharWidth();
                        else
                            cachedFontHeight = fm.height();
                        const bool useFastCalcAlgorithm
                            = ( strcmp( axis()->metaObject()->className(), "KDChart::CartesianAxis" ) == 0 );
                        const int first = 0;
                        const int last = headerLabelsCount - 1;
                        for ( int i = first;
                            i <= last;
                            i = ( useFastCalcAlgorithm && i < last ) ? last : ( i + 1 ) )
                        {
                            labelItem.setText( axis()->customizedLabel( headerLabels[ i ] ) );
                            const QSize siz = labelItem.sizeHint();
                            if ( diagramIsVertical ) {
                                h = qMax( h, qreal( siz.height() ) );
                                cachedLabelHeight = h;
                            } else {
                                cachedLabelWidth = w;
                                w = qMax( w, qreal( siz.width() ) );
                            }
                            calculateOverlap( i, first, last, diagramIsVertical ? siz.width() : siz.height(), centerAbscissaTicks,
                                              leftOverlap, rightOverlap );
                        }
                    }
                } else {
                    labelItem.setText(
                            axis()->customizedLabel(
                                    QString::number( diagramIsVertical ? plane->gridDimensionsList().first().end
                                                                       : plane->gridDimensionsList().last().end, 'f', 0 ) ) );
                    const QSize siz = labelItem.sizeHint();
                    if ( diagramIsVertical )
                        h = siz.height();
                    else
                        w = siz.width();
                    calculateOverlap( 0, 0, 0, siz.width(), centerAbscissaTicks,
                                      leftOverlap, rightOverlap );
                }
            }
            // we leave a little gap between axis labels and bottom (or top, resp.) side of axis
            h += labelGap;
        }
        // space for a possible title:
        if ( drawTitle ) {
            // we add the title height and leave a little gap between axis labels and axis title
            if ( diagramIsVertical ) {
                h += titleItem.sizeHint().height() + titleGap;
                w = titleItem.sizeHint().width() + 2.0;
            } else {
                h = titleItem.sizeHint().height() + 2.0;
                w += titleItem.sizeHint().width() + titleGap;
            }
        }
        // space for the ticks
        if ( diagramIsVertical )
            h += qAbs( axis()->tickLength() ) * 3.0;
        else
            w += qAbs( axis()->tickLength() ) * 3.0;
        result = QSize ( int( w ), int( h ) );

        //qDebug()<<"calculated size of x axis:"<<result;

        // If necessary adjust the widths
        // of the left (or right, resp.) side neighboring columns:
        amountOfLeftOverlap = leftOverlap;
        amountOfRightOverlap = rightOverlap;
    } else {
        int topOverlap = 0;
        int bottomOverlap = 0;

        qreal w = diagramIsVertical ? 0.0 : 10.0;
        qreal h = diagramIsVertical ? 10.0 : 0.0;
        if ( drawLabels ) {
            // if there're no label strings, we loop through the values
            // taking the longest (not largest) number - e.g. 0.00001 is longer than 100
            if ( !annotations.isEmpty() ) {
                KDAB_FOREACH( const QString& string, annotations.values() ) {
                    labelItem.setText( string );
                    const QSize siz = labelItem.sizeHint();
                    if ( diagramIsVertical )
                        w = qMax( w, qreal( siz.width() ) );
                    else
                        h = qMax( h, qreal( siz.height() ) );
                }
            } else if ( axis()->labels().isEmpty() ) {
                const DataDimension dimY = AbstractGrid::adjustedLowerUpperRange(
                        diagramIsVertical ? plane->gridDimensionsList().last()
                                          : plane->gridDimensionsList().first(), true, true );
                const qreal step = dimY.stepWidth;
                const qreal minValue = dimY.start;
                const qreal maxValue = dimY.end;
                const bool isLogarithmicY = dimY.calcMode == AbstractCoordinatePlane::Logarithmic;
                qreal labelValue = minValue;

                while ( labelValue <= maxValue ) {
                    const QString labelText = diagram()->unitPrefix( int( labelValue ), diagramOrientation, true ) +
                                            QString::number( labelValue ) +
                                            diagram()->unitSuffix( int( labelValue ), diagramOrientation, true );
                    labelItem.setText( axis()->customizedLabel( labelText ) );

                    const QSize siz = labelItem.sizeHint();
                    if ( diagramIsVertical )
                        w = qMax( w, qreal( siz.width() ) );
                    else
                        h = qMax( h, qreal( siz.height() ) );
                    calculateOverlap( 0, 0, 0, diagramIsVertical ? siz.height() : siz.width(), false,// bar diagram flag is ignored for Ordinates
                                    topOverlap, bottomOverlap );
                    calculateNextLabel( labelValue, step, isLogarithmicY, plane->gridDimensionsList().last().start );

                    if ( maxValue == 0 && minValue == 0 )
                        break;
                }

                if ( position == CartesianAxis::Top || position == CartesianAxis::Bottom ) {
                    h *= axisSize >= 0.0 ? axisSize : -1.0 / axisSize;
                } else {
                    w *= axisSize >= 0.0 ? axisSize : -1.0 / axisSize;
                }
            } else {
                // find the longest label text:
                const int first = 0;
                const int last = axis()->labels().count() - 1;
                const QStringList labelsList( axis()->labels() );
                for ( int i = first; i <= last; ++i )
                {
                    labelItem.setText( axis()->customizedLabel( labelsList[ i ] ) );
                    const QSize siz = labelItem.sizeHint();
                    if ( diagramIsVertical)
                        w = qMax( w, qreal( siz.width() ) );
                    else
                        h = qMax( h, qreal( siz.height() ) );
                    calculateOverlap( 0, 0, 0, diagramIsVertical ? siz.height() : siz.width(), false,// bar diagram flag is ignored for Ordinates
                                      topOverlap, bottomOverlap );
                }
            }
            // we leave a little gap between axis labels and left (or right, resp.) side of axis
            w += labelGap;
        }
        // space for a possible title:
        if ( drawTitle ) {
            // we add the title height and leave a little gap between axis labels and axis title
            if ( diagramIsVertical ) {
                w += titleItem.sizeHint().width() + titleGap;
                h = titleItem.sizeHint().height() + 2.0;
            } else {
                w = titleItem.sizeHint().width() + 2.0;
                h += titleItem.sizeHint().height() + titleGap;
            }
            //qDebug() << "left/right axis title item size-hint:" << titleItem.sizeHint();
        }
        // space for the ticks
        if ( diagramIsVertical )
            w += qAbs( axis()->tickLength() ) * 3.0;
        else
            h += qAbs( axis()->tickLength() ) * 3.0;

        result = QSize ( int( w ), int( h ) );
        //qDebug() << "left/right axis width:" << result << "   w:" << w;


        // If necessary adjust the heights
        // of the top (or bottom, resp.) side neighboring rows:
        amountOfTopOverlap = topOverlap;
        amountOfBottomOverlap = bottomOverlap;
    }
    return result;
}
/* pure virtual in QLayoutItem */
QSize CartesianAxis::minimumSize() const
{
    return maximumSize();
}
/* pure virtual in QLayoutItem */
QSize CartesianAxis::sizeHint() const
{
    return maximumSize();
}
/* pure virtual in QLayoutItem */
void CartesianAxis::setGeometry( const QRect& r )
{
    if ( d->geometry != r ) {
        d->geometry = r;
        setCachedSizeDirty();
    }
}
/* pure virtual in QLayoutItem */
QRect CartesianAxis::geometry() const
{
    return d->geometry;
}

void CartesianAxis::setCustomTickLength( int value )
{
    d->customTickLength = value;
}

int CartesianAxis::customTickLength() const
{
    return d->customTickLength;
}

int CartesianAxis::tickLength( bool subUnitTicks ) const
{
    const RulerAttributes& rulerAttr = rulerAttributes();
    return subUnitTicks ? rulerAttr.minorTickMarkLength() : rulerAttr.majorTickMarkLength();
}

QMap< qreal, QString > CartesianAxis::annotations() const
{
    return d->annotations;
}

void CartesianAxis::setAnnotations( const QMap< qreal, QString >& annotations )
{
    if ( d->annotations == annotations )
        return;

    d->annotations = annotations;
    update();
}

QList< qreal > CartesianAxis::customTicks() const
{
    return d->customTicksPositions;
}

void CartesianAxis::setCustomTicks( const QList< qreal >& customTicksPositions )
{
    if ( d->customTicksPositions == customTicksPositions )
        return;

    d->customTicksPositions = customTicksPositions;
    update();
}

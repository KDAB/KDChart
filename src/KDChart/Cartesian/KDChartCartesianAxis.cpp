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
    T operator()( T x, T y ) const { return isY ? y : x; }

    // lvalues
    template< class T >
    T& lvalue( T& x, T& y ) const { return isY ? y : x; }

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
    bool hasShorterLabels() const { return m_axis->shortLabels().count() == m_axis->labels().count() &&
                                           !m_axis->labels().isEmpty(); }
    bool isAtEnd() const { return m_position == std::numeric_limits< qreal >::infinity(); }
    void operator++();

    bool areAlmostEqual( qreal r1, qreal r2 ) const;

private:
    // these are generally set once in the constructor
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
    Q_ASSERT( std::numeric_limits< qreal >::has_infinity );
    XySwitch xy( m_axis->isOrdinate() );

    m_dimension = xy( plane->gridDimensionsList().first(), plane->gridDimensionsList().last() );
    GridAttributes gridAttributes = plane->gridAttributes( xy( Qt::Horizontal, Qt::Vertical ) );


    m_dimension = AbstractGrid::adjustedLowerUpperRange( m_dimension,
                                                         gridAttributes.adjustLowerBoundToGrid(),
                                                         gridAttributes.adjustUpperBoundToGrid() );

    const bool hasMajorTicks = m_axis->rulerAttributes().showMajorTickMarks() && m_dimension.stepWidth > 0;
    const bool hasMinorTicks = m_axis->rulerAttributes().showMinorTickMarks() && m_dimension.subStepWidth > 0;
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

    const qreal inf = std::numeric_limits< qreal >::infinity();

    if ( m_majorThinningFactor > 1 && hasShorterLabels() ) {
        m_manualLabelTexts = m_axis->shortLabels();
    } else {
        m_manualLabelTexts = m_axis->labels();
    }
    m_manualLabelIndex = m_manualLabelTexts.isEmpty() ? -1 : 0;

    // position the iterator just in front of the first tick to be drawn so that the logic from
    // operator++() can be reused to find the first tick

    if ( m_isLogarithmic ) {
        if ( m_dimension.start >= 0 ) {
            m_position = m_dimension.start ? pow( 10.0, floor( log10( m_dimension.start ) ) - 1.0 )
                                           : 1e-6;
            m_majorTick = hasMajorTicks ? m_position : inf;
            m_minorTick = hasMinorTicks ? m_position * 20.0 : inf;
        } else {
            m_position = -pow( 10.0, ceil( log10( -m_dimension.start ) ) + 1.0 );
            m_majorTick = hasMajorTicks ? m_position : inf;
            m_minorTick = hasMinorTicks ? m_position * 0.09 : inf;
        }
    } else {
        m_majorTick = hasMajorTicks ? m_dimension.start : inf;
        m_minorTick = hasMinorTicks ? m_dimension.start : inf;
        m_position = slightlyLessThan( m_dimension.start );
    }
    ++( *this );
}

bool TickIterator::areAlmostEqual( qreal r1, qreal r2 ) const
{
    if ( !m_isLogarithmic ) {
        return qAbs( r2 - r1 ) < ( m_dimension.end - m_dimension.start ) * 1e-6;
    } else {
        return qAbs( r2 - r1 ) < qMax( qAbs( r1 ), qAbs( r2 ) ) * 0.01;
    }
}

void TickIterator::operator++()
{
    if ( isAtEnd() ) {
        return;
    }
    const qreal inf = std::numeric_limits< qreal >::infinity();

    // make sure to find the next tick at a value strictly greater than m_position

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
        // advance the calculated ticks
        if ( m_isLogarithmic ) {
            while ( m_majorTick <= m_position ) {
                m_majorTick *= m_position >= 0 ? 10 : 0.1;
            }
            while ( m_minorTick <= m_position ) {
                // the next major tick position should be greater than this
                m_minorTick += m_majorTick * ( m_position >= 0 ? 0.1 : 1.0 );
            }
        } else {
            while ( m_majorTick <= m_position ) {
                m_majorTick += m_dimension.stepWidth;
            }
            while ( m_minorTick <= m_position ) {
                m_minorTick += m_dimension.subStepWidth;
            }
        }

        // now see which kind of tick we'll have
        if ( m_majorTick != inf && ( m_majorTick <= m_minorTick ||
                                     areAlmostEqual( m_majorTick, m_minorTick ) ) ) {
            m_position = m_majorTick;
            if ( m_minorTick != inf ) {
                // realign minor to major
                m_minorTick = m_majorTick;
            }

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
        } else if ( m_minorTick != inf ) {
            m_position = m_minorTick;
            m_text.clear();
            m_type = MinorTick;
        } else {
            m_position = inf;
        }
    }

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
    PainterSaver painterSaver( painter );
    paintCtx( &ctx );
}

const TextAttributes CartesianAxis::Private::titleTextAttributesWithAdjustedRotation() const
{
    TextAttributes titleTA( titleTextAttributes );
    int rotation = titleTA.rotation();
    if ( position == Left || position == Right ) {
        rotation += 270;
    }
    if ( rotation >= 360 ) {
        rotation -= 360;
    }
    // limit the allowed values to 0, 90, 180, 270
    rotation = ( rotation / 90 ) * 90;
    titleTA.setRotation( rotation );
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
            point.setX( geoRect.left() + geoRect.width() / 2 );
            point.setY( geoRect.top() + ( size.height() / 2 ) / axisTitleSpace );
            size.setWidth( qMin( size.width(), axis()->geometry().width() ) );
            break;
        case Bottom:
            point.setX( geoRect.left() + geoRect.width() / 2 );
            point.setY( geoRect.bottom() - ( size.height() / 2 ) / axisTitleSpace );
            size.setWidth( qMin( size.width(), axis()->geometry().width() ) );
            break;
        case Left:
            point.setX( geoRect.left() + ( size.width() / 2 ) / axisTitleSpace );
            point.setY( geoRect.top() + geoRect.height() / 2 );
            size.setHeight( qMin( size.height(), axis()->geometry().height() ) );
            break;
        case Right:
            point.setX( geoRect.right() - ( size.width() / 2 ) / axisTitleSpace );
            point.setY( geoRect.top() + geoRect.height() / 2 );
            size.setHeight( qMin( size.height(), axis()->geometry().height() ) );
            break;
        }
        const PainterSaver painterSaver( painter );
        painter->translate( point );
        titleItem.setGeometry( QRect( QPoint( -size.width() / 2, -size.height() / 2 ), size ) );
        titleItem.paint( painter );
    }
}

bool CartesianAxis::Private::isVertical() const
{
    // Determine the diagram that specifies the orientation of the diagram
    // That diagram is the reference diagram, if it exists, or otherwise the diagram itself.
    // Note: In KDChart 2.3 or earlier, only a bar diagram can be vertical instead of horizontal.
    const AbstractCartesianDiagram* refDiagram = qobject_cast< const AbstractCartesianDiagram * >( diagram() );
    if ( refDiagram && refDiagram->referenceDiagram() ) {
        refDiagram = refDiagram->referenceDiagram();
    }
    const BarDiagram* barDiagram = qobject_cast< const BarDiagram* >( refDiagram );
    Qt::Orientation diagramOrientation = barDiagram ? barDiagram->orientation() : Qt::Vertical;
    bool isDiagramVertical = diagramOrientation == Qt::Vertical;
    return ( axis()->isOrdinate() && isDiagramVertical ) || ( axis()->isAbscissa() && !isDiagramVertical );
}

void CartesianAxis::paintCtx( PaintContext* context )
{
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

    const bool centerTicks = referenceDiagramNeedsCenteredAbscissaTicks( d->diagram() ) && isAbscissa();

    XySwitch geoXy( d->isVertical() );

    QPainter* const painter = context->painter();

    // determine the position of the axis (also required for labels) and paint it

    qreal transversePosition;
    {
        DataDimension dimX = plane->gridDimensionsList().first();
        DataDimension dimY = plane->gridDimensionsList().last();
        QPointF start( dimX.start, dimY.start );
        QPointF end( dimX.end, dimY.end );
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

    // paint ticks and labels

    TextAttributes labelTA = textAttributes();

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
        delete prevTickLabel;
        prevTickLabel = 0;
        for ( TickIterator it( this, plane, labelThinningFactor ); !it.isAtEnd(); ++it ) {
            if ( !rulerAttributes().showZeroLabel() && it.areAlmostEqual( it.position(), 0.0 ) ) {
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
            QPolygon labelPoly = tickLabel->boundingPolygon();
            Q_ASSERT( labelPoly.count() == 4 );

            // for alignment, find the label polygon edge "most parallel" and closest to the axis

            int axisAngle = 0;
            switch ( position() ) {
            case Bottom:
                axisAngle = 0; break;
            case Top:
                axisAngle = 180; break;
            case Right:
                axisAngle = 270; break;
            case Left:
                axisAngle = 90; break;
            default:
                Q_ASSERT( false );
            }
            // the left axis is not actually pointing down and the top axis not actually pointing
            // left, but their corresponding closest edges of a rectangular unrotated label polygon are.

            int relAngle = axisAngle - labelTA.rotation() + 45;
            if ( relAngle < 0 ) {
                relAngle += 360;
            }
            int polyCorner1 = relAngle / 90;
            QPoint p1 = labelPoly.at( polyCorner1 );
            QPoint p2 = labelPoly.at( polyCorner1 == 3 ? 0 : ( polyCorner1 + 1 ) );

             // TODO: non-hardcoded tick-label spacing depending on font size and such
            //        we should use geoXy here when we switch to doing the spacing correctly
            QPointF labelPos = tickEnd;
            switch ( position() ) {
            case Left:
                labelPos += QPointF( -size.width(), 1. );
                // fall through
            case Right:
                // ### the 0.6 is a bit of a fudge factor and depends on the distance between label
                //     and axis. it might become necessary to properly calculate a good value there.
                labelPos += QPointF( 1., -0.5 * size.height() - 0.6 * ( p1.y() + p2.y() ) );
                break;
            case Top:
                labelPos += QPointF( 1., -size.height() );
                // fall through
            case Bottom:
                labelPos += QPointF( -0.5 * size.width() - 0.6 * ( p1.x() + p2.x() ), 0. );
                break;
            }

            tickLabel->setGeometry( QRect( labelPos.toPoint(), size.toSize() ) );

            if ( step == Painting ) {
                tickLabel->paint( painter );
            }

            // collision check the current label against the previous one

            // like in the old code, we don't shorten or decimate labels if they are already the
            // manual short type, or if they are the manual long type and on the vertical axis
            // ### they can still collide though, especially when they're rotated!
            if ( step == Layout ) {
                int spaceSavingRotation = geoXy( 270, 0 );
                bool canRotate = labelTA.autoRotate() && labelTA.rotation() != spaceSavingRotation;
                const bool canShortenLabels = !geoXy.isY && it.type() == TickIterator::MajorTickManualLong &&
                                              it.hasShorterLabels();
                bool collides = false;
                if ( it.type() == TickIterator::MajorTick || canShortenLabels || canRotate ) {
                    if ( !prevTickLabel ) {
                        // this is the first label of the axis, so just instantiate the second
                        prevTickLabel = tickLabel;
                        tickLabel = new TextLayoutItem( QString(), labelTA, plane->parent(),
                                                        KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft );
                    } else {
                        collides = tickLabel->intersects( *prevTickLabel, labelPos, prevTickLabelPos );
                        qSwap( prevTickLabel, tickLabel );
                    }
                    prevTickLabelPos = labelPos;
                }
                if ( collides ) {
                    // to make room, we try in order: shorten, rotate, decimate
                    if ( canRotate && !canShortenLabels ) {
                        labelTA.setRotation( spaceSavingRotation );
                        // tickLabel will be reused in the next round
                        tickLabel->setTextAttributes( labelTA );
                    } else {
                        labelThinningFactor++;
                    }
                    step--; // relayout
                    break;
                }
            }
        }
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
    switch ( position() ) {
    case Bottom:
    case Top:
        ret = Qt::Horizontal;
        break;
    case Left:
    case Right:
        ret = Qt::Vertical;
        break;
    default:
        Q_ASSERT( false );
        break;
    };
    return ret;
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
    if ( !diagram() ) {
        return QSize();
    }

    CartesianCoordinatePlane* plane = dynamic_cast< CartesianCoordinatePlane* >( diagram()->coordinatePlane() );
    Q_ASSERT( plane );
    QObject* refArea = plane->parent();

    // we ignore:
    // - label thinning (expensive, not worst case and we want worst case)
    // - label autorotation (expensive, obscure feature(?))
    // - axis length (it is determined by the plane / diagram / chart anyway)
    // - the title's influence on axis length; this one might be TODO. See KDCH-863.

    XySwitch geoXy( isVertical() );
    qreal size = 0; // this is the size transverse to the axis direction

    if ( mAxis->textAttributes().isVisible() ) {
        TextLayoutItem tickLabel( QString(), mAxis->textAttributes(), refArea,
                                  KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft );
        const qreal tickLabelSpacing = 1; // this is implicitly defined in paintCtx()

        for ( TickIterator it( axis(), plane, 1 ); !it.isAtEnd(); ++it ) {
            qreal labelSize = 0.0;
            if ( !it.text().isEmpty() ) {
                tickLabel.setText( it.text() );
                QSize sz = tickLabel.sizeHint();
                labelSize = geoXy( sz.height(), sz.width() );
            }
            qreal tickLength = axis()->tickLength( it.type() == TickIterator::MinorTick );
            size = qMax( size, tickLength + tickLabelSpacing + labelSize );
        }
    }

    const TextAttributes titleTA = titleTextAttributesWithAdjustedRotation();
    if ( titleTA.isVisible() && !axis()->titleText().isEmpty() ) {
        TextLayoutItem title( axis()->titleText(), titleTA, refArea, KDChartEnums::MeasureOrientationMinimum,
                              Qt::AlignHCenter | Qt::AlignVCenter );

        QFontMetricsF titleFM( title.realFont(), GlobalMeasureScaling::paintDevice() );
        size += geoXy( titleFM.height() * 0.33, titleFM.averageCharWidth() * 0.55 ); // spacing
        size += geoXy( title.sizeHint().height(), title.sizeHint().width() );
    }

    // the size parallel to the axis direction is not determined by us, so we just return 1
    return QSize( geoXy( 1, int( size ) ), geoXy( int ( size ), 1 ) );
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
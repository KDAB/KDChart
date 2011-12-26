//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartAbstractDiagram_p.h"

#include "KDChartThreeDPieAttributes.h"

#include <QAbstractTextDocumentLayout>
#include <QTextBlock>
#include <QApplication>

#include <KDABLibFakes>


using namespace KDChart;

DataValueTextInfo::DataValueTextInfo()
{
}

DataValueTextInfo::DataValueTextInfo( const QModelIndex& _index, const DataValueAttributes& _attrs,
                                      const QPointF& _pos, const QPointF& _markerPos, qreal _value )
    : index( _index )
    , attrs( _attrs )
    , pos( _pos )
    , markerPos( _markerPos )
    , value( _value )
{
}

DataValueTextInfo::DataValueTextInfo( const DataValueTextInfo& other )
    : index( other.index )
    , attrs( other.attrs )
    , pos( other.pos )
    , markerPos( other.markerPos )
    , value( other.value )
{
}

AbstractDiagram::Private::Private()
  : plane( 0 )
  , attributesModel( new PrivateAttributesModel(0,0) )
  , allowOverlappingDataValueTexts( false )
  , antiAliasing( true )
  , percent( false )
  , datasetDimension( 1 )
  , databoundariesDirty(true)
  , lastX( 0 )
  , mCachedFontMetrics( QFontMetrics( qApp->font() ) )
{
}

AbstractDiagram::Private::~Private()
{
  if( attributesModel && qobject_cast<PrivateAttributesModel*>(attributesModel) )
    delete attributesModel;
}

void AbstractDiagram::Private::init()
{
}

void AbstractDiagram::Private::init( AbstractCoordinatePlane* newPlane )
{
    plane = newPlane;
}

bool AbstractDiagram::Private::usesExternalAttributesModel()const
{
    return ( ! attributesModel.isNull() ) &&
           ( ! qobject_cast<PrivateAttributesModel*>(attributesModel) );
}

void AbstractDiagram::Private::setAttributesModel( AttributesModel* amodel )
{
    if( !attributesModel.isNull() &&
        qobject_cast<PrivateAttributesModel*>(attributesModel) ) {
        delete attributesModel;
    }
    attributesModel = amodel;
}

AbstractDiagram::Private::Private( const AbstractDiagram::Private& rhs ) :
    // Do not copy the plane
    plane( 0 ),
    attributesModelRootIndex( QModelIndex() ),
    attributesModel( rhs.attributesModel ),
    allowOverlappingDataValueTexts( rhs.allowOverlappingDataValueTexts ),
    antiAliasing( rhs.antiAliasing ),
    percent( rhs.percent ),
    datasetDimension( rhs.datasetDimension ),
    mCachedFontMetrics( rhs.cachedFontMetrics() )
{
    attributesModel = new PrivateAttributesModel( 0, 0);
    attributesModel->initFrom( rhs.attributesModel );
}

// FIXME: Optimize if necessary
qreal AbstractDiagram::Private::calcPercentValue( const QModelIndex & index ) const
{
    qreal sum = 0.0;
    for ( int col = 0; col < attributesModel->columnCount( QModelIndex() ); col++ )
        sum += attributesModel->data( attributesModel->index( index.row(), col, QModelIndex() ) ).toDouble(); // checked
    if ( sum == 0.0 )
        return 0.0;
    return attributesModel->data( attributesModel->mapFromSource( index ) ).toDouble() / sum * 100.0;
}

void AbstractDiagram::Private::appendDataValueTextInfoToList(
    AbstractDiagram * diagram,
    DataValueTextInfoList & list,
    const QModelIndex & index,
    const CartesianDiagramDataCompressor::CachePosition * position,
    const PositionPoints& points,
    const Position& autoPositionPositive,
    const Position& autoPositionNegative,
    const qreal value,
    qreal favoriteAngle /* = 0.0 */ )
{
    CartesianDiagramDataCompressor::DataValueAttributesList allAttrs(
        aggregatedAttrs( diagram, index, position ) );

    QMap<QModelIndex, DataValueAttributes>::const_iterator it;
    for (it = allAttrs.constBegin(); it != allAttrs.constEnd(); ++it) {
        DataValueAttributes dva = it.value();
        if ( !dva.isVisible() ) {
            continue;
        }

        const bool isPositive = (value >= 0.0);
        RelativePosition relPos( dva.position( isPositive ) );
        relPos.setReferencePoints( points );
        if ( relPos.referencePosition().isUnknown() ) {
            relPos.setReferencePosition( isPositive ? autoPositionPositive : autoPositionNegative );
        }

        const QPointF referencePoint = relPos.referencePoint();
        if ( !diagram->coordinatePlane()->isVisiblePoint( referencePoint ) ) {
            continue;
        }

        const qreal fontHeight = cachedFontMetrics( dva.textAttributes().
                calculatedFont( plane, KDChartEnums::MeasureOrientationMinimum ), diagram )->height();
        // Note: When printing data value texts the font height is used as reference size for both,
        //       horizontal and vertical padding, if the respective padding's Measure is using
        //       automatic reference area detection.
        QSizeF relativeMeasureSize( fontHeight, fontHeight );
        //qDebug()<<"fontHeight"<<fontHeight;

        if ( !dva.textAttributes().hasRotation() ) {
            TextAttributes ta = dva.textAttributes();
            ta.setRotation( favoriteAngle );
            dva.setTextAttributes( ta );
        }
        // Store the anchor point, that's already shifted according to horiz./vert. padding:
        list.append( DataValueTextInfo( it.key(), dva, relPos.calculatedPoint( relativeMeasureSize ),
                                        referencePoint, value ) );
    }
}

const QFontMetrics * AbstractDiagram::Private::cachedFontMetrics( const QFont& font, QPaintDevice * paintDevice)
{
    if( (font != mCachedFont) || (paintDevice != mCachedPaintDevice) )
        mCachedFontMetrics = QFontMetrics( font, paintDevice );
    return & mCachedFontMetrics;
}

const QFontMetrics AbstractDiagram::Private::cachedFontMetrics() const
{
    return mCachedFontMetrics;
}

QString AbstractDiagram::Private::formatNumber( qreal value, int decimalDigits ) const
{
    QString asString = QString::number( value, 'f' );
    int decimalPos = asString.indexOf( QLatin1Char( '.' ) );
    QString digits( asString.mid( decimalPos + 1, decimalDigits + 1 ) ); // keep one more for rounding

    if ( digits.count() > decimalDigits && digits.at( decimalDigits ) >= QLatin1Char( '5' ) ) {
        const qreal stepUp = pow( 0.1, decimalDigits ) * 0.5;
        value += stepUp;
        asString = QString::number( value, 'f' );
        decimalPos = asString.indexOf( QLatin1Char( '.' ) );
        digits = asString.mid( decimalPos + 1, decimalDigits );
    } else {
        digits.chop( 1 );
    }

    QString ret( asString.left( decimalPos ) );
    if ( decimalDigits > 0 ) {
        ret.append( QLatin1Char( '.' ) + digits );
    }

    return ret;
}

void AbstractDiagram::Private::forgetAlreadyPaintedDataValues()
{
    alreadyDrawnDataValueTexts.clear();
    prevPaintedDataValueText.clear();
}

void AbstractDiagram::Private::paintDataValueTextsAndMarkers(
    AbstractDiagram* diag,
    PaintContext* ctx,
    const DataValueTextInfoList & list,
    bool paintMarkers,
    bool justCalculateRect /* = false */,
    QRectF* cumulatedBoundingRect /* = 0 */ )
{
    if ( justCalculateRect && !cumulatedBoundingRect ) {
        qWarning() << Q_FUNC_INFO << "Neither painting nor finding the bounding rect, what are we doing?";
    }

    const PainterSaver painterSaver( ctx->painter() );
    ctx->painter()->setClipping( false );

    if ( paintMarkers && !justCalculateRect ) {
        KDAB_FOREACH ( const DataValueTextInfo& info, list ) {
            diag->paintMarker( ctx->painter(), info.index, info.markerPos );
        }
    }

    Measure m( 18.0, KDChartEnums::MeasureCalculationModeRelative,
               KDChartEnums::MeasureOrientationMinimum );
    m.setReferenceArea( ctx->coordinatePlane() );
    TextAttributes ta;
    ta.setFontSize( m );
    m.setAbsoluteValue( 6.0 );
    ta.setMinimalFontSize( m );

    forgetAlreadyPaintedDataValues();

    KDAB_FOREACH ( const DataValueTextInfo& info, list ) {
        const PainterSaver ps( ctx->painter() );

        if ( !diag->dataValueAttributes( info.index ).textAttributes().hasRotation() ) {
            ctx->painter()->translate( info.pos );
            ctx->painter()->rotate( info.attrs.textAttributes().rotation() );
            ctx->painter()->translate( -info.pos );
        }

        paintDataValueText( diag, ctx->painter(), info.index, info.pos, info.value,
                            justCalculateRect,
                            cumulatedBoundingRect );

        const QString comment = info.index.data( KDChart::CommentRole ).toString();
        if ( comment.isEmpty() ) {
            continue;
        }
        TextBubbleLayoutItem item( comment, ta, ctx->coordinatePlane()->parent(),
                                   KDChartEnums::MeasureOrientationMinimum,
                                   Qt::AlignHCenter | Qt::AlignVCenter );
        const QRect rect( info.pos.toPoint(), item.sizeHint() );

        if (cumulatedBoundingRect) {
            (*cumulatedBoundingRect) |= rect;
        }
        if ( !justCalculateRect ) {
            item.setGeometry( rect );
            item.paint( ctx->painter() );
        }
    }
}

QString AbstractDiagram::Private::formatDataValueText( const DataValueAttributes &dva,
                                                       const QModelIndex& index, qreal value ) const
{
    if ( !dva.isVisible() ) {
        return QString();
    }
    if ( dva.usePercentage() ) {
        value = calcPercentValue( index );
    }

    QString ret;
    if ( dva.dataLabel().isNull() ) {
        ret = formatNumber( value, dva.decimalDigits() );
    } else {
        ret = dva.dataLabel();
    }

    ret.prepend( dva.prefix() );
    ret.append( dva.suffix() );

    return ret;
}

void AbstractDiagram::Private::paintDataValueText( const AbstractDiagram* diag,
    QPainter* painter,
    const QModelIndex& index,
    const QPointF& pos,
    qreal value,
    bool justCalculateRect /* = false */,
    QRectF* cumulatedBoundingRect /* = 0 */ )
{
    const DataValueAttributes dva( diag->dataValueAttributes( index ) );
    QString text = formatDataValueText( dva, index, value );
    paintDataValueText( diag, painter, dva, pos, text, value >= 0.0,
                        justCalculateRect, cumulatedBoundingRect );
}

void AbstractDiagram::Private::paintDataValueText( const AbstractDiagram* diag,
    QPainter* painter,
    const DataValueAttributes& attrs,
    const QPointF& pos,
    QString text,
    bool valueIsPositive,
    bool justCalculateRect /* = false */,
    QRectF* cumulatedBoundingRect /* = 0 */ )
{
    if ( !attrs.isVisible() ) return;

    const TextAttributes ta( attrs.textAttributes() );
    if ( !ta.isVisible() ) {
        return;
    }
    /* for debugging:
    PainterSaver painterSaver( painter );
    painter->setPen( Qt::black );
    painter->drawLine( pos - QPointF( 2,2), pos + QPointF( 2,2) );
    painter->drawLine( pos - QPointF(-2,2), pos + QPointF(-2,2) );
    */

    QTextDocument doc;
    if ( Qt::mightBeRichText( text ) ) {
        doc.setHtml( text );
    } else {
        doc.setPlainText( text );
    }

    const RelativePosition relPos( attrs.position( valueIsPositive ) );
    const QFont calculatedFont( ta.calculatedFont( plane, KDChartEnums::MeasureOrientationMinimum ) );

    // what's up with lastX anyway?
    if ( !attrs.showRepetitiveDataLabels() && prevPaintedDataValueText == text && pos.x() > lastX ) {
        return;
    }
    prevPaintedDataValueText = text;

    lastX = pos.x(); // OMG WTF BBQ, this assumes left to right painting on a cartesian axis, right???
    const PainterSaver painterSaver( painter );
    painter->setPen( PrintingParameters::scalePen( ta.pen() ) );

    doc.setDefaultFont( calculatedFont );
    QAbstractTextDocumentLayout::PaintContext context;
    context.palette = diag->palette();
    context.palette.setColor(QPalette::Text, ta.pen().color() );

    // set text background
    BackgroundAttributes back(attrs.backgroundAttributes());
    if ( back.isVisible() ) {
        QTextBlockFormat fmt;
        fmt.setBackground( back.brush() );
        QTextCursor cursor( &doc );
        cursor.setPosition( 0 );
        cursor.movePosition( QTextCursor::End, QTextCursor::KeepAnchor, 1 );
        cursor.mergeBlockFormat( fmt );
    }

    QAbstractTextDocumentLayout* const layout = doc.documentLayout();
    layout->setPaintDevice( painter->device() );
    const QRectF plainRect = layout->frameBoundingRect( doc.rootFrame() );
    const QSizeF plainSize = layout->frameBoundingRect( doc.rootFrame() ).size();
    painter->translate( pos );

    /**
     * A few hints on how the positioning of the text frame is done:
     *
     * Let's assume we have a bar chart, a text for a positive value
     * to be drawn, and "North" as attrs.positivePosition().
     *
     * The reference point (pos) is then set to the top center point
     * of a bar. The offset now depends on the alignment:
     *
     *    Top: text is centered horizontally to the bar, bottom of
     *         text frame starts at top of bar
     *
     *    Bottom: text is centered horizontally to the bar, top of
     *            text frame starts at top of bar
     *
     *    Center: text is centered horizontally to the bar, center
     *            line of text frame is same as top of bar
     *
     *    TopLeft: right edge of text frame is horizontal center of
     *             bar, bottom of text frame is top of bar.
     *
     *    ...
     *
     * Positive and negative value labels are treated equally, "North"
     * also refers to the top of a negative bar, and *not* to the bottom.
     *
     *
     * "NorthEast" likewise refers to the top right edge of the bar,
     * "NorthWest" to the top left edge of the bar, and so on.
     *
     * In other words, attrs.positivePosition() always refers to a
     * position of the *bar*, and relPos.alignment() always refers
     * to an alignment of the text frame relative to this position.
     */

    int rotation = ta.rotation();
    if ( !valueIsPositive && attrs.mirrorNegativeValueTextRotation() ) {
        rotation *= -1;
    }
    qreal dx = - 0.5 * plainSize.width();
    qreal dy = - 0.5 * plainSize.height();

    if ( relPos.alignment() & Qt::AlignLeft ) {
        dx -= 0.5 * plainSize.width();
    } else if ( relPos.alignment() & Qt::AlignRight ) {
        dx += 0.5 * plainSize.width();
    }

    if ( relPos.alignment() & Qt::AlignTop ) {
        dy -= 0.5 * plainSize.height();
    } else if( relPos.alignment() & Qt::AlignBottom ) {
        dy += 0.5 * plainSize.height();
    }

    bool drawIt = true;
    // note: This flag can be set differently for every label text!
    // In theory a user could e.g. have some small red text on one of the
    // values that she wants to have written in any case - so we just
    // do not test if such texts would cover some of the others.
    if ( ! attrs.showOverlappingDataLabels() ) {
        const QRectF br( layout->blockBoundingRect( doc.begin() ) );
        qreal radRot = DEGTORAD( - ( ( ta.rotation() < 0 ) ? ta.rotation() + 360 : ta.rotation() ) );
        // qDebug() << Q_FUNC_INFO << radRot;
        qreal cosRot = cos( radRot );
        qreal sinRot = sin( radRot );
        QPolygon pr( br.toRect(), true );
        // YES, people, the following stuff NEEDS to be done that way!
        // Otherwise we will not get the texts' individual rotation
        // and/or the shifting of the texts correctly.
        // Just believe me - I did tests ..   :-)    (khz, 2008-02-19)
        for ( int i = 0; i < pr.count(); i++ ) {
            const QPoint p( pr.point( i ) );
            const qreal x = p.x() + dx;
            const qreal y = p.y() + dy;
            pr.setPoint( i, int( pos.x() + x * cosRot + y * sinRot ),
                            int( pos.y() - x * sinRot + y * cosRot ) );
        }
        // Using QPainterPath allows us to use intersects() (which has many early-exits)
        // instead of QPolygon::intersected (which calculates a slow and precise intersection polygon)
        QPainterPath path;
        path.addPolygon( pr );
        // qDebug() << "Comparing new poly" << br << "(rotated" << radRot << ") with"
        //          << alreadyDrawnDataValueTexts.count() << "already drawn data value texts";
        KDAB_FOREACH( const QPainterPath& oldPoly, alreadyDrawnDataValueTexts ) {
            if ( oldPoly.intersects( path ) ) {
                // qDebug() << "not painting this label due to overlap";
                drawIt = false;
                break;
            }
        }
        if( drawIt ) {
            alreadyDrawnDataValueTexts << path;
        }
    }
    if ( drawIt ) {
        QRectF rect = layout->frameBoundingRect(doc.rootFrame());
        rect.moveTo(pos.x() + dx, pos.y() + dy);

        if ( cumulatedBoundingRect ) {
            (*cumulatedBoundingRect) |= rect;
        }
        if ( !justCalculateRect ) {
            painter->translate( QPointF( dx, dy ) );
            painter->translate( plainRect.center() );
            painter->rotate( rotation );
            painter->translate( - plainRect.center() );
            layout->draw( painter, context );
        }
    }
}

QModelIndex AbstractDiagram::Private::indexAt( const QPoint& point ) const
{
    QModelIndexList l = indexesAt( point );
    qSort( l );
    if ( !l.isEmpty() )
        return l.first();
    else
        return QModelIndex();
}

QModelIndexList AbstractDiagram::Private::indexesAt( const QPoint& point ) const
{
    return reverseMapper.indexesAt( point ); // which could be empty
}

QModelIndexList AbstractDiagram::Private::indexesIn( const QRect& rect ) const
{
    return reverseMapper.indexesIn( rect );
}

CartesianDiagramDataCompressor::DataValueAttributesList AbstractDiagram::Private::aggregatedAttrs(
    AbstractDiagram * diagram,
    const QModelIndex & index,
    const CartesianDiagramDataCompressor::CachePosition * position ) const
{
    Q_UNUSED( position ); // used by cartesian diagrams only
    CartesianDiagramDataCompressor::DataValueAttributesList allAttrs;
    allAttrs[index] = diagram->dataValueAttributes( index );
    return allAttrs;
}

void AbstractDiagram::Private::setDatasetAttrs( int dataset, QVariant data, int role )
{
    // To store attributes for a dataset, we use the first column
    // that's associated with it. (i.e., with a dataset dimension
    // of two, the column of the keys). In most cases however, there's
    // only one data dimension, and thus also only one column per data set.
    int column = dataset * datasetDimension;
    for ( int i = 0; i < datasetDimension; i++ ) {
        attributesModel->setHeaderData( column + i, Qt::Horizontal, data, role );
        // For DataHiddenRole, also store the flag in the other data points
        // that belong to this data set, otherwise it's impossible to hide
        // data points in a plotter diagram because there will always be
        // one model index that belongs to this data point that is not hidden.
        // For more details on how hiding works, see the data compressor.
        // Also see KDCH-503 for which this is a workaround.
        if ( role != DataHiddenRole )
            break;
    }
}

QVariant AbstractDiagram::Private::datasetAttrs( int dataset, int role ) const
{
    // See setDataSetAttrs for explanation of column
    int column = dataset * datasetDimension;
    return attributesModel->headerData( column, Qt::Horizontal, role );
}

void AbstractDiagram::Private::resetDatasetAttrs( int dataset, int role )
{
    // See setDataSetAttrs for explanation of column
    int column = dataset * datasetDimension;
    attributesModel->resetHeaderData( column, Qt::Horizontal, role );
}


LineAttributesInfo::LineAttributesInfo()
{
}

LineAttributesInfo::LineAttributesInfo( const QModelIndex _index, const QPointF& _value, const QPointF& _nextValue )
    : index( _index )
    , value ( _value )
    , nextValue ( _nextValue )
{
}

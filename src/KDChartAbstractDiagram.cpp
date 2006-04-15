

#include <QPainter>
#include <QDebug>
#include <QAbstractProxyModel>
#include <QStandardItemModel>
#include <QSizeF>

#include "KDChartAbstractCoordinatePlane.h"
#include "KDChartDataValueAttributes.h"
#include "KDChartTextAttributes.h"
#include "KDChartMarkerAttributes.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractDiagram_p.h"
#include "KDChartDefaultsModel.h"
#include "KDChartAttributesModel.h"
#include "KDChartDatasetProxyModel.h"
#include "KDChartThreeDAttributes.h"
#include "KDChartThreeDLineAttributes.h"
#include "KDChartPainterSaver_p.h"

using namespace KDChart;

AbstractDiagram::Private::Private()
  : plane( 0 )
  , defaultsModel( new DefaultsModel() )
  , attributesModel( 0 )
  , datasetProxy ( 0 )
  , allowOverlappingDataValueTexts( false )
  , usePrivateAttributesModel( false )
  , xAxisResolution( 0.0 )
  , yAxisResolution( 0.0 )
  , percent( false )
{
}

AbstractDiagram::Private::~Private()
{
  delete defaultsModel;
}

void AbstractDiagram::Private::init()
{
}

void AbstractDiagram::Private::init( AbstractCoordinatePlane* newPlane )
{
  plane = newPlane;
}

#define d d_func()

AbstractDiagram::AbstractDiagram ( AbstractCoordinatePlane* parent )
    : QAbstractItemView ( parent ), _d( new Private() )
{
    _d->init( parent );
}

AbstractDiagram::~AbstractDiagram()
{
    delete _d;
}

void AbstractDiagram::init()
{
}


AbstractCoordinatePlane* AbstractDiagram::coordinatePlane() const
{
    return d->plane;
}

const QPair<QPointF, QPointF> AbstractDiagram::dataBoundaries () const
{
    if ( !checkInvariants() ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );
    double xMin = 0, xMax = 0, yMin = 0, yMax = 0;
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    xMax = rowCount-1; // since we start at 0
    for ( int i=0; i<colCount; ++i ) {
        for ( int j=0; j< rowCount; ++j ) {
            double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
            yMin = qMin( yMin, value );
            yMax = qMax( yMax, value );
        }
    }

    QPointF bottomLeft ( QPointF( xMin, yMin ) );
    QPointF topRight ( QPointF( xMax, yMax ) );
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}

void AbstractDiagram::setModel ( QAbstractItemModel * newModel )
{
    if ( d->datasetProxy )
    {
        if ( newModel != d->datasetProxy->sourceModel() )
        {
            d->datasetProxy->setSourceModel( newModel );
        }
    } else {
        QAbstractItemModel *oldSourceModel =
            d->attributesModel ? d->attributesModel->sourceModel() : 0;
        if ( !newModel || newModel == oldSourceModel ) return;

        // see usePrivateAttributesModel(bool) for details
        if( d->usePrivateAttributesModel ) {
            if ( !d->attributesModel )
                d->attributesModel = AttributesModel::instanceForModel( d->defaultsModel );
        } else {
            d->attributesModel = AttributesModel::instanceForModel( newModel );
            AttributesModel::deref( oldSourceModel );
        }
        d->defaultsModel->setSourceModel( d->attributesModel );
        d->attributesModel->setSourceModel( newModel );
        QAbstractItemView::setModel( d->defaultsModel );

        if ( oldSourceModel ) {
            disconnect( oldSourceModel, SIGNAL( modelReset() ),
                        this, SLOT( slotModelReset() ) );
            disconnect( oldSourceModel, SIGNAL( layoutChanged() ),
                        this, SLOT( slotModelReset() ) );
            disconnect( oldSourceModel, SIGNAL( dataChanged ( const QModelIndex &, const QModelIndex &) ),
                        this, SLOT( slotModelReset() ) );
        }
        connect( newModel, SIGNAL( modelReset() ), this, SLOT( slotModelReset() ) );
        connect( newModel, SIGNAL( layoutChanged() ), this, SLOT( slotModelReset() ) );
        connect( newModel, SIGNAL( dataChanged ( const QModelIndex &, const QModelIndex &) ),
                 this, SLOT( slotModelReset() ) );
    }
}

void AbstractDiagram::setRootIndex ( const QModelIndex& idx )
{
  if( d->datasetProxy ) {
    QAbstractItemView::setRootIndex( d->datasetProxy->mapFromSource(idx) );
    d->datasetProxy->setSourceRootIndex(idx);
  } else {    
    QAbstractItemView::setRootIndex( d->defaultsModel->mapFromSource(d->attributesModel->mapFromSource(idx)) );
  }
  if( d->plane ) slotModelReset();
}

void AbstractDiagram::setCoordinatePlane( AbstractCoordinatePlane* parent )
{
    d->plane = parent;
}

void AbstractDiagram::slotModelReset()
{
    d->plane->layoutDiagrams();
    d->plane->update();
}

void AbstractDiagram::setDataValueAttributes( const QModelIndex & index,
                                              const DataValueAttributes & a )
{
    model()->setData( index, QVariant::fromValue( a ), DataValueLabelAttributesRole );
}

DataValueAttributes AbstractDiagram::dataValueAttributes( int column ) const
{
    // FIXME throw exception if no model
     return model()->headerData( column, Qt::Vertical, DataValueLabelAttributesRole ).value<DataValueAttributes>();
}

void AbstractDiagram::setDataValueAttributes( int column, const DataValueAttributes & a )
{

    model()->setHeaderData( column, Qt::Vertical, QVariant::fromValue( a ), DataValueLabelAttributesRole );
}

DataValueAttributes AbstractDiagram::dataValueAttributes( const QModelIndex & index ) const
{
    return model()->data( index, KDChart::DataValueLabelAttributesRole ).value<DataValueAttributes>();
}

void AbstractDiagram::setAllDataValueAttributes( const DataValueAttributes & a )
{
    d->attributesModel->setModelData( QVariant::fromValue( a ), DataValueLabelAttributesRole );
}

DataValueAttributes AbstractDiagram::allDataValueAttributes() const
{
    return d->attributesModel->modelData( DataValueLabelAttributesRole ).value<DataValueAttributes>();
}

void AbstractDiagram::setAllowOverlappingDataValueTexts( bool allow )
{
    d->allowOverlappingDataValueTexts = allow;
}

bool AbstractDiagram::allowOverlappingDataValueTexts() const
{
    return d->allowOverlappingDataValueTexts;
}

void AbstractDiagram::setOrdinateResolution( double resolution )
{
    d->yAxisResolution = resolution;
}

double AbstractDiagram::ordinateResolution() const
{
    return d->yAxisResolution;
}

void AbstractDiagram::setAbscissaResolution( double resolution )
{
    d->xAxisResolution = resolution;
}

double AbstractDiagram::abscissaResolution() const
{
    return d->xAxisResolution;
}

void AbstractDiagram::setPercentMode ( bool percent )
{
    d->percent = percent;
}

bool AbstractDiagram::percentMode() const
{
    return d->percent;
}

DatasetProxyModel* AbstractDiagram::datasetProxy()
{
    if ( d->datasetProxy == 0 )
    {
        QStandardItemModel temp;
        DatasetProxyModel* proxy = new DatasetProxyModel ( this );

        QAbstractItemModel *oldSourceModel =
            d->attributesModel ? d->attributesModel->sourceModel() : 0;
        if ( oldSourceModel )
        {
            setModel ( &temp );
            proxy->setSourceModel ( oldSourceModel );
        }

        setModel ( proxy );
        d->datasetProxy = proxy;
    }

    return d->datasetProxy;
}

void AbstractDiagram::paintDataValueText( QPainter* painter,
                                          const QModelIndex& index,
                                          const QPointF& pos,
                                          double value )
{
    // paint one data series
    DataValueAttributes a =
        model()->data( index, DataValueLabelAttributesRole ).value<DataValueAttributes>();
    if ( !a.isVisible() ) return;
    PainterSaver painterSaver( painter );
    // FIXME draw the non-text bits, background, etc
    const TextAttributes &ta = a.textAttributes();
    if ( ta.isVisible() ) {
        QPen pen = painter->pen();
        pen.setColor( ta.color() );
        painter->setPen( pen );
        painter->setFont( ta.font() );
        painter->translate( pos );
        painter->rotate( ta.rotation() );
        painter->drawText( QPointF(0, 0), QString::number( value ) );
    }
}

void AbstractDiagram::paintDataValueTexts( QPainter* painter )
{
    if ( !checkInvariants() ) return;
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    for ( int i=0; i<colCount; ++i ) {
       for ( int j=0; j< rowCount; ++j ) {
           const QModelIndex index = model()->index( j, i, rootIndex() );
           double value = model()->data( index ).toDouble();
           const QPointF pos = coordinatePlane()->translate( QPointF( j, value ) );
           paintDataValueText( painter, index, pos, value );
       }
    }
}




void AbstractDiagram::paintMarker( QPainter* painter,
                                   const QModelIndex& index,
                                   const QPointF& pos )
{
    if ( !checkInvariants() ) return;
    DataValueAttributes a =
        model()->data( index, DataValueLabelAttributesRole ).value<DataValueAttributes>();
    if ( !a.isVisible() ) return;
    const MarkerAttributes &ma = a.markerAttributes();
    if ( !ma.isVisible() ) return;

    PainterSaver painterSaver( painter );
    QSizeF maSize( ma.markerSize() );
    QBrush indexBrush( brush( index ) );
    QPen indexPen( pen( index ) );
    if ( ma.markerColor().isValid() ) {
        indexBrush.setColor( ma.markerColor() );
        indexPen.setColor( ma.markerColor() );
    }

    paintMarker( painter, ma, indexBrush, indexPen, pos, maSize );
}


void AbstractDiagram::paintMarker( QPainter* painter,
                                   const MarkerAttributes& markerAttributes,
                                   const QBrush& brush, const QPen& pen,
                                   const QPointF& pos, const QSizeF& maSize )
{
    PainterSaver painterSaver( painter );
    painter->setBrush( brush );
    painter->setPen( pen );
    painter->setRenderHint ( QPainter::Antialiasing );
    painter->translate( pos );
    switch ( markerAttributes.markerStyle() ) {
        case MarkerAttributes::MarkerCircle:
            painter->drawEllipse( QRectF( 0 - maSize.height()/2, 0 - maSize.width()/2,
                        maSize.height(), maSize.width()) );
            break;
    case MarkerAttributes::MarkerSquare:
    {
        QRectF rect( 0 - maSize.height()/2, 0 - maSize.width()/2,
                     maSize.height(), maSize.width() );
        painter->fillRect( rect, painter->brush() );
        break;
    }
        case MarkerAttributes::MarkerDiamond:
            {
                QVector <QPointF > diamondPoints;
                QPointF top, left, bottom, right;
                top    = QPointF( 0, 0 - maSize.height()/2 );
                left   = QPointF( 0 - maSize.width()/2, 0 );
                bottom = QPointF( 0, maSize.height()/2 );
                right  = QPointF( maSize.width()/2, 0 );
                diamondPoints << top << left << bottom << right;
                painter->drawPolygon( diamondPoints );
                break;
            }
        //Pending Michel: do we need that? Ask: What is the idea about
        // Marker1Pixel and Marker4Pixels.
        case MarkerAttributes::Marker1Pixel:
            {
                QSizeF pSize(4,4);
                QPen pen;
                pen.setColor( painter->background().color() );
                QRectF centerRect( -pSize.height()/2, -pSize.width()/2,
                                   pSize.height(), pSize.width() );
                painter->setPen( pen );
                painter->drawEllipse( centerRect );
                break;
            }
        case MarkerAttributes::Marker4Pixels:
            {
                QSizeF pSize(8, 8);
                QPen pen;
                pen.setColor( painter->background().color() );
                QRectF centerRect( -pSize.height()/2, -pSize.width()/2,
                                   pSize.height(), pSize.width() );
                painter->setPen( pen );
                painter->drawEllipse( centerRect );
                break;
            }
        case MarkerAttributes::MarkerRing:
            {
                painter->setBrush( Qt::NoBrush );
                painter->drawEllipse( QRectF( 0 - maSize.height()/2, 0 - maSize.width()/2,
                                      maSize.height(), maSize.width()) );
                break;
            }
        case MarkerAttributes::MarkerCross:
            {
                QVector <QPointF > crossPoints;
                QPointF leftTop, leftBottom, centerBottomLeft, bottomLeft, bottomRight,
                        centerBottomRight,rightBottom, rightTop, centerTopRight, topRight, topLeft,
                        centerTopLeft;
                leftTop           = QPointF( -maSize.width()/2, -maSize.height()/4 );
                leftBottom        = QPointF( -maSize.width()/2, maSize.height()/4 );
                centerBottomLeft  = QPointF( -maSize.width()/4, maSize.height()/4 );
                bottomLeft        = QPointF( -maSize.width()/4, maSize.height()/2 );
                bottomRight       = QPointF( maSize.width()/4,  maSize.height()/2 );
                centerBottomRight = QPointF( maSize.width()/4, maSize.height()/4 );
                rightBottom       = QPointF( maSize.width()/2, maSize.height()/4 );
                rightTop          = QPointF( maSize.width()/2, -maSize.height()/4 );
                centerTopRight    = QPointF( maSize.width()/4, -maSize.height()/4 );
                topRight          = QPointF( maSize.width()/4, -maSize.height()/2 );
                topLeft           = QPointF( -maSize.width()/4, -maSize.height()/2 );
                centerTopLeft     = QPointF( -maSize.width()/4, -maSize.height()/4 );

                crossPoints << leftTop << leftBottom << centerBottomLeft
                    << bottomLeft << bottomRight << centerBottomRight
                    << rightBottom << rightTop << centerTopRight
                    << topRight << topLeft << centerTopLeft;

                painter->drawPolygon( crossPoints );
                break;
            }
        case MarkerAttributes::MarkerFastCross:
            {
                QPointF left, right, top, bottom;
                left  = QPointF( -maSize.width()/2, 0 );
                right = QPointF( maSize.width()/2, 0 );
                top   = QPointF( 0, -maSize.height()/2 );
                bottom= QPointF( 0, maSize.height()/2 );
                painter->drawLine( left, right );
                painter->drawLine(  top, bottom );
                break;
            }
        default:
            Q_ASSERT_X ( false, "paintMarkers()",
                         "Type item does not match a defined Marker Type." );
    }
}

void AbstractDiagram::paintMarkers( QPainter* painter )
{
    if ( !checkInvariants() ) return;
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    for ( int i=0; i<colCount; ++i ) {
       for ( int j=0; j< rowCount; ++j ) {
           const QModelIndex index = model()->index( j, i, rootIndex() );
           double value = model()->data( index ).toDouble();
           const QPointF pos = coordinatePlane()->translate( QPointF( j, value ) );
           paintMarker( painter, index, pos );
       }
    }
}


void AbstractDiagram::setPen( const QModelIndex& index, const QPen& pen )
{
     model()->setData( index, QVariant::fromValue( pen ), DatasetPenRole );
}

void AbstractDiagram::setPen( const QPen& pen )
{
    d->attributesModel->setModelData( QVariant::fromValue( pen ), DatasetPenRole );
}

void AbstractDiagram::setPen( int column,const QPen& pen )
{
    model()->setHeaderData( column, Qt::Vertical, QVariant::fromValue( pen ), DatasetPenRole );
}

QPen AbstractDiagram::pen( const QModelIndex& index ) const
{
  return model()->data( index, DatasetPenRole ).value<QPen>();
}

void AbstractDiagram::setBrush( const QModelIndex& index, const QBrush& brush )
{
     model()->setData( index, QVariant::fromValue( brush ), DatasetBrushRole );
}

void AbstractDiagram::setBrush( const QBrush& brush )
{
  d->attributesModel->setModelData( QVariant::fromValue( brush ), DatasetBrushRole );
}

void AbstractDiagram::setBrush( int column, const QBrush& brush )
{
  model()->setHeaderData( column, Qt::Vertical, QVariant::fromValue( brush ), DatasetBrushRole );
}

QBrush AbstractDiagram::brush( const QModelIndex& index ) const
{
  return model()->data( index, DatasetBrushRole ).value<QBrush>();
}

// implement QAbstractItemView:
QRect AbstractDiagram::visualRect(const QModelIndex &) const
{ return QRect(); }

void AbstractDiagram::scrollTo(const QModelIndex &, ScrollHint )
{}

QModelIndex AbstractDiagram::indexAt(const QPoint &) const
{ return QModelIndex(); }

QModelIndex AbstractDiagram::moveCursor(CursorAction, Qt::KeyboardModifiers )
{ return QModelIndex(); }

int AbstractDiagram::horizontalOffset() const
{ return 0; }

int AbstractDiagram::verticalOffset() const
{ return 0; }

bool AbstractDiagram::isIndexHidden(const QModelIndex &) const
{ return true; }

void AbstractDiagram::setSelection(const QRect &, QItemSelectionModel::SelectionFlags)
{}

QRegion AbstractDiagram::visualRegionForSelection(const QItemSelection &) const
{ return QRegion(); }


void KDChart::AbstractDiagram::useDefaultColors( )
{
    d->defaultsModel->setDefaultPaletteType( DefaultsModel::PaletteTypeDefault );
}

void KDChart::AbstractDiagram::useSubduedColors( )
{
    d->defaultsModel->setDefaultPaletteType( DefaultsModel::PaletteTypeSubdued );
}

void KDChart::AbstractDiagram::useRainbowColors( )
{
    d->defaultsModel->setDefaultPaletteType( DefaultsModel::PaletteTypeRainbow );
}

void AbstractDiagram::usePrivateAttributes( bool privateAttributes )
{
    // If we already have on set, adjust, otherwise setModel must be called
    // eventually and will take care of things. The defaults model is used
    // as reference model in place of the real source model, since we know
    // there is exactly one per diagram.
    if( d->attributesModel ) {
        QAbstractItemModel *sourceModel = d->attributesModel->sourceModel();
        if( d->usePrivateAttributesModel && !privateAttributes ) {
            // we no longer use a private one, share the one of our source
            AttributesModel::deref( d->defaultsModel );
            d->attributesModel = AttributesModel::instanceForModel( sourceModel );
        } else if ( !d->usePrivateAttributesModel && privateAttributes ) {
            AttributesModel::deref( sourceModel );
            d->attributesModel = AttributesModel::instanceForModel( d->defaultsModel );
        }
        d->attributesModel->setSourceModel( sourceModel );
        d->defaultsModel->setSourceModel( d->attributesModel );
    }
    d->usePrivateAttributesModel = privateAttributes;
}



QStringList AbstractDiagram::datasetLabels() const
{
    QStringList ret;
//    qDebug() << "AbstractDiagram::datasetLabels(): " << model()->columnCount() << "entries";
    for( int i = 0; i < model()->columnCount(); i++ )
        ret << model()->headerData( i, Qt::Vertical, Qt::DisplayRole ).toString();

    return ret;
}


QList<QBrush> AbstractDiagram::datasetBrushes() const
{
    QList<QBrush> ret;
//    qDebug() << "AbstractDiagram::datasetBrushes(): " << model()->columnCount() << "entries";
    for( int i = 0; i < model()->columnCount(); i++ ) {
        QBrush brush = model()->headerData( i, Qt::Vertical, DatasetBrushRole ).value<QBrush>();
//        qDebug() << "Brush = " << brush;
        ret << brush;
    }

    return ret;
}

QList<QPen> AbstractDiagram::datasetPens() const
{
    QList<QPen> ret;
    for( int i = 0; i < model()->columnCount(); i++ ) {
        QPen pen = model()->headerData( i, Qt::Vertical, DatasetPenRole ).value<QPen>();
        ret << pen;
    }
    return ret;
}

QList<MarkerAttributes> AbstractDiagram::datasetMarkers() const
{
    QList<MarkerAttributes> ret;
    for( int i = 0; i < model()->columnCount(); i++ ) {
        DataValueAttributes a =
            model()->headerData( i, Qt::Vertical, DataValueLabelAttributesRole ).value<DataValueAttributes>();
        const MarkerAttributes &ma = a.markerAttributes();
        ret << ma;
    }
    return ret;
}




bool AbstractDiagram::checkInvariants() const
{
    Q_ASSERT_X ( model(), "AbstractDiagram::checkInvariants()",
                 "There is no usable model set, for the diagram." );

    Q_ASSERT_X ( coordinatePlane(), "AbstractDiagram::checkInvariants()",
                 "There is no usable coordinate plane set, for the diagram." );

    return model() && coordinatePlane();
}

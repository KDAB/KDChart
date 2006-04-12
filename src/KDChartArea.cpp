#include <KDChartArea.h>
#include "KDChartArea_p.h"
#include <KDChartBackgroundAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartTextAttributes.h>
#include "KDChartPainterSaver_p.h"
#include <QtXml/QDomDocumentFragment>
#include <QPainter>

using namespace KDChart;

KDChartArea::Private::Private() :
    visible( true ),
    // PENDING(khz) dockingPointToPadding?, alignToDockingPoint?
    text(),
    textAttributes(),
    frameAttributes(),
    backgroundAttributes()
{
}


KDChartArea::Private::~Private() {}


void KDChartArea::Private::init()
{
}




KDChartArea::KDChartArea( QWidget* parent ) :
    QWidget( parent ), _d( new Private() )
{
}

KDChartArea::~KDChartArea()
{
    delete _d; _d = 0;
}


void KDChartArea::init()
{
}


#define d d_func()

QDomDocumentFragment KDChartArea::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment KDChartArea::toXML() const" );
    return QDomDocumentFragment();
}

#ifdef TEMPORARILY_REMOVED
void KDChartArea::paint( QPainter* painter, const PaintContext & context ) const
{
    Q_UNUSED( context );
    paintBackground( painter, d->backgroundAttributes, geometry() );

    // PENDING(kalle) Paint frame
}
#endif

void KDChartArea::alignToDockingPoint( const QPointF& dockingPoint,
                                       DockingPointType dockingPointType,
                                       Qt::Alignment alignmentFlags)
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void KDChartArea::alignToDockingPoint( const QPointF& dockingPoint,DockingPointType dockingPointType,Qt::Alignment alignmentFlags)" );
}

void KDChartArea::setDockingPointPadding( int horizontalPadding,
                                          int verticalPadding,
                                          bool fixed )
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void KDChartArea::setDockingPointPadding( int horizontalPadding,int verticalPadding,bool fixed )" );
}

void KDChartArea::setText( const QString& text )
{
    d->text = text;
}

QString KDChartArea::text() const
{
    return d->text;
}

void KDChartArea::setTextAttributes( const TextAttributes &a )
{
    d->textAttributes = a;
}

TextAttributes KDChartArea::textAttributes() const
{
    return d->textAttributes;
}

void KDChartArea::setFrameAttributes( const FrameAttributes &a )
{
    d->frameAttributes = a;
}

FrameAttributes KDChartArea::frameAttributes() const
{
    return d->frameAttributes;
}

void KDChartArea::setBackgroundAttributes( const BackgroundAttributes &a )
{
    d->backgroundAttributes = a;
}

BackgroundAttributes KDChartArea::backgroundAttributes() const
{
    return d->backgroundAttributes;
}

void KDChartArea:: paintBackground( QPainter* painter,
                                    BackgroundAttributes attributes,
                                    const QRectF& rect )
{
    if( !attributes.isVisible() )
        return;

    /* first draw the brush (may contain a pixmap)*/
    if( Qt::NoBrush != attributes.brush().style() ) {
        KDChart::PainterSaver painterSaver( painter );
        painter->setPen( Qt::NoPen );
        const QPointF newTopLeft( painter->deviceMatrix().map( rect.topLeft() ) );
        painter->setBrushOrigin( newTopLeft );
        painter->setBrush( attributes.brush() );
        painter->drawRect( rect );
    }
    /* next draw the backPixmap over the brush */
    if( !attributes.pixmap().isNull() &&
        attributes.pixmapMode() != BackgroundAttributes::BackgroundPixmapModeNone ) {
        QPointF ol = rect.topLeft();
        if( BackgroundAttributes::BackgroundPixmapModeCentered == attributes.pixmapMode() )
        {
            ol.setX( rect.center().x() - attributes.pixmap().width() / 2 );
            ol.setY( rect.center().y() - attributes.pixmap().height()/ 2 );
            painter->drawPixmap( ol, attributes.pixmap() );
        } else {
            QMatrix m;
            double zW = (double)rect.width()  / (double)attributes.pixmap().width();
            double zH = (double)rect.height() / (double)attributes.pixmap().height();
            switch( attributes.pixmapMode() ) {
            case BackgroundAttributes::BackgroundPixmapModeScaled:
            {
                double z;
                z = qMin( zW, zH );
                m.scale( z, z );
            }
            break;
            case BackgroundAttributes::BackgroundPixmapModeStretched:
                m.scale( zW, zH );
                break;
            default:
                ; // Cannot happen, previously checked
            }
            QPixmap pm = attributes.pixmap().transformed( m );
            ol.setX( rect.center().x() - pm.width() / 2 );
            ol.setY( rect.center().y() - pm.height()/ 2 );
            painter->drawPixmap( ol, pm );
        }
    }
}



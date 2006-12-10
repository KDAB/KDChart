#include <cmath>

#include <QtDebug>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include "KDChartTextLabelCache.h"

PrerenderedElement::PrerenderedElement()
    : m_referencePoint( KDChartEnums::PositionNorthWest )
{
}

void PrerenderedElement::setPosition( const QPointF& position )
{
    m_position = position;
}

const QPointF& PrerenderedElement::position() const
{
    return m_position;
}

void PrerenderedElement::setReferencePoint( KDChartEnums::PositionValue point )
{
    m_referencePoint = point;
}

KDChartEnums::PositionValue PrerenderedElement::referencePoint() const
{
    return m_referencePoint;
}

PrerenderedLabel::PrerenderedLabel()
    : PrerenderedElement()
{
}

const QPixmap& PrerenderedLabel::pixmap() const
{
    return m_pixmap;
}

void PrerenderedLabel::paint( const QFont& font, const QString& text,
                         double angle )
{
    // FIXME find a better value using font metrics of text (this
    // requires finding the diameter of the circle formed by rotating
    // the bounding rect around the center):
    const int Width = 1000;
    const int Height = Width;

    QRectF boundingRect;
    const QColor FullTransparent( 255, 255, 255, 0 );
#ifdef Q_WS_X11
    QImage pixmap( Width, Height, QImage::Format_ARGB32_Premultiplied );
    qWarning() << "PrerenderedLabel::paint: using QImage for prerendered labels "
               << "to work around XRender/Qt4 bug.";
#else
    QPixmap pixmap( Width, Height );
#endif
    // pixmap.fill( FullTransparent );
    {
        static const QPointF Center ( 0.0, 0.0 );
        QPointF textBottomRight;
        QPainter painter( &pixmap );
        painter.setRenderHint(QPainter::TextAntialiasing, true );
        painter.setRenderHint(QPainter::Antialiasing, true );

        painter.setPen( FullTransparent );
        painter.setBrush( FullTransparent );
        painter.drawRect( 0, 0, Width, Height );

        QMatrix matrix;
        matrix.translate( 0.5 * Width,  0.5 * Height );
        matrix.rotate( angle );
        painter.setWorldMatrix( matrix );

        painter.setPen( Qt::black );
        painter.setBrush( Qt::black );
        painter.setFont( font );
        QRectF container( -0.5 * Width, -0.5 * Height, Width, 0.5 * Height );
        painter.drawText( container, Qt::AlignHCenter | Qt::AlignBottom,
                          text, &boundingRect );
        m_referenceBottomLeft = QPointF( boundingRect.bottomLeft().x(), 0.0 );
        textBottomRight = QPointF( boundingRect.bottomRight().x(), 0.0 );
        m_textAscendVector = boundingRect.topRight() - textBottomRight;
        m_textBaseLineVector = textBottomRight - m_referenceBottomLeft;

        // FIXME translate topright by char height
        boundingRect = matrix.mapRect( boundingRect );
        m_referenceBottomLeft = matrix.map( m_referenceBottomLeft )
                                - boundingRect.topLeft();
        textBottomRight = matrix.map( textBottomRight )
                          - boundingRect.topLeft();
        m_textAscendVector = matrix.map( m_textAscendVector )
                             - matrix.map( Center );
        m_textBaseLineVector = matrix.map( m_textBaseLineVector )
                            - matrix.map( Center );
    }

    QPixmap temp( static_cast<int>( boundingRect.width() ),
                  static_cast<int>( boundingRect.height() ) );
    {
        temp.fill( FullTransparent );
        QPainter painter( &temp );
#ifdef Q_WS_X11
        painter.drawImage( QPointF( 0.0, 0.0 ), pixmap, boundingRect );
#else
        painter.drawPixmap( QPointF( 0.0, 0.0 ), pixmap, boundingRect );
#endif
#ifdef PRERENDEREDLABEL_DEBUG
        painter.setPen( QPen( Qt::red, 2 ) );
        painter.setBrush( Qt::red );
        // paint markers for the reference points
        QList<KDChartEnums::PositionValue> positions;
        positions << KDChartEnums::PositionCenter
                  << KDChartEnums::PositionNorthWest
                  << KDChartEnums::PositionNorth
                  << KDChartEnums::PositionNorthEast
                  << KDChartEnums::PositionEast
                  << KDChartEnums::PositionSouthEast
                  << KDChartEnums::PositionSouth
                  << KDChartEnums::PositionSouthWest
                  << KDChartEnums::PositionWest;
        Q_FOREACH( KDChartEnums::PositionValue position, positions ) {
            static const double Radius = 0.5;
            static const double Diameter = 2 * Radius;

            QPointF point ( referencePointLocation( position ) );
            painter.drawEllipse( QRectF( point - QPointF( Radius, Radius ),
                                         QSizeF( Diameter, Diameter ) ) );
        }
#endif
    }

    m_pixmap = temp;
}

QPointF PrerenderedLabel::referencePointLocation( KDChartEnums::PositionValue position ) const
{
    switch( position ) {
    case KDChartEnums::PositionCenter:
        return m_referenceBottomLeft + 0.5 * m_textBaseLineVector + 0.5 * m_textAscendVector;
    case KDChartEnums::PositionNorthWest:
        return m_referenceBottomLeft + m_textAscendVector;
    case KDChartEnums::PositionNorth:
        return m_referenceBottomLeft + 0.5 * m_textBaseLineVector + m_textAscendVector;
    case KDChartEnums::PositionNorthEast:
        return m_referenceBottomLeft + m_textBaseLineVector + m_textAscendVector;
    case KDChartEnums::PositionEast:
        return m_referenceBottomLeft + 0.5 * m_textAscendVector;
    case KDChartEnums::PositionSouthEast:
        return m_referenceBottomLeft + m_textBaseLineVector;
    case KDChartEnums::PositionSouth:
        return m_referenceBottomLeft + 0.5 * m_textBaseLineVector;
    case KDChartEnums::PositionSouthWest:
        return m_referenceBottomLeft;
    case KDChartEnums::PositionWest:
        return m_referenceBottomLeft + m_textBaseLineVector + 0.5 * m_textAscendVector;

    case KDChartEnums::PositionUnknown: // intentional fall-through
    case KDChartEnums::PositionFloating: // intentional fall-through
    default:
        return QPointF();
    }
}


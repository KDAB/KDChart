#include <QObject>
#include <QtDebug>
#include <QPainter>

#include "CoordinatePlane.h"

CoordinatePlane::CoordinatePlane ()
    : KDChart::CartesianCoordinatePlane()
    , m_bottomLeft ( -1.0, -1.0 )
    , m_topRight ( 11.0, 11.0 )
{
}

void CoordinatePlane::resizeEvent ( QResizeEvent*  )
{
    // set up the coordinate transformation:
    const QPointF center ( widthF()/2, heightF()/2 );

    transformation.diagramRect = QRectF ( 10.0, 10.0, width() - 20, height() - 20 );
    transformation.unitVectorX = ( 1.0 * transformation.diagramRect.width() ) / widthF();
    transformation.unitVectorY = ( -1.0 * transformation.diagramRect.height() ) / heightF();
    transformation.originTranslation = QPointF (
        m_bottomLeft.x() * -transformation.unitVectorX + transformation.diagramRect.x(),
        m_topRight.y() * -transformation.unitVectorY + transformation.diagramRect.y() );
    transformation.isoScaleX = 1.0;
    transformation.isoScaleY = 1.0;
}

const double CoordinatePlane::widthF()
{
    return m_topRight.x() - m_bottomLeft.x();
}

const double CoordinatePlane::heightF()
{
    return m_topRight.x() - m_bottomLeft.x();
}

void CoordinatePlane::paintEvent( QPaintEvent* e )
{
    KDChart::CartesianCoordinatePlane::paintEvent ( e );
    QColor gridColor ( "darkkhaki" );
    QColor lineColor ( "navy" );

    QPainter painter ( this );
    painter.setRenderHint( QPainter::Antialiasing );
    painter.setRenderHint( QPainter::TextAntialiasing );

    { // the grid lines:
        QVector<QLineF> lines;
        for ( int i = 0; i < ( int ) widthF(); ++i )
        {   // draw 5 horizontal and vertical grid lines
            lines.append( QLineF( translate ( QPointF( m_topRight.x() - i, m_topRight.y() ) ),
                                  translate ( QPointF( m_topRight.x() - i, 0.0 ) ) ) );
            lines.append( QLineF( translate ( QPointF( m_topRight.x() - i, 0.0 ), 0.0 ),
                                  translate ( QPointF( m_topRight.x() - i, 0.0 ), -1.0 ) ) );
        }
        lines.append( QLineF( translate ( QPointF( 0.0, 0.0 ), -1.0 ),
                              translate ( QPointF( m_topRight.x(), 0.0 ), -1.0 ) ) );
        lines.append( QLineF( translate ( QPointF( 0.0, m_topRight.y() ), -1.0 ),
                              translate ( QPointF( 0.0, 0.0 ), -1.0 ) ) );

        for ( int i = 0; i < ( int ) heightF(); ++i )
        {
            lines.append( QLineF( translate ( QPointF( 0.0, m_topRight.y() - i ) ),
                                  translate ( QPointF( m_topRight.x(), m_topRight.y() - i ) ) ) );
            lines.append( QLineF( translate ( QPointF( 0.0, m_topRight.y() - i ), 0.0 ),
                                  translate ( QPointF( 0.0, m_topRight.y() - i ), -1.0 ) ) );
        }
        painter.setPen ( gridColor );
        painter.drawLines( lines );
    }

    { // the zero lines:
        painter.setPen ( lineColor );

        painter.drawLine( translate ( QPointF( 0.0, 0.0 ) ),
                          translate ( QPointF ( m_topRight.x(), 0.0 ) ) );
        painter.drawLine( translate ( QPointF( 0.0, m_topRight.y() ) ),
                          translate ( QPointF ( 0.0, 0.0 ) ) );
        painter.drawLine( translate ( QPointF( 0.0, 0.0 ) ),
                          translate ( QPointF ( 0.0, 0.0 ), -1.0 ) );
    }

    QFont font = painter.font();
    font.setPointSize ( 8 );
    painter.setFont ( font );

    for ( int i = 1; i <= 10; ++i )
    {
        drawBar ( painter, i, i );
        drawAbscissaMarker ( painter, i, QString().setNum( i ) );
    }
}

void CoordinatePlane::drawBar ( QPainter& painter, int position, double value )
{
    static const QColor palette[] = {
        QColor ( "gold" ),
        QColor ( "lightseagreen" ),
        QColor ( "maroon" ),
        QColor ( "crimson" ),
        QColor ( "cadetblue" ),
        QColor ( "coral" ),
        QColor ( "blueviolet" ),
        QColor ( "darkolivegreen" ),
        QColor ( "dodgerblue" ),
        QColor ( "seagreen" ),
        QColor ( "indigo" ),
        QColor ( "yellowgreen" )
    };
    static const int NoOfColorsInPalette = sizeof palette / sizeof palette[0];

    const double barWidth = 0.6;
    const QPointF bottomLeft ( - barWidth/2.0 + position, 0.0 );
    const QPointF bottomRight ( barWidth/2.0 + position, 0.0 );
    const QPointF topLeft ( bottomLeft.x(), value );
    const QPointF topRight( bottomRight.x(), value );
    QColor barColor = palette[position % NoOfColorsInPalette];
    barColor.setAlphaF ( 0.75 );

    // draw the hidden lines (no fill color):
    QVector<QLineF> lines;
    lines.append( QLineF ( translate ( bottomLeft, 0.0 ), translate ( bottomRight, 0.0 ) ) );
    lines.append( QLineF ( translate ( bottomLeft, 0.0 ), translate ( bottomLeft, -barWidth ) ) );
    lines.append( QLineF ( translate ( bottomLeft, 0.0 ), translate( topLeft, 0.0 ) ) );
    painter.setPen ( barColor.dark() );
    painter.drawLines ( lines );

    { // right side plane:
        QPointF points[4];
        points[0] = translate( bottomRight, 0.0 );
        points[1] = translate( bottomRight, -barWidth );
        points[2] = translate( topRight, -barWidth );
        points[3] = translate( topRight, 0.0 );

        painter.setPen( barColor.dark() );
        painter.setBrush ( barColor );
        painter.drawPolygon ( points, 4 );
    }

    { // top side plane:
        QPointF points[4];
        points[0] = translate( topRight, 0.0 );
        points[1] = translate( topRight, -barWidth );
        points[2] = translate( topLeft, -barWidth );
        points[3] = translate( topLeft, 0.0 );

        painter.setPen( barColor.dark() );
        painter.setBrush ( barColor );
        painter.drawPolygon ( points, 4 );
    }

    { // front side plane:
        QPointF points[4];
        points[0] = translate( topRight, -barWidth );
        points[1] = translate( topLeft, -barWidth );
        points[2] = translate( bottomLeft, -barWidth );
        points[3] = translate( bottomRight, -barWidth );

        painter.setPen( barColor.dark() );
        painter.setBrush ( barColor.light( 125 ) );
        painter.drawPolygon ( points, 4 );
    }
}

void CoordinatePlane::drawAbscissaMarker ( QPainter& painter,  int position,  const QString& text )
{
    const double barWidth = 0.6; // FIXME: use parameter
    QColor markerColor ("darkslateblue" );
    QPointF topLeftTextAnchor ( translate (  QPointF(  position - 0.5,  -0.25 ),  -1.0 ) );
    QPointF bottomRightTextAnchor ( translate ( QPointF ( position + 0.5, -0.75 ), -1.0 ) );
    QSizeF textRectangleSize ( bottomRightTextAnchor.x() - topLeftTextAnchor.x(),
                               bottomRightTextAnchor.y() - topLeftTextAnchor.y() );
    QRectF textRectangle ( topLeftTextAnchor,  textRectangleSize );
    painter.drawLine( translate ( QPointF ( position, 0.0 ), -barWidth ),
                      translate ( QPointF (  position,  0.0 ),  -1.0 ) );
    painter.drawLine( translate ( QPointF ( position,   0.0 ),   -1.0 ),
                      translate ( QPointF ( position, -0.25 ), -1.0 ) );
    painter.drawText( textRectangle, Qt::AlignHCenter | Qt::AlignTop, text );
}

QPointF CoordinatePlane::translate ( const QPointF& diagramPoint, double perspective )
{
    static const double PerspectiveCorrection = 1.0;

    QPointF point ( diagramPoint );
    point.setX( diagramPoint.x() + perspective / 2 * PerspectiveCorrection );
    point.setY( diagramPoint.y() + perspective / 2 * PerspectiveCorrection );

    return transformation.translate ( point );
}

QPointF CoordinatePlane::translate (  const QPointF& diagramPoint )
{
    return transformation.translate( diagramPoint );
}

double CoordinatePlane::translateDistance ( const QPointF& p1, const QPointF& p2 )
{   // FIXME: it may make sense to add straight abscissa or ordinate
    // distances, since those do not require pythagoras calculation:
    QPointF t1 = translate ( p1 );
    QPointF t2 = translate ( p2 );
    return qAbs( sqrt ( pow( t1.x() - t2.x(), 2 ) + pow ( t1.y() - t2.y(), 2 ) ) );
}

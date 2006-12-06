#include <limits>

#include <QChar>
#include <QTextStream>

#include "TernaryPoint.h"

TernaryPoint::TernaryPoint()
    : m_a( -1.0 )
    , m_b( -1.0 )
{
    Q_ASSERT( !isValid() );
}

TernaryPoint::TernaryPoint( double a, double b )
    : m_a( -1.0 )
    , m_b( -1.0 )
{
    set( a, b );
}

void TernaryPoint::set( double a, double b )
{
    if ( a >= 0.0 && a <= 1.0
         && b >= 0.0 && b <= 1.0
         && 1.0 - a - b >= -2.0 * std::numeric_limits<double>::epsilon() ) {
        m_a = a;
        m_b = b;
        Q_ASSERT( isValid() ); // more a test for isValid
    } else {
        m_a = -1.0;
        m_b = -1.0;
        Q_ASSERT( ! isValid() );
    }
}

bool TernaryPoint::isValid() const
{
    return
        m_a >= 0.0 && m_a <= 1.0
        && m_b >= 0.0 && m_b <= 1.0
        && 1.0 - m_a + m_b >= - std::numeric_limits<double>::epsilon();
}

QDebug operator<<( QDebug stream, const TernaryPoint& point )
{
    QString string;
    QTextStream text( &string );
    text << "[TernaryPoint: ";
    if ( point.isValid() ) {
        text.setFieldWidth( 2 );
        text.setPadChar( QLatin1Char( '0' ) );
        text << ( int ) ( point.a() * 100.0 ) << "%|"
             << ( int ) ( point.b() * 100.0 ) << "%|"
             << ( int ) ( point.c() * 100.0 ) << "%]";
    } else {
        text << "a=" << point.a() << " - b=" << point.b() << " - INVALID]";
    }
    stream << string;
    return stream;
}


#ifndef TERNARYPOINT_H
#define TERNARYPOINT_H

#include <QtDebug>
#include <QPointF>

/**
  * @brief TernaryPoint defines a point within a ternary coordinate plane
  * \internal
  */
class TernaryPoint
{
public:
    TernaryPoint();
    TernaryPoint( double a, double b );

    double a() const { return m_a; }
    double b() const { return m_b; }
    double c() const { return 1.0 - m_a - m_b; }

    void set( double a, double b );

    bool isValid() const;

private:
    double m_a;
    double m_b;
};

QDebug operator<<( QDebug stream, const TernaryPoint& point );

QPointF translate( const TernaryPoint& );

#endif

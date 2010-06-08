#ifndef PAINTCONTEXT_H
#define PAINTCONTEXT_H

class QPainter;

#include <QRectF>
#include "KDChartGlobal.h"

namespace KDChart {

    class AbstractCoordinatePlane;

    /**
      * @brief Stores information about painting diagrams
      * \internal
      */
    class KDCHART_EXPORT PaintContext
    {
    public:
        PaintContext();
        ~PaintContext();

        const QRectF rectangle () const;
        void setRectangle( const QRectF& rect );

        QPainter* painter() const;
        void setPainter( QPainter* painter );

        AbstractCoordinatePlane* coordinatePlane() const;
        void setCoordinatePlane( AbstractCoordinatePlane* plane );

    private:
        class Private;
        Private * _d;
        Private * d_func() { return _d; }
        const Private * d_func() const { return _d; }
    };

}

#endif /* PAINTCONTEXT_H */


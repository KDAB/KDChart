#ifndef PRINTINGPARAMETERS_H
#define PRINTINGPARAMETERS_H

#include <QPen>
#include <QDebug>

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

namespace KDChart {
    /**
     * PrintingParameters stores the scale factor which lines has to been scaled with when printing.
     * It's essentially printer's logical DPI / widget's logical DPI
     * \internal
     */
    class PrintingParameters {
    public:
        static void setScaleFactor( const qreal scaleFactor );
        static void resetScaleFactor();
        static QPen scalePen( const QPen& pen );

    private:
        PrintingParameters();
        static PrintingParameters* instance();

        qreal scaleFactor;
    };
}

#endif

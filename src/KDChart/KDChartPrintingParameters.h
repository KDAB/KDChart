/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

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
class PrintingParameters
{
public:
    static void setScaleFactor(const qreal scaleFactor);
    static void resetScaleFactor();
    static QPen scalePen(const QPen &pen);

private:
    PrintingParameters();
    static PrintingParameters *instance();

    qreal scaleFactor = 1.0;
};
}

#endif

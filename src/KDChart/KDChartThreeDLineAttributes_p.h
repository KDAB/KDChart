/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef KDCHARTTHREEDLINESATTRIBUTES_P_H
#define KDCHARTTHREEDLINESATTRIBUTES_P_H

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

#include <KDChartAbstractThreeDAttributes_p.h>

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class ThreeDLineAttributes::Private : public AbstractThreeDAttributes::Private
{
    friend class ThreeDLineAttributes;

public:
    Private();

private:
    //ThreeDLines
    uint lineXRotation = 15;
    uint lineYRotation = 15;
};

KDCHART_DERIVED_PRIVATE_FOOTER(ThreeDLineAttributes, AbstractThreeDAttributes)

}

#endif // KDCHARTTHREEDLINESATTRIBUTES_P_H

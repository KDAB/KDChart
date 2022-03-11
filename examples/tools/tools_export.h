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

#ifndef TESTTOOLS_EXPORT_H
#define TESTTOOLS_EXPORT_H

#include <qglobal.h>

#if defined(TESTTOOLS_DLL) || defined(TESTTOOLS_MAKEDLL)
#ifdef KDCHART_BUILD_TESTTOOLS_LIB
#define TESTTOOLS_EXPORT Q_DECL_EXPORT
#else
#define TESTTOOLS_EXPORT Q_DECL_IMPORT
#endif
#else
#define TESTTOOLS_EXPORT
#endif

#endif // TESTTOOLS_EXPORT_H

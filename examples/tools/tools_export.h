/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
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

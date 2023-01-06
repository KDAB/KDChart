/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHART_EXPORT_H
#define KDCHART_EXPORT_H

#include <qglobal.h>

#ifdef KDCHART_STATICLIB
#undef KDCHART_SHAREDLIB
#define KDCHART_EXPORT
#define KDGANTT_EXPORT
#define KDCHART_COMPAT_EXPORT
#else
#ifdef KDCHART_BUILD_KDCHART_LIB
#define KDCHART_EXPORT Q_DECL_EXPORT
#define KDGANTT_EXPORT Q_DECL_EXPORT
#else
#define KDCHART_EXPORT Q_DECL_IMPORT
#define KDGANTT_EXPORT Q_DECL_IMPORT
#endif
#ifdef KDCHART_BUILD_KDCHART_COMPAT_LIB
#define KDCHART_COMPAT_EXPORT Q_DECL_EXPORT
#else
#define KDCHART_COMPAT_EXPORT Q_DECL_IMPORT
#endif
#ifdef KDCHART_BUILD_PLUGIN_LIB
#define KDCHART_PLUGIN_EXPORT Q_DECL_EXPORT
#else
#define KDCHART_PLUGIN_EXPORT Q_DECL_IMPORT
#endif
#endif

#endif // KDCHART_EXPORT_H

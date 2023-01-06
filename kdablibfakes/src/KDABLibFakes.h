/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDAB_LIB_FAKES_H
#define KDAB_LIB_FAKES_H

#if defined Q_OS_DARWIN
/* On Mac OS X, ensure that <cmath> will define std::isnan */
#define _GLIBCPP_USE_C99 1
#endif

#include <cmath>

#ifdef Q_OS_SOLARIS
#include <math.h>
#include <sunmath.h>
#endif

#include <qglobal.h>

#include <limits>

#define NaN std::numeric_limits<qreal>::quiet_NaN()
#define signalingNaN std::numeric_limits<qreal>::signaling_NaN()

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEGTORAD(d) (d) * M_PI / 180

// Smybian's math.h doesn't define a trunc function
#if defined(Q_OS_SYMBIAN) || defined(QT_SIMULATOR)
#define trunc(x) (qreal)(( int )(x + (x >= 0.0 ? -0.5 : 0.5)))
#endif

// We use our own ISNAN / ISINF in the code to detect
// that we defined them.
// reason: Windows / MacOS do not have isnan() / isinf()
#if defined(Q_OS_WIN)
#include <float.h>
#define ISNAN(x) _isnan(x)
#define ISINF(x) (!(_finite(x) + _isnan(x)))
#else
#define ISNAN(x) std::isnan(x)
#define ISINF(x) std::isinf(x)
#endif

#endif

/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#ifndef __KDCHARTGLOBAL_H__
#define __KDCHARTGLOBAL_H__

#include <qglobal.h>

#ifdef useless_cruft
#include <cfloat>

#if defined(unix) || defined(__unix__) || defined(Q_WS_MAC) || defined(Q_WS_LINUX)
#include <climits>
#else
#define MINDOUBLE DBL_MIN
#define MAXDOUBLE DBL_MAX
#endif

// http://support.microsoft.com/default.aspx?scid=kb;en-us;134980
// says this is fixed since MSVC 4.0!
#if defined(_MSC_VER)
#pragma warning(disable: 4251)
#endif
#endif // useless_cruft

#include "kdchart_export.h"



#ifdef useless_cruft
// since Qt 3.1 they do /not/ include limits.h or climits on windows anymore
// so we must include that manually
#if defined(_MSC_VER)
#include <climits>
#endif


#include <QList>
#include <QVector>
#endif // useless_cruft

#if _MSC_VER < 1300
#define KDCHART_DECLARE_PRIVATE_DERIVED( X )      \
public:                                           \
    class Private;                                \
protected:                                        \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private * );               \
private:                                          \
    void init();
#else
#define KDCHART_DECLARE_PRIVATE_DERIVED( X )      \
protected:                                        \
    class Private;                                \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private * );               \
private:                                          \
    void init();
#endif

#if _MSC_VER < 1300
#define KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( X, ParentType )      \
public:                                           \
    class Private;                                \
protected:                                        \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private *, ParentType );   \
private:                                          \
    void init();
#else
#define KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( X, ParentType )      \
protected:                                        \
    class Private;                                \
    inline Private * d_func();                    \
    inline const Private * d_func() const;        \
    explicit inline X( Private *, ParentType );   \
private:                                          \
    void init();
#endif

#define KDCHART_DECLARE_PRIVATE_DERIVED_QWIDGET( X )      \
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( X, QWidget* )

#define KDCHART_DECLARE_PRIVATE_BASE_VALUE( X )   \
public:                                           \
    inline void swap( X & other );                \
protected:                                        \
    class Private;                                \
    Private * d_func() { return _d; }             \
    const Private * d_func() const { return _d; } \
private:                                          \
    void init();                                  \
    Private * _d;

#if _MSC_VER < 1300
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( X ) \
public:                                           \
    class Private;                                    \
protected:                                        \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private * );                   \
private:                                              \
    void init();                                      \
    Private * _d;
#else
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( X ) \
protected:                                        \
    class Private;                                    \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private * );                   \
private:                                              \
    void init();                                      \
    Private * _d;
#endif

#if _MSC_VER < 1300
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( X ) \
public:                                           \
    class Private;                                    \
protected:                                        \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private *, QWidget* );                  \
private:                                              \
    void init();                                      \
    Private * _d;
#else
#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( X ) \
protected:                                        \
    class Private;                                    \
    Private * d_func() { return _d; }                 \
    const Private * d_func() const { return _d; }     \
    explicit inline X( Private *, QWidget* );                  \
private:                                              \
    void init();                                      \
    Private * _d;
#endif


#define KDCHART_DERIVED_PRIVATE_FOOTER( CLASS, PARENT )     \
inline CLASS::CLASS( Private * p )                          \
  : PARENT( p ) { init(); }                                 \
inline CLASS::Private * CLASS::d_func()                     \
{ return static_cast<Private*>( PARENT::d_func() ); }       \
inline const CLASS::Private * CLASS::d_func() const         \
{ return static_cast<const Private*>( PARENT::d_func() ); }



#include <QtAlgorithms> // qSwap
#ifndef QT_NO_STL
#include <algorithm>
#define KDCHART_DECLARE_SWAP_SPECIALISATION( X )            \
    inline void X::swap( X & other )                        \
    { qSwap( _d, other._d ); }                              \
    template <> inline void qSwap<X>( X & lhs, X & rhs )    \
    { lhs.swap( rhs ); }                                    \
    namespace std {                                         \
        template <> inline void swap<X>( X & lhs, X & rhs ) \
        { lhs.swap( rhs ); }                                \
    }
#else
#define KDCHART_DECLARE_SWAP_SPECIALISATION( X )            \
    template <> inline void qSwap<X>( X & lhs, X & rhs )    \
    { lhs.swap( rhs ); }
#endif

#if defined(Q_OS_WIN) && defined(QT_DLL)
#if _MSC_VER >= 1300
// workaround http://support.microsoft.com/default.aspx?scid=kb;en-us;309801
#include <QPointF>
#include <QVector>
template class Q_DECL_IMPORT QVector<QPointF>;
#endif
#endif

#include <Qt>

namespace KDChart {

enum DisplayRoles {
  DatasetPenRole = 0x0A79EF95,
  DatasetBrushRole,
  DataValueLabelAttributesRole,
  ThreeDAttributesRole,
  LineAttributesRole,
  ThreeDLineAttributesRole,
  BarAttributesRole,
  ThreeDBarAttributesRole,
  ExplodeFactorRole,
  ThreeDPieAttributesRole
};
}

#endif // __KDCHARTGLOBAL_H__

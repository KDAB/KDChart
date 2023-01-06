/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/
/** \file KDChartGlobal.h
\brief Contains KDChart macros.

Contains KDChart macros.  */

#ifndef __KDCHARTGLOBAL_H__
#define __KDCHARTGLOBAL_H__

#include <qglobal.h>

#include "kdchart_export.h"

#ifndef KDAB_SET_OBJECT_NAME
template<typename T>
inline T &__kdab__dereference_for_methodcall(T &o)
{
    return o;
}

template<typename T>
inline T &__kdab__dereference_for_methodcall(T *o)
{
    return *o;
}

#define KDAB_SET_OBJECT_NAME(x) __kdab__dereference_for_methodcall(x).setObjectName(QLatin1String(#x))
#endif

#define KDCHART_DECLARE_PRIVATE_DERIVED(X) \
public:                                    \
    class Private;                         \
                                           \
protected:                                 \
    inline Private *d_func();              \
    inline const Private *d_func() const;  \
    explicit inline X(Private *);          \
                                           \
private:                                   \
    void init();

#define KDCHART_DECLARE_PRIVATE_DERIVED_PARENT(X, ParentType) \
public:                                                       \
    class Private;                                            \
                                                              \
protected:                                                    \
    inline Private *d_func();                                 \
    inline const Private *d_func() const;                     \
    explicit inline X(Private *, ParentType);                 \
                                                              \
private:                                                      \
    void init();

#define KDCHART_DECLARE_PRIVATE_DERIVED_QWIDGET(X) \
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT(X, QWidget *)

#define KDCHART_DECLARE_PRIVATE_BASE_VALUE(X) \
public:                                       \
    inline void swap(X &other)                \
    {                                         \
        qSwap(_d, other._d);                  \
    }                                         \
                                              \
protected:                                    \
    class Private;                            \
    Private *d_func()                         \
    {                                         \
        return _d;                            \
    }                                         \
    const Private *d_func() const             \
    {                                         \
        return _d;                            \
    }                                         \
                                              \
private:                                      \
    void init();                              \
    Private *_d;

#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC(X) \
public:                                             \
    class Private;                                  \
                                                    \
protected:                                          \
    Private *d_func()                               \
    {                                               \
        return _d;                                  \
    }                                               \
    const Private *d_func() const                   \
    {                                               \
        return _d;                                  \
    }                                               \
    explicit inline X(Private *);                   \
                                                    \
private:                                            \
    void init();                                    \
    Private *_d;

#define KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET(X) \
public:                                                     \
    class Private;                                          \
                                                            \
protected:                                                  \
    Private *d_func()                                       \
    {                                                       \
        return _d;                                          \
    }                                                       \
    const Private *d_func() const                           \
    {                                                       \
        return _d;                                          \
    }                                                       \
    explicit inline X(Private *, QWidget *);                \
                                                            \
private:                                                    \
    void init();                                            \
    Private *_d;

#define KDCHART_DERIVED_PRIVATE_FOOTER(CLASS, PARENT)          \
    inline CLASS::CLASS(Private *p)                            \
        : PARENT(p)                                            \
    {                                                          \
        init();                                                \
    }                                                          \
    inline CLASS::Private *CLASS::d_func()                     \
    {                                                          \
        return static_cast<Private *>(PARENT::d_func());       \
    }                                                          \
    inline const CLASS::Private *CLASS::d_func() const         \
    {                                                          \
        return static_cast<const Private *>(PARENT::d_func()); \
    }

#define KDCHART_DECLARE_DERIVED_DIAGRAM(X, PLANE)     \
public:                                               \
    class Private;                                    \
                                                      \
protected:                                            \
    inline Private *d_func();                         \
    inline const Private *d_func() const;             \
    explicit inline X(Private *);                     \
    explicit inline X(Private *, QWidget *, PLANE *); \
                                                      \
private:                                              \
    void init();

#define KDCHART_IMPL_DERIVED_DIAGRAM(CLASS, PARENT, PLANE)     \
    inline CLASS::CLASS(Private *p)                            \
        : PARENT(p)                                            \
    {                                                          \
        init();                                                \
    }                                                          \
    inline CLASS::CLASS(                                       \
        Private *p, QWidget *parent, PLANE *plane)             \
        : PARENT(p, parent, plane)                             \
    {                                                          \
        init();                                                \
    }                                                          \
    inline CLASS::Private *CLASS::d_func()                     \
    {                                                          \
        return static_cast<Private *>(PARENT::d_func());       \
    }                                                          \
    inline const CLASS::Private *CLASS::d_func() const         \
    {                                                          \
        return static_cast<const Private *>(PARENT::d_func()); \
    }

#define KDCHART_IMPL_DERIVED_PLANE(CLASS, BASEPLANE)              \
    inline CLASS::CLASS(Private *p, Chart *parent)                \
        : BASEPLANE(p, parent)                                    \
    {                                                             \
        init();                                                   \
    }                                                             \
    inline CLASS::Private *CLASS::d_func()                        \
    {                                                             \
        return static_cast<Private *>(BASEPLANE::d_func());       \
    }                                                             \
    inline const CLASS::Private *CLASS::d_func() const            \
    {                                                             \
        return static_cast<const Private *>(BASEPLANE::d_func()); \
    }

#include <QtAlgorithms> // qSwap
#ifndef QT_NO_STL
#include <algorithm>
#define KDCHART_DECLARE_SWAP_SPECIALISATION(X) \
    QT_BEGIN_NAMESPACE                         \
    template<>                                 \
    inline void qSwap<X>(X & lhs, X & rhs)     \
    {                                          \
        lhs.swap(rhs);                         \
    }                                          \
    QT_END_NAMESPACE                           \
    namespace std {                            \
    template<>                                 \
    inline void swap<X>(X & lhs, X &rhs)       \
    {                                          \
        lhs.swap(rhs);                         \
    }                                          \
    }
#else
#define KDCHART_DECLARE_SWAP_SPECIALISATION(X) \
    QT_BEGIN_NAMESPACE                         \
    template<>                                 \
    inline void qSwap<X>(X & lhs, X & rhs)     \
    {                                          \
        lhs.swap(rhs);                         \
    }                                          \
    QT_END_NAMESPACE
#endif

#define KDCHART_DECLARE_SWAP_SPECIALISATION_DERIVED(X) \
    KDCHART_DECLARE_SWAP_SPECIALISATION(X)

#define KDCHART_DECLARE_SWAP_BASE(X) \
protected:                           \
    void doSwap(X &other)            \
    {                                \
        qSwap(_d, other._d);         \
    }

#define KDCHART_DECLARE_SWAP_DERIVED(X) \
    void swap(X &other)                 \
    {                                   \
        doSwap(other);                  \
    }

#if defined(Q_OS_WIN) && defined(QT_DLL)
#if defined(_MSC_VER) && _MSC_VER >= 1300
// workaround old msvc bug 309801 (link under support.microsoft.com no longer exists
#include <QPointF>
#include <QVector>
template class Q_DECL_IMPORT QVector<QPointF>;
#endif
#endif

#include <Qt>

namespace KDChart {

enum DisplayRoles
{
    DatasetPenRole = 0x0A79EF95,
    DatasetBrushRole,
    DataValueLabelAttributesRole,
    ThreeDAttributesRole,
    LineAttributesRole,
    ThreeDLineAttributesRole,
    BarAttributesRole,
    StockBarAttributesRole,
    ThreeDBarAttributesRole,
    PieAttributesRole,
    ThreeDPieAttributesRole,
    DataHiddenRole,
    ValueTrackerAttributesRole,
    CommentRole
};
}

#endif // __KDCHARTGLOBAL_H__

/* -*- Mode: C++ -*-
  KDChart - a multi-platform charting engine
  */

/****************************************************************************
** Copyright (C) 2005-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTPOSITION_H
#define KDCHARTPOSITION_H

#include <Qt>
#include <QStringList>
#include "KDChartGlobal.h"

class QDomDocumentFragment;


namespace KDChart {

/**
 * \class Position KDChartPosition.h
 * \brief Defines a position, using compass terminology.
 *
 * Using KDChartPosition you can specify one of nine
 * pre-defined, logical points (see the \c static \c const getter
 * methods below), in a similar way, as you would use a
 * compass to navigate on a map.
 *
 * \Note Often you will declare a \c Position together with the
 * KDChartRelativePosition class, to specify a logical point,
 * which then will be used to layout your chart at runtime,
 * e.g. for specifying the location of a floating Legend box.
 *
 * \sa KDChartRelativePosition
 */

class KDCHART_EXPORT Position
{
public:
    Position();
    Position(int value);

    int value() const;

    QString name() const;
    QString printableName() const;
    QDomDocumentFragment toXML() const;

    bool isWestSide() const;
    bool isNorthSide() const;
    bool isEastSide() const;
    bool isSouthSide() const;

    bool isCorner() const;

    static const Position& Unknown;
    static const Position& Center;
    static const Position& NorthWest;
    static const Position& North;
    static const Position& NorthEast;
    static const Position& East;
    static const Position& SouthEast;
    static const Position& South;
    static const Position& SouthWest;
    static const Position& West;

    static QStringList names(bool includeCenter);
    static QStringList printableNames(bool includeCenter);

    static Position fromName(const QString& name);
    static Position fromPrintableName(const QString& printableName);

    bool operator==( const Position& );
    bool operator!=( const Position& );

private:
    static const char *m_names[];
    int m_value;
}; // End of class Position

}


#endif // KDCHARTPOSITION_H

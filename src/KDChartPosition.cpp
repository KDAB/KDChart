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

#include <QObject>
#include <QString>

#include <KDChartPosition.h>
#include <KDChartEnums.h>
#include <QtXml/QDomDocumentFragment>


namespace KDChart {


/**
 * \internal
 * Static strings, to be translated in printable()
 */
static const char * staticPositionNames[] = {
    QT_TR_NOOP("unknown Position"),
    QT_TR_NOOP("Center"),
    QT_TR_NOOP("NorthWest"),
    QT_TR_NOOP("North"),
    QT_TR_NOOP("NorthEast"),
    QT_TR_NOOP("East"),
    QT_TR_NOOP("SouthEast"),
    QT_TR_NOOP("South"),
    QT_TR_NOOP("SouthWest"),
    QT_TR_NOOP("West")
};


/**
 * \internal
 * One value for unknown positions, and nine values for predefined positions.
 */
static Position staticPositionUnknown   = Position( KDChartEnums::PositionUnknown );
static Position staticPositionCenter    = Position( KDChartEnums::PositionCenter );
static Position staticPositionNorthWest = Position( KDChartEnums::PositionNorthWest );
static Position staticPositionNorth     = Position( KDChartEnums::PositionNorth );
static Position staticPositionNorthEast = Position( KDChartEnums::PositionNorthEast );
static Position staticPositionEast      = Position( KDChartEnums::PositionEast );
static Position staticPositionSouthEast = Position( KDChartEnums::PositionSouthEast );
static Position staticPositionSouth     = Position( KDChartEnums::PositionSouth );
static Position staticPositionSouthWest = Position( KDChartEnums::PositionSouthWest );
static Position staticPositionWest      = Position( KDChartEnums::PositionWest );

static int maxPositionValue = 9;

const Position& Position::Unknown   = staticPositionUnknown;
const Position& Position::Center    = staticPositionCenter;
const Position& Position::NorthWest = staticPositionNorthWest;
const Position& Position::North     = staticPositionNorth;
const Position& Position::NorthEast = staticPositionNorthEast;
const Position& Position::East      = staticPositionEast;
const Position& Position::SouthEast = staticPositionSouthEast;
const Position& Position::South     = staticPositionSouth;
const Position& Position::SouthWest = staticPositionSouthWest;
const Position& Position::West      = staticPositionWest;


/**
 * Default constructor. Creates a new Position, defaulting it to Position::Unknown.
 */
Position::Position()
{
    m_value = Position::Unknown.value();
}

/**
 * \internal
 * Constructor. Creates a new Position, defaulting it to the respective value.
 *
 * Valid values ranging from zero (unknown value) to 10.
 * If invalid value is passed, a Position::Unknown is created.
 *
 * \note Normally there is no need to call this constructor, but you would
 * rather use one of the nine pre-defined, static values, e.g. like this:
 * \verbatim
 * const KDChart::Position myPosition = KDChart::Position::NorthEast;
 * \endverbatim
 */
Position::Position(int value)
{
    if( 0 <= value && 10 >= value )
        m_value = value;
    else
        m_value = Position::Unknown.value();
}

/**
 * Returns an integer value corresponding to this Position.
 */
int Position::value() const
{
    return m_value;
}

bool Position::isWestSide() const
{
    return  m_value == Position::SouthWest.value() ||
            m_value == Position::West.value() ||
            m_value == Position::NorthWest.value();
}
bool Position::isNorthSide() const
{
    return  m_value == Position::NorthWest.value() ||
            m_value == Position::North.value() ||
            m_value == Position::NorthEast.value();
}
bool Position::isEastSide() const
{
    return  m_value == Position::NorthEast.value() ||
            m_value == Position::East.value() ||
            m_value == Position::SouthEast.value();
}
bool Position::isSouthSide() const
{
    return  m_value == Position::SouthWest.value() ||
            m_value == Position::South.value() ||
            m_value == Position::SouthEast.value();
}

bool Position::isCorner() const
{
    return  m_value == Position::NorthWest.value() ||
            m_value == Position::NorthEast.value() ||
            m_value == Position::SouthEast.value() ||
            m_value == Position::SouthWest.value();
}
bool Position::isPole() const
{
    return  m_value == Position::North.value() ||
            m_value == Position::South.value();
}

/**
 * Returns a non-translated string in English language, corresponding to this Position.
 */
QString Position::name() const
{
    return staticPositionNames[m_value];
}

/**
 * Returns a translated string, corresponding to this Position.
 */
QString Position::printableName() const
{
    return QObject::tr(staticPositionNames[m_value]);
}


/**
 * \brief Returns a list of all string, corresponding to
 * the pre-defined positions.
 *
 * \param includeCenter if set to \c true, the returned list
 * includes the Center position too.
 */
QStringList Position::names(bool includeCenter)
{
    QStringList list;
    const int start = includeCenter ? 1 : 2;
    for( int i=start; i<=maxPositionValue; ++i)
        list.append( Position(i).name() );
    return list;
}

/**
 * \brief Returns a list of all translated string, corresponding to
 * the pre-defined positions.
 *
 * \param includeCenter if set to \c true, the returned list
 * includes the Center position too.
 */
QStringList Position::printableNames(bool includeCenter)
{
    QStringList list;
    const int start = includeCenter ? 1 : 2;
    for( int i=start; i<=maxPositionValue; ++i)
        list.append( Position(i).printableName() );
    return list;
}

Position Position::fromName(const QString& name)
{
    for( int i=1; i<=maxPositionValue; ++i)
        if( Position(i).name() == name )
            return Position(i);
    return Position(0);
}

Position Position::fromPrintableName(const QString& printableName)
{
    for( int i=1; i<=maxPositionValue; ++i )
        if( Position(i).printableName() == printableName )
            return Position(i);
    return Position(0);
}


bool Position::operator==( const Position& r )
{
    return ( value() == r.value() );
}


bool Position::operator==( const int& value_ )
{
    return ( value() == value_ );
}


bool Position::operator!=( const Position& r )
{
    return ! ( *this == r );
}


QDomDocumentFragment Position::toXML() const
{
    // PENDING(kalle) Implement this, using the string returned by name()
    return QDomDocumentFragment();
}


}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::Position& p )
{
    dbg << "KDChart::Position("
	<< p.name() << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */

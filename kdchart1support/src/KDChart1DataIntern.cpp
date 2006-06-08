/* -*- Mode: C++ -*-
   KDChart1 - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart1 library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#include <KDChart1DataIntern.h>

/**
  \class KDChart1Data KDChart1Data.h

  \brief Encapsulates one data cell in a chart.

  Data cells either hold one value for the ordinate axis
  or they hold two values: one for the ordinate axis and
  an extra one for the abscissa axis.

  Ordinate values can either be of type double or non-existant,
  abscissa values can either be of type double or QDateTime or non-existant.

  \note All methods returning one of the data values (or one of the data
  types, resp.) expect a valNo parameter with either a 1 for the first value
  (the Y or ordinate value)
  or a 2 for the second value (the X or abscissa value).
  */


/**
  \fn KDChart1Data::KDChart1Data()

  Constructor. Creates a KDChart1Data object that represents no value.
  */

/**
  \fn KDChart1Data::KDChart1Data( double value )

  Constructor. Assigns a double value to this object.

  \param value the double value which this object should represent.
  */

/**
  \fn KDChart1Data::KDChart1Data( double yValue, double xValue )

  Constructor. Assigns two double values to this object.

  \param yValue the double value which this object should represent.
  \param xValue the position on the abscissa axis which this object
  should be assigned to.
  */


/**
  \fn KDChart1Data::KDChart1Data( double yValue, QDateTime xValue )

  Constructor. Assigns a double value and a QDateTime value to this object.

  \param yValue the double value which this object should represent.
  \param xValue the position on the abscissa axis which this object
  should be assigned to.
  */


/**
  \fn KDChart1Data::KDChart1Data( const QString& value )

  Constructor. Assigns a string value to this object.

  This data type may be used to store axis labels or legend texts
  in extra data cells.

  \param value the string value which this object should represent
  */


/**
  \fn bool KDChart1Data::hasValue(int) const

  Returns true if the object represents a value (double or QString)
  and false otherwise.

  \return true if the object represents a value
  */

/**
  \fn bool KDChart1Data::isDateTime(int) const

  Returns true if the object represents a QDateTime value and false
  otherwise.

  \return true if the object represents a QDateTime value
  */

/**
  \fn bool KDChart1Data::isString(int) const

  Returns true if the object represents a QString value and false
  otherwise.

  \return true if the object represents a QString value
  */

/**
  \fn bool KDChart1Data::isDouble(int) const

  Returns true if the object represents a double value and false
  otherwise.

  \return true if the object represents a double value
  */

/**
  \fn bool KDChart1Data::isNormalDouble(int) const

  Returns true if the object represents a double value that
  is neither KDCHART1_POS_INFINITE nor KDCHART1_NEG_INFINITE.

  \return true if the object represents a normal double value
  */

/**
  \fn bool KDChart1Data::isPosInfiniteDouble(int) const

  Returns true if the object represents a double value that
  is KDCHART1_POS_INFINITE.

  \return true if the object represents a KDCHART1_POS_INFINITE value
  */

/**
  \fn bool KDChart1Data::isNegInfiniteDouble(int) const

  Returns true if the object represents a double value that
  is KDCHART1_NEG_INFINITE.

  \return true if the object represents a KDCHART1_NEG_INFINITE value
  */


/**
  \fn double KDChart1Data::doubleValue( int valNo=1 ) const

  Returns the double value represented by this object,
  or DBL_MIN if this object does not represent a double value.

  You may use isDouble() or isNormalDouble() to check if it
  is appropriate to call this function.

  \return the double value represented by this object,
  or DBL_MIN if this object does not represent a double value
  */

/**
  \fn QDateTime KDChart1Data::dateTimeValue( int valNo=1 ) const

  Returns the QDateTime value represented by this object,
  or QDateTime() if this object does not represent a QDateTime value.

  You may use isDateTime() to check if it
  is appropriate to call this function.

  \return the QDateTime value represented by this object,
  or QDateTime() if this object does not represent a QDateTime value
  */

/**
  \fn QString KDChart1Data::stringValue( int valNo=1 ) const

  Returns the string value represented by this object,
  or QString::null if this object does not represent a string value.

  You may use isString() to check if it
  is appropriate to call this function.

  \note String values may be used to store axis labels or legend texts
  in extra data cells.

  \return the string value represented by this object,
  or QString::null if this object does not represent a string value
  */


/**
  \fn KDChart1Data::ValueType KDChart1Data::valueType( int valNo ) const

  Returns the type of the respective value.

  \param valNo specifies the value number: 1 means the Y value, 2 means the X value

  \return the type of the respective value
  */


/**
  \fn void KDChart1Data::clearValue()

  Resets both of the data types of this cell to KDChart1Data::NoValue
  and sets the property set ID to KDCHART1_PROPSET_NORMAL_DATA.
  */

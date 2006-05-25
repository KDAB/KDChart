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

#ifndef KDCHARTREALTIVEPOSITION_H
#define KDCHARTREALTIVEPOSITION_H

#include <QDebug>
#include <Qt>
#include "KDChartGlobal.h"
#include "KDChartEnums.h"
#include "KDChartMeasure.h"
#include "KDChartPosition.h"

class QDomDocumentFragment;


namespace KDChart {

    class AbstractArea;


/**
  \class RelativePosition KDChartRelativePosition.h
  \brief Defines relative position information: reference area, position
  in this area, horizontal / vertical padding, and rotating.

  \Note Using RelativePosition you can specify the relative parts
  of some position information, and you can specify the absolute parts:
  the reference area, and the position in this area.
  To get an absolute position, you will need to declare both, the relative
  and the absolute parts, otherwise the specification is incomplete and
  KD Chart will ignore it.
  */
class KDCHART_EXPORT RelativePosition
{
public:
    RelativePosition();
    RelativePosition( const RelativePosition& );

    ~RelativePosition();

    QDomDocumentFragment toXML() const;

    void setReferenceArea( AbstractArea* area ) { m_area = area; }
    AbstractArea* referenceArea() const { return m_area; }

    void setReferencePosition( const Position& position ) { m_position = position; }
    Position referencePosition() const { return m_position; }

    void setAlignment( Qt::Alignment flags ) { m_alignment = flags; }
    Qt::Alignment alignment() const { return m_alignment; }

    void setHorizontalPadding( const Measure& padding ) { m_horizontalPadding = padding; }
    Measure horizontalPadding() const { return m_horizontalPadding; }

    void setVerticalPadding( const Measure& padding ) { m_verticalPadding = padding; }
    Measure verticalPadding() const { return m_verticalPadding; }

    void setRotation( double rot ) { m_rotation = rot; }
    double rotation() const { return m_rotation; }

    bool operator==( const RelativePosition& );

private:
    AbstractArea* m_area;
    Position m_position;
    Qt::Alignment m_alignment;
    Measure m_horizontalPadding;
    Measure m_verticalPadding;
    double m_rotation;
}; // End of class RelativePosition

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::RelativePosition& );
#endif /* QT_NO_DEBUG_STREAM */


#endif // KDCHARTREALTIVEPOSITION_H

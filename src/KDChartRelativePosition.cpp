/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "KDChartRelativePosition.h"
#include <QtXml/QDomDocumentFragment>

namespace KDChart {

RelativePosition::RelativePosition()
    : m_area(0),
      m_alignment(Qt::AlignCenter),
      m_rotation(0)
{
    //this line left empty intentionally
}

RelativePosition::RelativePosition( const RelativePosition& r )
    : m_area(              r.referenceArea() ),
      m_position(          r.referencePosition() ),
      m_alignment(         r.alignment() ),
      m_horizontalPadding( r.horizontalPadding() ),
      m_verticalPadding(   r.verticalPadding() ),
      m_rotation(          r.rotation() )
{
    //this line left empty intentionally
}

RelativePosition::~RelativePosition()
{
    // this bloc left empty intentionally
}


bool RelativePosition::operator==( const RelativePosition& r )
{
    return( m_area              == r.referenceArea() &&
            m_position          == r.referencePosition() &&
            m_alignment         == r.alignment() &&
            m_horizontalPadding == r.horizontalPadding() &&
            m_verticalPadding   == r.verticalPadding() &&
            m_rotation          == r.rotation() );
}




QDomDocumentFragment RelativePosition::toXML() const
{
    // PENDING(kalle) Implement this
    return QDomDocumentFragment();
}
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::RelativePosition& rp)
{
    dbg << "KDChart::RelativePosition("
	<< "referencearea="<<rp.referenceArea()
	<< "referenceposition="<<rp.referencePosition()
	<< "alignment="<<rp.alignment()
	<< "horizontalpadding="<<rp.horizontalPadding()
	<< "verticalpadding="<<rp.verticalPadding()
	<< "rotation="<<rp.rotation()
	<< ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */

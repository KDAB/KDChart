/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005 KlarÃ¤lvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart licenses may use this file in
 ** accordance with the KDChart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTHEADERFOOTER_H
#define KDCHARTHEADERFOOTER_H

#include <KDChartArea.h>

class QDomDocumentFragment;

namespace KDChart {

    class TextAttributes;

class KDCHART_EXPORT HeaderFooter : public KDChartArea
{
    Q_OBJECT

    Q_DISABLE_COPY( HeaderFooter )
    KDCHART_DECLARE_PRIVATE_DERIVED_QWIDGET( HeaderFooter )

public:
    HeaderFooter( QWidget* parent );
    virtual ~HeaderFooter();

    virtual QDomDocumentFragment toXML() const;

    virtual HeaderFooter * clone() const;

    virtual QSize sizeHint() const;
    virtual void paintEvent( QPaintEvent* );

    // FIXME convenience wrapper around setReferenceArea()?
/*    enum HeaderFooterPosition { Header0Left,
                                Header0Center,
                                Header0Right,
                                Header1Left,
                                Header1Center,
                                Header1Right,
                                Header2Left,
                                Header2Center,
                                Header2Right,
                                Footer0Left,
                                Footer0Center,
                                Footer0Right,
                                Footer1Left,
                                Footer1Center,
                                Footer1Right,
                                Footer2Left,
                                Footer2Center,
                                Footer2Right };*/

    enum HeaderFooterPosition { NorthWest,
                                NorthWest1,
                                NorthWest2,
                                North,
                                North1,
                                North2,
                                NorthEast,
                                NorthEast1,
                                NorthEast2,
                                SouthEast,
                                SouthEast1,
                                SouthEast2,
                                South,
                                South1,
                                South2,
                                SouthWest,
                                SouthWest1,
                                SouthWest2 };

    void setPosition( HeaderFooterPosition position );
    HeaderFooterPosition position() const;

    void setText( const QString& text );
    QString text() const;

    void setTextAttributes( const TextAttributes &a );
    TextAttributes textAttributes() const;

private:
}; // End of class HeaderFooter

}


#endif // KDCHARTHEADERFOOTER_H

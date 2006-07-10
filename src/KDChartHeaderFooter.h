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

#ifndef KDCHARTHEADERFOOTER_H
#define KDCHARTHEADERFOOTER_H

#include "KDChartAbstractArea.h"
#include "KDChartPosition.h"

class QDomDocumentFragment;

namespace KDChart {

    class Chart;
    class TextAttributes;

class KDCHART_EXPORT HeaderFooter : public AbstractArea
{
    Q_OBJECT

    Q_DISABLE_COPY( HeaderFooter )
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( HeaderFooter, Chart* )

public:
    HeaderFooter( Chart* parent );
    virtual ~HeaderFooter();

    virtual QDomDocumentFragment toXML() const;

    virtual HeaderFooter * clone() const;

    virtual void paint( QPainter* );

    enum HeaderFooterType{ Header,
                           Footer };

    void setType( HeaderFooterType type );
    HeaderFooterType type() const;

    void setPosition( Position position );
    Position position() const;

    void setText( const QString& text );
    QString text() const;

    void setTextAttributes( const TextAttributes &a );
    TextAttributes textAttributes() const;

    /** pure virtual in QLayoutItem */
    virtual bool isEmpty() const;
    /** pure virtual in QLayoutItem */
    virtual Qt::Orientations expandingDirections() const;
    /** pure virtual in QLayoutItem */
    virtual QSize maximumSize() const;
    /** pure virtual in QLayoutItem */
    virtual QSize minimumSize() const;
    /** pure virtual in QLayoutItem */
    virtual QSize sizeHint() const;
    /** pure virtual in QLayoutItem */
    virtual void setGeometry( const QRect& r );
    /** pure virtual in QLayoutItem */
    virtual QRect geometry() const;


    /**
      * Called internally by KDChart::Chart
      */
    void setParent( Chart* parent );

signals:
    void destroyedHeaderFooter( HeaderFooter* );

private:
}; // End of class HeaderFooter

}


#endif // KDCHARTHEADERFOOTER_H

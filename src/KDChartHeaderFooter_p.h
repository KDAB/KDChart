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

#ifndef KDCHARTHEADERFOOTER_P_H
#define KDCHARTHEADERFOOTER_P_H

#include "KDChartHeaderFooter.h"
#include "KDChartAbstractArea_p.h"
#include <QMap>
class KDTextDocument;

namespace KDChart {
    class TextAttributes;
    class HeaderFooter;
}
using KDChart::TextAttributes;
using KDChart::HeaderFooter;

class KDChart::HeaderFooter::Private : public AbstractArea::Private
{
    friend class KDChart::HeaderFooter;
public:
    Private();
    ~Private();

private:
    // user-settable
    HeaderFooterType type;
    Position position;
    QString text;
    TextAttributes textAttributes;

    // internal
    mutable KDTextDocument* textDoc;
    QRectF rect;
};

inline KDChart::HeaderFooter::HeaderFooter( Private* d, QWidget* parent )
    : AbstractArea( d, parent ) { init(); }
inline KDChart::HeaderFooter::Private * KDChart::HeaderFooter::d_func()
{ return static_cast<Private*>( AbstractArea::d_func() ); }
inline const KDChart::HeaderFooter::Private * KDChart::HeaderFooter::d_func() const
{ return static_cast<const Private*>( AbstractArea::d_func() ); }




#endif /* KDCHARTHEADERFOOTER_P_H */


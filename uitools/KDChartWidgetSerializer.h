/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef __KDCHART_WIDGET_SERIALIZER_H__
#define __KDCHART_WIDGET_SERIALIZER_H__

#include "kdchart_export.h"

QT_BEGIN_NAMESPACE
class DomWidget;
QT_END_NAMESPACE

namespace KDChart
{
    class Widget;
    class UITOOLS_EXPORT WidgetSerializer
    {
    public:
        WidgetSerializer();
        ~WidgetSerializer();

        static bool saveExtraInfo( const Widget* widget, DomWidget *ui_widget );
        static bool loadExtraInfo( Widget* widget, DomWidget *ui_widget );
    };

}

#endif

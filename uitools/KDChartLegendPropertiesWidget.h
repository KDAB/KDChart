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
#ifndef __KDCHART_LEGEND_PROPERTIES_WIDGET_H__
#define __KDCHART_LEGEND_PROPERTIES_WIDGET_H__

#include <QWidget>

#include "kdchart_export.h"
#include "KDChartGlobal.h"

namespace KDChart {

    class Legend;

    /**
     * \class LegendPropertiesWidget KDChartLegendPropertiesWidget.h
     * \brief 
     *
     * 
     */
    class KDCHART_EXPORT LegendPropertiesWidget : public QWidget
    {
        Q_OBJECT

        Q_DISABLE_COPY( LegendPropertiesWidget )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( LegendPropertiesWidget )

    public:
        /**
         * Default Constructor
         *
         * Creates a new widget with all data initialized empty.
         *
         * \param parent the widget parent; passed on to QWidget
         */
        LegendPropertiesWidget( QWidget* parent = 0 );

        /** Destructor. */
        ~LegendPropertiesWidget();

        void setLegend( Legend* legend );

        void setInstantApply( bool value );
    };
}

#endif // KDChart_LEGEND_PROPERTIES_Widget_H

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

#ifndef __KDCHART_AXIS_PROPERTIES_WIDGET_H__
#define __KDCHART_AXIS_PROPERTIES_WIDGET_H__

#include <QWidget>

#include "kdchart_export.h"
#include "ui_KDChartAxisPropertiesWidget.h"
#include "KDChartGlobal.h"

namespace KDChart {

    class CartesianAxis;


    /**
     * \class AxisPropertiesWidget KDChartAxisPropertiesWidget.h
     * \brief
     *
     *
     */
    class UITOOLS_EXPORT AxisPropertiesWidget
	    : public QWidget, private Ui::KDChartAxisPropertiesWidget
    {
        Q_OBJECT

        Q_DISABLE_COPY( AxisPropertiesWidget )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( AxisPropertiesWidget )

    public:
        /**
         * Default Constructor
         *
         * Creates a new widget with all data initialized empty.
         *
         * \param parent the widget parent; passed on to QWidget
         */
        AxisPropertiesWidget( QWidget* parent = 0 );

        /** Destructor. */
        ~AxisPropertiesWidget();

        void setAxis( CartesianAxis* axis );
        void setInstantApply( bool value );
	void readFromAxis( const CartesianAxis * axis );
        void writeToAxis( CartesianAxis * axis );

    protected Q_SLOTS:

        void slotTitleChanged( const QString& text );
	void slotPositionChanged( int idx );

    Q_SIGNALS:
        // emitted whenever a value in the dialog changes
        void changed();
    };
}

#endif // KDChart_AXIS_PROPERTIES_Widget_H

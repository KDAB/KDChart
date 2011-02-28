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

#include "kdchartextrainfoextension.h"

#include <QObject>
#include <QDebug>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesignerUI4>

#include <KDChartChart>
#include <KDChartWidget>
#include <KDChartWidgetSerializer.h>
#include <KDChartChartSerializer.h>

using namespace KDChart;

KDChartExtraInfoExtension::KDChartExtraInfoExtension(Chart *chart, QDesignerFormEditorInterface *desCore, QObject *parentObj)
  : QObject(parentObj), QDesignerExtraInfoExtension(),
    mChart(chart), mCore(desCore)
{
}

bool KDChartExtraInfoExtension::saveWidgetExtraInfo(DomWidget *ui_widget)
{
    ChartSerializer s;
    return s.saveExtraInfo( mChart, ui_widget );
}

bool KDChartExtraInfoExtension::loadWidgetExtraInfo(DomWidget *ui_widget)
{
    ChartSerializer s;
    return s.loadExtraInfo( mChart, ui_widget );
}

QWidget * KDChartExtraInfoExtension::widget() const { return mChart; }
QDesignerFormEditorInterface * KDChartExtraInfoExtension::core() const { return mCore; }

////

KDChartWidgetExtraInfoExtension::KDChartWidgetExtraInfoExtension(Widget *widget, QDesignerFormEditorInterface *desCore, QObject *parentObj)
    : QObject(parentObj), QDesignerExtraInfoExtension(),
    mWidget(widget), mCore(desCore)
{
}

bool KDChartWidgetExtraInfoExtension::saveWidgetExtraInfo(DomWidget *ui_widget)
{
    WidgetSerializer s;
    return s.saveExtraInfo( mWidget, ui_widget );
}

bool KDChartWidgetExtraInfoExtension::loadWidgetExtraInfo(DomWidget *ui_widget)
{
    WidgetSerializer s;
    return s.loadExtraInfo( mWidget, ui_widget );
}

////

KDChartExtraInfoExtensionFactory::KDChartExtraInfoExtensionFactory(QDesignerFormEditorInterface *core, QExtensionManager *parentObj )
  : QExtensionFactory( parentObj ), mCore( core )
{
}

QObject * KDChartExtraInfoExtensionFactory::createExtension(QObject *object, const QString &iid, QObject *parentObj) const
{
  if (iid != Q_TYPEID(QDesignerExtraInfoExtension))
    return 0;

  if (Chart *w = qobject_cast<Chart*>(object))
    return new KDChartExtraInfoExtension( w, mCore, parentObj );

  if (Widget *w = qobject_cast<Widget*>(object))
      return new KDChartWidgetExtraInfoExtension( w, mCore, parentObj );

  return 0;
}

QWidget * KDChartWidgetExtraInfoExtension::widget() const { return mWidget; }
QDesignerFormEditorInterface * KDChartWidgetExtraInfoExtension::core() const { return mCore; }

/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTDESIGNEREXTENSION_H
#define KDCHARTDESIGNEREXTENSION_H

#include "kdchart_export.h"
#include <QObject>
#include <QList>

// QtDesigner module includes
#include <taskmenu.h>
#include <default_extensionfactory.h>

namespace KDChart {
  class Widget;
  class Chart;
}

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

// This is for extending the popupmenu on charts, to add "Edit chart properties..."
class KDCHART_PLUGIN_EXPORT KDChartBaseTaskMenu: public QObject, public QDesignerTaskMenuExtension
{
  Q_OBJECT
  Q_INTERFACES(QDesignerTaskMenuExtension)
  Q_DISABLE_COPY( KDChartBaseTaskMenu )
public:
    explicit KDChartBaseTaskMenu( QObject *parentW );
    ~KDChartBaseTaskMenu();

  /* \reimp */
  QAction *preferredEditAction() const;
  /* \reimp */
  QList<QAction*> taskActions() const;

protected:
    virtual void doEditChartProperties() = 0;

private slots:
  void editChartProperties();

private:
  QAction * mEditPropertiesAction;

};

class KDCHART_PLUGIN_EXPORT KDChartWidgetTaskMenu : public KDChartBaseTaskMenu
{
    public:
        KDChartWidgetTaskMenu( KDChart::Widget * chartWidget, QObject * parentW );
    protected:
        void doEditChartProperties();
    private:
        KDChart::Widget * mChart;
};

class KDCHART_PLUGIN_EXPORT KDChartChartTaskMenu : public KDChartBaseTaskMenu
{
    public:
        KDChartChartTaskMenu( KDChart::Chart * chart, QObject * parentW );
    protected:
        void doEditChartProperties();
    private:
        KDChart::Chart * mChart;
};

class KDCHART_PLUGIN_EXPORT KDChartWidgetTaskMenuFactory: public QExtensionFactory
{
    Q_OBJECT
    Q_DISABLE_COPY( KDChartWidgetTaskMenuFactory )
public:
    explicit KDChartWidgetTaskMenuFactory( QExtensionManager *extMgr = 0 );

protected:
    virtual QObject * createExtension( QObject *object, const QString &iid, QObject *parentW ) const;
};

class KDCHART_PLUGIN_EXPORT KDChartChartTaskMenuFactory: public QExtensionFactory
{
    Q_OBJECT
    Q_DISABLE_COPY( KDChartChartTaskMenuFactory )
public:
    explicit KDChartChartTaskMenuFactory( QExtensionManager *extMgr = 0 );

protected:
    virtual QObject * createExtension( QObject *object, const QString &iid, QObject *parentW ) const;
};


#endif /* KDCHARTDESIGNEREXTENSION_H */

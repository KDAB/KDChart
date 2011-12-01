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

#ifndef KDCHARTEXTRAINFOEXTENSION_H
#define KDCHARTEXTRAINFOEXTENSION_H

#include "kdchart_export.h"

#include <QtDesigner/QDesignerExtraInfoExtension>
#include <QtDesigner/QExtensionFactory>

#include <qpointer.h>

QT_BEGIN_NAMESPACE
class QDesignerFormEditorInterface;
QT_END_NAMESPACE

namespace KDChart {
  class Chart;
  class Widget;
}
#if QT_VERSION < 0x040300
// Workaround Qt typo; fixed in 4.3
class DomUi;
#define DomUI DomUi
#endif

class KDCHART_PLUGIN_EXPORT KDChartExtraInfoExtension : public QObject, public QDesignerExtraInfoExtension
{
  Q_OBJECT
  Q_INTERFACES(QDesignerExtraInfoExtension)
  Q_DISABLE_COPY( KDChartExtraInfoExtension )
public:
    KDChartExtraInfoExtension( KDChart::Chart *chart, QDesignerFormEditorInterface *core, QObject *parent);

  virtual QWidget *widget() const;
  virtual QDesignerFormEditorInterface *core() const;

  virtual bool saveUiExtraInfo(DomUI *) { return false; }
  virtual bool loadUiExtraInfo(DomUI *) { return false; }

  virtual bool saveWidgetExtraInfo(DomWidget *ui_widget);
  virtual bool loadWidgetExtraInfo(DomWidget *ui_widget);
private:
  QPointer<KDChart::Chart> mChart;
  QPointer<QDesignerFormEditorInterface> mCore;
};

class KDCHART_PLUGIN_EXPORT KDChartWidgetExtraInfoExtension : public QObject, public QDesignerExtraInfoExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerExtraInfoExtension)
    Q_DISABLE_COPY( KDChartWidgetExtraInfoExtension )
    public:
        KDChartWidgetExtraInfoExtension( KDChart::Widget *w,
                                         QDesignerFormEditorInterface *core, QObject *parent);

        virtual QWidget *widget() const;
        virtual QDesignerFormEditorInterface *core() const;

        virtual bool saveUiExtraInfo(DomUI *) { return false; }
        virtual bool loadUiExtraInfo(DomUI *) { return false; }

        virtual bool saveWidgetExtraInfo(DomWidget *ui_widget);
        virtual bool loadWidgetExtraInfo(DomWidget *ui_widget);
    private:
        QPointer<KDChart::Widget> mWidget;
        QPointer<QDesignerFormEditorInterface> mCore;
};

class KDCHART_PLUGIN_EXPORT KDChartExtraInfoExtensionFactory: public QExtensionFactory
{
  Q_OBJECT
  Q_DISABLE_COPY( KDChartExtraInfoExtensionFactory )
public:
  explicit KDChartExtraInfoExtensionFactory(QDesignerFormEditorInterface *core, QExtensionManager *parent = 0);

protected:
  virtual QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;

private:
  QDesignerFormEditorInterface *mCore;
};

#endif /* KDCHARTEXTRAINFOEXTENSION_H */

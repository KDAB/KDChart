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

#ifndef __KDCHART_PLUGINS_PLUGINS_H__
#define __KDCHART_PLUGINS_PLUGINS_H__

#include <QDesignerCustomWidgetCollectionInterface>

///\brief Qt Designer plug-in widget.
///
///See Qt Designer documentation for details on the following methods.
class KDChartWidgetPlugin : public QObject, public QDesignerCustomWidgetCollectionInterface {
  Q_OBJECT
  Q_INTERFACES( QDesignerCustomWidgetCollectionInterface )
public:
  explicit KDChartWidgetPlugin( QObject * parent=0 );
  QList<QDesignerCustomWidgetInterface*> customWidgets() const { return mCustomWidgets; }
private:
  QList<QDesignerCustomWidgetInterface*> mCustomWidgets;
};

#endif // __KDCHART_PLUGINS_PLUGINS_H__

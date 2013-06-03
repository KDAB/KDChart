/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef DIAGRAMTYPEDIALOG_H
#define DIAGRAMTYPEDIALOG_H

#include <QDialog>
#include <QtCore/QHash>

class QAbstractItemModel;
namespace KDChart
{
    class Chart;
}

class DiagramTypeDialog : public QDialog
{
    Q_OBJECT
public:
    enum DiagramType{ Bar, LyingBar, Line, Plotter, Pie };
    enum Subtype{ Normal, Percent, Stacked };
    explicit DiagramTypeDialog( KDChart::Chart *chart, QWidget *parent = 0 );
    ~DiagramTypeDialog();

    void setDefaultModels( QHash< DiagramType, QAbstractItemModel* > models );

Q_SIGNALS:
    void diagramTypeChanged( DiagramType, Subtype );

private:
    class Private;
    Private *d;
};

#endif // DIAGRAMTYPEDIALOG_H

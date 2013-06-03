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

#ifndef DATASETSETTINGS_H
#define DATASETSETTINGS_H

#include <QWidget>

namespace KDChart{
    class Chart;
}

class DatasetSettings : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( int datasetCount READ datasetCount WRITE setDatasetCount NOTIFY datasetCountChanged() )

public:
    explicit DatasetSettings( KDChart::Chart *chart, QWidget *parent = 0 );
    ~DatasetSettings();
    int datasetCount() const;
public Q_SLOTS:
    void setDatasetCount( int value );
    void indexChanged( int index );
    void diagramTypeChanged();

Q_SIGNALS:
    void datasetCountChanged();

private:
    class Private;
    Private *d;
};

#endif // DATASETSETTINGS_H

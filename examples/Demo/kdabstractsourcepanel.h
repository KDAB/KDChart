/****************************************************************************
 ** Copyright (C) 2007 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDABSTRACTSOURCEPANEL_H
#define KDABSTRACTSOURCEPANEL_H

class TableModel;
class QModelIndex;
class QAbstractTableModel;

#include <QtCore/QObject>

class KDAbstractSourcePanel : protected QObject
{
    Q_OBJECT

public:
    virtual ~KDAbstractSourcePanel();
    virtual const QStringList sourceNames() = 0;
    virtual QAbstractTableModel* table() = 0;
    virtual void reset() = 0;

public Q_SLOTS:
    virtual void currentCellChanged(const QModelIndex &index) = 0;

protected:
    TableModel* m_table;
};

#endif

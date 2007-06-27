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

#include <QtGui/QMainWindow>
#include <QtCore/QModelIndex>
#include <QtCore/QAbstractTableModel>

class TableModel;
#include "kdabstractsourcepanel.h"
#include "ui_kdbasicsourcepanel.h"

#define RANDOM_DIVIDEND 100000

class KDBasicSourcePanel : public QWidget, public KDAbstractSourcePanel, private Ui::KDBasicSourcePanel
{
    Q_OBJECT

public:
    KDBasicSourcePanel( QWidget* parent = 0 );
    ~KDBasicSourcePanel();
    // returns the table
    QAbstractTableModel* table();

public Q_SLOTS:
    void setTableModel();
    void updateTableModel();
    void updateCell(int value);
    void currentCellChanged(const QModelIndex &index);
    const QStringList sourceNames();
    void changeRow(int value);
    void reset();

private:
    inline int random(){ return rand()/RANDOM_DIVIDEND;}
    void setupSlider(QAbstractSlider* slider);
    int sliderPosition;

    int m_currentRowIndex;
    int m_currentColumnIndex;
    QMap< int, QAbstractSlider*> m_elements;
    QStringList m_sliderNames;

    int m_oldValue;
};

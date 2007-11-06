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

#include "kdbasicsourcepanel.h"
#include "tablemodel.h"

// maximal 10 here!
static const int COLUMNS = 6;
static const int ROWS = 5;

KDBasicSourcePanel::KDBasicSourcePanel( QWidget* parent )
    : QWidget( parent )
{
    // set the window
    setupUi(this);

    // set the data model
    m_table = new TableModel(COLUMNS);

    setTableModel();

    // setup the input elements
    sliderPosition = 0;
    setupSlider(Dial1);
    setupSlider(Dial2);
    setupSlider(Dial3);
    setupSlider(Slider1);
    setupSlider(Slider2);
    setupSlider(Slider3);

    // set system to the first row
    reset();

    QObject::connect(changeRowSlider,SIGNAL(valueChanged(int)), static_cast<QWidget*>(this), SLOT(changeRow(int)));
    QObject::connect(tableModelResetButton, SIGNAL(clicked()), static_cast<QWidget*>(this), SLOT(updateTableModel()));

    m_oldValue = 0;
}

KDBasicSourcePanel::~KDBasicSourcePanel()
{
    m_table->~TableModel();
    m_sliderNames.~QStringList();
    m_elements.~QMap< int, QAbstractSlider*>();
}

void KDBasicSourcePanel::setTableModel()
{
    // fill the data model with random numbers
    qsrand(9);
    for(int i = 0; i < ROWS; i++) {
        QVector< QVariant > row = m_table->getRow();
        for(int col = 0; col < COLUMNS;) {
            row[col++] = qrand();
        }

        QString headerString = tr("row %1").arg(i);
        m_table->addHeaderData(i, Qt::Vertical, QVariant(headerString));
        m_table->appendData( row );
    }
}

void KDBasicSourcePanel::updateTableModel()
{
    // fill the data model with random numbers
    qsrand(9);
    for(int i = 0; i < ROWS; i++) {
        for(int col = 0; col < COLUMNS;col++) {
            const QModelIndex index_ = m_table->getIndex(i, col);
            m_table->setData(index_, qrand(), Qt::EditRole);
        }
    }

    changeRowSlider->setValue(0);
}

/**
 *	table()
 *	@return	QAbstractTableModel*	returns the table
 */
QAbstractTableModel* KDBasicSourcePanel::table()
{
    return m_table;
}

/**
 *	updateCell()
 *	Slot called by the slider
 *	updates the cell related to the slider
 *
 *	@arg	int	value				the new value
 */

void KDBasicSourcePanel::updateCell(int value)
{
    QAbstractSlider* slider = qobject_cast<QAbstractSlider*>(this->QWidget::sender());
    int column = m_elements.key(slider);
    QModelIndex index = m_table->getIndex(m_currentRowIndex,column);
    m_table->setData(index, QVariant(value), Qt::EditRole);
}

/**
 *	currentCellChanged()
 *  Slot called by the KDChart Widget, tells the selected row and column
 *	and sets all sliders to the values in the row
 *
 *	@arg	const QModelIndex &index	the index of the selected cell
 */

void KDBasicSourcePanel::currentCellChanged(const QModelIndex &index)
{
    // set all sliders in the row
    m_currentRowIndex = index.row();
    m_currentColumnIndex = index.column();
    for(int i = 0; i < COLUMNS; i++){
        QModelIndex index_ = m_table->getIndex(index.row(), i);
        m_elements[i]->setValue(m_table->data(index_, Qt::EditRole).toInt());
    }
}

/**
 *	setupSlider()
 *	sets the given slider to the given position
 */

void KDBasicSourcePanel::setupSlider(QAbstractSlider* slider)
{
    slider->setMaximum(RAND_MAX);
    m_elements[sliderPosition] = slider;
    // sliders call updateCell if changed
    QObject::connect( slider, SIGNAL(valueChanged(int)), this, SLOT(updateCell(int)) );
    m_table->addHeaderData(sliderPosition,Qt::Horizontal, QVariant(slider->objectName()));
    m_sliderNames << slider->objectName();
    sliderPosition++;
}

const QStringList KDBasicSourcePanel::sourceNames()
{
    return m_sliderNames;
}

void KDBasicSourcePanel::changeRow(int value)
{
    bool toggle = true;
    m_currentRowIndex = 1;
    for(int j=0;j<m_table->columnCount();j++){
        for(int i=0; i < m_table->rowCount(); i++)
        {
            const QModelIndex index_ = m_table->getIndex(i, j);
            QVariant oldValue = m_table->data(index_);

            int ascending;
            int cascading;

            if(m_oldValue < value)
            {
                ascending = 500;
                cascading = -500;
            } else {
                ascending = -500;
                cascading = 500;
            }

            int newValue;
            if(toggle)
            {
                newValue = ((qrand()/1000)*value)+ascending;
                toggle = false;
            } else {
                newValue = ((qrand()/1000)*value)+cascading;
                toggle = true;
            }

            const QVariant date(oldValue.toInt()+(newValue));
            m_table->setData(index_,date, Qt::EditRole);

        }
    }
    m_oldValue = value;

}

void KDBasicSourcePanel::reset()
{
    currentCellChanged(m_table->getIndex(0,0));
}

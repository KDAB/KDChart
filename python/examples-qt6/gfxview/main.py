#!/usr/bin/env python

##
## This file is part of the KD Chart library.
##
## SPDX-FileCopyrightText: 2019-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
##
## SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
##
## Licensees holding valid commercial KD Chart licenses may use this file in
## accordance with the KD Chart Commercial License Agreement provided with
## the Software.
##
## Contact info@kdab.com if any conditions of this licensing are not
## clear to you.
##

import sys
import random

from PySide6.QtCore import QModelIndex, Qt, QDateTime
from PySide6.QtGui import QStandardItemModel, QStandardItem
from PySide6.QtWidgets import QApplication 
from PyKDChartQt6.KDGantt import AbstractRowController, Span, GraphicsView, StyleOptionGanttItem
from PyKDChartQt6 import KDGantt

class MyRowController(AbstractRowController):
    ROW_HEIGHT = 30

    def __init__(self):
        super(MyRowController, self).__init__()
        self.m_model = None
    
    def setModel(self, model):
        self.m_model = model

    def headerHeight(self):
        return 40

    def isRowVisible(self, index):
        return True

    def isRowExpanded(self, index):
        return False

    def rowGeometry(self, index):
        return Span(index.row() * self.ROW_HEIGHT, self.ROW_HEIGHT)

    def maximumItemHeight(self):
        return self.ROW_HEIGHT/2

    def totalHeight(self):
        if self.m_model == None:
            return 0
        else:
            return self.m_model.rowCount()*self.ROW_HEIGHT

    def indexAt(self, height):
        return self.m_model.index(height/self.ROW_HEIGHT, 0)

    def indexBelow(self, idx):
        if idx.isValid():
            return idx.model().index(idx.row()+1, idx.column(), idx.parent())
        else:
            return QModelIndex()


    def indexAbove(self, idx):
        if idx.isValid():
            return idx.model().index(idx.row()-1, idx.column(), idx.parent())
        else:
            return QModelIndex()

class MyStandardItemModel(QStandardItemModel):
    def setData(self, idx, value, role):
        super(MyStandardItemModel, self).setData(idx, value, rore)

def random_alignment():
    r = random.randint(0, 3)
    if r == 0:
         Qt.AlignTop
    elif r == 1:
        return Qt.AlignBottom
    elif r == 2:
        return Qt.AlignCenter

def random_position():
    r = random.randint(0, 3)
    if r == 0:
         StyleOptionGanttItem.Left
    elif r == 1:
        return StyleOptionGanttItem.Right
    elif r == 2:
        return StyleOptionGanttItem.Center

if __name__ == '__main__':
    app = QApplication(sys.argv)

    view = GraphicsView()
    rowController = MyRowController()
    view.setReadOnly(True);
    view.setRowController(rowController);
    view.show();

    model = MyStandardItemModel()
    for i in range(0, 10):
        items = []
        for j in range(0, 10):
            item = QStandardItem()
            item.setText("Item " + str(i))
            item.setData(2, KDGantt.ItemTypeRole);
            item.setData(QDateTime.currentDateTime().addDays(2 * j).addMSecs(100000 * i), KDGantt.StartTimeRole);
            item.setData(QDateTime.currentDateTime().addDays(2 * j + 1).addMSecs(100000 * i), KDGantt.EndTimeRole);
            item.setData(random_alignment(), Qt.TextAlignmentRole)
            item.setData(random_position(), KDGantt.TextPositionRole)
            item.setFlags(item.flags() & ~Qt.ItemIsSelectable)
            items.append(item)
        model.appendRow(items)

    rowController.setModel(model);
    view.setModel(model);
    sys.exit(app.exec_())


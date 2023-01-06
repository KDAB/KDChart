#!/usr/bin/env python

#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

''' GraphicsView Example '''

# pylint: disable=missing-function-docstring,missing-class-docstring

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
        super().__init__()
        self.model = None

    def setModel(self, newModel):
        self.model = newModel

    def headerHeight(self):  # pylint: disable=no-self-use
        return 40

    def isRowVisible(self, index):  # pylint: disable=no-self-use
        del index
        return True

    def isRowExpanded(self, index):  # pylint: disable=no-self-use
        del index
        return False

    def rowGeometry(self, index):
        return Span(index.row() * self.ROW_HEIGHT, self.ROW_HEIGHT)

    def maximumItemHeight(self):
        return self.ROW_HEIGHT/2

    def totalHeight(self):
        if self.model is None:
            return 0
        return self.model.rowCount()*self.ROW_HEIGHT

    def indexAt(self, height):
        return self.model.index(height/self.ROW_HEIGHT, 0)

    def indexBelow(self, idx):  # pylint: disable=no-self-use
        if idx.isValid():
            return idx.model().index(idx.row()+1, idx.column(), idx.parent())
        return QModelIndex()

    def indexAbove(self, idx):  # pylint: disable=no-self-use
        if idx.isValid():
            return idx.model().index(idx.row()-1, idx.column(), idx.parent())
        return QModelIndex()


def randomAlignment():
    r = random.randint(0, 3)
    if r == 1:
        return Qt.AlignBottom
    if r == 2:
        return Qt.AlignCenter
    return Qt.AlignTop


def randomPosition():
    r = random.randint(0, 3)
    if r == 1:
        return StyleOptionGanttItem.Right
    if r == 2:
        return StyleOptionGanttItem.Center
    return StyleOptionGanttItem.Left


if __name__ == '__main__':
    app = QApplication(sys.argv)

    view = GraphicsView()
    rowController = MyRowController()
    view.setReadOnly(True)
    view.setRowController(rowController)
    view.show()

    model = QStandardItemModel()
    for i in range(0, 10):
        items = []
        for j in range(0, 10):
            item = QStandardItem()
            item.setText("Item " + str(i))
            item.setData(2, KDGantt.ItemTypeRole)
            item.setData(QDateTime.currentDateTime().addDays(
                2 * j).addMSecs(100000 * i), KDGantt.StartTimeRole)
            item.setData(QDateTime.currentDateTime().addDays(
                2 * j + 1).addMSecs(100000 * i), KDGantt.EndTimeRole)
            item.setData(randomAlignment(), Qt.TextAlignmentRole)
            item.setData(randomPosition(), KDGantt.TextPositionRole)
            item.setFlags(item.flags() & ~Qt.ItemIsSelectable)
            items.append(item)
        model.appendRow(items)

    rowController.setModel(model)
    view.setModel(model)
    sys.exit(app.exec())

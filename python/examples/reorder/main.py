#!/usr/bin/env python

#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

''' Test model that allows reoordering the rows '''

# pylint: disable=missing-class-docstring,missing-function-docstring

import sys

from PySide2.QtCore import Qt,  QDateTime, QAbstractTableModel, QTimer, QModelIndex
from PySide2.QtWidgets import QApplication
from PyKDChart.KDGantt import View, Constraint, ConstraintModel
from PyKDChart import KDGantt


## Test model that uses beginMoveRows() ##
# pylint: disable=too-few-public-methods
class MyTaskModel(QAbstractTableModel):
    class Task():
        def __init__(self):
            self.title = ""
            self.start = QDateTime()
            self.end = QDateTime()

    def __init__(self, parent=None):
        super().__init__(parent)
        self.tasks = []

        for i in range(0, 6, 2):
            task = self.Task()
            task.title = "Item " + str(len(self.tasks) + 1)
            task.start = QDateTime.currentDateTime().addDays(i)
            task.end = task.start.addDays(1)
            self.tasks.append(task)

    def rowCount(self, index=QModelIndex()):
        if index.isValid():
            return 0
        return len(self.tasks)

    def columnCount(self, index=QModelIndex()):  # pylint: disable=no-self-use
        if index.isValid():
            return 0
        return 4

    def data(self, index, role=Qt.DisplayRole):
        if index.isValid() and index.row() < self.rowCount() and index.column() < self.columnCount():
            column = index.column()
            row = index.row()
            if column == 0 and role == Qt.DisplayRole:
                return self.tasks[row].title
            if column == 1 and role == Qt.DisplayRole:
                return KDGantt.TypeTask
            if column == 2 and (role in [KDGantt.StartTimeRole, Qt.DisplayRole]):
                return self.tasks[row].start
            if column == 3 and (role in [KDGantt.EndTimeRole, Qt.DisplayRole]):
                return self.tasks[row].end

        return None

    def moveRow(self, fromIndex, toIndex):
        if fromIndex == toIndex:
            return

        if fromIndex >= len(self.tasks) or toIndex >= len(self.tasks)+1:
            return

        if self.beginMoveRows(QModelIndex(), fromIndex, fromIndex, QModelIndex(), toIndex):
            self.tasks.insert(toIndex, self.tasks.pop(fromIndex))
            self.endMoveRows()
        else:
            return


if __name__ == '__main__':
    app = QApplication(sys.argv)
    view = View()
    model = MyTaskModel()

    constraints = ConstraintModel()
    constraints.addConstraint(Constraint(model.index(0, 0), model.index(1, 0)))
    constraints.addConstraint(Constraint(model.index(0, 0), model.index(2, 0)))
    constraints.addConstraint(Constraint(model.index(1, 0), model.index(3, 0)))

    view.setModel(model)
    view.setConstraintModel(constraints)
    view.show()

    # After 5 seconds, move row 1 toIndex pos 0: #
    QTimer.singleShot(5000, lambda: model.moveRow(1, 0))

    sys.exit(app.exec_())

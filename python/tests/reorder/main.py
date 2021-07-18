#!/usr/bin/env python

##
## Copyright (C) 2019-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
##
## This file is part of the KD Chart library.
##
## Licensees holding valid commercial KD Chart licenses may use this file in
## accordance with the KD Chart Commercial License Agreement provided with
## the Software.
##
##
## This file may be distributed and/or modified under the terms of the
## GNU General Public License version 2 and version 3 as published by the
## Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
##
## This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
## WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
## Contact info@kdab.com if any conditions of this licensing are not
## clear to you.
##

import sys
import random

from PySide2.QtCore import Qt, QObject, QDateTime, QAbstractTableModel, QTimer, QModelIndex
from PySide2.QtWidgets import QApplication
from PyKDChart.KDGantt import View, Constraint, ConstraintModel
from PyKDChart import KDGantt


## Test model that uses beginMoveRows() ##
class MyTaskModel(QAbstractTableModel):
    class Task(object):
        def __init__(self):
            self.title  = ""
            self.start = QDateTime()
            self.end = QDateTime()


    def __init__(self, parent = None):
        super(MyTaskModel, self).__init__(parent)
        self.m_tasks = []

        for i in range(0, 6, 2):
            task = self.Task()
            task.title = "Item " + str(len(self.m_tasks) + 1)
            task.start = QDateTime.currentDateTime().addDays(i)
            task.end = task.start.addDays(1)
            self.m_tasks.append(task)

    def rowCount(self, index = QModelIndex()):
        if index.isValid():
            return 0
        else:
            return len(self.m_tasks)

    def columnCount(self, index = QModelIndex()):
        if index.isValid():
            return 0
        else:
            return 4

    def data(self, index, role = Qt.DisplayRole):
        if index.isValid() and index.row() < self.rowCount() and index.column() < self.columnCount():
            column = index.column()
            row = index.row()
            if column == 0 and role == Qt.DisplayRole:
                return self.m_tasks[row].title  
            elif column == 1 and role == Qt.DisplayRole:
                return KDGantt.TypeTask
            elif column == 2 and (role == KDGantt.StartTimeRole or role == Qt.DisplayRole):
                return self.m_tasks[row].start
            elif column == 3 and (role == KDGantt.EndTimeRole or role == Qt.DisplayRole):
                return self.m_tasks[row].end

        return None

    def moveRow(self, from_, to):
        if from_ == to:
            return

        if from_ >= len(self.m_tasks) or to >= len(self.m_tasks)+1: 
            return

        if self.beginMoveRows(QModelIndex(), from_, from_, QModelIndex(), to):
            self.m_tasks.insert(to, self.m_tasks.pop(from_))
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

    # After 5 seconds, move row 1 to pos 0: #
    QTimer.singleShot(5000, lambda: model.moveRow(1, 0))

    sys.exit(app.exec_())

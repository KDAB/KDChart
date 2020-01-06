#!/usr/bin/env python

##
## Copyright (C) 2019-2020 Klaralvdalens Datakonsult AB.  All rights reserved.
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

from PySide2.QtCore import Qt, QDateTime
from PySide2.QtGui import QStandardItem, QStandardItemModel, QBrush
from PySide2.QtWidgets import QApplication, QListView, QAbstractItemView, QWidget, QSlider, QVBoxLayout
from PyKDChart.KDGantt import View, DateTimeGrid, Constraint, ConstraintModel, ListViewRowController
from PyKDChart import KDGantt

class MyStandardItem(QStandardItem):
    def __init__(self, v, role = Qt.DisplayRole):
        super(MyStandardItem, self).__init__()
        self.setData(v, role)

class MyListView(QListView):
    def __init__(self, parent = None):
        super(MyListView, self).__init__(parent)

        ## Make some space for the header
        self.setViewportMargins(0, self.fontMetrics().height() * 2 + 4 * self.frameWidth(), 0, 0)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setVerticalScrollMode(QAbstractItemView.ScrollPerPixel)

class MyWidget(QWidget):
    def __init__(self, parent = None):
        super(MyWidget, self).__init__(parent)

        self.view = View()
        self.grid = DateTimeGrid()
        self.slider = QSlider(self)
        self.model = QStandardItemModel(self)
        self.cmodel = ConstraintModel()

        ## proxyModel.setSourceModel( &model );
        for i in range(0, 10):
            item = MyStandardItem("Multi Item " + str(i))
            for j in range(0, 20, 3):
                item.appendRow([MyStandardItem("Item " + str(j)),
                                MyStandardItem(KDGantt.TypeTask),
                                MyStandardItem(QDateTime.currentDateTime().addDays(j), KDGantt.StartTimeRole),
                                MyStandardItem(QDateTime.currentDateTime().addDays(j + 1 + i / 7), KDGantt.EndTimeRole),
                                MyStandardItem(50)])

            item.appendRow([MyStandardItem("Event"),
                            MyStandardItem(KDGantt.TypeEvent),
                            MyStandardItem(QDateTime.currentDateTime(), KDGantt.StartTimeRole),
                            MyStandardItem(QDateTime(), KDGantt.EndTimeRole),
                            MyStandardItem("")])

            self.model.appendRow([item,
                                  MyStandardItem(KDGantt.TypeMulti),
                                  MyStandardItem(""),
                                  MyStandardItem(""),
                                  MyStandardItem("")])

        pidx = self.model.index( 0, 0 );
        pidx = self.model.index( 0, 0, pidx );
        self.cmodel.addConstraint(Constraint(self.model.index( 0, 0, pidx), self.model.index( 1, 0, pidx)))
        self.cmodel.addConstraint(Constraint(self.model.index( 1, 0, pidx), self.model.index( 0, 0, pidx)))
        self.cmodel.addConstraint(Constraint(self.model.index( 1, 0, pidx), self.model.index( 10, 0, pidx)))
        self.cmodel.addConstraint(Constraint(self.model.index( 3, 0, pidx), self.model.index( 5, 0, pidx)))
        self.cmodel.addConstraint(Constraint(self.model.index( 7, 0, pidx), self.model.index( 4, 0, pidx)))

        self.slider.setOrientation(Qt.Horizontal)
        self.slider.setRange(1, 1000)
        self.slider.setValue(100)
        l = QVBoxLayout(self)
        l.addWidget(self.view)
        l.addWidget(self.slider)
        self.grid.setStartDateTime(QDateTime.currentDateTime().addDays(-3))
        self.grid.setDayWidth(100)
        self.grid.setFreeDays([Qt.Saturday, Qt.Sunday])
        self.grid.setFreeDaysBrush(QBrush(Qt.red))

        lv = MyListView(self)
        self.view.setLeftView(lv)
        self.view.setRowController(ListViewRowController(lv, self.view.ganttProxyModel()))
        self.view.setGrid(self.grid)
        self.view.setModel(self.model)
        ##view.setConstraintModel( &cmodel );
        self.slider.valueChanged.connect(self.slotZoom)

    def slotZoom(self, z):
        self.grid.setDayWidth(z)

if __name__ == '__main__':
    app = QApplication(sys.argv)

    w = MyWidget()
    w.show()

    sys.exit(app.exec_())


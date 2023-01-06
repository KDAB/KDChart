#!/usr/bin/env python

# Contact info@kdab.com if any conditions of this licensing are not clear to you.
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

''' Basic View Example '''

# pylint: disable=missing-function-docstring,missing-class-docstring

import sys

from PySide2.QtCore import Qt, QDateTime
from PySide2.QtGui import QStandardItem, QStandardItemModel, QPixmap, QPixmapCache, QPainter
from PySide2.QtWidgets import QWidget, QSlider, QVBoxLayout, QPushButton, QMenu, QApplication, QDialog, QLabel
from PySide2.QtPrintSupport import QPrinter, QPrintDialog
from PyKDChart.KDGantt import Constraint, ConstraintModel, DateTimeGrid, View
from PyKDChart import KDGantt


# pylint: disable=too-few-public-methods
class MyStandardItem(QStandardItem):
    def __init__(self, v, role=None):
        super().__init__(None)

        if role is None:
            self.setData(v, Qt.DisplayRole)
        else:
            self.setData(v, role)


class MyWidget(QWidget):
    def __init__(self):
        super().__init__(None)

        self.view = View()
        self.grid = DateTimeGrid()
        self.slider = QSlider()
        self.model = QStandardItemModel()
        self.cmodel = ConstraintModel()

        for h in range(0, 19):
            topitem = MyStandardItem("Top Item " + str(h))
            for i in range(0, 19):
                item = MyStandardItem("Multi Item " + str(i))
                for j in range(0, 29, 3):
                    item.appendRow([MyStandardItem("Item " + str(j)),
                                    MyStandardItem(KDGantt.TypeTask),
                                    MyStandardItem(QDateTime.currentDateTime().addDays(j),
                                                   KDGantt.StartTimeRole),
                                    MyStandardItem(QDateTime.currentDateTime().addDays(int(j+1+i/7)),
                                                   KDGantt.EndTimeRole),
                                    MyStandardItem(50)])

                item.appendRow([MyStandardItem("Event"),
                                MyStandardItem(KDGantt.TypeEvent),
                                MyStandardItem(
                                    QDateTime.currentDateTime(), KDGantt.StartTimeRole),
                                MyStandardItem(
                                    QDateTime(), KDGantt.EndTimeRole),
                                MyStandardItem("")])

                topitem.appendRow([item,
                                   MyStandardItem(KDGantt.TypeMulti),
                                   MyStandardItem(""),
                                   MyStandardItem(""),
                                   MyStandardItem("")])

            self.model.appendRow([topitem,
                                  MyStandardItem(KDGantt.TypeMulti),
                                  MyStandardItem(""),
                                  MyStandardItem(""),
                                  MyStandardItem("")])

        self.model.appendRow([MyStandardItem("No data")])

        # cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 0, 3 ), proxyModel.index( 10, 3 ) ) );
        # cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 10, 3 ), proxyModel.index( 5, 3 ) ) );
        pidx = self.model.index(0, 0)
        pidx = self.model.index(0, 0, pidx)
        self.cmodel.addConstraint(Constraint(
            self.model.index(0, 0, pidx), self.model.index(1, 0, pidx)))
        self.cmodel.addConstraint(Constraint(
            self.model.index(1, 0, pidx), self.model.index(0, 0, pidx)))
        self.cmodel.addConstraint(Constraint(self.model.index(
            1, 0, pidx), self.model.index(10, 0, pidx)))
        self.cmodel.addConstraint(Constraint(
            self.model.index(3, 0, pidx), self.model.index(5, 0, pidx)))
        self.cmodel.addConstraint(Constraint(
            self.model.index(7, 0, pidx), self.model.index(4, 0, pidx)))

        self.slider.setOrientation(Qt.Horizontal)
        self.slider.setRange(1, 10000)
        self.slider.setValue(100)
        l = QVBoxLayout(self)
        l.addWidget(self.view)
        l.addWidget(self.slider)
        self.grid.setStartDateTime(QDateTime.currentDateTime().addDays(-3))
        self.grid.setDayWidth(100)
        # grid.setNoInformationBrush( Qt::NoBrush );
        self.view.setGrid(self.grid)
        self.view.setModel(self.model)
        # view.setConstraintModel( &cmodel );
        self.slider.valueChanged.connect(self.slotZoom)

        pb1 = QPushButton("Print Preview...", self)
        pb2 = QPushButton("Print...", self)

        l.addWidget(pb1)
        l.addWidget(pb2)

        pb1.clicked.connect(self.slotPrintPreview)
        pb2.clicked.connect(self.slotPrint)

        graphicsView = self.view.graphicsView()
        graphicsView.setHeaderContextMenuPolicy(Qt.CustomContextMenu)
        graphicsView.headerContextMenuRequested.connect(self.slotHeaderMenu)

    def slotZoom(self, z):
        self.grid.setDayWidth(z)

    def slotPrintPreview(self):
        pix = QPixmap(1000, 200)
        pix.fill(Qt.white)

        painter = QPainter(pix)
        self.view.print(painter, pix.rect())
        painter.end()

        label = QLabel(self)
        label.setPixmap(pix)
        label.show()

    def slotPrint(self):
        printer = QPrinter(QPrinter.HighResolution)
        printDialog = QPrintDialog(printer, self)

        if printDialog.exec_() == QDialog.Accepted:
            r = self.view.graphicsView().mapToScene(
                self.view.graphicsView().viewport().rect()).boundingRect()
            self.view.print(printer, r.left(), r.right())

    # pylint: disable=no-self-use
    def slotHeaderMenu(self, point):
        menu = QMenu()
        menu.addAction("This")
        menu.addAction("is")
        menu.addAction("just")
        menu.addAction("a")
        menu.addAction("test")
        menu.exec_(point)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    QPixmapCache.setCacheLimit(30*1024)

    w = MyWidget()
    w.show()

    sys.exit(app.exec_())

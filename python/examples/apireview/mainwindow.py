#!/usr/bin/env python

#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

''' MainWindow for the API Review Example '''

# pylint: disable=missing-function-docstring,missing-class-docstring

from PySide2.QtCore import Qt, QDateTime, QModelIndex
from PySide2.QtGui import QStandardItem, QStandardItemModel, QPixmap, QKeySequence, QPainter
from PySide2.QtWidgets import QMainWindow, QAction, QMenu, QLabel, QDialog, QWidget, QMenuBar
from PyKDChart.KDGantt import ConstraintModel, DateTimeGrid, StyleOptionGanttItem, Constraint
from PyKDChart import KDGantt

from ui_mainwindow import Ui_MainWindow
from entrydelegate import EntryDelegate
from entrydialog import EntryDialog

# pylint: disable=too-few-public-methods


class MyStandardItem(QStandardItem):
    def __init__(self, v, role=Qt.DisplayRole):
        super().__init__()
        self.setData(v, role)


class MainWindow(QMainWindow):
    # pylint: disable=too-many-instance-attributes
    def __init__(self, parent=None, flags=Qt.WindowFlags()):
        super().__init__(parent, flags)

        self.dayWidth = 70
        self.ui = Ui_MainWindow()  # pylint: disable=invalid-name
        self.ui.setupUi(self)

        self.initModel()
        self.initActions()
        self.initItemDelegate()
        self.initGrid()

        leftView = self.ui.ganttView.leftView()
        leftView.setColumnHidden(1, True)
        leftView.setColumnHidden(2, True)
        leftView.setColumnHidden(3, True)
        leftView.setColumnHidden(4, True)
        leftView.setColumnHidden(5, True)
        leftView.header().setStretchLastSection(True)

        self.ui.ganttView.leftView().customContextMenuRequested.connect(self.showContextMenu)
        self.ui.ganttView.selectionModel().selectionChanged.connect(self.enableActions)
        self.ui.ganttView.graphicsView().clicked.connect(self.slotClicked)
        self.ui.ganttView.graphicsView().qrealClicked.connect(self.slotDoubleClicked)

    def initModel(self):
        self.model = QStandardItemModel(0, 6, self)
        self.model.setHeaderData(0, Qt.Horizontal, "Task")
        self.ui.ganttView.setModel(self.model)

        self.l = QWidget(self)
        self.l.setWindowTitle("Legend")
        self.l.show()
        # self.l.setModel(self.model)

        self.constraintModel = ConstraintModel(self)
        self.ui.ganttView.setConstraintModel(self.constraintModel)

    def initActions(self):
        self.newEntryAction = QAction("New entry", self)
        self.newEntryAction.setShortcut(QKeySequence.New)
        self.newEntryAction.triggered.connect(self.addNewEntry)

        self.removeEntryAction = QAction("Remove entry", self)
        self.removeEntryAction.setShortcut(QKeySequence.Delete)
        self.removeEntryAction.triggered.connect(self.removeEntry)

        self.demoAction = QAction("Demo entry", self)
        self.demoAction.triggered.connect(self.addDemoEntry)

        self.printAction = QAction("Print Preview...", self)
        self.printAction.triggered.connect(self.printPreview)

        self.zoomInAction = QAction("Zoom In", self)
        self.zoomInAction.setShortcut(QKeySequence.ZoomIn)
        self.zoomInAction.triggered.connect(self.zoomIn)

        self.zoomOutAction = QAction("Zoom Out", self)
        self.zoomOutAction.setShortcut(QKeySequence.ZoomOut)
        self.zoomOutAction.triggered.connect(self.zoomOut)

        self.ui.ganttView.leftView().setContextMenuPolicy(Qt.CustomContextMenu)
        self.ui.ganttView.leftView().addAction(self.newEntryAction)
        self.ui.ganttView.leftView().addAction(self.removeEntryAction)

        menuBar = QMenuBar(self)
        self.setMenuBar(menuBar)
        entryMenu = menuBar.addMenu("Entry")
        entryMenu.addAction(self.newEntryAction)
        entryMenu.addAction(self.removeEntryAction)
        entryMenu.addSeparator()
        entryMenu.addAction(self.demoAction)
        entryMenu.addSeparator()
        entryMenu.addAction(self.printAction)

        zoomMenu = menuBar.addMenu("Zoom")
        zoomMenu.addAction(self.zoomInAction)
        zoomMenu.addAction(self.zoomOutAction)

        # self.enableActions(QItemSelection())

    def initItemDelegate(self):
        delegate = EntryDelegate(self.constraintModel, self)
        self.ui.ganttView.leftView().setItemDelegate(delegate)

    def initGrid(self):
        self.grid = DateTimeGrid()
        self.grid.setDayWidth(self.dayWidth)
        self.ui.ganttView.setGrid(self.grid)

    def showContextMenu(self, pos):
        if not self.ui.ganttView.leftView().indexAt(pos).isValid():
            self.ui.ganttView.selectionModel().clearSelection()

        menu = QMenu(self.ui.ganttView.leftView())
        menu.addAction(self.newEntryAction)
        menu.addAction(self.removeEntryAction)
        menu.exec_(self.ui.ganttView.leftView().viewport().mapToGlobal(pos))

    def enableActions(self, selected):
        if len(selected.indexes()) == 0:
            self.newEntryAction.setEnabled(True)
            self.removeEntryAction.setEnabled(False)
            return

        selectedIndex = selected.indexes()[0]
        dataType = self.model.data(self.model.index(selectedIndex.row(), 1))
        if dataType in [KDGantt.TypeEvent, KDGantt.TypeTask]:
            self.newEntryAction.setEnabled(False)
            self.removeEntryAction.setEnabled(True)
            return

        self.newEntryAction.setEnabled(True)
        self.removeEntryAction.setEnabled(True)

    def addNewEntry(self):
        dialog = EntryDialog(self.model)
        dialog.setWindowTitle("New Entry")
        if dialog.exec_() == QDialog.Rejected:
            dialog = None
            return

        selectedIndexes = self.ui.ganttView.selectionModel().selectedIndexes()
        if len(selectedIndexes) > 0:
            parent = selectedIndexes[0]
        else:
            parent = QModelIndex()

        if not self.model.insertRow(self.model.rowCount(parent), parent):
            return

        row = self.model.rowCount(parent) - 1
        if row == 0 and parent.isValid():
            self.model.insertColumns(self.model.columnCount(parent), 5, parent)

        self.model.setData(self.model.index(row, 0, parent), dialog.name())
        self.model.setData(self.model.index(row, 1, parent), dialog.type())
        if dialog.type() != KDGantt.TypeSummary:
            self.model.setData(self.model.index(row, 2, parent),
                               dialog.startDate(), KDGantt.StartTimeRole)
            self.model.setData(self.model.index(row, 3, parent),
                               dialog.endDate(), KDGantt.EndTimeRole)

        self.model.setData(self.model.index(
            row, 4, parent), dialog.completion())
        self.model.setData(self.model.index(row, 5, parent), dialog.legend())

        self.addConstraint(dialog.depends(), self.model.index(row, 0, parent))
        self.setReadOnly(self.model.index(row, 0, parent), dialog.readOnly())

        dialog = None

    def setReadOnly(self, index, readOnly):
        row = index.row()
        parent = index.parent()

        for column in range(0, 5):
            item = self.model.itemFromIndex(
                self.model.index(row, column, parent))
            flags = None
            if readOnly:
                flags = item.flags() & ~Qt.ItemIsEditable
            else:
                flags = item.flags() | Qt.ItemIsEditable
            item.setFlags(flags)

    def addConstraint(self, index1, index2):
        if not index1.isValid() or not index2.isValid():
            return

        c = Constraint(index1, index2)
        self.ui.ganttView.constraintModel().addConstraint(c)

    def addConstraintFromItem(self, item1, item2):
        self.addConstraint(self.model.indexFromItem(
            item1), self.model.indexFromItem(item2))

    def removeEntry(self):
        selectedIndexes = self.ui.ganttView.selectionModel().selectedIndexes()
        if len(selectedIndexes) > 0:
            index = selectedIndexes[0]
        else:
            index = QModelIndex()

        if not index.isValid():
            return

        self.model.removeRow(index.row(), index.parent())

    def addDemoEntry(self):
        softwareRelease = MyStandardItem("Software Release")
        codeFreeze = MyStandardItem("Code Freeze")
        codeFreeze.setData(KDGantt.TextPositionRole,
                           StyleOptionGanttItem.Right)
        packaging = MyStandardItem("Packaging")
        upload = MyStandardItem("Upload")
        testing = MyStandardItem("Testing")
        updateDocumentation = MyStandardItem("Update Documentation")

        now = QDateTime.currentDateTime()
        softwareRelease.appendRow([codeFreeze,
                                   MyStandardItem(KDGantt.TypeEvent),
                                   MyStandardItem(now, KDGantt.StartTimeRole)])
        softwareRelease.appendRow([packaging,
                                   MyStandardItem(KDGantt.TypeTask),
                                   MyStandardItem(now.addDays(
                                       5), KDGantt.StartTimeRole),
                                   MyStandardItem(now.addDays(10), KDGantt.EndTimeRole)])
        softwareRelease.appendRow([upload,
                                   MyStandardItem(KDGantt.TypeTask),
                                   MyStandardItem(now.addDays(10).addSecs(
                                       2 * 60 * 60), KDGantt.StartTimeRole),
                                   MyStandardItem(now.addDays(11), KDGantt.EndTimeRole)])
        softwareRelease.appendRow([testing,
                                   MyStandardItem(KDGantt.TypeTask),
                                   MyStandardItem(now.addSecs(
                                       3 * 60 * 60), KDGantt.StartTimeRole),
                                   MyStandardItem(now.addDays(5), KDGantt.EndTimeRole)])
        softwareRelease.appendRow([updateDocumentation,
                                   MyStandardItem(KDGantt.TypeTask),
                                   MyStandardItem(now.addSecs(
                                       3 * 60 * 60), KDGantt.StartTimeRole),
                                   MyStandardItem(now.addDays(3), KDGantt.EndTimeRole)])
        self.model.appendRow(
            [softwareRelease, MyStandardItem(KDGantt.TypeSummary)])
        self.addConstraintFromItem(codeFreeze, packaging)
        self.addConstraintFromItem(codeFreeze, testing)
        self.addConstraintFromItem(codeFreeze, updateDocumentation)
        self.addConstraintFromItem(packaging, upload)
        self.addConstraintFromItem(testing, packaging)
        self.addConstraintFromItem(updateDocumentation, packaging)

    def zoomIn(self):
        self.dayWidth += 10
        if self.dayWidth > 400:
            self.grid.setScale(DateTimeGrid.ScaleHour)
        self.grid.setDayWidth(self.dayWidth)

    def zoomOut(self):
        self.dayWidth -= 10

        self.dayWidth = max(self.dayWidth, 10)

        if self.dayWidth <= 400:
            self.grid.setScale(DateTimeGrid.ScaleDay)

        self.grid.setDayWidth(self.dayWidth)

    def printPreview(self):
        preview = QLabel(self, Qt.Window)
        preview.setAttribute(Qt.WA_DeleteOnClose)
        preview.setScaledContents(True)
        preview.setWindowTitle("Print Preview")
        pix = QPixmap(1000, 300)
        pix.fill(Qt.white)

        p = QPainter(pix)
        p.setRenderHints(QPainter.Antialiasing)
        self.ui.ganttView.print_(p, pix.rect())

        preview.setPixmap(pix)
        preview.show()

    def slotClicked(self, index):
        self.statusBar().showMessage("(%d,%d,_,%s) clicked" %
                                     (index.row(), index.column(), index.model()))

    def slotDoubleClicked(self, index):
        self.statusBar().showMessage("(%d,%d,_,%s) qreal clicked" %
                                     (index.row(), index.column(), index.model()))

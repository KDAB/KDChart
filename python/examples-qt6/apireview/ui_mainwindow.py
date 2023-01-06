#!/usr/bin/env python

# Contact info@kdab.com if any conditions of this licensing are not clear to you.
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#


''' MainWindow UI definition for the API Review Example '''

# pylint: disable=missing-function-docstring,missing-class-docstring

from PySide6 import QtCore, QtWidgets
from PyKDChartQt6.KDGantt import View


class Ui_MainWindow():  # pylint: disable=invalid-name
    def __init__(self):
        self.menubar = None
        self.statusbar = None
        self.ganttView = None
        self.vboxlayout = None
        self.centralwidget = None

    def setupUi(self, mainWindow):
        mainWindow.setObjectName("mainWindow")
        mainWindow.resize(556, 330)
        mainWindow.setContextMenuPolicy(QtCore.Qt.DefaultContextMenu)
        self.centralwidget = QtWidgets.QWidget(mainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.vboxlayout = QtWidgets.QVBoxLayout(self.centralwidget)
        self.vboxlayout.setObjectName("vboxlayout")
        self.ganttView = View(self.centralwidget)
        self.ganttView.setObjectName("ganttView")
        self.vboxlayout.addWidget(self.ganttView)
        mainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(mainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 556, 29))
        self.menubar.setObjectName("menubar")
        mainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(mainWindow)
        self.statusbar.setObjectName("statusbar")
        mainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(mainWindow)
        QtCore.QMetaObject.connectSlotsByName(mainWindow)

    # pylint: disable=no-self-use
    def retranslateUi(self, mainWindow):
        _translate = QtCore.QCoreApplication.translate
        mainWindow.setWindowTitle(_translate("mainWindow", "KD Gantt Example"))

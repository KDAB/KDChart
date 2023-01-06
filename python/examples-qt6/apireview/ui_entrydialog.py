#!/usr/bin/env python

#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

''' Entry Dialog UI definition for the API Review Example '''

# pylint: disable=missing-class-docstring,missing-function-docstring

from PySide6 import QtCore, QtWidgets


class Ui_entryDialog():  # pylint: disable=invalid-name
    # pylint: disable=too-many-instance-attributes
    def __init__(self):
        self.name = None
        self.vboxlayout = None
        self.hboxlayout = None
        self.hboxlayout1 = None
        self.gridlayout = None
        self.buttonBox = None
        self.depends = None
        self.dependsLabel = None
        self.label = None
        self.readOnly = None
        self.startDate = None
        self.endDate = None
        self.label2 = None
        self.label3 = None
        self.label4 = None
        self.label5 = None
        self.label6 = None
        self.legend = None
        self.completion = None
        self.type = None

    # pylint: disable=too-many-statements
    def setupUi(self, entryDialog):
        entryDialog.setObjectName("entryDialog")
        entryDialog.resize(439, 171)
        self.vboxlayout = QtWidgets.QVBoxLayout(entryDialog)
        self.vboxlayout.setObjectName("vboxlayout")
        self.hboxlayout = QtWidgets.QHBoxLayout()
        self.hboxlayout.setObjectName("hboxlayout")
        self.label = QtWidgets.QLabel(entryDialog)
        self.label.setObjectName("label")
        self.hboxlayout.addWidget(self.label)
        self.name = QtWidgets.QLineEdit(entryDialog)
        self.name.setObjectName("name")
        self.hboxlayout.addWidget(self.name)
        self.vboxlayout.addLayout(self.hboxlayout)
        self.hboxlayout1 = QtWidgets.QHBoxLayout()
        self.hboxlayout1.setObjectName("hboxlayout1")
        self.label6 = QtWidgets.QLabel(entryDialog)
        self.label6.setObjectName("label6")
        self.hboxlayout1.addWidget(self.label6)
        self.legend = QtWidgets.QLineEdit(entryDialog)
        self.legend.setObjectName("legend")
        self.hboxlayout1.addWidget(self.legend)
        self.vboxlayout.addLayout(self.hboxlayout1)
        self.gridlayout = QtWidgets.QGridLayout()
        self.gridlayout.setObjectName("gridlayout")
        self.label2 = QtWidgets.QLabel(entryDialog)
        self.label2.setObjectName("label2")
        self.gridlayout.addWidget(self.label2, 0, 0, 1, 1)
        self.type = QtWidgets.QComboBox(entryDialog)
        self.type.setObjectName("type")
        self.gridlayout.addWidget(self.type, 0, 1, 1, 1)
        spacerItem = QtWidgets.QSpacerItem(
            41, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridlayout.addItem(spacerItem, 0, 2, 1, 1)
        self.label3 = QtWidgets.QLabel(entryDialog)
        self.label3.setObjectName("label3")
        self.gridlayout.addWidget(self.label3, 0, 3, 1, 1)
        self.startDate = QtWidgets.QDateTimeEdit(entryDialog)
        self.startDate.setCalendarPopup(True)
        self.startDate.setObjectName("startDate")
        self.gridlayout.addWidget(self.startDate, 0, 4, 1, 1)
        self.label5 = QtWidgets.QLabel(entryDialog)
        self.label5.setObjectName("label5")
        self.gridlayout.addWidget(self.label5, 1, 0, 1, 1)
        self.completion = QtWidgets.QSpinBox(entryDialog)
        self.completion.setMaximum(100)
        self.completion.setObjectName("completion")
        self.gridlayout.addWidget(self.completion, 1, 1, 1, 1)
        self.label4 = QtWidgets.QLabel(entryDialog)
        self.label4.setObjectName("label4")
        self.gridlayout.addWidget(self.label4, 1, 3, 1, 1)
        self.endDate = QtWidgets.QDateTimeEdit(entryDialog)
        self.endDate.setCalendarPopup(True)
        self.endDate.setObjectName("endDate")
        self.gridlayout.addWidget(self.endDate, 1, 4, 1, 1)
        self.readOnly = QtWidgets.QCheckBox(entryDialog)
        self.readOnly.setObjectName("readOnly")
        self.gridlayout.addWidget(self.readOnly, 2, 0, 1, 2)
        self.dependsLabel = QtWidgets.QLabel(entryDialog)
        self.dependsLabel.setObjectName("dependsLabel")
        self.gridlayout.addWidget(self.dependsLabel, 2, 3, 1, 1)
        self.depends = QtWidgets.QComboBox(entryDialog)
        self.depends.setObjectName("depends")
        self.depends.addItem("")
        self.gridlayout.addWidget(self.depends, 2, 4, 1, 1)
        self.vboxlayout.addLayout(self.gridlayout)
        self.buttonBox = QtWidgets.QDialogButtonBox(entryDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(
            QtWidgets.QDialogButtonBox.Cancel | QtWidgets.QDialogButtonBox.NoButton | QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.vboxlayout.addWidget(self.buttonBox)

        self.retranslateUi(entryDialog)
        self.buttonBox.accepted.connect(entryDialog.accept)
        self.buttonBox.rejected.connect(entryDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(entryDialog)

    def retranslateUi(self, entryDialog):
        del entryDialog  # might be needed
        _translate = QtCore.QCoreApplication.translate
        self.label.setText(_translate("entryDialog", "Name:"))
        self.label6.setText(_translate("entryDialog", "Legend:"))
        self.label2.setText(_translate("entryDialog", "Type:"))
        self.label3.setText(_translate("entryDialog", "Start date:"))
        self.label5.setText(_translate("entryDialog", "Completion:"))
        self.completion.setSuffix(_translate("entryDialog", "%"))
        self.label4.setText(_translate("entryDialog", "End date:"))
        self.readOnly.setText(_translate("entryDialog", "Read Only"))
        self.dependsLabel.setText(_translate("entryDialog", "Depends on:"))
        self.depends.setItemText(0, _translate("entryDialog", "- None -"))

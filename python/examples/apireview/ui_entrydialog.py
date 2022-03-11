#!/usr/bin/env python

##
## This file is part of the KD Chart library.
##
## SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

from PySide2 import QtCore, QtGui, QtWidgets

class Ui_EntryDialog(object):
    def setupUi(self, EntryDialog):
        EntryDialog.setObjectName("EntryDialog")
        EntryDialog.resize(439, 171)
        self.vboxlayout = QtWidgets.QVBoxLayout(EntryDialog)
        self.vboxlayout.setObjectName("vboxlayout")
        self.hboxlayout = QtWidgets.QHBoxLayout()
        self.hboxlayout.setObjectName("hboxlayout")
        self.label = QtWidgets.QLabel(EntryDialog)
        self.label.setObjectName("label")
        self.hboxlayout.addWidget(self.label)
        self.name = QtWidgets.QLineEdit(EntryDialog)
        self.name.setObjectName("name")
        self.hboxlayout.addWidget(self.name)
        self.vboxlayout.addLayout(self.hboxlayout)
        self.hboxlayout1 = QtWidgets.QHBoxLayout()
        self.hboxlayout1.setObjectName("hboxlayout1")
        self.label_6 = QtWidgets.QLabel(EntryDialog)
        self.label_6.setObjectName("label_6")
        self.hboxlayout1.addWidget(self.label_6)
        self.legend = QtWidgets.QLineEdit(EntryDialog)
        self.legend.setObjectName("legend")
        self.hboxlayout1.addWidget(self.legend)
        self.vboxlayout.addLayout(self.hboxlayout1)
        self.gridlayout = QtWidgets.QGridLayout()
        self.gridlayout.setObjectName("gridlayout")
        self.label_2 = QtWidgets.QLabel(EntryDialog)
        self.label_2.setObjectName("label_2")
        self.gridlayout.addWidget(self.label_2, 0, 0, 1, 1)
        self.type = QtWidgets.QComboBox(EntryDialog)
        self.type.setObjectName("type")
        self.gridlayout.addWidget(self.type, 0, 1, 1, 1)
        spacerItem = QtWidgets.QSpacerItem(41, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridlayout.addItem(spacerItem, 0, 2, 1, 1)
        self.label_3 = QtWidgets.QLabel(EntryDialog)
        self.label_3.setObjectName("label_3")
        self.gridlayout.addWidget(self.label_3, 0, 3, 1, 1)
        self.startDate = QtWidgets.QDateTimeEdit(EntryDialog)
        self.startDate.setCalendarPopup(True)
        self.startDate.setObjectName("startDate")
        self.gridlayout.addWidget(self.startDate, 0, 4, 1, 1)
        self.label_5 = QtWidgets.QLabel(EntryDialog)
        self.label_5.setObjectName("label_5")
        self.gridlayout.addWidget(self.label_5, 1, 0, 1, 1)
        self.completion = QtWidgets.QSpinBox(EntryDialog)
        self.completion.setMaximum(100)
        self.completion.setObjectName("completion")
        self.gridlayout.addWidget(self.completion, 1, 1, 1, 1)
        self.label_4 = QtWidgets.QLabel(EntryDialog)
        self.label_4.setObjectName("label_4")
        self.gridlayout.addWidget(self.label_4, 1, 3, 1, 1)
        self.endDate = QtWidgets.QDateTimeEdit(EntryDialog)
        self.endDate.setCalendarPopup(True)
        self.endDate.setObjectName("endDate")
        self.gridlayout.addWidget(self.endDate, 1, 4, 1, 1)
        self.readOnly = QtWidgets.QCheckBox(EntryDialog)
        self.readOnly.setObjectName("readOnly")
        self.gridlayout.addWidget(self.readOnly, 2, 0, 1, 2)
        self.dependsLabel = QtWidgets.QLabel(EntryDialog)
        self.dependsLabel.setObjectName("dependsLabel")
        self.gridlayout.addWidget(self.dependsLabel, 2, 3, 1, 1)
        self.depends = QtWidgets.QComboBox(EntryDialog)
        self.depends.setObjectName("depends")
        self.depends.addItem("")
        self.gridlayout.addWidget(self.depends, 2, 4, 1, 1)
        self.vboxlayout.addLayout(self.gridlayout)
        self.buttonBox = QtWidgets.QDialogButtonBox(EntryDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.NoButton|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.vboxlayout.addWidget(self.buttonBox)

        self.retranslateUi(EntryDialog)
        self.buttonBox.accepted.connect(EntryDialog.accept)
        self.buttonBox.rejected.connect(EntryDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(EntryDialog)

    def retranslateUi(self, EntryDialog):
        _translate = QtCore.QCoreApplication.translate
        self.label.setText(_translate("EntryDialog", "Name:"))
        self.label_6.setText(_translate("EntryDialog", "Legend:"))
        self.label_2.setText(_translate("EntryDialog", "Type:"))
        self.label_3.setText(_translate("EntryDialog", "Start date:"))
        self.label_5.setText(_translate("EntryDialog", "Completion:"))
        self.completion.setSuffix(_translate("EntryDialog", "%"))
        self.label_4.setText(_translate("EntryDialog", "End date:"))
        self.readOnly.setText(_translate("EntryDialog", "Read Only"))
        self.dependsLabel.setText(_translate("EntryDialog", "Depends on:"))
        self.depends.setItemText(0, _translate("EntryDialog", "- None -"))


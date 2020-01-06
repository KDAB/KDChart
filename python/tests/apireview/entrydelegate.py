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

from PySide2.QtCore import Qt, QEvent
from PySide2.QtWidgets import QItemDelegate
from PyKDChart import KDGantt

class EntryDelegate(QItemDelegate):
    def __init__(self, constraintModel, parent = None):
        super(EntryDelegate, self).__init__(parent)

        self.constraintModel = constraintModel

    def editorEvent(self, event, model, option, index):
        if event.type() != QEvent.MouseButtonDblClick:
            return False

        if not index.isValid():
            return super(QItemDelegate, self).editorEvent(event, model, option, index)

        dialog = EntryDialog(model)
        dialog.initFrom(index, constraintModel)
        dialog.setWindowTitle("Edit Entry")
        dialog.exec_()
        
        row = index.row()
        parent = index.parent()

        model.setData(model.index(row, 0, parent), dialog.name())
        model.setData(model.index(row, 1, parent), dialog.type())

        if dialog.type() != KDGantt.TypeSummary:
            model.setData(model.index(row, 2, parent), dialog.startDate())
            model.setData(model.index(row, 3, parent), dialog.endDate())

        model.setData(model.index(row, 4, parent), dialog.completion())
        model.setData(model.index(row, 5, parent), dialog.legend())
    
        self.addConstraint(dialog.depends(), model.index(row, 0, parent))
        self.setReadOnly(model.index(row, 0, parent), dialog.readOnly())

        dialog = None
        return True

    def setReadOnly(self, index, readOnly):
        row = index.row()
        parent = index.parent()
        model = index.model()

        for column in range(0, 5):
            item = model.itemFromIndex(model.index(row, column, parent))
            if readOnly:
                flags = item.flags() & ~Qt.ItemIsEditable
            else:
                flags = item.flags() | Qt.ItemIsEditable
            item.setFlags(flags)

    def addConstraint(self, index1, index2):
        if not index1.isValid() or not index2.isValid():
            return

        c = Constraint(index1, index2)
        self.constraintModel.addConstraint(c)

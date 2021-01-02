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

from PySide2.QtCore import Qt, QDateTime, QTime
from PySide2.QtGui import QStandardItemModel
from PySide2.QtWidgets import QApplication, QHeaderView
from PyKDChart.KDGantt import DateTimeScaleFormatter, View,  DateTimeGrid
from PyKDChart import KDGantt

## A custom formatter that displays tabs for every 10 minutes.
class MyDateTimeScaleFormatter(DateTimeScaleFormatter):
    def __init__(self):
        super(MyDateTimeScaleFormatter, self).__init__(DateTimeScaleFormatter.Hour, "hh")

    def nextRangeBegin(self, datetime):
        return self.currentRangeBegin(datetime).addSecs(60 * 10)

    def currentRangeBegin(self, datetime):
        dt = QDateTime(datetime)
        dt.setTime(QTime(dt.time().hour(), (dt.time().minute() / 10) * 10, 0, 0))
        return dt

    def text(self, datetime):
        return  (":%s\nXX"%datetime.time().toString("mm"))

## A custom headerview that is taller than standard
## so we can fit more lines into it on the graphicsview
## side.
class MyHeaderView(QHeaderView):
    def __init__(self, parent = None):
        super(MyHeaderView, self).__init__(Qt.Horizontal, parent)

    def sizeHint(self):
        s = super(MyHeaderView, self).sizeHint()
        s.setHeight(s.height() * 3)
        return s

if __name__ == '__main__':
    app = QApplication(sys.argv)

    model = QStandardItemModel(1, 1)
    model.setHeaderData(0, Qt.Horizontal, "Task")

    ## A view with some alternative header labels 
    view1 = View()
    grid1 = DateTimeGrid()
    #grid1.setUserDefinedUpperScale(DateTimeScaleFormatter(DateTimeScaleFormatter.Year, "yyyy", "In the year %1.", Qt.AlignLeft))
    #grid1.setUserDefinedLowerScale(DateTimeScaleFormatter(DateTimeScaleFormatter.Month, "MMMM", "In the month %1.", Qt.AlignRight))
    grid1.setScale(DateTimeGrid.ScaleUserDefined)
    grid1.setDayWidth(6.0);
    view1.setGrid(grid1)
    view1.setModel(model)
    view1.show()

    ## A view with header and vertical grid lines for every 10 minutes 
    view2 = View()
    tw = view2.leftView()
    h = MyHeaderView()
    tw.setHeader(h)
    grid2 = DateTimeGrid()
    grid2.setDayWidth(5000)
    grid2.setUserDefinedUpperScale(DateTimeScaleFormatter(DateTimeScaleFormatter.Hour, "hh"))
    grid2.setUserDefinedLowerScale(MyDateTimeScaleFormatter())
    grid2.setScale(DateTimeGrid.ScaleUserDefined)
    view2.setGrid(grid2)
    view2.setModel(model)
    view2.show()

    sys.exit(app.exec_())

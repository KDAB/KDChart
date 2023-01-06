#!/usr/bin/env python

# Contact info@kdab.com if any conditions of this licensing are not clear to you.
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

''' Example custom formatter that displays tabs for every 10 minutes '''

# pylint: disable=missing-function-docstring,missing-class-docstring

import sys

from PySide2.QtCore import Qt, QDateTime, QTime
from PySide2.QtGui import QStandardItemModel
from PySide2.QtWidgets import QApplication, QHeaderView
from PyKDChart.KDGantt import DateTimeScaleFormatter, View,  DateTimeGrid


class MyDateTimeScaleFormatter(DateTimeScaleFormatter):
    def __init__(self):
        super().__init__(DateTimeScaleFormatter.Hour, "hh")

    def nextRangeBegin(self, datetime):
        return self.currentRangeBegin(datetime).addSecs(60 * 10)

    def currentRangeBegin(self, datetime):  # pylint: disable=no-self-use
        dtInstance = QDateTime(datetime)
        dtInstance.setTime(QTime(dtInstance.time().hour(),
                           (dtInstance.time().minute() / 10) * 10, 0, 0))
        return dtInstance

    def text(self, datetime):  # pylint: disable=no-self-use
        return (":%s\nXX" % datetime.time().toString("mm"))

# A custom headerview that is taller than standard
# so we can fit more lines into it on the graphicsview
# side.


# pylint: disable=too-few-public-methods
class MyHeaderView(QHeaderView):
    def __init__(self, parent=None):
        super().__init__(Qt.Horizontal, parent)

    def sizeHint(self):
        s = super().sizeHint()
        s.setHeight(s.height() * 3)
        return s


if __name__ == '__main__':
    app = QApplication(sys.argv)

    model = QStandardItemModel(1, 1)
    model.setHeaderData(0, Qt.Horizontal, "Task")

    # A view with some alternative header labels
    view1 = View()
    grid1 = DateTimeGrid()
    # grid1.setUserDefinedUpperScale(DateTimeScaleFormatter(DateTimeScaleFormatter.Year,
    #                                                      "yyyy", "In the year %1.", Qt.AlignLeft))
    # grid1.setUserDefinedLowerScale(DateTimeScaleFormatter(DateTimeScaleFormatter.Month,
    #                                                      "MMMM", "In the month %1.", Qt.AlignRight))
    grid1.setScale(DateTimeGrid.ScaleUserDefined)
    grid1.setDayWidth(6.0)
    view1.setGrid(grid1)
    view1.setModel(model)
    view1.show()

    # A view with header and vertical grid lines for every 10 minutes
    view2 = View()
    tw = view2.leftView()
    h = MyHeaderView()
    tw.setHeader(h)
    grid2 = DateTimeGrid()
    grid2.setDayWidth(5000)
    grid2.setUserDefinedUpperScale(
        DateTimeScaleFormatter(DateTimeScaleFormatter.Hour, "hh"))
    grid2.setUserDefinedLowerScale(MyDateTimeScaleFormatter())
    grid2.setScale(DateTimeGrid.ScaleUserDefined)
    view2.setGrid(grid2)
    view2.setModel(model)
    view2.show()

    sys.exit(app.exec_())

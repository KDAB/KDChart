add_executable(DrawIntoPainter-example DrawIntoPainter.qrc framewidget.cpp main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(DrawIntoPainter-example ${QT_LIBRARIES} kdchart testtools)

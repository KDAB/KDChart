add_executable(DrawIntoPainter-test DrawIntoPainter.qrc framewidget.cpp main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(DrawIntoPainter-test ${QT_LIBRARIES} kdchart testtools)
add_test(NAME DrawIntoPainter-test COMMAND DrawIntoPainter-test)

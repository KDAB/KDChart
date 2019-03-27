add_executable(LegendsLegendAdvanced-example Legends.qrc addlegenddialog.ui derivedaddlegenddialog.cpp main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(LegendsLegendAdvanced-example ${QT_LIBRARIES} kdchart testtools)

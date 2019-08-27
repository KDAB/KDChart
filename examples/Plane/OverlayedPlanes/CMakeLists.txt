add_executable(PlaneOverlayedPlanes-example OverlayedPlanes.qrc main.cpp mainwindow.cpp mainwindow.ui)
target_link_libraries(PlaneOverlayedPlanes-example ${QT_LIBRARIES} kdchart testtools)

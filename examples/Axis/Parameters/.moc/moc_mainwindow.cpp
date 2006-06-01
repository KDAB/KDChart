/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu Jun 1 11:53:29 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x08,
      68,   60,   11,   11, 0x08,
      99,   60,   11,   11, 0x08,
     130,   60,   11,   11, 0x08,
     162,   12,   11,   11, 0x08,
     211,  209,   11,   11, 0x08,
     247,  209,   11,   11, 0x08,
     284,   60,   11,   11, 0x08,
     316,  209,   11,   11, 0x08,
     359,  352,   11,   11, 0x08,
     402,  396,   11,   11, 0x08,
     429,  396,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0text\0on_lineTypeCB_currentIndexChanged(QString)\0checked\0"
    "on_paintLegendCB_toggled(bool)\0on_paintValuesCB_toggled(bool)\0"
    "on_paintMarkersCB_toggled(bool)\0"
    "on_markersStyleCB_currentIndexChanged(QString)\0i\0"
    "on_markersWidthSB_valueChanged(int)\0"
    "on_markersHeightSB_valueChanged(int)\0on_displayAreasCB_toggled(bool)\0"
    "on_transparencySB_valueChanged(int)\0factor\0"
    "on_zoomFactorSB_valueChanged(double)\0value\0on_hSBar_valueChanged(int)\0"
    "on_vSBar_valueChanged(int)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
	return static_cast<void*>(const_cast<MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_lineTypeCB_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: on_paintLegendCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: on_paintValuesCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: on_paintMarkersCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: on_markersStyleCB_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: on_markersWidthSB_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_markersHeightSB_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: on_displayAreasCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: on_transparencySB_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: on_zoomFactorSB_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: on_hSBar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: on_vSBar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 12;
    }
    return _id;
}

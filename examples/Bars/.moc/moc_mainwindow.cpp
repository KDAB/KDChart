/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu Apr 13 08:58:41 2006
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
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x08,
      67,   59,   11,   11, 0x08,
      98,   59,   11,   11, 0x08,
     133,   59,   11,   11, 0x08,
     165,  163,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0text\0on_barTypeCB_currentIndexChanged(QString)\0checked\0"
    "on_paintValuesCB_toggled(bool)\0on_paintThreeDBarsCB_toggled(bool)\0"
    "on_markColumnCB_toggled(bool)\0i\0on_markColumnSB_valueChanged(int)\0"
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
        case 0: on_barTypeCB_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: on_paintValuesCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: on_paintThreeDBarsCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: on_markColumnCB_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: on_markColumnSB_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 5;
    }
    return _id;
}

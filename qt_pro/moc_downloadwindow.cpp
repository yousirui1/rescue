/****************************************************************************
** Meta object code from reading C++ file 'downloadwindow.h'
**
** Created: Tue Jun 16 23:40:07 2020
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "downloadwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'downloadwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DownloadWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      23,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   36,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DownloadWindow[] = {
    "DownloadWindow\0\0number\0display(int)\0"
    "info\0setProgressValue(progress_info*)\0"
};

const QMetaObject DownloadWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DownloadWindow,
      qt_meta_data_DownloadWindow, 0 }
};

const QMetaObject *DownloadWindow::metaObject() const
{
    return &staticMetaObject;
}

void *DownloadWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DownloadWindow))
        return static_cast<void*>(const_cast< DownloadWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int DownloadWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setProgressValue((*reinterpret_cast< progress_info*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DownloadWindow::display(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

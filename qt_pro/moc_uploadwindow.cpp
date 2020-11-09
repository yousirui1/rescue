/****************************************************************************
** Meta object code from reading C++ file 'uploadwindow.h'
**
** Created: Mon Nov 2 16:53:02 2020
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "uploadwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uploadwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Uploadwindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   34,   13,   13, 0x0a,
      80,   72,   13,   13, 0x0a,
     103,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Uploadwindow[] = {
    "Uploadwindow\0\0number\0display(int)\0"
    "info\0setProgressValue(progress_info*)\0"
    "err_msg\0showErrorDialog(char*)\0Reboot()\0"
};

const QMetaObject Uploadwindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Uploadwindow,
      qt_meta_data_Uploadwindow, 0 }
};

const QMetaObject *Uploadwindow::metaObject() const
{
    return &staticMetaObject;
}

void *Uploadwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Uploadwindow))
        return static_cast<void*>(const_cast< Uploadwindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int Uploadwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setProgressValue((*reinterpret_cast< progress_info*(*)>(_a[1]))); break;
        case 2: showErrorDialog((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: Reboot(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Uploadwindow::display(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

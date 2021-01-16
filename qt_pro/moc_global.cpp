/****************************************************************************
** Meta object code from reading C++ file 'global.h'
**
** Created: Thu Jan 14 15:16:10 2021
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "global.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'global.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Global[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,    8,    7,    7, 0x05,
      28,    7,    7,    7, 0x05,
      46,   41,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      71,    7,    7,    7, 0x0a,
      92,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Global[] = {
    "Global\0\0number\0display(int)\0close_pipe()\0"
    "info\0setValue(progress_info*)\0"
    "setCurrentIndex(int)\0onUpdateTime()\0"
};

const QMetaObject Global::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Global,
      qt_meta_data_Global, 0 }
};

const QMetaObject *Global::metaObject() const
{
    return &staticMetaObject;
}

void *Global::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Global))
        return static_cast<void*>(const_cast< Global*>(this));
    return QObject::qt_metacast(_clname);
}

int Global::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: close_pipe(); break;
        case 2: setValue((*reinterpret_cast< progress_info*(*)>(_a[1]))); break;
        case 3: setCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: onUpdateTime(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Global::display(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Global::close_pipe()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Global::setValue(progress_info * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

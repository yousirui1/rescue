/****************************************************************************
** Meta object code from reading C++ file 'qbutton2.h'
**
** Created: Mon Dec 7 16:54:43 2020
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qbutton2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qbutton2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QButton2[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_QButton2[] = {
    "QButton2\0"
};

const QMetaObject QButton2::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_QButton2,
      qt_meta_data_QButton2, 0 }
};

const QMetaObject *QButton2::metaObject() const
{
    return &staticMetaObject;
}

void *QButton2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QButton2))
        return static_cast<void*>(const_cast< QButton2*>(this));
    return QPushButton::qt_metacast(_clname);
}

int QButton2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

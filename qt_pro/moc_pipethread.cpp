/****************************************************************************
** Meta object code from reading C++ file 'pipethread.h'
**
** Created: Mon Mar 8 09:23:49 2021
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "pipethread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pipethread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PipeThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PipeThread[] = {
    "PipeThread\0\0close_pipe()\0"
};

const QMetaObject PipeThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_PipeThread,
      qt_meta_data_PipeThread, 0 }
};

const QMetaObject *PipeThread::metaObject() const
{
    return &staticMetaObject;
}

void *PipeThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PipeThread))
        return static_cast<void*>(const_cast< PipeThread*>(this));
    return QThread::qt_metacast(_clname);
}

int PipeThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: close_pipe(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

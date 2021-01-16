/****************************************************************************
** Meta object code from reading C++ file 'global.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt_pro/global.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'global.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Global[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,    8,    7,    7, 0x05,
      28,    7,    7,    7, 0x05,
      46,   41,    7,    7, 0x05,
      75,   71,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      93,    7,    7,    7, 0x0a,
     114,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Global[] = {
    "Global\0\0number\0display(int)\0close_pipe()\0"
    "info\0setValue(progress_info*)\0msg\0"
    "setError(QString)\0setCurrentIndex(int)\0"
    "onUpdateTime()\0"
};

void Global::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Global *_t = static_cast<Global *>(_o);
        switch (_id) {
        case 0: _t->display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->close_pipe(); break;
        case 2: _t->setValue((*reinterpret_cast< progress_info*(*)>(_a[1]))); break;
        case 3: _t->setError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onUpdateTime(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Global::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Global::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Global,
      qt_meta_data_Global, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Global::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Global::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
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
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
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

// SIGNAL 3
void Global::setError(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE

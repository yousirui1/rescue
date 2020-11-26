/****************************************************************************
** Meta object code from reading C++ file 'QRoundProgressBar.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt_pro/QRoundProgressBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QRoundProgressBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QRoundProgressBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   19,   18,   18, 0x0a,
      55,   51,   18,   18, 0x0a,
      78,   74,   18,   18, 0x0a,
     101,   97,   18,   18, 0x0a,
     118,   97,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QRoundProgressBar[] = {
    "QRoundProgressBar\0\0min,max\0"
    "setRange(double,double)\0min\0"
    "setMinimum(double)\0max\0setMaximum(double)\0"
    "val\0setValue(double)\0setValue(int)\0"
};

void QRoundProgressBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QRoundProgressBar *_t = static_cast<QRoundProgressBar *>(_o);
        switch (_id) {
        case 0: _t->setRange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->setMinimum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setMaximum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QRoundProgressBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QRoundProgressBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QRoundProgressBar,
      qt_meta_data_QRoundProgressBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QRoundProgressBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QRoundProgressBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QRoundProgressBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QRoundProgressBar))
        return static_cast<void*>(const_cast< QRoundProgressBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int QRoundProgressBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

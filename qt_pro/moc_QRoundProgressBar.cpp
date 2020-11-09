/****************************************************************************
** Meta object code from reading C++ file 'QRoundProgressBar.h'
**
** Created: Mon Nov 2 16:53:02 2020
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QRoundProgressBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QRoundProgressBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QRoundProgressBar[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

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

const QMetaObject QRoundProgressBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QRoundProgressBar,
      qt_meta_data_QRoundProgressBar, 0 }
};

const QMetaObject *QRoundProgressBar::metaObject() const
{
    return &staticMetaObject;
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
        switch (_id) {
        case 0: setRange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: setMinimum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: setMaximum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dialogwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt_pro/dialogwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DialogWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   13,   13,   13, 0x08,
      57,   13,   13,   13, 0x08,
      74,   13,   13,   13, 0x08,
      99,   13,   13,   13, 0x08,
     130,   13,   13,   13, 0x08,
     163,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DialogWindow[] = {
    "DialogWindow\0\0number\0display(int)\0"
    "on_cacel_btn_clicked()\0slots_dhcpable()\0"
    "on_confirm_btn_clicked()\0"
    "on_install_cacel_btn_clicked()\0"
    "on_install_confirm_btn_clicked()\0"
    "on_error_confirm_btn_clicked()\0"
};

void DialogWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DialogWindow *_t = static_cast<DialogWindow *>(_o);
        switch (_id) {
        case 0: _t->display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_cacel_btn_clicked(); break;
        case 2: _t->slots_dhcpable(); break;
        case 3: _t->on_confirm_btn_clicked(); break;
        case 4: _t->on_install_cacel_btn_clicked(); break;
        case 5: _t->on_install_confirm_btn_clicked(); break;
        case 6: _t->on_error_confirm_btn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DialogWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DialogWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DialogWindow,
      qt_meta_data_DialogWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DialogWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DialogWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DialogWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogWindow))
        return static_cast<void*>(const_cast< DialogWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int DialogWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DialogWindow::display(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dialogwindow.h'
**
** Created: Mon Nov 2 16:53:02 2020
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialogwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DialogWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

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

const QMetaObject DialogWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DialogWindow,
      qt_meta_data_DialogWindow, 0 }
};

const QMetaObject *DialogWindow::metaObject() const
{
    return &staticMetaObject;
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
        switch (_id) {
        case 0: display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_cacel_btn_clicked(); break;
        case 2: slots_dhcpable(); break;
        case 3: on_confirm_btn_clicked(); break;
        case 4: on_install_cacel_btn_clicked(); break;
        case 5: on_install_confirm_btn_clicked(); break;
        case 6: on_error_confirm_btn_clicked(); break;
        default: ;
        }
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

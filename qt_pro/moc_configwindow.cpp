/****************************************************************************
** Meta object code from reading C++ file 'configwindow.h'
**
** Created: Wed Dec 16 16:44:56 2020
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "configwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'configwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConfigWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   13,   13,   13, 0x08,
      62,   13,   13,   13, 0x08,
      86,   13,   13,   13, 0x08,
     112,   13,   13,   13, 0x08,
     129,   13,   13,   13, 0x08,
     154,   13,   13,   13, 0x08,
     181,  176,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConfigWindow[] = {
    "ConfigWindow\0\0number\0display(int)\0"
    "on_laststepButton_clicked()\0"
    "on_nextButton_clicked()\0"
    "on_formatButton_clicked()\0slots_dhcpable()\0"
    "install_cacenl_clicked()\0install_confirm_btn()\0"
    "arg1\0on_num_edit_textChanged(QString)\0"
};

const QMetaObject ConfigWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConfigWindow,
      qt_meta_data_ConfigWindow, 0 }
};

const QMetaObject *ConfigWindow::metaObject() const
{
    return &staticMetaObject;
}

void *ConfigWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigWindow))
        return static_cast<void*>(const_cast< ConfigWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConfigWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_laststepButton_clicked(); break;
        case 2: on_nextButton_clicked(); break;
        case 3: on_formatButton_clicked(); break;
        case 4: slots_dhcpable(); break;
        case 5: install_cacenl_clicked(); break;
        case 6: install_confirm_btn(); break;
        case 7: on_num_edit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ConfigWindow::display(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
/****************************************************************************
** Meta object code from reading C++ file 'oswindow.h'
**
** Created: Thu Jan 14 15:04:32 2021
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "oswindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oswindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OSWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,    9,    9,    9, 0x08,
      57,    9,    9,    9, 0x08,
      89,   84,    9,    9, 0x08,
     132,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OSWindow[] = {
    "OSWindow\0\0number\0display(int)\0"
    "on_pushButton_12_clicked()\0"
    "on_pushButton_11_clicked()\0arg1\0"
    "on_template_size_edit_textChanged(QString)\0"
    "on_template_name_edit_editingFinished()\0"
};

const QMetaObject OSWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OSWindow,
      qt_meta_data_OSWindow, 0 }
};

const QMetaObject *OSWindow::metaObject() const
{
    return &staticMetaObject;
}

void *OSWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OSWindow))
        return static_cast<void*>(const_cast< OSWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int OSWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_pushButton_12_clicked(); break;
        case 2: on_pushButton_11_clicked(); break;
        case 3: on_template_size_edit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: on_template_name_edit_editingFinished(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void OSWindow::display(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UI/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata0[324];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 23), // "on_OpenDatabase_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 22), // "on_NewDatabase_clicked"
QT_MOC_LITERAL(4, 59, 21), // "on_comboBox_activated"
QT_MOC_LITERAL(5, 81, 4), // "arg1"
QT_MOC_LITERAL(6, 86, 16), // "createConnection"
QT_MOC_LITERAL(7, 103, 12), // "databaseName"
QT_MOC_LITERAL(8, 116, 15), // "closeConnection"
QT_MOC_LITERAL(9, 132, 9), // "showTable"
QT_MOC_LITERAL(10, 142, 9), // "tableName"
QT_MOC_LITERAL(11, 152, 30), // "on_pushButtonNewRecord_clicked"
QT_MOC_LITERAL(12, 183, 33), // "on_pushButtonDeleteRecord_cli..."
QT_MOC_LITERAL(13, 217, 22), // "on_CreateTable_clicked"
QT_MOC_LITERAL(14, 240, 22), // "on_DeleteTable_clicked"
QT_MOC_LITERAL(15, 263, 14), // "updateComboBox"
QT_MOC_LITERAL(16, 278, 13), // "onTableCreate"
QT_MOC_LITERAL(17, 292, 13), // "onTableDelete"
QT_MOC_LITERAL(18, 306, 12), // "buttonEnable"
QT_MOC_LITERAL(19, 319, 4) // "flag"

    },
    "MainWindow\0on_OpenDatabase_clicked\0\0"
    "on_NewDatabase_clicked\0on_comboBox_activated\0"
    "arg1\0createConnection\0databaseName\0"
    "closeConnection\0showTable\0tableName\0"
    "on_pushButtonNewRecord_clicked\0"
    "on_pushButtonDeleteRecord_clicked\0"
    "on_CreateTable_clicked\0on_DeleteTable_clicked\0"
    "updateComboBox\0onTableCreate\0onTableDelete\0"
    "buttonEnable\0flag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    1,   86,    2, 0x08 /* Private */,
       6,    1,   89,    2, 0x08 /* Private */,
       8,    0,   92,    2, 0x08 /* Private */,
       9,    1,   93,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    0,   99,    2, 0x08 /* Private */,
      15,    0,  100,    2, 0x08 /* Private */,
      16,    0,  101,    2, 0x08 /* Private */,
      17,    0,  102,    2, 0x08 /* Private */,
      18,    1,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Bool, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   19,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_OpenDatabase_clicked(); break;
        case 1: _t->on_NewDatabase_clicked(); break;
        case 2: _t->on_comboBox_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: { bool _r = _t->createConnection((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->closeConnection(); break;
        case 5: _t->showTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->on_pushButtonNewRecord_clicked(); break;
        case 7: _t->on_pushButtonDeleteRecord_clicked(); break;
        case 8: _t->on_CreateTable_clicked(); break;
        case 9: _t->on_DeleteTable_clicked(); break;
        case 10: _t->updateComboBox(); break;
        case 11: _t->onTableCreate(); break;
        case 12: _t->onTableDelete(); break;
        case 13: _t->buttonEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

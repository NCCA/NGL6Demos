/****************************************************************************
** Meta object code from reading C++ file 'GLWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/GLWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GLWindow_t {
    QByteArrayData data[19];
    char stringdata[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GLWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GLWindow_t qt_meta_stringdata_GLWindow = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 15),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 5),
QT_MOC_LITERAL(4, 32, 12),
QT_MOC_LITERAL(5, 45, 2),
QT_MOC_LITERAL(6, 48, 12),
QT_MOC_LITERAL(7, 61, 2),
QT_MOC_LITERAL(8, 64, 12),
QT_MOC_LITERAL(9, 77, 2),
QT_MOC_LITERAL(10, 80, 9),
QT_MOC_LITERAL(11, 90, 9),
QT_MOC_LITERAL(12, 100, 9),
QT_MOC_LITERAL(13, 110, 12),
QT_MOC_LITERAL(14, 123, 12),
QT_MOC_LITERAL(15, 136, 12),
QT_MOC_LITERAL(16, 149, 13),
QT_MOC_LITERAL(17, 163, 2),
QT_MOC_LITERAL(18, 166, 9)
    },
    "GLWindow\0toggleWireframe\0\0_mode\0"
    "setXRotation\0_x\0setYRotation\0_y\0"
    "setZRotation\0_z\0setXScale\0setYScale\0"
    "setZScale\0setXPosition\0setYPosition\0"
    "setZPosition\0setObjectMode\0_i\0setColour\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x0a,
       4,    1,   77,    2, 0x0a,
       6,    1,   80,    2, 0x0a,
       8,    1,   83,    2, 0x0a,
      10,    1,   86,    2, 0x0a,
      11,    1,   89,    2, 0x0a,
      12,    1,   92,    2, 0x0a,
      13,    1,   95,    2, 0x0a,
      14,    1,   98,    2, 0x0a,
      15,    1,  101,    2, 0x0a,
      16,    1,  104,    2, 0x0a,
      18,    0,  107,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,

       0        // eod
};

void GLWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLWindow *_t = static_cast<GLWindow *>(_o);
        switch (_id) {
        case 0: _t->toggleWireframe((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setXRotation((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setYRotation((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setZRotation((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setXScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setYScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setZScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->setXPosition((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setYPosition((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setZPosition((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setObjectMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setColour(); break;
        default: ;
        }
    }
}

const QMetaObject GLWindow::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLWindow.data,
      qt_meta_data_GLWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *GLWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLWindow.stringdata))
        return static_cast<void*>(const_cast< GLWindow*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

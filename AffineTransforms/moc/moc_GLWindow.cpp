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
    QByteArrayData data[24];
    char stringdata[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GLWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GLWindow_t qt_meta_stringdata_GLWindow = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 11),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 9),
QT_MOC_LITERAL(4, 32, 2),
QT_MOC_LITERAL(5, 35, 10),
QT_MOC_LITERAL(6, 46, 6),
QT_MOC_LITERAL(7, 53, 13),
QT_MOC_LITERAL(8, 67, 6),
QT_MOC_LITERAL(9, 74, 15),
QT_MOC_LITERAL(10, 90, 13),
QT_MOC_LITERAL(11, 104, 8),
QT_MOC_LITERAL(12, 113, 2),
QT_MOC_LITERAL(13, 116, 2),
QT_MOC_LITERAL(14, 119, 2),
QT_MOC_LITERAL(15, 122, 12),
QT_MOC_LITERAL(16, 135, 9),
QT_MOC_LITERAL(17, 145, 9),
QT_MOC_LITERAL(18, 155, 2),
QT_MOC_LITERAL(19, 158, 2),
QT_MOC_LITERAL(20, 161, 2),
QT_MOC_LITERAL(21, 164, 14),
QT_MOC_LITERAL(22, 179, 8),
QT_MOC_LITERAL(23, 188, 6)
    },
    "GLWindow\0matrixDirty\0\0ngl::Mat4\0_m\0"
    "vboChanged\0_index\0toggleNormals\0_value\0"
    "toggleWireframe\0setNormalSize\0setScale\0"
    "_x\0_y\0_z\0setTranslate\0setRotate\0"
    "setColour\0_r\0_g\0_b\0setMatrixOrder\0"
    "setEuler\0_angle\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    1,   72,    2, 0x0a,
       7,    1,   75,    2, 0x0a,
       9,    1,   78,    2, 0x0a,
      10,    1,   81,    2, 0x0a,
      11,    3,   84,    2, 0x0a,
      15,    3,   91,    2, 0x0a,
      16,    3,   98,    2, 0x0a,
      17,    3,  105,    2, 0x0a,
      21,    1,  112,    2, 0x0a,
      22,    4,  115,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float,   12,   13,   14,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float,   12,   13,   14,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float,   12,   13,   14,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float,   18,   19,   20,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float,   23,   12,   13,   14,

       0        // eod
};

void GLWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLWindow *_t = static_cast<GLWindow *>(_o);
        switch (_id) {
        case 0: _t->matrixDirty((*reinterpret_cast< ngl::Mat4(*)>(_a[1]))); break;
        case 1: _t->vboChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->toggleNormals((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->toggleWireframe((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setNormalSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setScale((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 6: _t->setTranslate((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 7: _t->setRotate((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 8: _t->setColour((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 9: _t->setMatrixOrder((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setEuler((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GLWindow::*_t)(ngl::Mat4 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWindow::matrixDirty)) {
                *result = 0;
            }
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
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void GLWindow::matrixDirty(ngl::Mat4 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

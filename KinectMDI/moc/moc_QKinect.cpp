/****************************************************************************
** Meta object code from reading C++ file 'QKinect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/QKinect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QKinect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QKinect_t {
    QByteArrayData data[29];
    char stringdata[352];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_QKinect_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_QKinect_t qt_meta_stringdata_QKinect = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 8),
QT_MOC_LITERAL(2, 17, 0),
QT_MOC_LITERAL(3, 18, 6),
QT_MOC_LITERAL(4, 25, 9),
QT_MOC_LITERAL(5, 35, 9),
QT_MOC_LITERAL(6, 45, 11),
QT_MOC_LITERAL(7, 57, 12),
QT_MOC_LITERAL(8, 70, 14),
QT_MOC_LITERAL(9, 85, 16),
QT_MOC_LITERAL(10, 102, 17),
QT_MOC_LITERAL(11, 120, 12),
QT_MOC_LITERAL(12, 133, 5),
QT_MOC_LITERAL(13, 139, 10),
QT_MOC_LITERAL(14, 150, 8),
QT_MOC_LITERAL(15, 159, 21),
QT_MOC_LITERAL(16, 181, 8),
QT_MOC_LITERAL(17, 190, 11),
QT_MOC_LITERAL(18, 202, 13),
QT_MOC_LITERAL(19, 216, 22),
QT_MOC_LITERAL(20, 239, 6),
QT_MOC_LITERAL(21, 246, 10),
QT_MOC_LITERAL(22, 257, 9),
QT_MOC_LITERAL(23, 267, 10),
QT_MOC_LITERAL(24, 278, 9),
QT_MOC_LITERAL(25, 288, 16),
QT_MOC_LITERAL(26, 305, 16),
QT_MOC_LITERAL(27, 322, 10),
QT_MOC_LITERAL(28, 333, 17)
    },
    "QKinect\0setAngle\0\0_angle\0setLedOff\0"
    "setRedLed\0setGreenLed\0setYellowLed\0"
    "setRedLedFlash\0setGreenLedFlash\0"
    "setYellowLedFlash\0setVideoMode\0_mode\0"
    "resetAngle\0getDepth\0std::vector<uint8_t>&\0"
    "o_buffer\0getDepthRaw\0getDepth16Bit\0"
    "std::vector<uint16_t>&\0getRGB\0startDepth\0"
    "stopDepth\0startVideo\0stopVideo\0"
    "toggleVideoState\0toggleDepthState\0"
    "getContext\0freenect_context*\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QKinect[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  119,    2, 0x0a,
       4,    0,  122,    2, 0x0a,
       5,    0,  123,    2, 0x0a,
       6,    0,  124,    2, 0x0a,
       7,    0,  125,    2, 0x0a,
       8,    0,  126,    2, 0x0a,
       9,    0,  127,    2, 0x0a,
      10,    0,  128,    2, 0x0a,
      11,    1,  129,    2, 0x0a,
      13,    0,  132,    2, 0x0a,
      14,    1,  133,    2, 0x0a,
      17,    1,  136,    2, 0x0a,
      18,    1,  139,    2, 0x0a,
      20,    1,  142,    2, 0x0a,
      21,    0,  145,    2, 0x0a,
      22,    0,  146,    2, 0x0a,
      23,    0,  147,    2, 0x0a,
      24,    0,  148,    2, 0x0a,
      25,    1,  149,    2, 0x0a,
      26,    1,  152,    2, 0x0a,
      27,    0,  155,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 15,   16,
    QMetaType::Bool, 0x80000000 | 15,   16,
    QMetaType::Bool, 0x80000000 | 19,   16,
    QMetaType::Bool, 0x80000000 | 15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    0x80000000 | 28,

       0        // eod
};

void QKinect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QKinect *_t = static_cast<QKinect *>(_o);
        switch (_id) {
        case 0: _t->setAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setLedOff(); break;
        case 2: _t->setRedLed(); break;
        case 3: _t->setGreenLed(); break;
        case 4: _t->setYellowLed(); break;
        case 5: _t->setRedLedFlash(); break;
        case 6: _t->setGreenLedFlash(); break;
        case 7: _t->setYellowLedFlash(); break;
        case 8: _t->setVideoMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->resetAngle(); break;
        case 10: { bool _r = _t->getDepth((*reinterpret_cast< std::vector<uint8_t>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->getDepthRaw((*reinterpret_cast< std::vector<uint8_t>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->getDepth16Bit((*reinterpret_cast< std::vector<uint16_t>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->getRGB((*reinterpret_cast< std::vector<uint8_t>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: _t->startDepth(); break;
        case 15: _t->stopDepth(); break;
        case 16: _t->startVideo(); break;
        case 17: _t->stopVideo(); break;
        case 18: _t->toggleVideoState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->toggleDepthState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: { freenect_context* _r = _t->getContext();
            if (_a[0]) *reinterpret_cast< freenect_context**>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject QKinect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QKinect.data,
      qt_meta_data_QKinect,  qt_static_metacall, 0, 0}
};


const QMetaObject *QKinect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QKinect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QKinect.stringdata))
        return static_cast<void*>(const_cast< QKinect*>(this));
    return QObject::qt_metacast(_clname);
}

int QKinect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

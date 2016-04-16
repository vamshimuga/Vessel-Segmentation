/****************************************************************************
** Meta object code from reading C++ file 'miacurvedmprslicer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaCommonModules/miacurvedmprslicer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miacurvedmprslicer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaCurvedMPRSlicer_t {
    QByteArrayData data[21];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaCurvedMPRSlicer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaCurvedMPRSlicer_t qt_meta_stringdata_MiaCurvedMPRSlicer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MiaCurvedMPRSlicer"
QT_MOC_LITERAL(1, 19, 15), // "getInputMarkers"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 18), // "QList<MiaMarker*>&"
QT_MOC_LITERAL(4, 55, 4), // "list"
QT_MOC_LITERAL(5, 60, 13), // "setRotateAxis"
QT_MOC_LITERAL(6, 74, 11), // "MiaPoint4D&"
QT_MOC_LITERAL(7, 86, 4), // "axis"
QT_MOC_LITERAL(8, 91, 16), // "setRotationAngle"
QT_MOC_LITERAL(9, 108, 5), // "angle"
QT_MOC_LITERAL(10, 114, 16), // "setBulkTransform"
QT_MOC_LITERAL(11, 131, 19), // "MiaAffineTransform&"
QT_MOC_LITERAL(12, 151, 9), // "transfomr"
QT_MOC_LITERAL(13, 161, 11), // "markerAdded"
QT_MOC_LITERAL(14, 173, 10), // "MiaMarker*"
QT_MOC_LITERAL(15, 184, 6), // "marker"
QT_MOC_LITERAL(16, 191, 13), // "markerUpdated"
QT_MOC_LITERAL(17, 205, 13), // "markerRemoved"
QT_MOC_LITERAL(18, 219, 15), // "setRotationMode"
QT_MOC_LITERAL(19, 235, 12), // "RotationMode"
QT_MOC_LITERAL(20, 248, 4) // "mode"

    },
    "MiaCurvedMPRSlicer\0getInputMarkers\0\0"
    "QList<MiaMarker*>&\0list\0setRotateAxis\0"
    "MiaPoint4D&\0axis\0setRotationAngle\0"
    "angle\0setBulkTransform\0MiaAffineTransform&\0"
    "transfomr\0markerAdded\0MiaMarker*\0"
    "marker\0markerUpdated\0markerRemoved\0"
    "setRotationMode\0RotationMode\0mode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaCurvedMPRSlicer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   57,    2, 0x0a /* Public */,
       8,    1,   60,    2, 0x0a /* Public */,
      10,    1,   63,    2, 0x0a /* Public */,
      13,    1,   66,    2, 0x0a /* Public */,
      16,    1,   69,    2, 0x0a /* Public */,
      17,    1,   72,    2, 0x0a /* Public */,
      18,    1,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Float,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 19,   20,

       0        // eod
};

void MiaCurvedMPRSlicer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaCurvedMPRSlicer *_t = static_cast<MiaCurvedMPRSlicer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getInputMarkers((*reinterpret_cast< QList<MiaMarker*>(*)>(_a[1]))); break;
        case 1: _t->setRotateAxis((*reinterpret_cast< MiaPoint4D(*)>(_a[1]))); break;
        case 2: _t->setRotationAngle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setBulkTransform((*reinterpret_cast< MiaAffineTransform(*)>(_a[1]))); break;
        case 4: _t->markerAdded((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 5: _t->markerUpdated((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 6: _t->markerRemoved((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 7: _t->setRotationMode((*reinterpret_cast< RotationMode(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaCurvedMPRSlicer::*_t)(QList<MiaMarker*> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaCurvedMPRSlicer::getInputMarkers)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MiaCurvedMPRSlicer::staticMetaObject = {
    { &MiaModule::staticMetaObject, qt_meta_stringdata_MiaCurvedMPRSlicer.data,
      qt_meta_data_MiaCurvedMPRSlicer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaCurvedMPRSlicer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaCurvedMPRSlicer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaCurvedMPRSlicer.stringdata0))
        return static_cast<void*>(const_cast< MiaCurvedMPRSlicer*>(this));
    return MiaModule::qt_metacast(_clname);
}

int MiaCurvedMPRSlicer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MiaCurvedMPRSlicer::getInputMarkers(QList<MiaMarker*> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

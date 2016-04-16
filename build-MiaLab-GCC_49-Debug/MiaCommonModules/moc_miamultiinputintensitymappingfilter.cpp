/****************************************************************************
** Meta object code from reading C++ file 'miamultiinputintensitymappingfilter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaCommonModules/miamultiinputintensitymappingfilter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miamultiinputintensitymappingfilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaMultiInputIntensityMappingFilter_t {
    QByteArrayData data[12];
    char stringdata0[160];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaMultiInputIntensityMappingFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaMultiInputIntensityMappingFilter_t qt_meta_stringdata_MiaMultiInputIntensityMappingFilter = {
    {
QT_MOC_LITERAL(0, 0, 35), // "MiaMultiInputIntensityMapping..."
QT_MOC_LITERAL(1, 36, 17), // "setMultiplyFactor"
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 1), // "a"
QT_MOC_LITERAL(4, 57, 1), // "b"
QT_MOC_LITERAL(5, 59, 24), // "setBrainSurfaceThickness"
QT_MOC_LITERAL(6, 84, 1), // "c"
QT_MOC_LITERAL(7, 86, 29), // "setBackgroundValueOutsideMask"
QT_MOC_LITERAL(8, 116, 16), // "setFavoritePhase"
QT_MOC_LITERAL(9, 133, 6), // "offset"
QT_MOC_LITERAL(10, 140, 9), // "threshold"
QT_MOC_LITERAL(11, 150, 9) // "normalize"

    },
    "MiaMultiInputIntensityMappingFilter\0"
    "setMultiplyFactor\0\0a\0b\0setBrainSurfaceThickness\0"
    "c\0setBackgroundValueOutsideMask\0"
    "setFavoritePhase\0offset\0threshold\0"
    "normalize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaMultiInputIntensityMappingFilter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x0a /* Public */,
       5,    1,   39,    2, 0x0a /* Public */,
       7,    1,   42,    2, 0x0a /* Public */,
       8,    3,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void, QMetaType::Float,    6,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float,    9,   10,   11,

       0        // eod
};

void MiaMultiInputIntensityMappingFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaMultiInputIntensityMappingFilter *_t = static_cast<MiaMultiInputIntensityMappingFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setMultiplyFactor((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->setBrainSurfaceThickness((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->setBackgroundValueOutsideMask((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setFavoritePhase((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject MiaMultiInputIntensityMappingFilter::staticMetaObject = {
    { &MiaModule2In1Out::staticMetaObject, qt_meta_stringdata_MiaMultiInputIntensityMappingFilter.data,
      qt_meta_data_MiaMultiInputIntensityMappingFilter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaMultiInputIntensityMappingFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaMultiInputIntensityMappingFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaMultiInputIntensityMappingFilter.stringdata0))
        return static_cast<void*>(const_cast< MiaMultiInputIntensityMappingFilter*>(this));
    return MiaModule2In1Out::qt_metacast(_clname);
}

int MiaMultiInputIntensityMappingFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaModule2In1Out::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

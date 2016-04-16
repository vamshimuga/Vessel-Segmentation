/****************************************************************************
** Meta object code from reading C++ file 'miaimageforestingtransform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaCommonModules/miaimageforestingtransform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miaimageforestingtransform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaImageForestingTransform_t {
    QByteArrayData data[13];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaImageForestingTransform_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaImageForestingTransform_t qt_meta_stringdata_MiaImageForestingTransform = {
    {
QT_MOC_LITERAL(0, 0, 26), // "MiaImageForestingTransform"
QT_MOC_LITERAL(1, 27, 15), // "getInputMarkers"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 18), // "QList<MiaMarker*>&"
QT_MOC_LITERAL(4, 63, 4), // "list"
QT_MOC_LITERAL(5, 68, 11), // "markerAdded"
QT_MOC_LITERAL(6, 80, 10), // "MiaMarker*"
QT_MOC_LITERAL(7, 91, 6), // "marker"
QT_MOC_LITERAL(8, 98, 13), // "markerUpdated"
QT_MOC_LITERAL(9, 112, 13), // "markerRemoved"
QT_MOC_LITERAL(10, 126, 17), // "getOutputTagImage"
QT_MOC_LITERAL(11, 144, 12), // "MiaImageND**"
QT_MOC_LITERAL(12, 157, 3) // "img"

    },
    "MiaImageForestingTransform\0getInputMarkers\0"
    "\0QList<MiaMarker*>&\0list\0markerAdded\0"
    "MiaMarker*\0marker\0markerUpdated\0"
    "markerRemoved\0getOutputTagImage\0"
    "MiaImageND**\0img"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaImageForestingTransform[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   42,    2, 0x0a /* Public */,
       8,    1,   45,    2, 0x0a /* Public */,
       9,    1,   48,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void MiaImageForestingTransform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaImageForestingTransform *_t = static_cast<MiaImageForestingTransform *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getInputMarkers((*reinterpret_cast< QList<MiaMarker*>(*)>(_a[1]))); break;
        case 1: _t->markerAdded((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 2: _t->markerUpdated((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 3: _t->markerRemoved((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 4: _t->getOutputTagImage((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 3:
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
            typedef void (MiaImageForestingTransform::*_t)(QList<MiaMarker*> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImageForestingTransform::getInputMarkers)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MiaImageForestingTransform::staticMetaObject = {
    { &MiaModule::staticMetaObject, qt_meta_stringdata_MiaImageForestingTransform.data,
      qt_meta_data_MiaImageForestingTransform,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaImageForestingTransform::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaImageForestingTransform::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaImageForestingTransform.stringdata0))
        return static_cast<void*>(const_cast< MiaImageForestingTransform*>(this));
    return MiaModule::qt_metacast(_clname);
}

int MiaImageForestingTransform::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MiaImageForestingTransform::getInputMarkers(QList<MiaMarker*> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

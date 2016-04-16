/****************************************************************************
** Meta object code from reading C++ file 'miabasicmprandvrtnetwork.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaApp/MiaNetworks/miabasicmprandvrtnetwork.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miabasicmprandvrtnetwork.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaBasicMPRandVRTNetwork_t {
    QByteArrayData data[5];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaBasicMPRandVRTNetwork_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaBasicMPRandVRTNetwork_t qt_meta_stringdata_MiaBasicMPRandVRTNetwork = {
    {
QT_MOC_LITERAL(0, 0, 24), // "MiaBasicMPRandVRTNetwork"
QT_MOC_LITERAL(1, 25, 29), // "setInteractionToWindowSetting"
QT_MOC_LITERAL(2, 55, 0), // ""
QT_MOC_LITERAL(3, 56, 20), // "setInteractionToZoom"
QT_MOC_LITERAL(4, 77, 19) // "setInteractionToPan"

    },
    "MiaBasicMPRandVRTNetwork\0"
    "setInteractionToWindowSetting\0\0"
    "setInteractionToZoom\0setInteractionToPan"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaBasicMPRandVRTNetwork[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MiaBasicMPRandVRTNetwork::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaBasicMPRandVRTNetwork *_t = static_cast<MiaBasicMPRandVRTNetwork *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setInteractionToWindowSetting(); break;
        case 1: _t->setInteractionToZoom(); break;
        case 2: _t->setInteractionToPan(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MiaBasicMPRandVRTNetwork::staticMetaObject = {
    { &MiaNetwork::staticMetaObject, qt_meta_stringdata_MiaBasicMPRandVRTNetwork.data,
      qt_meta_data_MiaBasicMPRandVRTNetwork,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaBasicMPRandVRTNetwork::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaBasicMPRandVRTNetwork::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaBasicMPRandVRTNetwork.stringdata0))
        return static_cast<void*>(const_cast< MiaBasicMPRandVRTNetwork*>(this));
    return MiaNetwork::qt_metacast(_clname);
}

int MiaBasicMPRandVRTNetwork::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaNetwork::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

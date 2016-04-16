/****************************************************************************
** Meta object code from reading C++ file 'miaregistrationnetwork.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaApp/MiaNetworks/miaregistrationnetwork.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miaregistrationnetwork.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaRegistrationNetwork_t {
    QByteArrayData data[9];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaRegistrationNetwork_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaRegistrationNetwork_t qt_meta_stringdata_MiaRegistrationNetwork = {
    {
QT_MOC_LITERAL(0, 0, 22), // "MiaRegistrationNetwork"
QT_MOC_LITERAL(1, 23, 15), // "openMovingImage"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 17), // "startRegistration"
QT_MOC_LITERAL(4, 58, 20), // "registrationFinished"
QT_MOC_LITERAL(5, 79, 23), // "saveTransformParameters"
QT_MOC_LITERAL(6, 103, 23), // "loadTransformParameters"
QT_MOC_LITERAL(7, 127, 23), // "saveRegistrationResults"
QT_MOC_LITERAL(8, 151, 23) // "resetRegistrationFilter"

    },
    "MiaRegistrationNetwork\0openMovingImage\0"
    "\0startRegistration\0registrationFinished\0"
    "saveTransformParameters\0loadTransformParameters\0"
    "saveRegistrationResults\0resetRegistrationFilter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaRegistrationNetwork[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MiaRegistrationNetwork::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaRegistrationNetwork *_t = static_cast<MiaRegistrationNetwork *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openMovingImage(); break;
        case 1: _t->startRegistration(); break;
        case 2: _t->registrationFinished(); break;
        case 3: _t->saveTransformParameters(); break;
        case 4: _t->loadTransformParameters(); break;
        case 5: _t->saveRegistrationResults(); break;
        case 6: _t->resetRegistrationFilter(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MiaRegistrationNetwork::staticMetaObject = {
    { &MiaBasicMPRandVRTNetwork::staticMetaObject, qt_meta_stringdata_MiaRegistrationNetwork.data,
      qt_meta_data_MiaRegistrationNetwork,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaRegistrationNetwork::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaRegistrationNetwork::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaRegistrationNetwork.stringdata0))
        return static_cast<void*>(const_cast< MiaRegistrationNetwork*>(this));
    return MiaBasicMPRandVRTNetwork::qt_metacast(_clname);
}

int MiaRegistrationNetwork::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaBasicMPRandVRTNetwork::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

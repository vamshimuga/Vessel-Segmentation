/****************************************************************************
** Meta object code from reading C++ file 'miaimageregistrationfilter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaLevelSetModules/miaimageregistrationfilter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miaimageregistrationfilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaImageRegistrationFilter_t {
    QByteArrayData data[18];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaImageRegistrationFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaImageRegistrationFilter_t qt_meta_stringdata_MiaImageRegistrationFilter = {
    {
QT_MOC_LITERAL(0, 0, 26), // "MiaImageRegistrationFilter"
QT_MOC_LITERAL(1, 27, 24), // "requestUpdateMovingImage"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 12), // "MiaImageND**"
QT_MOC_LITERAL(4, 66, 3), // "img"
QT_MOC_LITERAL(5, 70, 27), // "requestUpdateWeightingImage"
QT_MOC_LITERAL(6, 98, 22), // "requestUpdateKeyPoints"
QT_MOC_LITERAL(7, 121, 17), // "std::vector<int>&"
QT_MOC_LITERAL(8, 139, 4), // "list"
QT_MOC_LITERAL(9, 144, 18), // "registrationUpdate"
QT_MOC_LITERAL(10, 163, 20), // "registrationComplete"
QT_MOC_LITERAL(11, 184, 25), // "forwardReigstrationUpdate"
QT_MOC_LITERAL(12, 210, 27), // "forwardRegistrationComplete"
QT_MOC_LITERAL(13, 238, 17), // "startRegistration"
QT_MOC_LITERAL(14, 256, 24), // "registrationRunIntoError"
QT_MOC_LITERAL(15, 281, 17), // "resetRegistration"
QT_MOC_LITERAL(16, 299, 19), // "setMaximumIteration"
QT_MOC_LITERAL(17, 319, 1) // "n"

    },
    "MiaImageRegistrationFilter\0"
    "requestUpdateMovingImage\0\0MiaImageND**\0"
    "img\0requestUpdateWeightingImage\0"
    "requestUpdateKeyPoints\0std::vector<int>&\0"
    "list\0registrationUpdate\0registrationComplete\0"
    "forwardReigstrationUpdate\0"
    "forwardRegistrationComplete\0"
    "startRegistration\0registrationRunIntoError\0"
    "resetRegistration\0setMaximumIteration\0"
    "n"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaImageRegistrationFilter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,
       9,    0,   78,    2, 0x06 /* Public */,
      10,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   80,    2, 0x0a /* Public */,
      12,    0,   81,    2, 0x0a /* Public */,
      13,    0,   82,    2, 0x0a /* Public */,
      14,    0,   83,    2, 0x0a /* Public */,
      15,    0,   84,    2, 0x0a /* Public */,
      16,    1,   85,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,

       0        // eod
};

void MiaImageRegistrationFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaImageRegistrationFilter *_t = static_cast<MiaImageRegistrationFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestUpdateMovingImage((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        case 1: _t->requestUpdateWeightingImage((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        case 2: _t->requestUpdateKeyPoints((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 3: _t->registrationUpdate(); break;
        case 4: _t->registrationComplete(); break;
        case 5: _t->forwardReigstrationUpdate(); break;
        case 6: _t->forwardRegistrationComplete(); break;
        case 7: _t->startRegistration(); break;
        case 8: _t->registrationRunIntoError(); break;
        case 9: _t->resetRegistration(); break;
        case 10: _t->setMaximumIteration((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaImageRegistrationFilter::*_t)(MiaImageND * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImageRegistrationFilter::requestUpdateMovingImage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaImageRegistrationFilter::*_t)(MiaImageND * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImageRegistrationFilter::requestUpdateWeightingImage)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MiaImageRegistrationFilter::*_t)(std::vector<int> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImageRegistrationFilter::requestUpdateKeyPoints)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MiaImageRegistrationFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImageRegistrationFilter::registrationUpdate)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MiaImageRegistrationFilter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImageRegistrationFilter::registrationComplete)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject MiaImageRegistrationFilter::staticMetaObject = {
    { &MiaModule::staticMetaObject, qt_meta_stringdata_MiaImageRegistrationFilter.data,
      qt_meta_data_MiaImageRegistrationFilter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaImageRegistrationFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaImageRegistrationFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaImageRegistrationFilter.stringdata0))
        return static_cast<void*>(const_cast< MiaImageRegistrationFilter*>(this));
    return MiaModule::qt_metacast(_clname);
}

int MiaImageRegistrationFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaModule::qt_metacall(_c, _id, _a);
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
void MiaImageRegistrationFilter::requestUpdateMovingImage(MiaImageND * * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MiaImageRegistrationFilter::requestUpdateWeightingImage(MiaImageND * * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MiaImageRegistrationFilter::requestUpdateKeyPoints(std::vector<int> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MiaImageRegistrationFilter::registrationUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void MiaImageRegistrationFilter::registrationComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

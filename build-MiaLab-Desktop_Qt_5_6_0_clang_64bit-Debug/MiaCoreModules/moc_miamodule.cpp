/****************************************************************************
** Meta object code from reading C++ file 'miamodule.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaCoreModules/miamodule.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miamodule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaModule_t {
    QByteArrayData data[17];
    char stringdata0[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaModule_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaModule_t qt_meta_stringdata_MiaModule = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MiaModule"
QT_MOC_LITERAL(1, 10, 13), // "outputUpdated"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 19), // "outputMarkerUpdated"
QT_MOC_LITERAL(4, 45, 16), // "MarkerUpdateType"
QT_MOC_LITERAL(5, 62, 3), // "msg"
QT_MOC_LITERAL(6, 66, 18), // "requestUpdateInput"
QT_MOC_LITERAL(7, 85, 12), // "MiaImageND**"
QT_MOC_LITERAL(8, 98, 3), // "img"
QT_MOC_LITERAL(9, 102, 16), // "parameterChanged"
QT_MOC_LITERAL(10, 119, 10), // "parameters"
QT_MOC_LITERAL(11, 130, 8), // "modified"
QT_MOC_LITERAL(12, 139, 12), // "inputUpdated"
QT_MOC_LITERAL(13, 152, 14), // "getOutputImage"
QT_MOC_LITERAL(14, 167, 13), // "setParameters"
QT_MOC_LITERAL(15, 181, 21), // "reLatedModuleModified"
QT_MOC_LITERAL(16, 203, 18) // "inputMarkerUpdated"

    },
    "MiaModule\0outputUpdated\0\0outputMarkerUpdated\0"
    "MarkerUpdateType\0msg\0requestUpdateInput\0"
    "MiaImageND**\0img\0parameterChanged\0"
    "parameters\0modified\0inputUpdated\0"
    "getOutputImage\0setParameters\0"
    "reLatedModuleModified\0inputMarkerUpdated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaModule[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,
       6,    1,   68,    2, 0x06 /* Public */,
       9,    1,   71,    2, 0x06 /* Public */,
      11,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   75,    2, 0x0a /* Public */,
      13,    1,   76,    2, 0x0a /* Public */,
      14,    1,   79,    2, 0x0a /* Public */,
      15,    0,   82,    2, 0x0a /* Public */,
      16,    1,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QVariantMap,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QVariantMap,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void MiaModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaModule *_t = static_cast<MiaModule *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->outputUpdated(); break;
        case 1: _t->outputMarkerUpdated((*reinterpret_cast< MarkerUpdateType(*)>(_a[1]))); break;
        case 2: _t->requestUpdateInput((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        case 3: _t->parameterChanged((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 4: _t->modified(); break;
        case 5: _t->inputUpdated(); break;
        case 6: _t->getOutputImage((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        case 7: _t->setParameters((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 8: _t->reLatedModuleModified(); break;
        case 9: _t->inputMarkerUpdated((*reinterpret_cast< MarkerUpdateType(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaModule::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaModule::outputUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaModule::*_t)(MarkerUpdateType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaModule::outputMarkerUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MiaModule::*_t)(MiaImageND * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaModule::requestUpdateInput)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MiaModule::*_t)(const QMap<QString,QVariant> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaModule::parameterChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MiaModule::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaModule::modified)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject MiaModule::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MiaModule.data,
      qt_meta_data_MiaModule,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaModule::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaModule.stringdata0))
        return static_cast<void*>(const_cast< MiaModule*>(this));
    return QObject::qt_metacast(_clname);
}

int MiaModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MiaModule::outputUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MiaModule::outputMarkerUpdated(MarkerUpdateType _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MiaModule::requestUpdateInput(MiaImageND * * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MiaModule::parameterChanged(const QMap<QString,QVariant> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MiaModule::modified()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

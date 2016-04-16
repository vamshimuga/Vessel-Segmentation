/****************************************************************************
** Meta object code from reading C++ file 'miaparameterconnector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaCommonModules/miaparameterconnector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miaparameterconnector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaParameterConnector_t {
    QByteArrayData data[7];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaParameterConnector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaParameterConnector_t qt_meta_stringdata_MiaParameterConnector = {
    {
QT_MOC_LITERAL(0, 0, 21), // "MiaParameterConnector"
QT_MOC_LITERAL(1, 22, 21), // "upperParameterChanged"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 10), // "parameters"
QT_MOC_LITERAL(4, 56, 21), // "lowerParameterChanged"
QT_MOC_LITERAL(5, 78, 27), // "upperModuleParameterChanged"
QT_MOC_LITERAL(6, 106, 27) // "lowerModuleParameterChanged"

    },
    "MiaParameterConnector\0upperParameterChanged\0"
    "\0parameters\0lowerParameterChanged\0"
    "upperModuleParameterChanged\0"
    "lowerModuleParameterChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaParameterConnector[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   40,    2, 0x0a /* Public */,
       6,    1,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantMap,    3,
    QMetaType::Void, QMetaType::QVariantMap,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariantMap,    3,
    QMetaType::Void, QMetaType::QVariantMap,    3,

       0        // eod
};

void MiaParameterConnector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaParameterConnector *_t = static_cast<MiaParameterConnector *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->upperParameterChanged((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 1: _t->lowerParameterChanged((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 2: _t->upperModuleParameterChanged((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 3: _t->lowerModuleParameterChanged((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaParameterConnector::*_t)(const QMap<QString,QVariant> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaParameterConnector::upperParameterChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaParameterConnector::*_t)(const QMap<QString,QVariant> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaParameterConnector::lowerParameterChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MiaParameterConnector::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MiaParameterConnector.data,
      qt_meta_data_MiaParameterConnector,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaParameterConnector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaParameterConnector::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaParameterConnector.stringdata0))
        return static_cast<void*>(const_cast< MiaParameterConnector*>(this));
    return QObject::qt_metacast(_clname);
}

int MiaParameterConnector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void MiaParameterConnector::upperParameterChanged(const QMap<QString,QVariant> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MiaParameterConnector::lowerParameterChanged(const QMap<QString,QVariant> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

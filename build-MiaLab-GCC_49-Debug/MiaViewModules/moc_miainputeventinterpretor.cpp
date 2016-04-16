/****************************************************************************
** Meta object code from reading C++ file 'miainputeventinterpretor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaViewModules/miainputeventinterpretor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miainputeventinterpretor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaInputEventInterpretor_t {
    QByteArrayData data[19];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaInputEventInterpretor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaInputEventInterpretor_t qt_meta_stringdata_MiaInputEventInterpretor = {
    {
QT_MOC_LITERAL(0, 0, 24), // "MiaInputEventInterpretor"
QT_MOC_LITERAL(1, 25, 16), // "parameterChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 10), // "parameters"
QT_MOC_LITERAL(4, 54, 22), // "forwardButtonPressedAt"
QT_MOC_LITERAL(5, 77, 10), // "MiaPoint4D"
QT_MOC_LITERAL(6, 88, 2), // "pt"
QT_MOC_LITERAL(7, 91, 13), // "MiaTransform&"
QT_MOC_LITERAL(8, 105, 14), // "sceneTransform"
QT_MOC_LITERAL(9, 120, 22), // "forwardButtonReleaseAt"
QT_MOC_LITERAL(10, 143, 19), // "forwardMouseMovedAt"
QT_MOC_LITERAL(11, 163, 25), // "forwardMouseDoubleClickAt"
QT_MOC_LITERAL(12, 189, 17), // "forwardKeyPressed"
QT_MOC_LITERAL(13, 207, 3), // "key"
QT_MOC_LITERAL(14, 211, 18), // "forwardKeyReleased"
QT_MOC_LITERAL(15, 230, 12), // "updateXValue"
QT_MOC_LITERAL(16, 243, 3), // "var"
QT_MOC_LITERAL(17, 247, 12), // "updateYValue"
QT_MOC_LITERAL(18, 260, 13) // "setParameters"

    },
    "MiaInputEventInterpretor\0parameterChanged\0"
    "\0parameters\0forwardButtonPressedAt\0"
    "MiaPoint4D\0pt\0MiaTransform&\0sceneTransform\0"
    "forwardButtonReleaseAt\0forwardMouseMovedAt\0"
    "forwardMouseDoubleClickAt\0forwardKeyPressed\0"
    "key\0forwardKeyReleased\0updateXValue\0"
    "var\0updateYValue\0setParameters"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaInputEventInterpretor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    2,   77,    2, 0x06 /* Public */,
       9,    2,   82,    2, 0x06 /* Public */,
      10,    2,   87,    2, 0x06 /* Public */,
      11,    2,   92,    2, 0x06 /* Public */,
      12,    2,   97,    2, 0x06 /* Public */,
      14,    2,  102,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    1,  107,    2, 0x0a /* Public */,
      17,    1,  110,    2, 0x0a /* Public */,
      15,    1,  113,    2, 0x0a /* Public */,
      17,    1,  116,    2, 0x0a /* Public */,
      18,    1,  119,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantMap,    3,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7,    6,    8,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,   13,    8,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,   13,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,   16,
    QMetaType::Void, QMetaType::Float,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::QVariantMap,    3,

       0        // eod
};

void MiaInputEventInterpretor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaInputEventInterpretor *_t = static_cast<MiaInputEventInterpretor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parameterChanged((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 1: _t->forwardButtonPressedAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 2: _t->forwardButtonReleaseAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 3: _t->forwardMouseMovedAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 4: _t->forwardMouseDoubleClickAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 5: _t->forwardKeyPressed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 6: _t->forwardKeyReleased((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 7: _t->updateXValue((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->updateYValue((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->updateXValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->updateYValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setParameters((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaInputEventInterpretor::*_t)(const QMap<QString,QVariant> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaInputEventInterpretor::parameterChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaInputEventInterpretor::*_t)(MiaPoint4D , MiaTransform & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaInputEventInterpretor::forwardButtonPressedAt)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MiaInputEventInterpretor::*_t)(MiaPoint4D , MiaTransform & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaInputEventInterpretor::forwardButtonReleaseAt)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MiaInputEventInterpretor::*_t)(MiaPoint4D , MiaTransform & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaInputEventInterpretor::forwardMouseMovedAt)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MiaInputEventInterpretor::*_t)(MiaPoint4D , MiaTransform & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaInputEventInterpretor::forwardMouseDoubleClickAt)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MiaInputEventInterpretor::*_t)(int , MiaTransform & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaInputEventInterpretor::forwardKeyPressed)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MiaInputEventInterpretor::*_t)(int , MiaTransform & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaInputEventInterpretor::forwardKeyReleased)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject MiaInputEventInterpretor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MiaInputEventInterpretor.data,
      qt_meta_data_MiaInputEventInterpretor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaInputEventInterpretor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaInputEventInterpretor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaInputEventInterpretor.stringdata0))
        return static_cast<void*>(const_cast< MiaInputEventInterpretor*>(this));
    return QObject::qt_metacast(_clname);
}

int MiaInputEventInterpretor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MiaInputEventInterpretor::parameterChanged(const QMap<QString,QVariant> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MiaInputEventInterpretor::forwardButtonPressedAt(MiaPoint4D _t1, MiaTransform & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MiaInputEventInterpretor::forwardButtonReleaseAt(MiaPoint4D _t1, MiaTransform & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MiaInputEventInterpretor::forwardMouseMovedAt(MiaPoint4D _t1, MiaTransform & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MiaInputEventInterpretor::forwardMouseDoubleClickAt(MiaPoint4D _t1, MiaTransform & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MiaInputEventInterpretor::forwardKeyPressed(int _t1, MiaTransform & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MiaInputEventInterpretor::forwardKeyReleased(int _t1, MiaTransform & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'miamarker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaCoreModules/miamarker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miamarker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaMarker_t {
    QByteArrayData data[9];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaMarker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaMarker_t qt_meta_stringdata_MiaMarker = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MiaMarker"
QT_MOC_LITERAL(1, 10, 19), // "markerWillBeRemoved"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "MiaMarker*"
QT_MOC_LITERAL(4, 42, 6), // "marker"
QT_MOC_LITERAL(5, 49, 17), // "markerModeChanged"
QT_MOC_LITERAL(6, 67, 13), // "markerUpdated"
QT_MOC_LITERAL(7, 81, 14), // "markerSelected"
QT_MOC_LITERAL(8, 96, 15) // "markerCompleted"

    },
    "MiaMarker\0markerWillBeRemoved\0\0"
    "MiaMarker*\0marker\0markerModeChanged\0"
    "markerUpdated\0markerSelected\0"
    "markerCompleted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaMarker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       6,    1,   45,    2, 0x06 /* Public */,
       7,    1,   48,    2, 0x06 /* Public */,
       8,    1,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void MiaMarker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaMarker *_t = static_cast<MiaMarker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markerWillBeRemoved((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 1: _t->markerModeChanged((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 2: _t->markerUpdated((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 3: _t->markerSelected((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 4: _t->markerCompleted((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
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
        case 4:
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
            typedef void (MiaMarker::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaMarker::markerWillBeRemoved)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaMarker::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaMarker::markerModeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MiaMarker::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaMarker::markerUpdated)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MiaMarker::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaMarker::markerSelected)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MiaMarker::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaMarker::markerCompleted)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject MiaMarker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MiaMarker.data,
      qt_meta_data_MiaMarker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaMarker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaMarker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaMarker.stringdata0))
        return static_cast<void*>(const_cast< MiaMarker*>(this));
    return QObject::qt_metacast(_clname);
}

int MiaMarker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void MiaMarker::markerWillBeRemoved(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MiaMarker::markerModeChanged(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MiaMarker::markerUpdated(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MiaMarker::markerSelected(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MiaMarker::markerCompleted(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE

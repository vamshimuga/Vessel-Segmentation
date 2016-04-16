/****************************************************************************
** Meta object code from reading C++ file 'miasparsefieldlevelsetsegmenter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaLevelSetModules/miasparsefieldlevelsetsegmenter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miasparsefieldlevelsetsegmenter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaSparseFieldLevelSetSegmenter_t {
    QByteArrayData data[21];
    char stringdata0[427];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaSparseFieldLevelSetSegmenter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaSparseFieldLevelSetSegmenter_t qt_meta_stringdata_MiaSparseFieldLevelSetSegmenter = {
    {
QT_MOC_LITERAL(0, 0, 31), // "MiaSparseFieldLevelSetSegmenter"
QT_MOC_LITERAL(1, 32, 36), // "requestUpdateInitialLevelSetF..."
QT_MOC_LITERAL(2, 69, 0), // ""
QT_MOC_LITERAL(3, 70, 12), // "MiaImageND**"
QT_MOC_LITERAL(4, 83, 3), // "img"
QT_MOC_LITERAL(5, 87, 19), // "getInputCenterlines"
QT_MOC_LITERAL(6, 107, 18), // "QList<MiaMarker*>&"
QT_MOC_LITERAL(7, 126, 4), // "list"
QT_MOC_LITERAL(8, 131, 30), // "requestUpdateAdvectionFunction"
QT_MOC_LITERAL(9, 162, 23), // "levelsetFunctionUpdated"
QT_MOC_LITERAL(10, 186, 22), // "levelsetEnterNewPeriod"
QT_MOC_LITERAL(11, 209, 1), // "n"
QT_MOC_LITERAL(12, 211, 25), // "levelsetFunctionCompleted"
QT_MOC_LITERAL(13, 237, 30), // "forwardLevelSetFunctionUpdated"
QT_MOC_LITERAL(14, 268, 32), // "forwardLevelsetFunctionCompleted"
QT_MOC_LITERAL(15, 301, 29), // "forwardLevelsetEnterNewPeriod"
QT_MOC_LITERAL(16, 331, 21), // "levelsetFunctionReset"
QT_MOC_LITERAL(17, 353, 22), // "advectionFunctionReset"
QT_MOC_LITERAL(18, 376, 12), // "getStatusMap"
QT_MOC_LITERAL(19, 389, 19), // "getNarrowBandPoints"
QT_MOC_LITERAL(20, 409, 17) // "std::vector<int>&"

    },
    "MiaSparseFieldLevelSetSegmenter\0"
    "requestUpdateInitialLevelSetFunction\0"
    "\0MiaImageND**\0img\0getInputCenterlines\0"
    "QList<MiaMarker*>&\0list\0"
    "requestUpdateAdvectionFunction\0"
    "levelsetFunctionUpdated\0levelsetEnterNewPeriod\0"
    "n\0levelsetFunctionCompleted\0"
    "forwardLevelSetFunctionUpdated\0"
    "forwardLevelsetFunctionCompleted\0"
    "forwardLevelsetEnterNewPeriod\0"
    "levelsetFunctionReset\0advectionFunctionReset\0"
    "getStatusMap\0getNarrowBandPoints\0"
    "std::vector<int>&"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaSparseFieldLevelSetSegmenter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       5,    1,   82,    2, 0x06 /* Public */,
       8,    1,   85,    2, 0x06 /* Public */,
       9,    0,   88,    2, 0x06 /* Public */,
      10,    1,   89,    2, 0x06 /* Public */,
      12,    0,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   93,    2, 0x0a /* Public */,
      14,    0,   94,    2, 0x0a /* Public */,
      15,    1,   95,    2, 0x0a /* Public */,
      16,    0,   98,    2, 0x0a /* Public */,
      17,    0,   99,    2, 0x0a /* Public */,
      18,    1,  100,    2, 0x0a /* Public */,
      19,    1,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 20,    7,

       0        // eod
};

void MiaSparseFieldLevelSetSegmenter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaSparseFieldLevelSetSegmenter *_t = static_cast<MiaSparseFieldLevelSetSegmenter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestUpdateInitialLevelSetFunction((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        case 1: _t->getInputCenterlines((*reinterpret_cast< QList<MiaMarker*>(*)>(_a[1]))); break;
        case 2: _t->requestUpdateAdvectionFunction((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        case 3: _t->levelsetFunctionUpdated(); break;
        case 4: _t->levelsetEnterNewPeriod((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->levelsetFunctionCompleted(); break;
        case 6: _t->forwardLevelSetFunctionUpdated(); break;
        case 7: _t->forwardLevelsetFunctionCompleted(); break;
        case 8: _t->forwardLevelsetEnterNewPeriod((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->levelsetFunctionReset(); break;
        case 10: _t->advectionFunctionReset(); break;
        case 11: _t->getStatusMap((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        case 12: _t->getNarrowBandPoints((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaSparseFieldLevelSetSegmenter::*_t)(MiaImageND * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaSparseFieldLevelSetSegmenter::requestUpdateInitialLevelSetFunction)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaSparseFieldLevelSetSegmenter::*_t)(QList<MiaMarker*> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaSparseFieldLevelSetSegmenter::getInputCenterlines)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MiaSparseFieldLevelSetSegmenter::*_t)(MiaImageND * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaSparseFieldLevelSetSegmenter::requestUpdateAdvectionFunction)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MiaSparseFieldLevelSetSegmenter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaSparseFieldLevelSetSegmenter::levelsetFunctionUpdated)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MiaSparseFieldLevelSetSegmenter::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaSparseFieldLevelSetSegmenter::levelsetEnterNewPeriod)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MiaSparseFieldLevelSetSegmenter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaSparseFieldLevelSetSegmenter::levelsetFunctionCompleted)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject MiaSparseFieldLevelSetSegmenter::staticMetaObject = {
    { &MiaModule::staticMetaObject, qt_meta_stringdata_MiaSparseFieldLevelSetSegmenter.data,
      qt_meta_data_MiaSparseFieldLevelSetSegmenter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaSparseFieldLevelSetSegmenter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaSparseFieldLevelSetSegmenter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaSparseFieldLevelSetSegmenter.stringdata0))
        return static_cast<void*>(const_cast< MiaSparseFieldLevelSetSegmenter*>(this));
    return MiaModule::qt_metacast(_clname);
}

int MiaSparseFieldLevelSetSegmenter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MiaSparseFieldLevelSetSegmenter::requestUpdateInitialLevelSetFunction(MiaImageND * * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MiaSparseFieldLevelSetSegmenter::getInputCenterlines(QList<MiaMarker*> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MiaSparseFieldLevelSetSegmenter::requestUpdateAdvectionFunction(MiaImageND * * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MiaSparseFieldLevelSetSegmenter::levelsetFunctionUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void MiaSparseFieldLevelSetSegmenter::levelsetEnterNewPeriod(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MiaSparseFieldLevelSetSegmenter::levelsetFunctionCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'miadrawingmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaViewModules/miadrawingmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miadrawingmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaDrawingManager_t {
    QByteArrayData data[35];
    char stringdata0[500];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaDrawingManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaDrawingManager_t qt_meta_stringdata_MiaDrawingManager = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MiaDrawingManager"
QT_MOC_LITERAL(1, 18, 13), // "markerUpdated"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "MiaMarker*"
QT_MOC_LITERAL(4, 44, 6), // "marker"
QT_MOC_LITERAL(5, 51, 20), // "markerHasBeenRemoved"
QT_MOC_LITERAL(6, 72, 11), // "markerAdded"
QT_MOC_LITERAL(7, 84, 17), // "markerModeChanged"
QT_MOC_LITERAL(8, 102, 16), // "parameterChanged"
QT_MOC_LITERAL(9, 119, 19), // "QMap<QString,float>"
QT_MOC_LITERAL(10, 139, 10), // "parameters"
QT_MOC_LITERAL(11, 150, 13), // "mouseReleased"
QT_MOC_LITERAL(12, 164, 15), // "buttonPressedAt"
QT_MOC_LITERAL(13, 180, 10), // "MiaPoint4D"
QT_MOC_LITERAL(14, 191, 2), // "pt"
QT_MOC_LITERAL(15, 194, 13), // "MiaTransform&"
QT_MOC_LITERAL(16, 208, 14), // "sceneTransform"
QT_MOC_LITERAL(17, 223, 16), // "buttonReleasedAt"
QT_MOC_LITERAL(18, 240, 12), // "mouseMovedAt"
QT_MOC_LITERAL(19, 253, 20), // "mouseDoubleClickedAt"
QT_MOC_LITERAL(20, 274, 10), // "keyPressed"
QT_MOC_LITERAL(21, 285, 3), // "key"
QT_MOC_LITERAL(22, 289, 11), // "keyReleased"
QT_MOC_LITERAL(23, 301, 19), // "selectAnotherMarker"
QT_MOC_LITERAL(24, 321, 13), // "markerRemoved"
QT_MOC_LITERAL(25, 335, 22), // "currentMarkerCompleted"
QT_MOC_LITERAL(26, 358, 16), // "selectionChanged"
QT_MOC_LITERAL(27, 375, 14), // "QItemSelection"
QT_MOC_LITERAL(28, 390, 8), // "selected"
QT_MOC_LITERAL(29, 399, 10), // "deselected"
QT_MOC_LITERAL(30, 410, 13), // "getAllMarkers"
QT_MOC_LITERAL(31, 424, 18), // "QList<MiaMarker*>&"
QT_MOC_LITERAL(32, 443, 4), // "list"
QT_MOC_LITERAL(33, 448, 26), // "forwardMarkerUpdatedSignal"
QT_MOC_LITERAL(34, 475, 24) // "forwardMarkerModeChanged"

    },
    "MiaDrawingManager\0markerUpdated\0\0"
    "MiaMarker*\0marker\0markerHasBeenRemoved\0"
    "markerAdded\0markerModeChanged\0"
    "parameterChanged\0QMap<QString,float>\0"
    "parameters\0mouseReleased\0buttonPressedAt\0"
    "MiaPoint4D\0pt\0MiaTransform&\0sceneTransform\0"
    "buttonReleasedAt\0mouseMovedAt\0"
    "mouseDoubleClickedAt\0keyPressed\0key\0"
    "keyReleased\0selectAnotherMarker\0"
    "markerRemoved\0currentMarkerCompleted\0"
    "selectionChanged\0QItemSelection\0"
    "selected\0deselected\0getAllMarkers\0"
    "QList<MiaMarker*>&\0list\0"
    "forwardMarkerUpdatedSignal\0"
    "forwardMarkerModeChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaDrawingManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,
       5,    1,  112,    2, 0x06 /* Public */,
       6,    1,  115,    2, 0x06 /* Public */,
       7,    1,  118,    2, 0x06 /* Public */,
       8,    1,  121,    2, 0x06 /* Public */,
      11,    0,  124,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    2,  125,    2, 0x0a /* Public */,
      17,    2,  130,    2, 0x0a /* Public */,
      18,    2,  135,    2, 0x0a /* Public */,
      19,    2,  140,    2, 0x0a /* Public */,
      20,    2,  145,    2, 0x0a /* Public */,
      22,    2,  150,    2, 0x0a /* Public */,
      23,    1,  155,    2, 0x0a /* Public */,
      24,    1,  158,    2, 0x0a /* Public */,
      25,    1,  161,    2, 0x0a /* Public */,
      26,    2,  164,    2, 0x0a /* Public */,
      30,    1,  169,    2, 0x0a /* Public */,
      33,    1,  172,    2, 0x0a /* Public */,
      34,    1,  175,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,   16,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,   16,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,   16,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,   16,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,   21,   16,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,   21,   16,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 27, 0x80000000 | 27,   28,   29,
    QMetaType::Void, 0x80000000 | 31,   32,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void MiaDrawingManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaDrawingManager *_t = static_cast<MiaDrawingManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markerUpdated((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 1: _t->markerHasBeenRemoved((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 2: _t->markerAdded((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 3: _t->markerModeChanged((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 4: _t->parameterChanged((*reinterpret_cast< const QMap<QString,float>(*)>(_a[1]))); break;
        case 5: _t->mouseReleased(); break;
        case 6: _t->buttonPressedAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 7: _t->buttonReleasedAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 8: _t->mouseMovedAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 9: _t->mouseDoubleClickedAt((*reinterpret_cast< MiaPoint4D(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 10: _t->keyPressed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 11: _t->keyReleased((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< MiaTransform(*)>(_a[2]))); break;
        case 12: _t->selectAnotherMarker((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 13: _t->markerRemoved((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 14: _t->currentMarkerCompleted((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 15: _t->selectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 16: _t->getAllMarkers((*reinterpret_cast< QList<MiaMarker*>(*)>(_a[1]))); break;
        case 17: _t->forwardMarkerUpdatedSignal((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
        case 18: _t->forwardMarkerModeChanged((*reinterpret_cast< MiaMarker*(*)>(_a[1]))); break;
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
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QItemSelection >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker* >(); break;
            }
            break;
        case 18:
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
            typedef void (MiaDrawingManager::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaDrawingManager::markerUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaDrawingManager::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaDrawingManager::markerHasBeenRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MiaDrawingManager::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaDrawingManager::markerAdded)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MiaDrawingManager::*_t)(MiaMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaDrawingManager::markerModeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MiaDrawingManager::*_t)(const QMap<QString,float> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaDrawingManager::parameterChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MiaDrawingManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaDrawingManager::mouseReleased)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject MiaDrawingManager::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_MiaDrawingManager.data,
      qt_meta_data_MiaDrawingManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaDrawingManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaDrawingManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaDrawingManager.stringdata0))
        return static_cast<void*>(const_cast< MiaDrawingManager*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int MiaDrawingManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void MiaDrawingManager::markerUpdated(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MiaDrawingManager::markerHasBeenRemoved(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MiaDrawingManager::markerAdded(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MiaDrawingManager::markerModeChanged(MiaMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MiaDrawingManager::parameterChanged(const QMap<QString,float> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MiaDrawingManager::mouseReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

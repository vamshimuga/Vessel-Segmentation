/****************************************************************************
** Meta object code from reading C++ file 'miaImage2qimage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaViewModules/miaImage2qimage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miaImage2qimage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaImage2QImage_t {
    QByteArrayData data[8];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaImage2QImage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaImage2QImage_t qt_meta_stringdata_MiaImage2QImage = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MiaImage2QImage"
QT_MOC_LITERAL(1, 16, 17), // "qimageWillRemoved"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 23), // "MiaGraphics2DImageItem*"
QT_MOC_LITERAL(4, 59, 3), // "img"
QT_MOC_LITERAL(5, 63, 11), // "qimageAdded"
QT_MOC_LITERAL(6, 75, 21), // "qimageGeometryChanged"
QT_MOC_LITERAL(7, 97, 12) // "inputUpdated"

    },
    "MiaImage2QImage\0qimageWillRemoved\0\0"
    "MiaGraphics2DImageItem*\0img\0qimageAdded\0"
    "qimageGeometryChanged\0inputUpdated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaImage2QImage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MiaImage2QImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaImage2QImage *_t = static_cast<MiaImage2QImage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->qimageWillRemoved((*reinterpret_cast< MiaGraphics2DImageItem*(*)>(_a[1]))); break;
        case 1: _t->qimageAdded((*reinterpret_cast< MiaGraphics2DImageItem*(*)>(_a[1]))); break;
        case 2: _t->qimageGeometryChanged((*reinterpret_cast< MiaGraphics2DImageItem*(*)>(_a[1]))); break;
        case 3: _t->inputUpdated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaImage2QImage::*_t)(MiaGraphics2DImageItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImage2QImage::qimageWillRemoved)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MiaImage2QImage::*_t)(MiaGraphics2DImageItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImage2QImage::qimageAdded)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MiaImage2QImage::*_t)(MiaGraphics2DImageItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImage2QImage::qimageGeometryChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject MiaImage2QImage::staticMetaObject = {
    { &MiaModule::staticMetaObject, qt_meta_stringdata_MiaImage2QImage.data,
      qt_meta_data_MiaImage2QImage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaImage2QImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaImage2QImage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaImage2QImage.stringdata0))
        return static_cast<void*>(const_cast< MiaImage2QImage*>(this));
    return MiaModule::qt_metacast(_clname);
}

int MiaImage2QImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaModule::qt_metacall(_c, _id, _a);
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
void MiaImage2QImage::qimageWillRemoved(MiaGraphics2DImageItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MiaImage2QImage::qimageAdded(MiaGraphics2DImageItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MiaImage2QImage::qimageGeometryChanged(MiaGraphics2DImageItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

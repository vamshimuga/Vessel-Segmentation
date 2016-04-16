/****************************************************************************
** Meta object code from reading C++ file 'miaimageview2d.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaViewModules/miaimageview2d.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miaimageview2d.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaImageView2D_t {
    QByteArrayData data[13];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaImageView2D_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaImageView2D_t qt_meta_stringdata_MiaImageView2D = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MiaImageView2D"
QT_MOC_LITERAL(1, 15, 16), // "parameterChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "parameters"
QT_MOC_LITERAL(4, 44, 12), // "removeQImage"
QT_MOC_LITERAL(5, 57, 23), // "MiaGraphics2DImageItem*"
QT_MOC_LITERAL(6, 81, 3), // "img"
QT_MOC_LITERAL(7, 85, 9), // "addQImage"
QT_MOC_LITERAL(8, 95, 20), // "imageGeometryChanged"
QT_MOC_LITERAL(9, 116, 13), // "setParameters"
QT_MOC_LITERAL(10, 130, 18), // "removeMarkerModule"
QT_MOC_LITERAL(11, 149, 23), // "MiaMarker2GraphicsItem*"
QT_MOC_LITERAL(12, 173, 6) // "module"

    },
    "MiaImageView2D\0parameterChanged\0\0"
    "parameters\0removeQImage\0MiaGraphics2DImageItem*\0"
    "img\0addQImage\0imageGeometryChanged\0"
    "setParameters\0removeMarkerModule\0"
    "MiaMarker2GraphicsItem*\0module"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaImageView2D[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   47,    2, 0x0a /* Public */,
       7,    1,   50,    2, 0x0a /* Public */,
       8,    1,   53,    2, 0x0a /* Public */,
       9,    1,   56,    2, 0x0a /* Public */,
      10,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantMap,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QVariantMap,    3,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void MiaImageView2D::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaImageView2D *_t = static_cast<MiaImageView2D *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parameterChanged((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 1: _t->removeQImage((*reinterpret_cast< MiaGraphics2DImageItem*(*)>(_a[1]))); break;
        case 2: _t->addQImage((*reinterpret_cast< MiaGraphics2DImageItem*(*)>(_a[1]))); break;
        case 3: _t->imageGeometryChanged((*reinterpret_cast< MiaGraphics2DImageItem*(*)>(_a[1]))); break;
        case 4: _t->setParameters((*reinterpret_cast< const QMap<QString,QVariant>(*)>(_a[1]))); break;
        case 5: _t->removeMarkerModule((*reinterpret_cast< MiaMarker2GraphicsItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MiaMarker2GraphicsItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaImageView2D::*_t)(const QMap<QString,QVariant> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaImageView2D::parameterChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MiaImageView2D::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_MiaImageView2D.data,
      qt_meta_data_MiaImageView2D,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaImageView2D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaImageView2D::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaImageView2D.stringdata0))
        return static_cast<void*>(const_cast< MiaImageView2D*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int MiaImageView2D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MiaImageView2D::parameterChanged(const QMap<QString,QVariant> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

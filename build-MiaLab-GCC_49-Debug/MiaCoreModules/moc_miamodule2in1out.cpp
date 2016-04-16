/****************************************************************************
** Meta object code from reading C++ file 'miamodule2in1out.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MiaLab/MiaCoreModules/miamodule2in1out.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miamodule2in1out.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MiaModule2In1Out_t {
    QByteArrayData data[5];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MiaModule2In1Out_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MiaModule2In1Out_t qt_meta_stringdata_MiaModule2In1Out = {
    {
QT_MOC_LITERAL(0, 0, 16), // "MiaModule2In1Out"
QT_MOC_LITERAL(1, 17, 24), // "requestUpdateSecondInput"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 12), // "MiaImageND**"
QT_MOC_LITERAL(4, 56, 3) // "img"

    },
    "MiaModule2In1Out\0requestUpdateSecondInput\0"
    "\0MiaImageND**\0img"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MiaModule2In1Out[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void MiaModule2In1Out::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MiaModule2In1Out *_t = static_cast<MiaModule2In1Out *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestUpdateSecondInput((*reinterpret_cast< MiaImageND**(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MiaModule2In1Out::*_t)(MiaImageND * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MiaModule2In1Out::requestUpdateSecondInput)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MiaModule2In1Out::staticMetaObject = {
    { &MiaModule::staticMetaObject, qt_meta_stringdata_MiaModule2In1Out.data,
      qt_meta_data_MiaModule2In1Out,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MiaModule2In1Out::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MiaModule2In1Out::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MiaModule2In1Out.stringdata0))
        return static_cast<void*>(const_cast< MiaModule2In1Out*>(this));
    return MiaModule::qt_metacast(_clname);
}

int MiaModule2In1Out::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MiaModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MiaModule2In1Out::requestUpdateSecondInput(MiaImageND * * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

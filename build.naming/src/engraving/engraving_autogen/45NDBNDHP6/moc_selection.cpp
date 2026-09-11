/****************************************************************************
** Meta object code from reading C++ file 'selection.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/selection.h"
#include "engraving/api/v1/part.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'selection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv19SelectionE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Selection::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv19SelectionE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Selection",
        "select",
        "",
        "apiv1::EngravingItem*",
        "e",
        "add",
        "selectRange",
        "startTick",
        "endTick",
        "startStaff",
        "endStaff",
        "deselect",
        "clear",
        "elements",
        "QQmlListProperty<apiv1::EngravingItem>",
        "isRange",
        "startSegment",
        "apiv1::Segment*",
        "endSegment"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'select'
        QtMocHelpers::MethodData<bool(apiv1::EngravingItem *, bool)>(1, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Bool, 5 },
        }}),
        // Method 'select'
        QtMocHelpers::MethodData<bool(apiv1::EngravingItem *)>(1, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'selectRange'
        QtMocHelpers::MethodData<bool(int, int, int, int)>(6, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 7 }, { QMetaType::Int, 8 }, { QMetaType::Int, 9 }, { QMetaType::Int, 10 },
        }}),
        // Method 'deselect'
        QtMocHelpers::MethodData<bool(apiv1::EngravingItem *)>(11, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'clear'
        QtMocHelpers::MethodData<bool()>(12, 2, QMC::AccessPublic, QMetaType::Bool),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'elements'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(13, 0x80000000 | 14, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'isRange'
        QtMocHelpers::PropertyData<bool>(15, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'startSegment'
        QtMocHelpers::PropertyData<apiv1::Segment*>(16, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'endSegment'
        QtMocHelpers::PropertyData<apiv1::Segment*>(18, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'startStaff'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'endStaff'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Selection, qt_meta_tag_ZN2mu9engraving5apiv19SelectionE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Selection::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19SelectionE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19SelectionE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv19SelectionE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Selection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Selection *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = _t->select((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 1: { bool _r = _t->select((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->selectRange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->deselect((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->clear();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->elements(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isRange(); break;
        case 2: *reinterpret_cast<apiv1::Segment**>(_v) = _t->startSegment(); break;
        case 3: *reinterpret_cast<apiv1::Segment**>(_v) = _t->endSegment(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->startStaff(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->endStaff(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Selection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Selection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19SelectionE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::Selection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP

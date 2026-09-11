/****************************************************************************
** Meta object code from reading C++ file 'apistructs.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/apistructs.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'apistructs.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv18FractionE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Fraction::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv18FractionE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Fraction",
        "plus",
        "apiv1::Fraction*",
        "",
        "other",
        "minus",
        "times",
        "v",
        "dividedBy",
        "greaterThan",
        "lessThan",
        "equals",
        "identical",
        "numerator",
        "denominator",
        "ticks",
        "str",
        "real",
        "reduced",
        "inverse",
        "absValue"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'plus'
        QtMocHelpers::MethodData<apiv1::Fraction *(apiv1::Fraction *)>(1, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { 0x80000000 | 2, 4 },
        }}),
        // Method 'minus'
        QtMocHelpers::MethodData<apiv1::Fraction *(apiv1::Fraction *)>(5, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { 0x80000000 | 2, 4 },
        }}),
        // Method 'times'
        QtMocHelpers::MethodData<apiv1::Fraction *(apiv1::Fraction *)>(6, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { 0x80000000 | 2, 4 },
        }}),
        // Method 'times'
        QtMocHelpers::MethodData<apiv1::Fraction *(int)>(6, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 7 },
        }}),
        // Method 'dividedBy'
        QtMocHelpers::MethodData<apiv1::Fraction *(apiv1::Fraction *)>(8, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { 0x80000000 | 2, 4 },
        }}),
        // Method 'dividedBy'
        QtMocHelpers::MethodData<apiv1::Fraction *(int)>(8, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 7 },
        }}),
        // Method 'greaterThan'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(9, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 2, 4 },
        }}),
        // Method 'lessThan'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(10, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 2, 4 },
        }}),
        // Method 'equals'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(11, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 2, 4 },
        }}),
        // Method 'identical'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(12, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 2, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'numerator'
        QtMocHelpers::PropertyData<int>(13, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'denominator'
        QtMocHelpers::PropertyData<int>(14, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'ticks'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'str'
        QtMocHelpers::PropertyData<QString>(16, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'real'
        QtMocHelpers::PropertyData<qreal>(17, QMetaType::QReal, QMC::DefaultPropertyFlags),
        // property 'reduced'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(18, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'inverse'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(19, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'absValue'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(20, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Fraction, qt_meta_tag_ZN2mu9engraving5apiv18FractionE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Fraction::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv18FractionE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv18FractionE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv18FractionE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Fraction::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Fraction *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::Fraction* _r = _t->plus((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 1: { apiv1::Fraction* _r = _t->minus((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 2: { apiv1::Fraction* _r = _t->times((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 3: { apiv1::Fraction* _r = _t->times((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 4: { apiv1::Fraction* _r = _t->dividedBy((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 5: { apiv1::Fraction* _r = _t->dividedBy((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->greaterThan((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->lessThan((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->equals((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->identical((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->numerator(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->denominator(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->ticks(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->toString(); break;
        case 4: *reinterpret_cast<qreal*>(_v) = _t->toDouble(); break;
        case 5: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->reduced(); break;
        case 6: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->inverse(); break;
        case 7: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->absValue(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Fraction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Fraction::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv18FractionE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::Fraction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv123OrnamentIntervalWrapperE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::OrnamentIntervalWrapper::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv123OrnamentIntervalWrapperE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::OrnamentIntervalWrapper",
        "setOrnamentInterval",
        "",
        "engraving::OrnamentInterval",
        "m_o",
        "step",
        "IntervalStep",
        "type",
        "IntervalType",
        "isPerfect"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'setOrnamentInterval'
        QtMocHelpers::SlotData<void(engraving::OrnamentInterval)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'step'
        QtMocHelpers::PropertyData<IntervalStep>(5, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'type'
        QtMocHelpers::PropertyData<IntervalType>(7, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'isPerfect'
        QtMocHelpers::PropertyData<bool>(9, QMetaType::Bool, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<OrnamentIntervalWrapper, qt_meta_tag_ZN2mu9engraving5apiv123OrnamentIntervalWrapperE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::OrnamentIntervalWrapper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv123OrnamentIntervalWrapperE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv123OrnamentIntervalWrapperE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv123OrnamentIntervalWrapperE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::OrnamentIntervalWrapper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<OrnamentIntervalWrapper *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->setOrnamentInterval((*reinterpret_cast<std::add_pointer_t<engraving::OrnamentInterval>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<IntervalStep*>(_v) = _t->step(); break;
        case 1: *reinterpret_cast<IntervalType*>(_v) = _t->type(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isPerfect(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::OrnamentIntervalWrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::OrnamentIntervalWrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv123OrnamentIntervalWrapperE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::OrnamentIntervalWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv115IntervalWrapperE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::IntervalWrapper::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv115IntervalWrapperE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::IntervalWrapper",
        "setInterval",
        "",
        "engraving::Interval",
        "m_i",
        "flip",
        "diatonic",
        "chromatic",
        "isZero"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'setInterval'
        QtMocHelpers::SlotData<void(engraving::Interval)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'flip'
        QtMocHelpers::MethodData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'diatonic'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'chromatic'
        QtMocHelpers::PropertyData<int>(7, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'isZero'
        QtMocHelpers::PropertyData<bool>(8, QMetaType::Bool, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<IntervalWrapper, qt_meta_tag_ZN2mu9engraving5apiv115IntervalWrapperE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::IntervalWrapper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv115IntervalWrapperE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv115IntervalWrapperE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv115IntervalWrapperE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::IntervalWrapper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<IntervalWrapper *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->setInterval((*reinterpret_cast<std::add_pointer_t<engraving::Interval>>(_a[1]))); break;
        case 1: _t->flip(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->diatonic(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->chromatic(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isZero(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::IntervalWrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::IntervalWrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv115IntervalWrapperE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::IntervalWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP

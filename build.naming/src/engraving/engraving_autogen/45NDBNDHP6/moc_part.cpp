/****************************************************************************
** Meta object code from reading C++ file 'part.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/part.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'part.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv14PartE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Part::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv14PartE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Part",
        "instrumentAtTick",
        "apiv1::Instrument*",
        "",
        "tick",
        "apiv1::Fraction*",
        "longNameAtTick",
        "shortNameAtTick",
        "instrumentNameAtTick",
        "instrumentIdAtTick",
        "currentHarpDiagramAtTick",
        "apiv1::EngravingItem*",
        "nextHarpDiagramFromTick",
        "prevHarpDiagramFromTick",
        "tickOfCurrentHarpDiagram",
        "startTrack",
        "endTrack",
        "instrumentId",
        "musicXmlId",
        "harmonyCount",
        "hasChordSymbol",
        "hasDrumStaff",
        "hasPitchedStaff",
        "hasTabStaff",
        "lyricCount",
        "midiChannel",
        "midiProgram",
        "longName",
        "shortName",
        "partName",
        "show",
        "instruments",
        "QQmlListProperty<apiv1::Instrument>",
        "staves",
        "QQmlListProperty<apiv1::Staff>",
        "masterPart",
        "apiv1::Part*"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'instrumentAtTick'
        QtMocHelpers::MethodData<apiv1::Instrument *(int)>(1, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'instrumentAtTick'
        QtMocHelpers::MethodData<apiv1::Instrument *(apiv1::Fraction *)>(1, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'longNameAtTick'
        QtMocHelpers::MethodData<QString(apiv1::Fraction *)>(6, 3, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'shortNameAtTick'
        QtMocHelpers::MethodData<QString(apiv1::Fraction *)>(7, 3, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'instrumentNameAtTick'
        QtMocHelpers::MethodData<QString(apiv1::Fraction *)>(8, 3, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'instrumentIdAtTick'
        QtMocHelpers::MethodData<QString(apiv1::Fraction *)>(9, 3, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'currentHarpDiagramAtTick'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(apiv1::Fraction *)>(10, 3, QMC::AccessPublic, 0x80000000 | 11, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'nextHarpDiagramFromTick'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(apiv1::Fraction *)>(12, 3, QMC::AccessPublic, 0x80000000 | 11, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'prevHarpDiagramFromTick'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(apiv1::Fraction *)>(13, 3, QMC::AccessPublic, 0x80000000 | 11, {{
            { 0x80000000 | 5, 4 },
        }}),
        // Method 'tickOfCurrentHarpDiagram'
        QtMocHelpers::MethodData<apiv1::Fraction *(apiv1::Fraction *)>(14, 3, QMC::AccessPublic, 0x80000000 | 5, {{
            { 0x80000000 | 5, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'startTrack'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'endTrack'
        QtMocHelpers::PropertyData<int>(16, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'instrumentId'
        QtMocHelpers::PropertyData<QString>(17, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'musicXmlId'
        QtMocHelpers::PropertyData<QString>(18, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'harmonyCount'
        QtMocHelpers::PropertyData<int>(19, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'hasChordSymbol'
        QtMocHelpers::PropertyData<bool>(20, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'hasDrumStaff'
        QtMocHelpers::PropertyData<bool>(21, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'hasPitchedStaff'
        QtMocHelpers::PropertyData<bool>(22, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'hasTabStaff'
        QtMocHelpers::PropertyData<bool>(23, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'lyricCount'
        QtMocHelpers::PropertyData<int>(24, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'midiChannel'
        QtMocHelpers::PropertyData<int>(25, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'midiProgram'
        QtMocHelpers::PropertyData<int>(26, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'longName'
        QtMocHelpers::PropertyData<QString>(27, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'shortName'
        QtMocHelpers::PropertyData<QString>(28, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'partName'
        QtMocHelpers::PropertyData<QString>(29, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'show'
        QtMocHelpers::PropertyData<bool>(30, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'instruments'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Instrument>>(31, 0x80000000 | 32, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'staves'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Staff>>(33, 0x80000000 | 34, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'masterPart'
        QtMocHelpers::PropertyData<apiv1::Part*>(35, 0x80000000 | 36, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Part, qt_meta_tag_ZN2mu9engraving5apiv14PartE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Part::staticMetaObject = { {
    QMetaObject::SuperData::link<ScoreElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14PartE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14PartE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv14PartE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Part::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Part *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::Instrument* _r = _t->instrumentAtTick((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Instrument**>(_a[0]) = std::move(_r); }  break;
        case 1: { apiv1::Instrument* _r = _t->instrumentAtTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Instrument**>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->longNameAtTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->shortNameAtTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->instrumentNameAtTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->instrumentIdAtTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { apiv1::EngravingItem* _r = _t->currentHarpDiagramAtTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 7: { apiv1::EngravingItem* _r = _t->nextHarpDiagramFromTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 8: { apiv1::EngravingItem* _r = _t->prevHarpDiagramFromTick((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 9: { apiv1::Fraction* _r = _t->tickOfCurrentHarpDiagram((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->startTrack(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->endTrack(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->instrumentId(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->musicXmlId(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->harmonyCount(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->hasChordSymbol(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->hasDrumStaff(); break;
        case 7: *reinterpret_cast<bool*>(_v) = _t->hasPitchedStaff(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->hasTabStaff(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->lyricCount(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->midiChannel(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->midiProgram(); break;
        case 12: *reinterpret_cast<QString*>(_v) = _t->longName(); break;
        case 13: *reinterpret_cast<QString*>(_v) = _t->shortName(); break;
        case 14: *reinterpret_cast<QString*>(_v) = _t->partName(); break;
        case 15: *reinterpret_cast<bool*>(_v) = _t->show(); break;
        case 16: *reinterpret_cast<QQmlListProperty<apiv1::Instrument>*>(_v) = _t->instruments(); break;
        case 17: *reinterpret_cast<QQmlListProperty<apiv1::Staff>*>(_v) = _t->staves(); break;
        case 18: *reinterpret_cast<apiv1::Part**>(_v) = _t->masterPart(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 15: _t->setShow(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Part::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Part::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14PartE_t>.strings))
        return static_cast<void*>(this);
    return ScoreElement::qt_metacast(_clname);
}

int mu::engraving::apiv1::Part::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ScoreElement::qt_metacall(_c, _id, _a);
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
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP

/****************************************************************************
** Meta object code from reading C++ file 'cursor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/cursor.h"
#include "engraving/api/v1/elements.h"
#include "engraving/api/v1/score.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cursor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv16CursorE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Cursor::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv16CursorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Cursor",
        "rewind",
        "",
        "RewindMode",
        "mode",
        "rewindToTick",
        "tick",
        "rewindToFraction",
        "apiv1::Fraction*",
        "f",
        "time",
        "includeRepeats",
        "next",
        "nextMeasure",
        "prev",
        "add",
        "apiv1::EngravingItem*",
        "addNote",
        "pitch",
        "addToChord",
        "addRest",
        "addTuplet",
        "ratio",
        "duration",
        "setDuration",
        "z",
        "n",
        "track",
        "staffIdx",
        "staff",
        "apiv1::Staff*",
        "voice",
        "filter",
        "utick",
        "fraction",
        "tempo",
        "keySignature",
        "score",
        "apiv1::Score*",
        "element",
        "segment",
        "apiv1::Segment*",
        "measure",
        "apiv1::Measure*",
        "stringNumber",
        "inputStateMode",
        "InputStateMode",
        "SCORE_START",
        "SELECTION_START",
        "SELECTION_END",
        "INPUT_STATE_INDEPENDENT",
        "INPUT_STATE_SYNC_WITH_SCORE"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'rewind'
        QtMocHelpers::MethodData<void(enum RewindMode)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'rewindToTick'
        QtMocHelpers::MethodData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Method 'rewindToFraction'
        QtMocHelpers::MethodData<void(apiv1::Fraction *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Method 'time'
        QtMocHelpers::MethodData<double(bool)>(10, 2, QMC::AccessPublic, QMetaType::Double, {{
            { QMetaType::Bool, 11 },
        }}),
        // Method 'time'
        QtMocHelpers::MethodData<double()>(10, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Double),
        // Method 'next'
        QtMocHelpers::MethodData<bool()>(12, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'nextMeasure'
        QtMocHelpers::MethodData<bool()>(13, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'prev'
        QtMocHelpers::MethodData<bool()>(14, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'add'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 2 },
        }}),
        // Method 'addNote'
        QtMocHelpers::MethodData<void(int, bool)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Bool, 19 },
        }}),
        // Method 'addNote'
        QtMocHelpers::MethodData<void(int)>(17, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'addRest'
        QtMocHelpers::MethodData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addTuplet'
        QtMocHelpers::MethodData<void(apiv1::Fraction *, apiv1::Fraction *)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 22 }, { 0x80000000 | 8, 23 },
        }}),
        // Method 'setDuration'
        QtMocHelpers::MethodData<void(int, int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 25 }, { QMetaType::Int, 26 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'track'
        QtMocHelpers::PropertyData<int>(27, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'staffIdx'
        QtMocHelpers::PropertyData<int>(28, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'staff'
        QtMocHelpers::PropertyData<apiv1::Staff*>(29, 0x80000000 | 30, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'voice'
        QtMocHelpers::PropertyData<int>(31, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'filter'
        QtMocHelpers::PropertyData<int>(32, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'tick'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'utick'
        QtMocHelpers::PropertyData<int>(33, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'fraction'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(34, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'tempo'
        QtMocHelpers::PropertyData<qreal>(35, QMetaType::QReal, QMC::DefaultPropertyFlags),
        // property 'keySignature'
        QtMocHelpers::PropertyData<int>(36, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'score'
        QtMocHelpers::PropertyData<apiv1::Score*>(37, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'element'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(39, 0x80000000 | 16, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'segment'
        QtMocHelpers::PropertyData<apiv1::Segment*>(40, 0x80000000 | 41, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'measure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(42, 0x80000000 | 43, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'stringNumber'
        QtMocHelpers::PropertyData<int>(44, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'inputStateMode'
        QtMocHelpers::PropertyData<enum InputStateMode>(45, 0x80000000 | 46, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'RewindMode'
        QtMocHelpers::EnumData<enum RewindMode>(3, 3, QMC::EnumFlags{}).add({
            {   47, RewindMode::SCORE_START },
            {   48, RewindMode::SELECTION_START },
            {   49, RewindMode::SELECTION_END },
        }),
        // enum 'InputStateMode'
        QtMocHelpers::EnumData<enum InputStateMode>(46, 46, QMC::EnumFlags{}).add({
            {   50, InputStateMode::INPUT_STATE_INDEPENDENT },
            {   51, InputStateMode::INPUT_STATE_SYNC_WITH_SCORE },
        }),
    };
    return QtMocHelpers::metaObjectData<Cursor, qt_meta_tag_ZN2mu9engraving5apiv16CursorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Cursor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16CursorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16CursorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv16CursorE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Cursor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Cursor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->rewind((*reinterpret_cast<std::add_pointer_t<enum RewindMode>>(_a[1]))); break;
        case 1: _t->rewindToTick((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->rewindToFraction((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1]))); break;
        case 3: { double _r = _t->time((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])));
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 4: { double _r = _t->time();
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->next();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->nextMeasure();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->prev();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->add((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        case 9: _t->addNote((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 10: _t->addNote((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->addRest(); break;
        case 12: _t->addTuplet((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[2]))); break;
        case 13: _t->setDuration((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->track(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->staffIdx(); break;
        case 2: *reinterpret_cast<apiv1::Staff**>(_v) = _t->staff(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->voice(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->filter(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->tick(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->utick(); break;
        case 7: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->qmlFraction(); break;
        case 8: *reinterpret_cast<qreal*>(_v) = _t->tempo(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->qmlKeySignature(); break;
        case 10: *reinterpret_cast<apiv1::Score**>(_v) = _t->score(); break;
        case 11: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->element(); break;
        case 12: *reinterpret_cast<apiv1::Segment**>(_v) = _t->qmlSegment(); break;
        case 13: *reinterpret_cast<apiv1::Measure**>(_v) = _t->measure(); break;
        case 14: *reinterpret_cast<int*>(_v) = _t->inputStateString(); break;
        case 15: *reinterpret_cast<enum InputStateMode*>(_v) = _t->inputStateMode(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTrack(*reinterpret_cast<int*>(_v)); break;
        case 1: _t->setStaffIdx(*reinterpret_cast<int*>(_v)); break;
        case 2: _t->setStaff(*reinterpret_cast<apiv1::Staff**>(_v)); break;
        case 3: _t->setVoice(*reinterpret_cast<int*>(_v)); break;
        case 4: _t->setFilter(*reinterpret_cast<int*>(_v)); break;
        case 10: _t->setScore(*reinterpret_cast<apiv1::Score**>(_v)); break;
        case 14: _t->setInputStateString(*reinterpret_cast<int*>(_v)); break;
        case 15: _t->setInputStateMode(*reinterpret_cast<enum InputStateMode*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Cursor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Cursor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16CursorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::Cursor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP

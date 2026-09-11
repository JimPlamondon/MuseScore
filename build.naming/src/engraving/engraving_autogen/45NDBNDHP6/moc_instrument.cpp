/****************************************************************************
** Meta object code from reading C++ file 'instrument.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/instrument.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'instrument.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv17ChannelE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Channel::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv17ChannelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Channel",
        "name",
        "isHarmonyChannel",
        "volume",
        "pan",
        "chorus",
        "reverb",
        "mute",
        "midiProgram",
        "midiBank"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'name'
        QtMocHelpers::PropertyData<QString>(1, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'isHarmonyChannel'
        QtMocHelpers::PropertyData<bool>(2, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'volume'
        QtMocHelpers::PropertyData<int>(3, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'pan'
        QtMocHelpers::PropertyData<int>(4, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'chorus'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'reverb'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'mute'
        QtMocHelpers::PropertyData<bool>(7, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'midiProgram'
        QtMocHelpers::PropertyData<int>(8, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'midiBank'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Channel, qt_meta_tag_ZN2mu9engraving5apiv17ChannelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Channel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17ChannelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17ChannelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv17ChannelE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Channel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Channel *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isHarmonyChannel(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->volume(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->pan(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->chorus(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->reverb(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->mute(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->midiProgram(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->midiBank(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setVolume(*reinterpret_cast<int*>(_v)); break;
        case 3: _t->setPan(*reinterpret_cast<int*>(_v)); break;
        case 4: _t->setChorus(*reinterpret_cast<int*>(_v)); break;
        case 5: _t->setReverb(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->setMute(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->setMidiProgram(*reinterpret_cast<int*>(_v)); break;
        case 8: _t->setMidiBank(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Channel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Channel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17ChannelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::Channel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv110StringDataE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::StringData::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv110StringDataE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::StringData",
        "strings",
        "QVariantList",
        "frets"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'strings'
        QtMocHelpers::PropertyData<QVariantList>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'frets'
        QtMocHelpers::PropertyData<int>(3, QMetaType::Int, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<StringData, qt_meta_tag_ZN2mu9engraving5apiv110StringDataE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::StringData::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv110StringDataE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv110StringDataE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv110StringDataE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::StringData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<StringData *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->stringList(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->frets(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::StringData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::StringData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv110StringDataE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::StringData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv17DrumsetE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Drumset::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv17DrumsetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Drumset",
        "isValid",
        "",
        "pitch",
        "noteHead",
        "noteHeads",
        "type",
        "line",
        "voice",
        "stemDirection",
        "name",
        "translatedName",
        "shortcut",
        "variants",
        "QVariantList",
        "panelRow",
        "panelColumn",
        "defaultPitchForLine",
        "nextPitch",
        "prevPitch",
        "setName",
        "setNoteHead",
        "setLine",
        "setVoice",
        "setStemDirection",
        "setShortcut",
        "is",
        "apiv1::Drumset*",
        "other"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'isValid'
        QtMocHelpers::MethodData<bool(int)>(1, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'noteHead'
        QtMocHelpers::MethodData<int(int)>(4, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'noteHeads'
        QtMocHelpers::MethodData<int(int, int)>(5, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 6 },
        }}),
        // Method 'line'
        QtMocHelpers::MethodData<int(int)>(7, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'voice'
        QtMocHelpers::MethodData<int(int)>(8, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'stemDirection'
        QtMocHelpers::MethodData<int(int)>(9, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'name'
        QtMocHelpers::MethodData<QString(int)>(10, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'translatedName'
        QtMocHelpers::MethodData<QString(int)>(11, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'shortcut'
        QtMocHelpers::MethodData<QString(int)>(12, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'variants'
        QtMocHelpers::MethodData<QVariantList(int)>(13, 2, QMC::AccessPublic, 0x80000000 | 14, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'panelRow'
        QtMocHelpers::MethodData<int(int)>(15, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'panelColumn'
        QtMocHelpers::MethodData<int(int)>(16, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'defaultPitchForLine'
        QtMocHelpers::MethodData<int(int)>(17, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 7 },
        }}),
        // Method 'nextPitch'
        QtMocHelpers::MethodData<int(int)>(18, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'prevPitch'
        QtMocHelpers::MethodData<int(int)>(19, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'setName'
        QtMocHelpers::MethodData<void(int, const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 10 },
        }}),
        // Method 'setNoteHead'
        QtMocHelpers::MethodData<void(int, int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 },
        }}),
        // Method 'setLine'
        QtMocHelpers::MethodData<void(int, int)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 7 },
        }}),
        // Method 'setVoice'
        QtMocHelpers::MethodData<void(int, int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 8 },
        }}),
        // Method 'setStemDirection'
        QtMocHelpers::MethodData<void(int, int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 9 },
        }}),
        // Method 'setShortcut'
        QtMocHelpers::MethodData<void(int, const QString &)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 12 },
        }}),
        // Method 'is'
        QtMocHelpers::MethodData<bool(apiv1::Drumset *)>(26, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 27, 28 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Drumset, qt_meta_tag_ZN2mu9engraving5apiv17DrumsetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Drumset::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17DrumsetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17DrumsetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv17DrumsetE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Drumset::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Drumset *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = _t->isValid((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 1: { int _r = _t->noteHead((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->noteHeads((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->line((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 4: { int _r = _t->voice((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->stemDirection((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->name((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 7: { QString _r = _t->translatedName((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 8: { QString _r = _t->shortcut((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 9: { QVariantList _r = _t->variants((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->panelRow((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->panelColumn((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->defaultPitchForLine((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->nextPitch((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 14: { int _r = _t->prevPitch((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->setName((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 16: _t->setNoteHead((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 17: _t->setLine((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 18: _t->setVoice((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 19: _t->setStemDirection((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 20: _t->setShortcut((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: { bool _r = _t->is((*reinterpret_cast<std::add_pointer_t<apiv1::Drumset*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Drumset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Drumset::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17DrumsetE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::Drumset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 22;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv110InstrumentE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Instrument::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv110InstrumentE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Instrument",
        "cloneDrumset",
        "apiv1::Drumset*",
        "",
        "is",
        "apiv1::Instrument*",
        "other",
        "instrumentId",
        "musicXmlId",
        "longName",
        "shortName",
        "stringData",
        "apiv1::StringData*",
        "drumset",
        "channels",
        "QQmlListProperty<apiv1::Channel>"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'cloneDrumset'
        QtMocHelpers::MethodData<apiv1::Drumset *()>(1, 3, QMC::AccessPublic, 0x80000000 | 2),
        // Method 'is'
        QtMocHelpers::MethodData<bool(apiv1::Instrument *)>(4, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'instrumentId'
        QtMocHelpers::PropertyData<QString>(7, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'musicXmlId'
        QtMocHelpers::PropertyData<QString>(8, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'longName'
        QtMocHelpers::PropertyData<QString>(9, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'shortName'
        QtMocHelpers::PropertyData<QString>(10, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'stringData'
        QtMocHelpers::PropertyData<apiv1::StringData*>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'drumset'
        QtMocHelpers::PropertyData<apiv1::Drumset*>(13, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'channels'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Channel>>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Instrument, qt_meta_tag_ZN2mu9engraving5apiv110InstrumentE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Instrument::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv110InstrumentE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv110InstrumentE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv110InstrumentE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Instrument::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Instrument *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::Drumset* _r = _t->cloneDrumset();
            if (_a[0]) *reinterpret_cast<apiv1::Drumset**>(_a[0]) = std::move(_r); }  break;
        case 1: { bool _r = _t->is((*reinterpret_cast<std::add_pointer_t<apiv1::Instrument*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->instrumentId(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->musicXmlId(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->longName(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->shortName(); break;
        case 4: *reinterpret_cast<apiv1::StringData**>(_v) = _t->stringData(); break;
        case 5: *reinterpret_cast<apiv1::Drumset**>(_v) = _t->drumset(); break;
        case 6: *reinterpret_cast<QQmlListProperty<apiv1::Channel>*>(_v) = _t->channels(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Instrument::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Instrument::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv110InstrumentE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::Instrument::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP

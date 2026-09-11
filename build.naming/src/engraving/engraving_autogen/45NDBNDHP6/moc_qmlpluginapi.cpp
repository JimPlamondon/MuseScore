/****************************************************************************
** Meta object code from reading C++ file 'qmlpluginapi.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/qmlpluginapi.h"
#include "engraving/api/v1/elements.h"
#include "engraving/api/v1/score.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qmlpluginapi.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv19PluginAPIE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::PluginAPI::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv19PluginAPIE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::PluginAPI",
        "run",
        "",
        "closeRequested",
        "scoreStateChanged",
        "QMap<QString,QVariant>",
        "state",
        "newScore",
        "apiv1::Score*",
        "name",
        "part",
        "measures",
        "newElement",
        "apiv1::EngravingItem*",
        "removeElement",
        "wrapped",
        "cmd",
        "newQProcess",
        "apiv1::MsProcess*",
        "writeScore",
        "ext",
        "readScore",
        "noninteractive",
        "closeScore",
        "score",
        "log",
        "logn",
        "log2",
        "openLog",
        "closeLog",
        "fraction",
        "apiv1::Fraction*",
        "numerator",
        "denominator",
        "fractionFromTicks",
        "ticks",
        "ornamentInterval",
        "apiv1::OrnamentIntervalWrapper*",
        "step",
        "type",
        "interval",
        "apiv1::IntervalWrapper*",
        "chromatic",
        "diatonic",
        "intervalFromOrnamentInterval",
        "o",
        "quit",
        "menuPath",
        "title",
        "version",
        "description",
        "pluginType",
        "dockArea",
        "requiresScore",
        "thumbnailName",
        "categoryCode",
        "division",
        "mscoreVersion",
        "mscoreMajorVersion",
        "mscoreMinorVersion",
        "mscoreUpdateVersion",
        "mscoreDPI",
        "curScore",
        "mu::engraving::apiv1::Score*",
        "scores",
        "QQmlListProperty<mu::engraving::apiv1::Score>",
        "Element",
        "mu::engraving::apiv1::Enum*",
        "Accidental",
        "AccidentalBracket",
        "OrnamentStyle",
        "Align",
        "Placement",
        "PlacementH",
        "TextPlace",
        "Direction",
        "DirectionH",
        "Orientation",
        "AutoOnOff",
        "AutoCustomHide",
        "VoiceAssignment",
        "SpacerType",
        "LayoutBreak",
        "DurationType",
        "NoteValueType",
        "Beam",
        "Glissando",
        "GlissandoStyle",
        "HarmonyType",
        "HarmonyVoicing",
        "HDuration",
        "FrameType",
        "VerticalAlignment",
        "TremoloBarType",
        "PreferSharpFlat",
        "NoteHeadType",
        "NoteHeadScheme",
        "NoteHeadGroup",
        "NoteType",
        "PlayEventType",
        "Segment",
        "BarLineType",
        "Tid",
        "Lyrics",
        "Spanner",
        "MMRestRangeBracketType",
        "TupletNumberType",
        "TupletBracketType",
        "TripletFeelType",
        "GuitarBendType",
        "GuitarBendShowHoldLine",
        "ClefType",
        "ClefToBarlinePosition",
        "DynamicType",
        "DynamicSpeed",
        "LineType",
        "HookType",
        "KeyMode",
        "ArpeggioType",
        "IntervalStep",
        "IntervalType",
        "InstrumentLabelVisibility",
        "OrnamentShowAccidental",
        "PartialSpannerDirection",
        "ChordStylePreset",
        "PlayingTechniqueType",
        "GradualTempoChangeType",
        "ChangeMethod",
        "ChangeDirection",
        "AccidentalRole",
        "AccidentalVal",
        "FermataType",
        "ChordLineType",
        "SlurStyleType",
        "TremoloType",
        "TremoloChordType",
        "BracketType",
        "JumpType",
        "MarkerType",
        "MeasureNumberMode",
        "StaffGroup",
        "OttavaType",
        "HairpinType",
        "TrillType",
        "VibratoType",
        "ArticulationTextType",
        "LyricsDashSystemStart",
        "NoteLineEndPlacement",
        "SpannerSegmentType",
        "TiePlacement",
        "TieDotsPlacement",
        "TimeSigType",
        "TimeSigPlacement",
        "TimeSigStyle",
        "TimeSigVSMargin",
        "NoteSpellingType",
        "Key",
        "UpdateMode",
        "LayoutFlag",
        "LayoutMode",
        "TappingHand",
        "LHTappingSymbol",
        "RHTappingSymbol",
        "LHTappingShowItems",
        "ParenthesesMode",
        "RepeatPlayCountPreset",
        "MeasureNumberPlacement",
        "SymId",
        "Cursor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'run'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'closeRequested'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scoreStateChanged'
        QtMocHelpers::SignalData<void(const QMap<QString,QVariant> &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Method 'newScore'
        QtMocHelpers::MethodData<apiv1::Score *(const QString &, const QString &, int)>(7, 2, QMC::AccessPublic, 0x80000000 | 8, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { QMetaType::Int, 11 },
        }}),
        // Method 'newElement'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(int)>(12, 2, QMC::AccessPublic, 0x80000000 | 13, {{
            { QMetaType::Int, 2 },
        }}),
        // Method 'removeElement'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 13, 15 },
        }}),
        // Method 'cmd'
        QtMocHelpers::MethodData<void(const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
        // Method 'newQProcess'
        QtMocHelpers::MethodData<apiv1::MsProcess *()>(17, 2, QMC::AccessPublic, 0x80000000 | 18),
        // Method 'writeScore'
        QtMocHelpers::MethodData<bool(apiv1::Score *, const QString &, const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 8, 2 }, { QMetaType::QString, 9 }, { QMetaType::QString, 20 },
        }}),
        // Method 'readScore'
        QtMocHelpers::MethodData<apiv1::Score *(const QString &, bool)>(21, 2, QMC::AccessPublic, 0x80000000 | 8, {{
            { QMetaType::QString, 9 }, { QMetaType::Bool, 22 },
        }}),
        // Method 'readScore'
        QtMocHelpers::MethodData<apiv1::Score *(const QString &)>(21, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 8, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'closeScore'
        QtMocHelpers::MethodData<void(apiv1::Score *)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 24 },
        }}),
        // Method 'closeScore'
        QtMocHelpers::MethodData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'log'
        QtMocHelpers::MethodData<void(const QString &)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
        // Method 'logn'
        QtMocHelpers::MethodData<void(const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
        // Method 'log2'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 }, { QMetaType::QString, 2 },
        }}),
        // Method 'openLog'
        QtMocHelpers::MethodData<void(const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
        // Method 'closeLog'
        QtMocHelpers::MethodData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'fraction'
        QtMocHelpers::MethodData<apiv1::Fraction *(int, int) const>(30, 2, QMC::AccessPublic, 0x80000000 | 31, {{
            { QMetaType::Int, 32 }, { QMetaType::Int, 33 },
        }}),
        // Method 'fractionFromTicks'
        QtMocHelpers::MethodData<apiv1::Fraction *(int) const>(34, 2, QMC::AccessPublic, 0x80000000 | 31, {{
            { QMetaType::Int, 35 },
        }}),
        // Method 'ornamentInterval'
        QtMocHelpers::MethodData<apiv1::OrnamentIntervalWrapper *(int, int) const>(36, 2, QMC::AccessPublic, 0x80000000 | 37, {{
            { QMetaType::Int, 38 }, { QMetaType::Int, 39 },
        }}),
        // Method 'interval'
        QtMocHelpers::MethodData<apiv1::IntervalWrapper *(int, int) const>(40, 2, QMC::AccessPublic, 0x80000000 | 41, {{
            { QMetaType::Int, 42 }, { QMetaType::Int, 43 },
        }}),
        // Method 'intervalFromOrnamentInterval'
        QtMocHelpers::MethodData<apiv1::IntervalWrapper *(apiv1::OrnamentIntervalWrapper *) const>(44, 2, QMC::AccessPublic, 0x80000000 | 41, {{
            { 0x80000000 | 37, 45 },
        }}),
        // Method 'quit'
        QtMocHelpers::MethodData<void()>(46, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'menuPath'
        QtMocHelpers::PropertyData<QString>(47, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'title'
        QtMocHelpers::PropertyData<QString>(48, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'version'
        QtMocHelpers::PropertyData<QString>(49, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'description'
        QtMocHelpers::PropertyData<QString>(50, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'pluginType'
        QtMocHelpers::PropertyData<QString>(51, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'dockArea'
        QtMocHelpers::PropertyData<QString>(52, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'requiresScore'
        QtMocHelpers::PropertyData<bool>(53, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'thumbnailName'
        QtMocHelpers::PropertyData<QString>(54, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'categoryCode'
        QtMocHelpers::PropertyData<QString>(55, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'division'
        QtMocHelpers::PropertyData<int>(56, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreVersion'
        QtMocHelpers::PropertyData<int>(57, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreMajorVersion'
        QtMocHelpers::PropertyData<int>(58, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreMinorVersion'
        QtMocHelpers::PropertyData<int>(59, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreUpdateVersion'
        QtMocHelpers::PropertyData<int>(60, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreDPI'
        QtMocHelpers::PropertyData<qreal>(61, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'curScore'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Score*>(62, 0x80000000 | 63, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'scores'
        QtMocHelpers::PropertyData<QQmlListProperty<mu::engraving::apiv1::Score>>(64, 0x80000000 | 65, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'Element'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(66, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Accidental'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(68, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AccidentalBracket'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(69, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'OrnamentStyle'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(70, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Align'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(71, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Placement'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(72, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PlacementH'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(73, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TextPlace'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(74, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Direction'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(75, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DirectionH'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(76, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Orientation'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(77, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AutoOnOff'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(78, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AutoCustomHide'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(79, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'VoiceAssignment'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(80, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SpacerType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(81, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LayoutBreak'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(82, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DurationType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(83, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteValueType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(84, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Beam'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(85, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Glissando'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(86, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GlissandoStyle'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(87, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HarmonyType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(88, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HarmonyVoicing'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(89, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HDuration'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(90, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'FrameType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(91, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'VerticalAlignment'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(92, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TremoloBarType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(93, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PreferSharpFlat'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(94, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteHeadType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(95, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteHeadScheme'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(96, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteHeadGroup'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(97, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(98, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PlayEventType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(99, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Segment'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(100, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'BarLineType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(101, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Tid'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(102, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Lyrics'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(103, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Spanner'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(104, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MMRestRangeBracketType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(105, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TupletNumberType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(106, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TupletBracketType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(107, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TripletFeelType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(108, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GuitarBendType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(109, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GuitarBendShowHoldLine'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(110, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ClefType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(111, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ClefToBarlinePosition'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(112, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DynamicType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(113, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DynamicSpeed'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(114, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LineType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(115, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HookType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(116, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'KeyMode'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(117, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ArpeggioType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(118, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'IntervalStep'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(119, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'IntervalType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(120, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'InstrumentLabelVisibility'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(121, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'OrnamentShowAccidental'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(122, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PartialSpannerDirection'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(123, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChordStylePreset'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(124, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PlayingTechniqueType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(125, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GradualTempoChangeType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(126, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChangeMethod'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(127, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChangeDirection'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(128, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AccidentalRole'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(129, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AccidentalVal'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(130, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'FermataType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(131, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChordLineType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(132, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SlurStyleType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(133, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TremoloType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(134, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TremoloChordType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(135, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'BracketType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(136, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'JumpType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(137, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MarkerType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(138, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MeasureNumberMode'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(139, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'StaffGroup'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(140, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'OttavaType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(141, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HairpinType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(142, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TrillType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(143, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'VibratoType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(144, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ArticulationTextType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(145, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LyricsDashSystemStart'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(146, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteLineEndPlacement'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(147, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SpannerSegmentType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(148, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TiePlacement'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(149, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TieDotsPlacement'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(150, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(151, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigPlacement'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(152, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigStyle'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(153, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigVSMargin'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(154, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteSpellingType'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(155, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Key'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(156, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'UpdateMode'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(157, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LayoutFlag'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(158, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LayoutMode'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(159, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TappingHand'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(160, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LHTappingSymbol'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(161, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'RHTappingSymbol'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(162, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LHTappingShowItems'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(163, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ParenthesesMode'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(164, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'RepeatPlayCountPreset'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(165, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MeasureNumberPlacement'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(166, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SymId'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(167, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Cursor'
        QtMocHelpers::PropertyData<mu::engraving::apiv1::Enum*>(168, 0x80000000 | 67, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PluginAPI, qt_meta_tag_ZN2mu9engraving5apiv19PluginAPIE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::PluginAPI::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19PluginAPIE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19PluginAPIE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv19PluginAPIE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::PluginAPI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PluginAPI *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->run(); break;
        case 1: _t->closeRequested(); break;
        case 2: _t->scoreStateChanged((*reinterpret_cast<std::add_pointer_t<QMap<QString,QVariant>>>(_a[1]))); break;
        case 3: { apiv1::Score* _r = _t->newScore((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast<apiv1::Score**>(_a[0]) = std::move(_r); }  break;
        case 4: { apiv1::EngravingItem* _r = _t->newElement((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 5: _t->removeElement((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        case 6: _t->cmd((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: { apiv1::MsProcess* _r = _t->newQProcess();
            if (_a[0]) *reinterpret_cast<apiv1::MsProcess**>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->writeScore((*reinterpret_cast<std::add_pointer_t<apiv1::Score*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { apiv1::Score* _r = _t->readScore((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::Score**>(_a[0]) = std::move(_r); }  break;
        case 10: { apiv1::Score* _r = _t->readScore((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Score**>(_a[0]) = std::move(_r); }  break;
        case 11: _t->closeScore((*reinterpret_cast<std::add_pointer_t<apiv1::Score*>>(_a[1]))); break;
        case 12: _t->closeScore(); break;
        case 13: _t->log((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->logn((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->log2((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 16: _t->openLog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->closeLog(); break;
        case 18: { apiv1::Fraction* _r = _t->fraction((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 19: { apiv1::Fraction* _r = _t->fractionFromTicks((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 20: { apiv1::OrnamentIntervalWrapper* _r = _t->ornamentInterval((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::OrnamentIntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 21: { apiv1::IntervalWrapper* _r = _t->interval((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::IntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 22: { apiv1::IntervalWrapper* _r = _t->intervalFromOrnamentInterval((*reinterpret_cast<std::add_pointer_t<apiv1::OrnamentIntervalWrapper*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::IntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 23: _t->quit(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PluginAPI::*)()>(_a, &PluginAPI::run, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PluginAPI::*)()>(_a, &PluginAPI::closeRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PluginAPI::*)(const QMap<QString,QVariant> & )>(_a, &PluginAPI::scoreStateChanged, 2))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 118:
        case 117:
        case 116:
        case 115:
        case 114:
        case 113:
        case 112:
        case 111:
        case 110:
        case 109:
        case 108:
        case 107:
        case 106:
        case 105:
        case 104:
        case 103:
        case 102:
        case 101:
        case 100:
        case 99:
        case 98:
        case 97:
        case 96:
        case 95:
        case 94:
        case 93:
        case 92:
        case 91:
        case 90:
        case 89:
        case 88:
        case 87:
        case 86:
        case 85:
        case 84:
        case 83:
        case 82:
        case 81:
        case 80:
        case 79:
        case 78:
        case 77:
        case 76:
        case 75:
        case 74:
        case 73:
        case 72:
        case 71:
        case 70:
        case 69:
        case 68:
        case 67:
        case 66:
        case 65:
        case 64:
        case 63:
        case 62:
        case 61:
        case 60:
        case 59:
        case 58:
        case 57:
        case 56:
        case 55:
        case 54:
        case 53:
        case 52:
        case 51:
        case 50:
        case 49:
        case 48:
        case 47:
        case 46:
        case 45:
        case 44:
        case 43:
        case 42:
        case 41:
        case 40:
        case 39:
        case 38:
        case 37:
        case 36:
        case 35:
        case 34:
        case 33:
        case 32:
        case 31:
        case 30:
        case 29:
        case 28:
        case 27:
        case 26:
        case 25:
        case 24:
        case 23:
        case 22:
        case 21:
        case 20:
        case 19:
        case 18:
        case 17:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< mu::engraving::apiv1::Enum* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->menuPath(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->title(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->version(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->description(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->pluginType(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->dockArea(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->requiresScore(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->thumbnailName(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->categoryCode(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->division(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->mscoreVersion(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->mscoreMajorVersion(); break;
        case 12: *reinterpret_cast<int*>(_v) = _t->mscoreMinorVersion(); break;
        case 13: *reinterpret_cast<int*>(_v) = _t->mscoreUpdateVersion(); break;
        case 14: *reinterpret_cast<qreal*>(_v) = _t->mscoreDPI(); break;
        case 15: *reinterpret_cast<mu::engraving::apiv1::Score**>(_v) = _t->curScore(); break;
        case 16: *reinterpret_cast<QQmlListProperty<mu::engraving::apiv1::Score>*>(_v) = _t->scores(); break;
        case 17: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_elementTypeEnum(); break;
        case 18: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_accidentalTypeEnum(); break;
        case 19: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_accidentalBracketEnum(); break;
        case 20: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_ornamentStyleEnum(); break;
        case 21: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_alignEnum(); break;
        case 22: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_placementEnum(); break;
        case 23: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_placementHEnum(); break;
        case 24: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_textPlaceEnum(); break;
        case 25: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_directionEnum(); break;
        case 26: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_directionHEnum(); break;
        case 27: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_orientationEnum(); break;
        case 28: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_autoOnOffEnum(); break;
        case 29: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_autoCustomHideEnum(); break;
        case 30: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_voiceAssignmentEnum(); break;
        case 31: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_spacerTypeEnum(); break;
        case 32: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_layoutBreakTypeEnum(); break;
        case 33: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_durationTypeEnum(); break;
        case 34: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_noteValueTypeEnum(); break;
        case 35: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_beamModeEnum(); break;
        case 36: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_glissandoTypeEnum(); break;
        case 37: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_glissandoStyleEnum(); break;
        case 38: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_harmonyTypeEnum(); break;
        case 39: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_harmonyVoicingEnum(); break;
        case 40: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_hDurationEnum(); break;
        case 41: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_frameTypeEnum(); break;
        case 42: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_verticalAlignmentEnum(); break;
        case 43: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tremoloBarTypeEnum(); break;
        case 44: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_preferSharpFlatEnum(); break;
        case 45: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_noteHeadTypeEnum(); break;
        case 46: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_noteHeadSchemeEnum(); break;
        case 47: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_noteHeadGroupEnum(); break;
        case 48: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_noteTypeEnum(); break;
        case 49: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_playEventTypeEnum(); break;
        case 50: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_segmentTypeEnum(); break;
        case 51: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_barLineTypeEnum(); break;
        case 52: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tidEnum(); break;
        case 53: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_lyricsSyllabicEnum(); break;
        case 54: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_spannerAnchorEnum(); break;
        case 55: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_mMRestRangeBracketTypeEnum(); break;
        case 56: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tupletNumberTypeEnum(); break;
        case 57: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tupletBracketTypeEnum(); break;
        case 58: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tripletFeelTypeEnum(); break;
        case 59: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_guitarBendTypeEnum(); break;
        case 60: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_guitarBendShowHoldLineEnum(); break;
        case 61: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_clefTypeEnum(); break;
        case 62: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_clefToBarlinePositionEnum(); break;
        case 63: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_dynamicTypeEnum(); break;
        case 64: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_dynamicSpeedEnum(); break;
        case 65: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_lineTypeEnum(); break;
        case 66: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_hookTypeEnum(); break;
        case 67: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_keyModeEnum(); break;
        case 68: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_arpeggioTypeEnum(); break;
        case 69: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_intervalStepEnum(); break;
        case 70: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_intervalTypeEnum(); break;
        case 71: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_instrumentLabelVisibilityEnum(); break;
        case 72: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_ornamentShowAccidentalEnum(); break;
        case 73: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_partialSpannerDirectionEnum(); break;
        case 74: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_chordStylePresetEnum(); break;
        case 75: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_playingTechniqueTypeEnum(); break;
        case 76: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_gradualTempoChangeTypeEnum(); break;
        case 77: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_changeMethodEnum(); break;
        case 78: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_changeDirectionEnum(); break;
        case 79: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_accidentalRoleEnum(); break;
        case 80: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_accidentalValEnum(); break;
        case 81: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_fermataTypeEnum(); break;
        case 82: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_chordLineTypeEnum(); break;
        case 83: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_slurStyleTypeEnum(); break;
        case 84: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tremoloTypeEnum(); break;
        case 85: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tremoloChordTypeEnum(); break;
        case 86: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_bracketTypeEnum(); break;
        case 87: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_jumpTypeEnum(); break;
        case 88: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_markerTypeEnum(); break;
        case 89: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_measureNumberModeEnum(); break;
        case 90: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_staffGroupEnum(); break;
        case 91: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_ottavaTypeEnum(); break;
        case 92: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_hairpinTypeEnum(); break;
        case 93: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_trillTypeEnum(); break;
        case 94: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_vibratoTypeEnum(); break;
        case 95: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_articulationTextTypeEnum(); break;
        case 96: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_lyricsDashSystemStartEnum(); break;
        case 97: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_noteLineEndPlacementEnum(); break;
        case 98: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_spannerSegmentTypeEnum(); break;
        case 99: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tiePlacementEnum(); break;
        case 100: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tieDotsPlacementEnum(); break;
        case 101: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_timeSigTypeEnum(); break;
        case 102: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_timeSigPlacementEnum(); break;
        case 103: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_timeSigStyleEnum(); break;
        case 104: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_timeSigVSMarginEnum(); break;
        case 105: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_noteSpellingTypeEnum(); break;
        case 106: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_keyEnum(); break;
        case 107: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_updateModeEnum(); break;
        case 108: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_layoutFlagEnum(); break;
        case 109: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_layoutModeEnum(); break;
        case 110: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_tappingHandEnum(); break;
        case 111: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_lHTappingSymbolEnum(); break;
        case 112: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_rHTappingSymbolEnum(); break;
        case 113: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_lHTappingShowItemsEnum(); break;
        case 114: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_parenthesesModeEnum(); break;
        case 115: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_repeatPlayCountPresetEnum(); break;
        case 116: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_measureNumberPlacementEnum(); break;
        case 117: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_symIdEnum(); break;
        case 118: *reinterpret_cast<mu::engraving::apiv1::Enum**>(_v) = _t->get_cursorEnum(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMenuPath(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setTitle(*reinterpret_cast<QString*>(_v)); break;
        case 2: _t->setVersion(*reinterpret_cast<QString*>(_v)); break;
        case 3: _t->setDescription(*reinterpret_cast<QString*>(_v)); break;
        case 4: _t->setPluginType(*reinterpret_cast<QString*>(_v)); break;
        case 5: _t->setDockArea(*reinterpret_cast<QString*>(_v)); break;
        case 6: _t->setRequiresScore(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->setThumbnailName(*reinterpret_cast<QString*>(_v)); break;
        case 8: _t->setCategoryCode(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::PluginAPI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::PluginAPI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19PluginAPIE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "muse::extensions::apiv1::IPluginApiV1"))
        return static_cast< muse::extensions::apiv1::IPluginApiV1*>(this);
    if (!strcmp(_clname, "muse::Contextable"))
        return static_cast< muse::Contextable*>(this);
    return QQuickItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::PluginAPI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 119;
    }
    return _id;
}

// SIGNAL 0
void mu::engraving::apiv1::PluginAPI::run()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void mu::engraving::apiv1::PluginAPI::closeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void mu::engraving::apiv1::PluginAPI::scoreStateChanged(const QMap<QString,QVariant> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP

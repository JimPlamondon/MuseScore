/****************************************************************************
** Meta object code from reading C++ file 'engravingapiv1.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/engravingapiv1.h"
#include "engraving/api/v1/elements.h"
#include "engraving/api/v1/score.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'engravingapiv1.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv114EngravingApiV1E_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::EngravingApiV1::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv114EngravingApiV1E_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::EngravingApiV1",
        "newScore",
        "apiv1::Score*",
        "",
        "name",
        "part",
        "measures",
        "newElement",
        "apiv1::EngravingItem*",
        "type",
        "removeElement",
        "wrapped",
        "cmd",
        "code",
        "writeScore",
        "s",
        "ext",
        "readScore",
        "noninteractive",
        "closeScore",
        "log",
        "m",
        "logn",
        "log2",
        "t",
        "openLog",
        "f",
        "closeLog",
        "fraction",
        "apiv1::Fraction*",
        "numerator",
        "denominator",
        "fractionFromTicks",
        "ticks",
        "defaultOrnamentInterval",
        "apiv1::OrnamentIntervalWrapper*",
        "ornamentInterval",
        "step",
        "interval",
        "apiv1::IntervalWrapper*",
        "diatonic",
        "chromatic",
        "intervalFromOrnamentInterval",
        "o",
        "quit",
        "division",
        "mscoreVersion",
        "mscoreMajorVersion",
        "mscoreMinorVersion",
        "mscoreUpdateVersion",
        "mscoreDPI",
        "curScore",
        "Element",
        "QJSValue",
        "TextStyleType",
        "Accidental",
        "apiv1::Enum*",
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
        // Method 'newScore'
        QtMocHelpers::MethodData<apiv1::Score *(const QString &, const QString &, int)>(1, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { QMetaType::Int, 6 },
        }}),
        // Method 'newElement'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(int)>(7, 3, QMC::AccessPublic, 0x80000000 | 8, {{
            { QMetaType::Int, 9 },
        }}),
        // Method 'removeElement'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(10, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 11 },
        }}),
        // Method 'cmd'
        QtMocHelpers::MethodData<void(const QString &)>(12, 3, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'writeScore'
        QtMocHelpers::MethodData<bool(apiv1::Score *, const QString &, const QString &)>(14, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 2, 15 }, { QMetaType::QString, 4 }, { QMetaType::QString, 16 },
        }}),
        // Method 'readScore'
        QtMocHelpers::MethodData<apiv1::Score *(const QString &, bool)>(17, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::QString, 4 }, { QMetaType::Bool, 18 },
        }}),
        // Method 'readScore'
        QtMocHelpers::MethodData<apiv1::Score *(const QString &)>(17, 3, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 2, {{
            { QMetaType::QString, 4 },
        }}),
        // Method 'closeScore'
        QtMocHelpers::MethodData<void(apiv1::Score *)>(19, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 2, 15 },
        }}),
        // Method 'log'
        QtMocHelpers::MethodData<void(const QString &)>(20, 3, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Method 'logn'
        QtMocHelpers::MethodData<void(const QString &)>(22, 3, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Method 'log2'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(23, 3, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 21 },
        }}),
        // Method 'openLog'
        QtMocHelpers::MethodData<void(const QString &)>(25, 3, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 26 },
        }}),
        // Method 'closeLog'
        QtMocHelpers::MethodData<void()>(27, 3, QMC::AccessPublic, QMetaType::Void),
        // Method 'fraction'
        QtMocHelpers::MethodData<apiv1::Fraction *(int, int) const>(28, 3, QMC::AccessPublic, 0x80000000 | 29, {{
            { QMetaType::Int, 30 }, { QMetaType::Int, 31 },
        }}),
        // Method 'fractionFromTicks'
        QtMocHelpers::MethodData<apiv1::Fraction *(int) const>(32, 3, QMC::AccessPublic, 0x80000000 | 29, {{
            { QMetaType::Int, 33 },
        }}),
        // Method 'defaultOrnamentInterval'
        QtMocHelpers::MethodData<apiv1::OrnamentIntervalWrapper *() const>(34, 3, QMC::AccessPublic, 0x80000000 | 35),
        // Method 'ornamentInterval'
        QtMocHelpers::MethodData<apiv1::OrnamentIntervalWrapper *(int, int) const>(36, 3, QMC::AccessPublic, 0x80000000 | 35, {{
            { QMetaType::Int, 37 }, { QMetaType::Int, 9 },
        }}),
        // Method 'interval'
        QtMocHelpers::MethodData<apiv1::IntervalWrapper *(int, int) const>(38, 3, QMC::AccessPublic, 0x80000000 | 39, {{
            { QMetaType::Int, 40 }, { QMetaType::Int, 41 },
        }}),
        // Method 'intervalFromOrnamentInterval'
        QtMocHelpers::MethodData<apiv1::IntervalWrapper *(apiv1::OrnamentIntervalWrapper *) const>(42, 3, QMC::AccessPublic, 0x80000000 | 39, {{
            { 0x80000000 | 35, 43 },
        }}),
        // Method 'quit'
        QtMocHelpers::MethodData<void()>(44, 3, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'division'
        QtMocHelpers::PropertyData<int>(45, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreVersion'
        QtMocHelpers::PropertyData<int>(46, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreMajorVersion'
        QtMocHelpers::PropertyData<int>(47, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreMinorVersion'
        QtMocHelpers::PropertyData<int>(48, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreUpdateVersion'
        QtMocHelpers::PropertyData<int>(49, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'mscoreDPI'
        QtMocHelpers::PropertyData<qreal>(50, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'curScore'
        QtMocHelpers::PropertyData<apiv1::Score*>(51, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Element'
        QtMocHelpers::PropertyData<QJSValue>(52, 0x80000000 | 53, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TextStyleType'
        QtMocHelpers::PropertyData<QJSValue>(54, 0x80000000 | 53, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Accidental'
        QtMocHelpers::PropertyData<apiv1::Enum*>(55, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AccidentalBracket'
        QtMocHelpers::PropertyData<apiv1::Enum*>(57, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'OrnamentStyle'
        QtMocHelpers::PropertyData<apiv1::Enum*>(58, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Align'
        QtMocHelpers::PropertyData<apiv1::Enum*>(59, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Placement'
        QtMocHelpers::PropertyData<apiv1::Enum*>(60, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PlacementH'
        QtMocHelpers::PropertyData<apiv1::Enum*>(61, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TextPlace'
        QtMocHelpers::PropertyData<apiv1::Enum*>(62, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Direction'
        QtMocHelpers::PropertyData<apiv1::Enum*>(63, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DirectionH'
        QtMocHelpers::PropertyData<apiv1::Enum*>(64, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Orientation'
        QtMocHelpers::PropertyData<apiv1::Enum*>(65, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AutoOnOff'
        QtMocHelpers::PropertyData<apiv1::Enum*>(66, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AutoCustomHide'
        QtMocHelpers::PropertyData<apiv1::Enum*>(67, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'VoiceAssignment'
        QtMocHelpers::PropertyData<apiv1::Enum*>(68, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SpacerType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(69, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LayoutBreak'
        QtMocHelpers::PropertyData<apiv1::Enum*>(70, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DurationType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(71, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteValueType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(72, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Beam'
        QtMocHelpers::PropertyData<apiv1::Enum*>(73, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Glissando'
        QtMocHelpers::PropertyData<apiv1::Enum*>(74, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GlissandoStyle'
        QtMocHelpers::PropertyData<apiv1::Enum*>(75, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HarmonyType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(76, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HarmonyVoicing'
        QtMocHelpers::PropertyData<apiv1::Enum*>(77, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HDuration'
        QtMocHelpers::PropertyData<apiv1::Enum*>(78, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'FrameType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(79, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'VerticalAlignment'
        QtMocHelpers::PropertyData<apiv1::Enum*>(80, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TremoloBarType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(81, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PreferSharpFlat'
        QtMocHelpers::PropertyData<apiv1::Enum*>(82, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteHeadType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(83, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteHeadScheme'
        QtMocHelpers::PropertyData<apiv1::Enum*>(84, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteHeadGroup'
        QtMocHelpers::PropertyData<apiv1::Enum*>(85, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(86, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PlayEventType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(87, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Segment'
        QtMocHelpers::PropertyData<apiv1::Enum*>(88, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'BarLineType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(89, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Tid'
        QtMocHelpers::PropertyData<apiv1::Enum*>(90, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Lyrics'
        QtMocHelpers::PropertyData<apiv1::Enum*>(91, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Spanner'
        QtMocHelpers::PropertyData<apiv1::Enum*>(92, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MMRestRangeBracketType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(93, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TupletNumberType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(94, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TupletBracketType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(95, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TripletFeelType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(96, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GuitarBendType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(97, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GuitarBendShowHoldLine'
        QtMocHelpers::PropertyData<apiv1::Enum*>(98, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ClefType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(99, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ClefToBarlinePosition'
        QtMocHelpers::PropertyData<apiv1::Enum*>(100, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DynamicType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(101, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'DynamicSpeed'
        QtMocHelpers::PropertyData<apiv1::Enum*>(102, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LineType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(103, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HookType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(104, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'KeyMode'
        QtMocHelpers::PropertyData<apiv1::Enum*>(105, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ArpeggioType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(106, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'IntervalStep'
        QtMocHelpers::PropertyData<apiv1::Enum*>(107, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'IntervalType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(108, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'InstrumentLabelVisibility'
        QtMocHelpers::PropertyData<apiv1::Enum*>(109, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'OrnamentShowAccidental'
        QtMocHelpers::PropertyData<apiv1::Enum*>(110, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PartialSpannerDirection'
        QtMocHelpers::PropertyData<apiv1::Enum*>(111, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChordStylePreset'
        QtMocHelpers::PropertyData<apiv1::Enum*>(112, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'PlayingTechniqueType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(113, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'GradualTempoChangeType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(114, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChangeMethod'
        QtMocHelpers::PropertyData<apiv1::Enum*>(115, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChangeDirection'
        QtMocHelpers::PropertyData<apiv1::Enum*>(116, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AccidentalRole'
        QtMocHelpers::PropertyData<apiv1::Enum*>(117, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'AccidentalVal'
        QtMocHelpers::PropertyData<apiv1::Enum*>(118, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'FermataType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(119, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ChordLineType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(120, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SlurStyleType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(121, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TremoloType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(122, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TremoloChordType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(123, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'BracketType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(124, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'JumpType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(125, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MarkerType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(126, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MeasureNumberMode'
        QtMocHelpers::PropertyData<apiv1::Enum*>(127, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'StaffGroup'
        QtMocHelpers::PropertyData<apiv1::Enum*>(128, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'OttavaType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(129, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'HairpinType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(130, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TrillType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(131, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'VibratoType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(132, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ArticulationTextType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(133, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LyricsDashSystemStart'
        QtMocHelpers::PropertyData<apiv1::Enum*>(134, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteLineEndPlacement'
        QtMocHelpers::PropertyData<apiv1::Enum*>(135, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SpannerSegmentType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(136, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TiePlacement'
        QtMocHelpers::PropertyData<apiv1::Enum*>(137, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TieDotsPlacement'
        QtMocHelpers::PropertyData<apiv1::Enum*>(138, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(139, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigPlacement'
        QtMocHelpers::PropertyData<apiv1::Enum*>(140, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigStyle'
        QtMocHelpers::PropertyData<apiv1::Enum*>(141, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TimeSigVSMargin'
        QtMocHelpers::PropertyData<apiv1::Enum*>(142, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'NoteSpellingType'
        QtMocHelpers::PropertyData<apiv1::Enum*>(143, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Key'
        QtMocHelpers::PropertyData<apiv1::Enum*>(144, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'UpdateMode'
        QtMocHelpers::PropertyData<apiv1::Enum*>(145, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LayoutFlag'
        QtMocHelpers::PropertyData<apiv1::Enum*>(146, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LayoutMode'
        QtMocHelpers::PropertyData<apiv1::Enum*>(147, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'TappingHand'
        QtMocHelpers::PropertyData<apiv1::Enum*>(148, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LHTappingSymbol'
        QtMocHelpers::PropertyData<apiv1::Enum*>(149, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'RHTappingSymbol'
        QtMocHelpers::PropertyData<apiv1::Enum*>(150, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'LHTappingShowItems'
        QtMocHelpers::PropertyData<apiv1::Enum*>(151, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'ParenthesesMode'
        QtMocHelpers::PropertyData<apiv1::Enum*>(152, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'RepeatPlayCountPreset'
        QtMocHelpers::PropertyData<apiv1::Enum*>(153, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'MeasureNumberPlacement'
        QtMocHelpers::PropertyData<apiv1::Enum*>(154, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'SymId'
        QtMocHelpers::PropertyData<apiv1::Enum*>(155, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'Cursor'
        QtMocHelpers::PropertyData<apiv1::Enum*>(156, 0x80000000 | 56, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<EngravingApiV1, qt_meta_tag_ZN2mu9engraving5apiv114EngravingApiV1E_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::EngravingApiV1::staticMetaObject = { {
    QMetaObject::SuperData::link<muse::api::ApiObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv114EngravingApiV1E_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv114EngravingApiV1E_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv114EngravingApiV1E_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::EngravingApiV1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<EngravingApiV1 *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::Score* _r = _t->newScore((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast<apiv1::Score**>(_a[0]) = std::move(_r); }  break;
        case 1: { apiv1::EngravingItem* _r = _t->newElement((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 2: _t->removeElement((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        case 3: _t->cmd((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: { bool _r = _t->writeScore((*reinterpret_cast<std::add_pointer_t<apiv1::Score*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { apiv1::Score* _r = _t->readScore((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::Score**>(_a[0]) = std::move(_r); }  break;
        case 6: { apiv1::Score* _r = _t->readScore((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Score**>(_a[0]) = std::move(_r); }  break;
        case 7: _t->closeScore((*reinterpret_cast<std::add_pointer_t<apiv1::Score*>>(_a[1]))); break;
        case 8: _t->log((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->logn((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->log2((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->openLog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->closeLog(); break;
        case 13: { apiv1::Fraction* _r = _t->fraction((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 14: { apiv1::Fraction* _r = _t->fractionFromTicks((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 15: { apiv1::OrnamentIntervalWrapper* _r = _t->defaultOrnamentInterval();
            if (_a[0]) *reinterpret_cast<apiv1::OrnamentIntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 16: { apiv1::OrnamentIntervalWrapper* _r = _t->ornamentInterval((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::OrnamentIntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 17: { apiv1::IntervalWrapper* _r = _t->interval((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::IntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 18: { apiv1::IntervalWrapper* _r = _t->intervalFromOrnamentInterval((*reinterpret_cast<std::add_pointer_t<apiv1::OrnamentIntervalWrapper*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::IntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 19: _t->quit(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
        case 7:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QJSValue >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->division(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->mscoreVersion(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->mscoreMajorVersion(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->mscoreMinorVersion(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->mscoreUpdateVersion(); break;
        case 5: *reinterpret_cast<qreal*>(_v) = _t->mscoreDPI(); break;
        case 6: *reinterpret_cast<apiv1::Score**>(_v) = _t->curScore(); break;
        case 7: *reinterpret_cast<QJSValue*>(_v) = _t->getElement(); break;
        case 8: *reinterpret_cast<QJSValue*>(_v) = _t->getTextStyleType(); break;
        case 9: *reinterpret_cast<apiv1::Enum**>(_v) = _t->accidentalTypeEnum(); break;
        case 10: *reinterpret_cast<apiv1::Enum**>(_v) = _t->accidentalBracketEnum(); break;
        case 11: *reinterpret_cast<apiv1::Enum**>(_v) = _t->ornamentStyleEnum(); break;
        case 12: *reinterpret_cast<apiv1::Enum**>(_v) = _t->alignEnum(); break;
        case 13: *reinterpret_cast<apiv1::Enum**>(_v) = _t->placementEnum(); break;
        case 14: *reinterpret_cast<apiv1::Enum**>(_v) = _t->placementHEnum(); break;
        case 15: *reinterpret_cast<apiv1::Enum**>(_v) = _t->textPlaceEnum(); break;
        case 16: *reinterpret_cast<apiv1::Enum**>(_v) = _t->directionEnum(); break;
        case 17: *reinterpret_cast<apiv1::Enum**>(_v) = _t->directionHEnum(); break;
        case 18: *reinterpret_cast<apiv1::Enum**>(_v) = _t->orientationEnum(); break;
        case 19: *reinterpret_cast<apiv1::Enum**>(_v) = _t->autoOnOffEnum(); break;
        case 20: *reinterpret_cast<apiv1::Enum**>(_v) = _t->autoCustomHideEnum(); break;
        case 21: *reinterpret_cast<apiv1::Enum**>(_v) = _t->voiceAssignmentEnum(); break;
        case 22: *reinterpret_cast<apiv1::Enum**>(_v) = _t->spacerTypeEnum(); break;
        case 23: *reinterpret_cast<apiv1::Enum**>(_v) = _t->layoutBreakTypeEnum(); break;
        case 24: *reinterpret_cast<apiv1::Enum**>(_v) = _t->durationTypeEnum(); break;
        case 25: *reinterpret_cast<apiv1::Enum**>(_v) = _t->noteValueTypeEnum(); break;
        case 26: *reinterpret_cast<apiv1::Enum**>(_v) = _t->beamModeEnum(); break;
        case 27: *reinterpret_cast<apiv1::Enum**>(_v) = _t->glissandoTypeEnum(); break;
        case 28: *reinterpret_cast<apiv1::Enum**>(_v) = _t->glissandoStyleEnum(); break;
        case 29: *reinterpret_cast<apiv1::Enum**>(_v) = _t->harmonyTypeEnum(); break;
        case 30: *reinterpret_cast<apiv1::Enum**>(_v) = _t->harmonyVoicingEnum(); break;
        case 31: *reinterpret_cast<apiv1::Enum**>(_v) = _t->hDurationEnum(); break;
        case 32: *reinterpret_cast<apiv1::Enum**>(_v) = _t->frameTypeEnum(); break;
        case 33: *reinterpret_cast<apiv1::Enum**>(_v) = _t->verticalAlignmentEnum(); break;
        case 34: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tremoloBarTypeEnum(); break;
        case 35: *reinterpret_cast<apiv1::Enum**>(_v) = _t->preferSharpFlatEnum(); break;
        case 36: *reinterpret_cast<apiv1::Enum**>(_v) = _t->noteHeadTypeEnum(); break;
        case 37: *reinterpret_cast<apiv1::Enum**>(_v) = _t->noteHeadSchemeEnum(); break;
        case 38: *reinterpret_cast<apiv1::Enum**>(_v) = _t->noteHeadGroupEnum(); break;
        case 39: *reinterpret_cast<apiv1::Enum**>(_v) = _t->noteTypeEnum(); break;
        case 40: *reinterpret_cast<apiv1::Enum**>(_v) = _t->playEventTypeEnum(); break;
        case 41: *reinterpret_cast<apiv1::Enum**>(_v) = _t->segmentTypeEnum(); break;
        case 42: *reinterpret_cast<apiv1::Enum**>(_v) = _t->barLineTypeEnum(); break;
        case 43: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tidEnum(); break;
        case 44: *reinterpret_cast<apiv1::Enum**>(_v) = _t->lyricsSyllabicEnum(); break;
        case 45: *reinterpret_cast<apiv1::Enum**>(_v) = _t->spannerAnchorEnum(); break;
        case 46: *reinterpret_cast<apiv1::Enum**>(_v) = _t->mMRestRangeBracketTypeEnum(); break;
        case 47: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tupletNumberTypeEnum(); break;
        case 48: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tupletBracketTypeEnum(); break;
        case 49: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tripletFeelTypeEnum(); break;
        case 50: *reinterpret_cast<apiv1::Enum**>(_v) = _t->guitarBendTypeEnum(); break;
        case 51: *reinterpret_cast<apiv1::Enum**>(_v) = _t->guitarBendShowHoldLineEnum(); break;
        case 52: *reinterpret_cast<apiv1::Enum**>(_v) = _t->clefTypeEnum(); break;
        case 53: *reinterpret_cast<apiv1::Enum**>(_v) = _t->clefToBarlinePositionEnum(); break;
        case 54: *reinterpret_cast<apiv1::Enum**>(_v) = _t->dynamicTypeEnum(); break;
        case 55: *reinterpret_cast<apiv1::Enum**>(_v) = _t->dynamicSpeedEnum(); break;
        case 56: *reinterpret_cast<apiv1::Enum**>(_v) = _t->lineTypeEnum(); break;
        case 57: *reinterpret_cast<apiv1::Enum**>(_v) = _t->hookTypeEnum(); break;
        case 58: *reinterpret_cast<apiv1::Enum**>(_v) = _t->keyModeEnum(); break;
        case 59: *reinterpret_cast<apiv1::Enum**>(_v) = _t->arpeggioTypeEnum(); break;
        case 60: *reinterpret_cast<apiv1::Enum**>(_v) = _t->intervalStepEnum(); break;
        case 61: *reinterpret_cast<apiv1::Enum**>(_v) = _t->intervalTypeEnum(); break;
        case 62: *reinterpret_cast<apiv1::Enum**>(_v) = _t->instrumentLabelVisibilityEnum(); break;
        case 63: *reinterpret_cast<apiv1::Enum**>(_v) = _t->ornamentShowAccidentalEnum(); break;
        case 64: *reinterpret_cast<apiv1::Enum**>(_v) = _t->partialSpannerDirectionEnum(); break;
        case 65: *reinterpret_cast<apiv1::Enum**>(_v) = _t->chordStylePresetEnum(); break;
        case 66: *reinterpret_cast<apiv1::Enum**>(_v) = _t->playingTechniqueTypeEnum(); break;
        case 67: *reinterpret_cast<apiv1::Enum**>(_v) = _t->gradualTempoChangeTypeEnum(); break;
        case 68: *reinterpret_cast<apiv1::Enum**>(_v) = _t->changeMethodEnum(); break;
        case 69: *reinterpret_cast<apiv1::Enum**>(_v) = _t->changeDirectionEnum(); break;
        case 70: *reinterpret_cast<apiv1::Enum**>(_v) = _t->accidentalRoleEnum(); break;
        case 71: *reinterpret_cast<apiv1::Enum**>(_v) = _t->accidentalValEnum(); break;
        case 72: *reinterpret_cast<apiv1::Enum**>(_v) = _t->fermataTypeEnum(); break;
        case 73: *reinterpret_cast<apiv1::Enum**>(_v) = _t->chordLineTypeEnum(); break;
        case 74: *reinterpret_cast<apiv1::Enum**>(_v) = _t->slurStyleTypeEnum(); break;
        case 75: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tremoloTypeEnum(); break;
        case 76: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tremoloChordTypeEnum(); break;
        case 77: *reinterpret_cast<apiv1::Enum**>(_v) = _t->bracketTypeEnum(); break;
        case 78: *reinterpret_cast<apiv1::Enum**>(_v) = _t->jumpTypeEnum(); break;
        case 79: *reinterpret_cast<apiv1::Enum**>(_v) = _t->markerTypeEnum(); break;
        case 80: *reinterpret_cast<apiv1::Enum**>(_v) = _t->measureNumberModeEnum(); break;
        case 81: *reinterpret_cast<apiv1::Enum**>(_v) = _t->staffGroupEnum(); break;
        case 82: *reinterpret_cast<apiv1::Enum**>(_v) = _t->ottavaTypeEnum(); break;
        case 83: *reinterpret_cast<apiv1::Enum**>(_v) = _t->hairpinTypeEnum(); break;
        case 84: *reinterpret_cast<apiv1::Enum**>(_v) = _t->trillTypeEnum(); break;
        case 85: *reinterpret_cast<apiv1::Enum**>(_v) = _t->vibratoTypeEnum(); break;
        case 86: *reinterpret_cast<apiv1::Enum**>(_v) = _t->articulationTextTypeEnum(); break;
        case 87: *reinterpret_cast<apiv1::Enum**>(_v) = _t->lyricsDashSystemStartEnum(); break;
        case 88: *reinterpret_cast<apiv1::Enum**>(_v) = _t->noteLineEndPlacementEnum(); break;
        case 89: *reinterpret_cast<apiv1::Enum**>(_v) = _t->spannerSegmentTypeEnum(); break;
        case 90: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tiePlacementEnum(); break;
        case 91: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tieDotsPlacementEnum(); break;
        case 92: *reinterpret_cast<apiv1::Enum**>(_v) = _t->timeSigTypeEnum(); break;
        case 93: *reinterpret_cast<apiv1::Enum**>(_v) = _t->timeSigPlacementEnum(); break;
        case 94: *reinterpret_cast<apiv1::Enum**>(_v) = _t->timeSigStyleEnum(); break;
        case 95: *reinterpret_cast<apiv1::Enum**>(_v) = _t->timeSigVSMarginEnum(); break;
        case 96: *reinterpret_cast<apiv1::Enum**>(_v) = _t->noteSpellingTypeEnum(); break;
        case 97: *reinterpret_cast<apiv1::Enum**>(_v) = _t->keyEnum(); break;
        case 98: *reinterpret_cast<apiv1::Enum**>(_v) = _t->updateModeEnum(); break;
        case 99: *reinterpret_cast<apiv1::Enum**>(_v) = _t->layoutFlagEnum(); break;
        case 100: *reinterpret_cast<apiv1::Enum**>(_v) = _t->layoutModeEnum(); break;
        case 101: *reinterpret_cast<apiv1::Enum**>(_v) = _t->tappingHandEnum(); break;
        case 102: *reinterpret_cast<apiv1::Enum**>(_v) = _t->lHTappingSymbolEnum(); break;
        case 103: *reinterpret_cast<apiv1::Enum**>(_v) = _t->rHTappingSymbolEnum(); break;
        case 104: *reinterpret_cast<apiv1::Enum**>(_v) = _t->lHTappingShowItemsEnum(); break;
        case 105: *reinterpret_cast<apiv1::Enum**>(_v) = _t->parenthesesModeEnum(); break;
        case 106: *reinterpret_cast<apiv1::Enum**>(_v) = _t->repeatPlayCountPresetEnum(); break;
        case 107: *reinterpret_cast<apiv1::Enum**>(_v) = _t->measureNumberPlacementEnum(); break;
        case 108: *reinterpret_cast<apiv1::Enum**>(_v) = _t->symIdEnum(); break;
        case 109: *reinterpret_cast<apiv1::Enum**>(_v) = _t->cursorEnum(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::EngravingApiV1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::EngravingApiV1::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv114EngravingApiV1E_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "muse::extensions::apiv1::IApiV1Object"))
        return static_cast< muse::extensions::apiv1::IApiV1Object*>(this);
    return muse::api::ApiObject::qt_metacast(_clname);
}

int mu::engraving::apiv1::EngravingApiV1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = muse::api::ApiObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 110;
    }
    return _id;
}
QT_WARNING_POP

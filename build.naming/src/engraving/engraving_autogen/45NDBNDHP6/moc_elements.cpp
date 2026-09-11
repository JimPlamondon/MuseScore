/****************************************************************************
** Meta object code from reading C++ file 'elements.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/elements.h"
#include "engraving/api/v1/part.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'elements.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv113EngravingItemE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::EngravingItem::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv113EngravingItemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::EngravingItem",
        "clone",
        "apiv1::EngravingItem*",
        "",
        "subtypeName",
        "_name",
        "parent",
        "staff",
        "apiv1::Staff*",
        "staffIdx",
        "effectiveStaffIdx",
        "vStaffIdx",
        "offsetX",
        "offsetY",
        "posX",
        "posY",
        "pos",
        "QPointF",
        "pagePos",
        "canvasPos",
        "bbox",
        "QRectF",
        "subtype",
        "up",
        "header",
        "trailer",
        "isMovable",
        "enabled",
        "addToSkyline",
        "subType",
        "selected",
        "generated",
        "color",
        "QColor",
        "visible",
        "z",
        "small",
        "hideWhenEmpty",
        "QJSValue",
        "hideStavesWhenIndividuallyEmpty",
        "showCourtesy",
        "keysig_mode",
        "lineType",
        "headType",
        "headGroup",
        "articulationAnchor",
        "direction",
        "horizontalDirection",
        "stemDirection",
        "slurDirection",
        "mirrorHead",
        "QVariant",
        "hasParentheses",
        "pause",
        "barlineType",
        "barlineSpan",
        "barlineSpanFrom",
        "barlineSpanTo",
        "barlineShowTips",
        "offset",
        "ghost",
        "play",
        "boxHeight",
        "boxWidth",
        "boxAutoSize",
        "topGap",
        "bottomGap",
        "leftMargin",
        "rightMargin",
        "topMargin",
        "bottomMargin",
        "paddingToNotationAbove",
        "paddingToNotationBelow",
        "layoutBreakType",
        "autoscale",
        "size",
        "imageHeight",
        "imageWidth",
        "imageFramed",
        "fretFrameTextScale",
        "fretFrameDiagramScale",
        "fretFrameColumnGap",
        "fretFrameRowGap",
        "fretFrameChordPerRow",
        "fretFrameHAlign",
        "fretFrameDiagramsOrder",
        "scale",
        "lockAspectRatio",
        "sizeIsSpatium",
        "text",
        "htmlText",
        "userModified",
        "beamPos",
        "beamNoSlope",
        "crossStaffMove",
        "userLen",
        "space",
        "tempo",
        "tempoFollowText",
        "tempoAlignRightOfRehearsalMark",
        "accidentalBracket",
        "accidentalType",
        "stackingOrderOffset",
        "numeratorString",
        "denominatorString",
        "fbprefix",
        "fbdigit",
        "fbsuffix",
        "fbcontinuationline",
        "fbparenthesis1",
        "fbparenthesis2",
        "fbparenthesis3",
        "fbparenthesis4",
        "fbparenthesis5",
        "ottavaType",
        "numbersOnly",
        "trillType",
        "vibratoType",
        "hairpinCircledTip",
        "hairpinType",
        "hairpinHeight",
        "hairpinContHeight",
        "veloChange",
        "veloChangeMethod",
        "veloChangeSpeed",
        "dynamicType",
        "singleNoteDynamics",
        "changeMethod",
        "placement",
        "hPlacement",
        "mmRestRangeBracketType",
        "velocity",
        "jumpTo",
        "playUntil",
        "continueAt",
        "label",
        "markerType",
        "musicSymbolSize",
        "markerCenterOnSymbol",
        "arpUserLen1",
        "arpUserLen2",
        "glissType",
        "glissText",
        "glissShowText",
        "glissandoStyle",
        "glissEaseIn",
        "glissEaseOut",
        "diagonal",
        "groups",
        "lineStyle",
        "lineColor",
        "lineWidth",
        "timeStretch",
        "ornamentStyle",
        "intervalAbove",
        "intervalBelow",
        "ornamentShowAccidental",
        "ornamentShowCueNote",
        "startOnUpperNote",
        "timesig",
        "timesigStretch",
        "timesigType",
        "mmRestNumberPos",
        "mmRestNumberOffset",
        "mmRestNumberVisible",
        "measureRepeatNumberPos",
        "verse",
        "volta_ending",
        "lineVisible",
        "mag",
        "useDrumset",
        "role",
        "track",
        "fretStrings",
        "fretFrets",
        "showNut",
        "fretOffset",
        "fretNumPos",
        "orientation",
        "fretShowFingering",
        "fretFingering",
        "harmonyVoiceLiteral",
        "harmonyVoicing",
        "harmonyDuration",
        "harmonyBassScale",
        "harmonyDoNotStackModifiers",
        "systemBracket",
        "gap",
        "autoplace",
        "dashLineLen",
        "dashGapLen",
        "symbol",
        "playRepeats",
        "createSystemHeader",
        "staffLines",
        "lineDistance",
        "stepOffset",
        "staffShowBarlines",
        "staffShowLedgerlines",
        "staffStemless",
        "staffInvisible",
        "staffColor",
        "headScheme",
        "staffGenClef",
        "staffGenTimesig",
        "staffGenKeysig",
        "staffYoffset",
        "bracketSpan",
        "bracketColumn",
        "inameLayoutPosition",
        "subStyle",
        "fontFace",
        "fontSize",
        "fontStyle",
        "lineSpacing",
        "frameType",
        "frameWidth",
        "framePadding",
        "frameRound",
        "frameFgColor",
        "frameBgColor",
        "sizeSpatiumDependent",
        "textSizeSpatiumDependent",
        "musicalSymbolsScale",
        "align",
        "textScriptAlign",
        "systemFlag",
        "beginText",
        "beginTextAlign",
        "beginTextPosition",
        "beginTextPlace",
        "beginHookType",
        "beginHookHeight",
        "beginLineArrowHeight",
        "beginLineArrowWidth",
        "beginFilledArrowHeight",
        "beginFilledArrowWidth",
        "beginFontFace",
        "beginFontSize",
        "beginFontStyle",
        "beginTextOffset",
        "gapBetweenTextAndLine",
        "continueText",
        "continueTextAlign",
        "continueTextPosition",
        "continueTextPlace",
        "continueFontFace",
        "continueFontSize",
        "continueFontStyle",
        "continueTextOffset",
        "endText",
        "endTextAlign",
        "endTextPosition",
        "endTextPlace",
        "endHookType",
        "endHookHeight",
        "endLineArrowHeight",
        "endLineArrowWidth",
        "endFilledArrowHeight",
        "endFilledArrowWidth",
        "endFontFace",
        "endFontSize",
        "endFontStyle",
        "endTextOffset",
        "notelinePlacement",
        "avoidBarLines",
        "dynamicsSize",
        "centerOnNotehead",
        "anchorToEndOfPrevious",
        "snapToDynamics",
        "snapBefore",
        "snapAfter",
        "voiceAssignment",
        "centerBetweenStaves",
        "posAbove",
        "locationStaves",
        "locationVoices",
        "locationMeasures",
        "locationFractions",
        "locationGrace",
        "locationNote",
        "voice",
        "position",
        "concertClefType",
        "transposingClefType",
        "clefToBarlinePos",
        "isHeader",
        "concertKey",
        "actualKey",
        "action",
        "minDistance",
        "arpeggioType",
        "chordLineType",
        "chordLineStraight",
        "chordLineWavy",
        "tremoloType",
        "tremoloStrokeStyle",
        "harmonyType",
        "arpeggioSpan",
        "bracketHookLength",
        "bracketHookPos",
        "bracketRightSide",
        "bendType",
        "bendCurve",
        "bendVertexOffset",
        "bendShowHoldLine",
        "bendStartTimeFactor",
        "bendEndTimeFactor",
        "guitarDiveTabPos",
        "guitarBendAmount",
        "vibratoLineType",
        "guitarDiveIsSlack",
        "tremoloBarType",
        "tremoloBarCurve",
        "startWithLongNames",
        "startWithMeasureOne",
        "firstSystemIndentation",
        "path",
        "preferSharpFlat",
        "playTechType",
        "tempoChangeType",
        "tempoEasingMethod",
        "tempoChangeFactor",
        "isDiagram",
        "active",
        "fretPosition",
        "ignoredStrings",
        "generateText",
        "transposeMode",
        "tiePlacement",
        "minLength",
        "partialSpannerDirection",
        "positionLinkedToMaster",
        "appearanceLinkedToMaster",
        "textLinkedToMaster",
        "excludeFromParts",
        "stringsCount",
        "preset",
        "visibleStrings",
        "scoreFont",
        "symbolsSize",
        "symbolAngle",
        "applyToAllStaves",
        "isCourtesy",
        "excludeVerticalAlign",
        "playCountTextSetting",
        "playCountText",
        "alignWithOtherRests",
        "fraction",
        "apiv1::Fraction*",
        "beat"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'clone'
        QtMocHelpers::MethodData<apiv1::EngravingItem *() const>(1, 3, QMC::AccessPublic, 0x80000000 | 2),
        // Method 'subtypeName'
        QtMocHelpers::MethodData<QString() const>(4, 3, QMC::AccessPublic, QMetaType::QString),
        // Method '_name'
        QtMocHelpers::MethodData<QString() const>(5, 3, QMC::AccessPublic, QMetaType::QString),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'parent'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(6, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'staff'
        QtMocHelpers::PropertyData<apiv1::Staff*>(7, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'staffIdx'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'effectiveStaffIdx'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'vStaffIdx'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'offsetX'
        QtMocHelpers::PropertyData<float>(12, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'offsetY'
        QtMocHelpers::PropertyData<float>(13, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'posX'
        QtMocHelpers::PropertyData<float>(14, QMetaType::Float, QMC::DefaultPropertyFlags),
        // property 'posY'
        QtMocHelpers::PropertyData<float>(15, QMetaType::Float, QMC::DefaultPropertyFlags),
        // property 'pos'
        QtMocHelpers::PropertyData<QPointF>(16, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'pagePos'
        QtMocHelpers::PropertyData<QPointF>(18, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'canvasPos'
        QtMocHelpers::PropertyData<QPointF>(19, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'bbox'
        QtMocHelpers::PropertyData<QRectF>(20, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'subtype'
        QtMocHelpers::PropertyData<int>(22, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'up'
        QtMocHelpers::PropertyData<bool>(23, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'header'
        QtMocHelpers::PropertyData<bool>(24, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'trailer'
        QtMocHelpers::PropertyData<bool>(25, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'isMovable'
        QtMocHelpers::PropertyData<bool>(26, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'enabled'
        QtMocHelpers::PropertyData<bool>(27, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'addToSkyline'
        QtMocHelpers::PropertyData<bool>(28, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'subType'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'selected'
        QtMocHelpers::PropertyData<bool>(30, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'generated'
        QtMocHelpers::PropertyData<bool>(31, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'color'
        QtMocHelpers::PropertyData<QColor>(32, 0x80000000 | 33, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'visible'
        QtMocHelpers::PropertyData<bool>(34, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'z'
        QtMocHelpers::PropertyData<int>(35, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'small'
        QtMocHelpers::PropertyData<bool>(36, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'hideWhenEmpty'
        QtMocHelpers::PropertyData<QJSValue>(37, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'hideStavesWhenIndividuallyEmpty'
        QtMocHelpers::PropertyData<bool>(39, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'showCourtesy'
        QtMocHelpers::PropertyData<bool>(40, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'keysig_mode'
        QtMocHelpers::PropertyData<QJSValue>(41, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'lineType'
        QtMocHelpers::PropertyData<QJSValue>(42, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'headType'
        QtMocHelpers::PropertyData<QJSValue>(43, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'headGroup'
        QtMocHelpers::PropertyData<QJSValue>(44, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'articulationAnchor'
        QtMocHelpers::PropertyData<int>(45, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'direction'
        QtMocHelpers::PropertyData<QJSValue>(46, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'horizontalDirection'
        QtMocHelpers::PropertyData<QJSValue>(47, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'stemDirection'
        QtMocHelpers::PropertyData<QJSValue>(48, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'slurDirection'
        QtMocHelpers::PropertyData<QJSValue>(49, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'mirrorHead'
        QtMocHelpers::PropertyData<QVariant>(50, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'hasParentheses'
        QtMocHelpers::PropertyData<QVariant>(52, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'pause'
        QtMocHelpers::PropertyData<qreal>(53, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'barlineType'
        QtMocHelpers::PropertyData<QVariant>(54, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'barlineSpan'
        QtMocHelpers::PropertyData<QVariant>(55, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'barlineSpanFrom'
        QtMocHelpers::PropertyData<int>(56, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'barlineSpanTo'
        QtMocHelpers::PropertyData<int>(57, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'barlineShowTips'
        QtMocHelpers::PropertyData<bool>(58, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'offset'
        QtMocHelpers::PropertyData<QPointF>(59, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'ghost'
        QtMocHelpers::PropertyData<bool>(60, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'play'
        QtMocHelpers::PropertyData<bool>(61, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'boxHeight'
        QtMocHelpers::PropertyData<QVariant>(62, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'boxWidth'
        QtMocHelpers::PropertyData<QVariant>(63, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'boxAutoSize'
        QtMocHelpers::PropertyData<bool>(64, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'topGap'
        QtMocHelpers::PropertyData<QVariant>(65, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'bottomGap'
        QtMocHelpers::PropertyData<QVariant>(66, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'leftMargin'
        QtMocHelpers::PropertyData<qreal>(67, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'rightMargin'
        QtMocHelpers::PropertyData<qreal>(68, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'topMargin'
        QtMocHelpers::PropertyData<qreal>(69, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bottomMargin'
        QtMocHelpers::PropertyData<qreal>(70, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'paddingToNotationAbove'
        QtMocHelpers::PropertyData<QVariant>(71, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'paddingToNotationBelow'
        QtMocHelpers::PropertyData<QVariant>(72, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'layoutBreakType'
        QtMocHelpers::PropertyData<QVariant>(73, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'autoscale'
        QtMocHelpers::PropertyData<bool>(74, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'size'
        QtMocHelpers::PropertyData<QVariant>(75, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'imageHeight'
        QtMocHelpers::PropertyData<qreal>(76, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'imageWidth'
        QtMocHelpers::PropertyData<qreal>(77, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'imageFramed'
        QtMocHelpers::PropertyData<bool>(78, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretFrameTextScale'
        QtMocHelpers::PropertyData<qreal>(79, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretFrameDiagramScale'
        QtMocHelpers::PropertyData<qreal>(80, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretFrameColumnGap'
        QtMocHelpers::PropertyData<QVariant>(81, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'fretFrameRowGap'
        QtMocHelpers::PropertyData<QVariant>(82, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'fretFrameChordPerRow'
        QtMocHelpers::PropertyData<int>(83, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretFrameHAlign'
        QtMocHelpers::PropertyData<int>(84, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretFrameDiagramsOrder'
        QtMocHelpers::PropertyData<QVariant>(85, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'scale'
        QtMocHelpers::PropertyData<QVariant>(86, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'lockAspectRatio'
        QtMocHelpers::PropertyData<bool>(87, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'sizeIsSpatium'
        QtMocHelpers::PropertyData<bool>(88, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'text'
        QtMocHelpers::PropertyData<QVariant>(89, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'htmlText'
        QtMocHelpers::PropertyData<QVariant>(90, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'userModified'
        QtMocHelpers::PropertyData<bool>(91, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'beamPos'
        QtMocHelpers::PropertyData<QVariant>(92, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beamNoSlope'
        QtMocHelpers::PropertyData<bool>(93, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'crossStaffMove'
        QtMocHelpers::PropertyData<int>(94, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'userLen'
        QtMocHelpers::PropertyData<QVariant>(95, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'space'
        QtMocHelpers::PropertyData<QVariant>(96, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'tempo'
        QtMocHelpers::PropertyData<QVariant>(97, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'tempoFollowText'
        QtMocHelpers::PropertyData<bool>(98, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tempoAlignRightOfRehearsalMark'
        QtMocHelpers::PropertyData<bool>(99, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'accidentalBracket'
        QtMocHelpers::PropertyData<int>(100, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'accidentalType'
        QtMocHelpers::PropertyData<int>(101, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'stackingOrderOffset'
        QtMocHelpers::PropertyData<int>(102, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'numeratorString'
        QtMocHelpers::PropertyData<QVariant>(103, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'denominatorString'
        QtMocHelpers::PropertyData<QVariant>(104, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'fbprefix'
        QtMocHelpers::PropertyData<int>(105, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbdigit'
        QtMocHelpers::PropertyData<int>(106, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbsuffix'
        QtMocHelpers::PropertyData<int>(107, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbcontinuationline'
        QtMocHelpers::PropertyData<int>(108, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbparenthesis1'
        QtMocHelpers::PropertyData<int>(109, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbparenthesis2'
        QtMocHelpers::PropertyData<int>(110, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbparenthesis3'
        QtMocHelpers::PropertyData<int>(111, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbparenthesis4'
        QtMocHelpers::PropertyData<int>(112, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fbparenthesis5'
        QtMocHelpers::PropertyData<int>(113, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'ottavaType'
        QtMocHelpers::PropertyData<int>(114, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'numbersOnly'
        QtMocHelpers::PropertyData<bool>(115, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'trillType'
        QtMocHelpers::PropertyData<int>(116, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'vibratoType'
        QtMocHelpers::PropertyData<int>(117, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'hairpinCircledTip'
        QtMocHelpers::PropertyData<bool>(118, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'hairpinType'
        QtMocHelpers::PropertyData<int>(119, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'hairpinHeight'
        QtMocHelpers::PropertyData<QVariant>(120, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'hairpinContHeight'
        QtMocHelpers::PropertyData<QVariant>(121, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'veloChange'
        QtMocHelpers::PropertyData<int>(122, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'veloChangeMethod'
        QtMocHelpers::PropertyData<QVariant>(123, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'veloChangeSpeed'
        QtMocHelpers::PropertyData<QVariant>(124, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'dynamicType'
        QtMocHelpers::PropertyData<QVariant>(125, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'singleNoteDynamics'
        QtMocHelpers::PropertyData<bool>(126, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'changeMethod'
        QtMocHelpers::PropertyData<QVariant>(127, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'placement'
        QtMocHelpers::PropertyData<QVariant>(128, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'hPlacement'
        QtMocHelpers::PropertyData<QVariant>(129, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'mmRestRangeBracketType'
        QtMocHelpers::PropertyData<int>(130, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'velocity'
        QtMocHelpers::PropertyData<int>(131, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'jumpTo'
        QtMocHelpers::PropertyData<QVariant>(132, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'playUntil'
        QtMocHelpers::PropertyData<QVariant>(133, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'continueAt'
        QtMocHelpers::PropertyData<QVariant>(134, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'label'
        QtMocHelpers::PropertyData<QVariant>(135, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'markerType'
        QtMocHelpers::PropertyData<int>(136, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'musicSymbolSize'
        QtMocHelpers::PropertyData<qreal>(137, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'markerCenterOnSymbol'
        QtMocHelpers::PropertyData<bool>(138, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'arpUserLen1'
        QtMocHelpers::PropertyData<qreal>(139, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'arpUserLen2'
        QtMocHelpers::PropertyData<qreal>(140, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'glissType'
        QtMocHelpers::PropertyData<int>(141, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'glissText'
        QtMocHelpers::PropertyData<QVariant>(142, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'glissShowText'
        QtMocHelpers::PropertyData<bool>(143, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'glissandoStyle'
        QtMocHelpers::PropertyData<QVariant>(144, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'glissEaseIn'
        QtMocHelpers::PropertyData<int>(145, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'glissEaseOut'
        QtMocHelpers::PropertyData<int>(146, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'diagonal'
        QtMocHelpers::PropertyData<bool>(147, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'groups'
        QtMocHelpers::PropertyData<QVariant>(148, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'lineStyle'
        QtMocHelpers::PropertyData<QVariant>(149, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'lineColor'
        QtMocHelpers::PropertyData<QVariant>(150, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'lineWidth'
        QtMocHelpers::PropertyData<QVariant>(151, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'timeStretch'
        QtMocHelpers::PropertyData<qreal>(152, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'ornamentStyle'
        QtMocHelpers::PropertyData<QVariant>(153, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'intervalAbove'
        QtMocHelpers::PropertyData<QVariant>(154, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'intervalBelow'
        QtMocHelpers::PropertyData<QVariant>(155, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'ornamentShowAccidental'
        QtMocHelpers::PropertyData<int>(156, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'ornamentShowCueNote'
        QtMocHelpers::PropertyData<QVariant>(157, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'startOnUpperNote'
        QtMocHelpers::PropertyData<bool>(158, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'timesig'
        QtMocHelpers::PropertyData<QVariant>(159, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'timesigStretch'
        QtMocHelpers::PropertyData<QVariant>(160, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'timesigType'
        QtMocHelpers::PropertyData<int>(161, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'mmRestNumberPos'
        QtMocHelpers::PropertyData<QVariant>(162, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'mmRestNumberOffset'
        QtMocHelpers::PropertyData<QVariant>(163, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'mmRestNumberVisible'
        QtMocHelpers::PropertyData<bool>(164, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'measureRepeatNumberPos'
        QtMocHelpers::PropertyData<QVariant>(165, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'verse'
        QtMocHelpers::PropertyData<int>(166, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'volta_ending'
        QtMocHelpers::PropertyData<QVariant>(167, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'lineVisible'
        QtMocHelpers::PropertyData<bool>(168, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'mag'
        QtMocHelpers::PropertyData<qreal>(169, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'useDrumset'
        QtMocHelpers::PropertyData<int>(170, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'role'
        QtMocHelpers::PropertyData<QVariant>(171, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'track'
        QtMocHelpers::PropertyData<int>(172, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretStrings'
        QtMocHelpers::PropertyData<int>(173, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretFrets'
        QtMocHelpers::PropertyData<int>(174, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'showNut'
        QtMocHelpers::PropertyData<bool>(175, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretOffset'
        QtMocHelpers::PropertyData<int>(176, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretNumPos'
        QtMocHelpers::PropertyData<int>(177, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'orientation'
        QtMocHelpers::PropertyData<QVariant>(178, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'fretShowFingering'
        QtMocHelpers::PropertyData<bool>(179, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretFingering'
        QtMocHelpers::PropertyData<QVariant>(180, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'harmonyVoiceLiteral'
        QtMocHelpers::PropertyData<bool>(181, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'harmonyVoicing'
        QtMocHelpers::PropertyData<QVariant>(182, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'harmonyDuration'
        QtMocHelpers::PropertyData<QVariant>(183, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'harmonyBassScale'
        QtMocHelpers::PropertyData<QVariant>(184, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'harmonyDoNotStackModifiers'
        QtMocHelpers::PropertyData<bool>(185, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'systemBracket'
        QtMocHelpers::PropertyData<int>(186, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'gap'
        QtMocHelpers::PropertyData<bool>(187, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'autoplace'
        QtMocHelpers::PropertyData<bool>(188, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'dashLineLen'
        QtMocHelpers::PropertyData<qreal>(189, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'dashGapLen'
        QtMocHelpers::PropertyData<qreal>(190, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'symbol'
        QtMocHelpers::PropertyData<QVariant>(191, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'playRepeats'
        QtMocHelpers::PropertyData<bool>(192, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'createSystemHeader'
        QtMocHelpers::PropertyData<bool>(193, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffLines'
        QtMocHelpers::PropertyData<int>(194, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'lineDistance'
        QtMocHelpers::PropertyData<QVariant>(195, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'stepOffset'
        QtMocHelpers::PropertyData<int>(196, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffShowBarlines'
        QtMocHelpers::PropertyData<bool>(197, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffShowLedgerlines'
        QtMocHelpers::PropertyData<bool>(198, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffStemless'
        QtMocHelpers::PropertyData<bool>(199, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffInvisible'
        QtMocHelpers::PropertyData<bool>(200, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffColor'
        QtMocHelpers::PropertyData<QColor>(201, 0x80000000 | 33, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'headScheme'
        QtMocHelpers::PropertyData<QVariant>(202, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'staffGenClef'
        QtMocHelpers::PropertyData<bool>(203, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffGenTimesig'
        QtMocHelpers::PropertyData<bool>(204, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffGenKeysig'
        QtMocHelpers::PropertyData<bool>(205, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffYoffset'
        QtMocHelpers::PropertyData<QVariant>(206, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'bracketSpan'
        QtMocHelpers::PropertyData<int>(207, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bracketColumn'
        QtMocHelpers::PropertyData<int>(208, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'inameLayoutPosition'
        QtMocHelpers::PropertyData<int>(209, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'subStyle'
        QtMocHelpers::PropertyData<QVariant>(210, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'fontFace'
        QtMocHelpers::PropertyData<QVariant>(211, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'fontSize'
        QtMocHelpers::PropertyData<qreal>(212, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fontStyle'
        QtMocHelpers::PropertyData<int>(213, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'lineSpacing'
        QtMocHelpers::PropertyData<qreal>(214, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'frameType'
        QtMocHelpers::PropertyData<int>(215, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'frameWidth'
        QtMocHelpers::PropertyData<QVariant>(216, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'framePadding'
        QtMocHelpers::PropertyData<QVariant>(217, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'frameRound'
        QtMocHelpers::PropertyData<int>(218, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'frameFgColor'
        QtMocHelpers::PropertyData<QColor>(219, 0x80000000 | 33, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'frameBgColor'
        QtMocHelpers::PropertyData<QColor>(220, 0x80000000 | 33, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'sizeSpatiumDependent'
        QtMocHelpers::PropertyData<bool>(221, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'textSizeSpatiumDependent'
        QtMocHelpers::PropertyData<bool>(222, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'musicalSymbolsScale'
        QtMocHelpers::PropertyData<qreal>(223, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'align'
        QtMocHelpers::PropertyData<QVariant>(224, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'textScriptAlign'
        QtMocHelpers::PropertyData<int>(225, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'systemFlag'
        QtMocHelpers::PropertyData<bool>(226, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'beginText'
        QtMocHelpers::PropertyData<QVariant>(227, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginTextAlign'
        QtMocHelpers::PropertyData<QVariant>(228, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginTextPosition'
        QtMocHelpers::PropertyData<QVariant>(229, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginTextPlace'
        QtMocHelpers::PropertyData<QVariant>(230, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginHookType'
        QtMocHelpers::PropertyData<QVariant>(231, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginHookHeight'
        QtMocHelpers::PropertyData<QVariant>(232, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginLineArrowHeight'
        QtMocHelpers::PropertyData<QVariant>(233, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginLineArrowWidth'
        QtMocHelpers::PropertyData<QVariant>(234, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginFilledArrowHeight'
        QtMocHelpers::PropertyData<QVariant>(235, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginFilledArrowWidth'
        QtMocHelpers::PropertyData<QVariant>(236, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginFontFace'
        QtMocHelpers::PropertyData<QVariant>(237, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beginFontSize'
        QtMocHelpers::PropertyData<qreal>(238, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'beginFontStyle'
        QtMocHelpers::PropertyData<int>(239, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'beginTextOffset'
        QtMocHelpers::PropertyData<QPointF>(240, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'gapBetweenTextAndLine'
        QtMocHelpers::PropertyData<QVariant>(241, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'continueText'
        QtMocHelpers::PropertyData<QVariant>(242, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'continueTextAlign'
        QtMocHelpers::PropertyData<QVariant>(243, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'continueTextPosition'
        QtMocHelpers::PropertyData<QVariant>(244, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'continueTextPlace'
        QtMocHelpers::PropertyData<QVariant>(245, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'continueFontFace'
        QtMocHelpers::PropertyData<QVariant>(246, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'continueFontSize'
        QtMocHelpers::PropertyData<qreal>(247, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'continueFontStyle'
        QtMocHelpers::PropertyData<int>(248, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'continueTextOffset'
        QtMocHelpers::PropertyData<QPointF>(249, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endText'
        QtMocHelpers::PropertyData<QVariant>(250, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endTextAlign'
        QtMocHelpers::PropertyData<QVariant>(251, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endTextPosition'
        QtMocHelpers::PropertyData<QVariant>(252, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endTextPlace'
        QtMocHelpers::PropertyData<QVariant>(253, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endHookType'
        QtMocHelpers::PropertyData<QVariant>(254, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endHookHeight'
        QtMocHelpers::PropertyData<QVariant>(255, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endLineArrowHeight'
        QtMocHelpers::PropertyData<QVariant>(256, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endLineArrowWidth'
        QtMocHelpers::PropertyData<QVariant>(257, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endFilledArrowHeight'
        QtMocHelpers::PropertyData<QVariant>(258, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endFilledArrowWidth'
        QtMocHelpers::PropertyData<QVariant>(259, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endFontFace'
        QtMocHelpers::PropertyData<QVariant>(260, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'endFontSize'
        QtMocHelpers::PropertyData<qreal>(261, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'endFontStyle'
        QtMocHelpers::PropertyData<int>(262, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'endTextOffset'
        QtMocHelpers::PropertyData<QPointF>(263, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'notelinePlacement'
        QtMocHelpers::PropertyData<QVariant>(264, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'avoidBarLines'
        QtMocHelpers::PropertyData<bool>(265, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'dynamicsSize'
        QtMocHelpers::PropertyData<qreal>(266, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'centerOnNotehead'
        QtMocHelpers::PropertyData<bool>(267, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'anchorToEndOfPrevious'
        QtMocHelpers::PropertyData<bool>(268, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'snapToDynamics'
        QtMocHelpers::PropertyData<bool>(269, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'snapBefore'
        QtMocHelpers::PropertyData<bool>(270, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'snapAfter'
        QtMocHelpers::PropertyData<bool>(271, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'voiceAssignment'
        QtMocHelpers::PropertyData<QVariant>(272, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'centerBetweenStaves'
        QtMocHelpers::PropertyData<QVariant>(273, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'posAbove'
        QtMocHelpers::PropertyData<QVariant>(274, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'locationStaves'
        QtMocHelpers::PropertyData<int>(275, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'locationVoices'
        QtMocHelpers::PropertyData<int>(276, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'locationMeasures'
        QtMocHelpers::PropertyData<int>(277, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'locationFractions'
        QtMocHelpers::PropertyData<QVariant>(278, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'locationGrace'
        QtMocHelpers::PropertyData<int>(279, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'locationNote'
        QtMocHelpers::PropertyData<int>(280, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'voice'
        QtMocHelpers::PropertyData<int>(281, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'position'
        QtMocHelpers::PropertyData<QVariant>(282, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'concertClefType'
        QtMocHelpers::PropertyData<QVariant>(283, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'transposingClefType'
        QtMocHelpers::PropertyData<QVariant>(284, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'clefToBarlinePos'
        QtMocHelpers::PropertyData<int>(285, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'isHeader'
        QtMocHelpers::PropertyData<bool>(286, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'concertKey'
        QtMocHelpers::PropertyData<int>(287, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'actualKey'
        QtMocHelpers::PropertyData<int>(288, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'action'
        QtMocHelpers::PropertyData<QVariant>(289, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'minDistance'
        QtMocHelpers::PropertyData<QVariant>(290, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'arpeggioType'
        QtMocHelpers::PropertyData<int>(291, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'chordLineType'
        QtMocHelpers::PropertyData<int>(292, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'chordLineStraight'
        QtMocHelpers::PropertyData<bool>(293, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'chordLineWavy'
        QtMocHelpers::PropertyData<bool>(294, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tremoloType'
        QtMocHelpers::PropertyData<int>(295, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tremoloStrokeStyle'
        QtMocHelpers::PropertyData<int>(296, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'harmonyType'
        QtMocHelpers::PropertyData<int>(297, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'arpeggioSpan'
        QtMocHelpers::PropertyData<int>(298, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bracketHookLength'
        QtMocHelpers::PropertyData<QVariant>(299, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'bracketHookPos'
        QtMocHelpers::PropertyData<int>(300, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bracketRightSide'
        QtMocHelpers::PropertyData<bool>(301, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bendType'
        QtMocHelpers::PropertyData<int>(302, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bendCurve'
        QtMocHelpers::PropertyData<QVariant>(303, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'bendVertexOffset'
        QtMocHelpers::PropertyData<QPointF>(304, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'bendShowHoldLine'
        QtMocHelpers::PropertyData<int>(305, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bendStartTimeFactor'
        QtMocHelpers::PropertyData<qreal>(306, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bendEndTimeFactor'
        QtMocHelpers::PropertyData<qreal>(307, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'guitarDiveTabPos'
        QtMocHelpers::PropertyData<int>(308, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'guitarBendAmount'
        QtMocHelpers::PropertyData<int>(309, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'vibratoLineType'
        QtMocHelpers::PropertyData<int>(310, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'guitarDiveIsSlack'
        QtMocHelpers::PropertyData<bool>(311, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tremoloBarType'
        QtMocHelpers::PropertyData<int>(312, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tremoloBarCurve'
        QtMocHelpers::PropertyData<QVariant>(313, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'startWithLongNames'
        QtMocHelpers::PropertyData<bool>(314, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'startWithMeasureOne'
        QtMocHelpers::PropertyData<bool>(315, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'firstSystemIndentation'
        QtMocHelpers::PropertyData<bool>(316, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'path'
        QtMocHelpers::PropertyData<QVariant>(317, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'preferSharpFlat'
        QtMocHelpers::PropertyData<int>(318, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'playTechType'
        QtMocHelpers::PropertyData<QVariant>(319, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'tempoChangeType'
        QtMocHelpers::PropertyData<QVariant>(320, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'tempoEasingMethod'
        QtMocHelpers::PropertyData<QVariant>(321, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'tempoChangeFactor'
        QtMocHelpers::PropertyData<qreal>(322, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'isDiagram'
        QtMocHelpers::PropertyData<bool>(323, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'active'
        QtMocHelpers::PropertyData<bool>(324, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fretPosition'
        QtMocHelpers::PropertyData<int>(325, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'ignoredStrings'
        QtMocHelpers::PropertyData<QVariant>(326, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'generateText'
        QtMocHelpers::PropertyData<bool>(327, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'transposeMode'
        QtMocHelpers::PropertyData<int>(328, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tiePlacement'
        QtMocHelpers::PropertyData<QVariant>(329, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'minLength'
        QtMocHelpers::PropertyData<QVariant>(330, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'partialSpannerDirection'
        QtMocHelpers::PropertyData<QVariant>(331, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'positionLinkedToMaster'
        QtMocHelpers::PropertyData<bool>(332, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'appearanceLinkedToMaster'
        QtMocHelpers::PropertyData<bool>(333, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'textLinkedToMaster'
        QtMocHelpers::PropertyData<bool>(334, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'excludeFromParts'
        QtMocHelpers::PropertyData<bool>(335, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'stringsCount'
        QtMocHelpers::PropertyData<int>(336, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'preset'
        QtMocHelpers::PropertyData<QVariant>(337, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'visibleStrings'
        QtMocHelpers::PropertyData<QVariant>(338, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'scoreFont'
        QtMocHelpers::PropertyData<QVariant>(339, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'symbolsSize'
        QtMocHelpers::PropertyData<qreal>(340, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'symbolAngle'
        QtMocHelpers::PropertyData<qreal>(341, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'applyToAllStaves'
        QtMocHelpers::PropertyData<bool>(342, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'isCourtesy'
        QtMocHelpers::PropertyData<bool>(343, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'excludeVerticalAlign'
        QtMocHelpers::PropertyData<bool>(344, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'playCountTextSetting'
        QtMocHelpers::PropertyData<QVariant>(345, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'playCountText'
        QtMocHelpers::PropertyData<QVariant>(346, 0x80000000 | 51, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'alignWithOtherRests'
        QtMocHelpers::PropertyData<bool>(347, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fraction'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(348, 0x80000000 | 349, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'beat'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(350, 0x80000000 | 349, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<EngravingItem, qt_meta_tag_ZN2mu9engraving5apiv113EngravingItemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::EngravingItem::staticMetaObject = { {
    QMetaObject::SuperData::link<apiv1::ScoreElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv113EngravingItemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv113EngravingItemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv113EngravingItemE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::EngravingItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<EngravingItem *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::EngravingItem* _r = _t->clone();
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 1: { QString _r = _t->subtypeName();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->_name();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 38:
        case 37:
        case 36:
        case 35:
        case 33:
        case 32:
        case 31:
        case 30:
        case 27:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QJSValue >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->parent(); break;
        case 1: *reinterpret_cast<apiv1::Staff**>(_v) = _t->staff(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->staffIdx(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->effectiveStaffIdx(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->vStaffIdx(); break;
        case 5: *reinterpret_cast<float*>(_v) = _t->offsetX(); break;
        case 6: *reinterpret_cast<float*>(_v) = _t->offsetY(); break;
        case 7: *reinterpret_cast<float*>(_v) = _t->posX(); break;
        case 8: *reinterpret_cast<float*>(_v) = _t->posY(); break;
        case 9: *reinterpret_cast<QPointF*>(_v) = _t->pos(); break;
        case 10: *reinterpret_cast<QPointF*>(_v) = _t->pagePos(); break;
        case 11: *reinterpret_cast<QPointF*>(_v) = _t->canvasPos(); break;
        case 12: *reinterpret_cast<QRectF*>(_v) = _t->bbox(); break;
        case 13: *reinterpret_cast<int*>(_v) = _t->subtype(); break;
        case 14: *reinterpret_cast<bool*>(_v) = _t->up(); break;
        case 15: *reinterpret_cast<bool*>(_v) = _t->header(); break;
        case 16: *reinterpret_cast<bool*>(_v) = _t->trailer(); break;
        case 17: *reinterpret_cast<bool*>(_v) = _t->isMovable(); break;
        case 18: *reinterpret_cast<bool*>(_v) = _t->enabled(); break;
        case 19: *reinterpret_cast<bool*>(_v) = _t->addToSkyline(); break;
        case 20: *reinterpret_cast<int*>(_v) = _t->get_subType(); break;
        case 21: *reinterpret_cast<bool*>(_v) = _t->get_selected(); break;
        case 22: *reinterpret_cast<bool*>(_v) = _t->get_generated(); break;
        case 23: *reinterpret_cast<QColor*>(_v) = _t->get_color(); break;
        case 24: *reinterpret_cast<bool*>(_v) = _t->get_visible(); break;
        case 25: *reinterpret_cast<int*>(_v) = _t->get_z(); break;
        case 26: *reinterpret_cast<bool*>(_v) = _t->get_small(); break;
        case 27: *reinterpret_cast<QJSValue*>(_v) = _t->get_hideWhenEmpty(); break;
        case 28: *reinterpret_cast<bool*>(_v) = _t->get_hideStavesWhenIndividuallyEmpty(); break;
        case 29: *reinterpret_cast<bool*>(_v) = _t->get_showCourtesy(); break;
        case 30: *reinterpret_cast<QJSValue*>(_v) = _t->get_keysig_mode(); break;
        case 31: *reinterpret_cast<QJSValue*>(_v) = _t->get_lineType(); break;
        case 32: *reinterpret_cast<QJSValue*>(_v) = _t->get_headType(); break;
        case 33: *reinterpret_cast<QJSValue*>(_v) = _t->get_headGroup(); break;
        case 34: *reinterpret_cast<int*>(_v) = _t->get_articulationAnchor(); break;
        case 35: *reinterpret_cast<QJSValue*>(_v) = _t->get_direction(); break;
        case 36: *reinterpret_cast<QJSValue*>(_v) = _t->get_horizontalDirection(); break;
        case 37: *reinterpret_cast<QJSValue*>(_v) = _t->get_stemDirection(); break;
        case 38: *reinterpret_cast<QJSValue*>(_v) = _t->get_slurDirection(); break;
        case 39: *reinterpret_cast<QVariant*>(_v) = _t->get_mirrorHead(); break;
        case 40: *reinterpret_cast<QVariant*>(_v) = _t->get_hasParentheses(); break;
        case 41: *reinterpret_cast<qreal*>(_v) = _t->get_pause(); break;
        case 42: *reinterpret_cast<QVariant*>(_v) = _t->get_barlineType(); break;
        case 43: *reinterpret_cast<QVariant*>(_v) = _t->get_barlineSpan(); break;
        case 44: *reinterpret_cast<int*>(_v) = _t->get_barlineSpanFrom(); break;
        case 45: *reinterpret_cast<int*>(_v) = _t->get_barlineSpanTo(); break;
        case 46: *reinterpret_cast<bool*>(_v) = _t->get_barlineShowTips(); break;
        case 47: *reinterpret_cast<QPointF*>(_v) = _t->get_offset(); break;
        case 48: *reinterpret_cast<bool*>(_v) = _t->get_ghost(); break;
        case 49: *reinterpret_cast<bool*>(_v) = _t->get_play(); break;
        case 50: *reinterpret_cast<QVariant*>(_v) = _t->get_boxHeight(); break;
        case 51: *reinterpret_cast<QVariant*>(_v) = _t->get_boxWidth(); break;
        case 52: *reinterpret_cast<bool*>(_v) = _t->get_boxAutoSize(); break;
        case 53: *reinterpret_cast<QVariant*>(_v) = _t->get_topGap(); break;
        case 54: *reinterpret_cast<QVariant*>(_v) = _t->get_bottomGap(); break;
        case 55: *reinterpret_cast<qreal*>(_v) = _t->get_leftMargin(); break;
        case 56: *reinterpret_cast<qreal*>(_v) = _t->get_rightMargin(); break;
        case 57: *reinterpret_cast<qreal*>(_v) = _t->get_topMargin(); break;
        case 58: *reinterpret_cast<qreal*>(_v) = _t->get_bottomMargin(); break;
        case 59: *reinterpret_cast<QVariant*>(_v) = _t->get_paddingToNotationAbove(); break;
        case 60: *reinterpret_cast<QVariant*>(_v) = _t->get_paddingToNotationBelow(); break;
        case 61: *reinterpret_cast<QVariant*>(_v) = _t->get_layoutBreakType(); break;
        case 62: *reinterpret_cast<bool*>(_v) = _t->get_autoscale(); break;
        case 63: *reinterpret_cast<QVariant*>(_v) = _t->get_size(); break;
        case 64: *reinterpret_cast<qreal*>(_v) = _t->get_imageHeight(); break;
        case 65: *reinterpret_cast<qreal*>(_v) = _t->get_imageWidth(); break;
        case 66: *reinterpret_cast<bool*>(_v) = _t->get_imageFramed(); break;
        case 67: *reinterpret_cast<qreal*>(_v) = _t->get_fretFrameTextScale(); break;
        case 68: *reinterpret_cast<qreal*>(_v) = _t->get_fretFrameDiagramScale(); break;
        case 69: *reinterpret_cast<QVariant*>(_v) = _t->get_fretFrameColumnGap(); break;
        case 70: *reinterpret_cast<QVariant*>(_v) = _t->get_fretFrameRowGap(); break;
        case 71: *reinterpret_cast<int*>(_v) = _t->get_fretFrameChordPerRow(); break;
        case 72: *reinterpret_cast<int*>(_v) = _t->get_fretFrameHAlign(); break;
        case 73: *reinterpret_cast<QVariant*>(_v) = _t->get_fretFrameDiagramsOrder(); break;
        case 74: *reinterpret_cast<QVariant*>(_v) = _t->get_scale(); break;
        case 75: *reinterpret_cast<bool*>(_v) = _t->get_lockAspectRatio(); break;
        case 76: *reinterpret_cast<bool*>(_v) = _t->get_sizeIsSpatium(); break;
        case 77: *reinterpret_cast<QVariant*>(_v) = _t->get_text(); break;
        case 78: *reinterpret_cast<QVariant*>(_v) = _t->get_htmlText(); break;
        case 79: *reinterpret_cast<bool*>(_v) = _t->get_userModified(); break;
        case 80: *reinterpret_cast<QVariant*>(_v) = _t->get_beamPos(); break;
        case 81: *reinterpret_cast<bool*>(_v) = _t->get_beamNoSlope(); break;
        case 82: *reinterpret_cast<int*>(_v) = _t->get_crossStaffMove(); break;
        case 83: *reinterpret_cast<QVariant*>(_v) = _t->get_userLen(); break;
        case 84: *reinterpret_cast<QVariant*>(_v) = _t->get_space(); break;
        case 85: *reinterpret_cast<QVariant*>(_v) = _t->get_tempo(); break;
        case 86: *reinterpret_cast<bool*>(_v) = _t->get_tempoFollowText(); break;
        case 87: *reinterpret_cast<bool*>(_v) = _t->get_tempoAlignRightOfRehearsalMark(); break;
        case 88: *reinterpret_cast<int*>(_v) = _t->get_accidentalBracket(); break;
        case 89: *reinterpret_cast<int*>(_v) = _t->get_accidentalType(); break;
        case 90: *reinterpret_cast<int*>(_v) = _t->get_stackingOrderOffset(); break;
        case 91: *reinterpret_cast<QVariant*>(_v) = _t->get_numeratorString(); break;
        case 92: *reinterpret_cast<QVariant*>(_v) = _t->get_denominatorString(); break;
        case 93: *reinterpret_cast<int*>(_v) = _t->get_fbprefix(); break;
        case 94: *reinterpret_cast<int*>(_v) = _t->get_fbdigit(); break;
        case 95: *reinterpret_cast<int*>(_v) = _t->get_fbsuffix(); break;
        case 96: *reinterpret_cast<int*>(_v) = _t->get_fbcontinuationline(); break;
        case 97: *reinterpret_cast<int*>(_v) = _t->get_fbparenthesis1(); break;
        case 98: *reinterpret_cast<int*>(_v) = _t->get_fbparenthesis2(); break;
        case 99: *reinterpret_cast<int*>(_v) = _t->get_fbparenthesis3(); break;
        case 100: *reinterpret_cast<int*>(_v) = _t->get_fbparenthesis4(); break;
        case 101: *reinterpret_cast<int*>(_v) = _t->get_fbparenthesis5(); break;
        case 102: *reinterpret_cast<int*>(_v) = _t->get_ottavaType(); break;
        case 103: *reinterpret_cast<bool*>(_v) = _t->get_numbersOnly(); break;
        case 104: *reinterpret_cast<int*>(_v) = _t->get_trillType(); break;
        case 105: *reinterpret_cast<int*>(_v) = _t->get_vibratoType(); break;
        case 106: *reinterpret_cast<bool*>(_v) = _t->get_hairpinCircledTip(); break;
        case 107: *reinterpret_cast<int*>(_v) = _t->get_hairpinType(); break;
        case 108: *reinterpret_cast<QVariant*>(_v) = _t->get_hairpinHeight(); break;
        case 109: *reinterpret_cast<QVariant*>(_v) = _t->get_hairpinContHeight(); break;
        case 110: *reinterpret_cast<int*>(_v) = _t->get_veloChange(); break;
        case 111: *reinterpret_cast<QVariant*>(_v) = _t->get_veloChangeMethod(); break;
        case 112: *reinterpret_cast<QVariant*>(_v) = _t->get_veloChangeSpeed(); break;
        case 113: *reinterpret_cast<QVariant*>(_v) = _t->get_dynamicType(); break;
        case 114: *reinterpret_cast<bool*>(_v) = _t->get_singleNoteDynamics(); break;
        case 115: *reinterpret_cast<QVariant*>(_v) = _t->get_changeMethod(); break;
        case 116: *reinterpret_cast<QVariant*>(_v) = _t->get_placement(); break;
        case 117: *reinterpret_cast<QVariant*>(_v) = _t->get_hPlacement(); break;
        case 118: *reinterpret_cast<int*>(_v) = _t->get_mmRestRangeBracketType(); break;
        case 119: *reinterpret_cast<int*>(_v) = _t->get_velocity(); break;
        case 120: *reinterpret_cast<QVariant*>(_v) = _t->get_jumpTo(); break;
        case 121: *reinterpret_cast<QVariant*>(_v) = _t->get_playUntil(); break;
        case 122: *reinterpret_cast<QVariant*>(_v) = _t->get_continueAt(); break;
        case 123: *reinterpret_cast<QVariant*>(_v) = _t->get_label(); break;
        case 124: *reinterpret_cast<int*>(_v) = _t->get_markerType(); break;
        case 125: *reinterpret_cast<qreal*>(_v) = _t->get_musicSymbolSize(); break;
        case 126: *reinterpret_cast<bool*>(_v) = _t->get_markerCenterOnSymbol(); break;
        case 127: *reinterpret_cast<qreal*>(_v) = _t->get_arpUserLen1(); break;
        case 128: *reinterpret_cast<qreal*>(_v) = _t->get_arpUserLen2(); break;
        case 129: *reinterpret_cast<int*>(_v) = _t->get_glissType(); break;
        case 130: *reinterpret_cast<QVariant*>(_v) = _t->get_glissText(); break;
        case 131: *reinterpret_cast<bool*>(_v) = _t->get_glissShowText(); break;
        case 132: *reinterpret_cast<QVariant*>(_v) = _t->get_glissandoStyle(); break;
        case 133: *reinterpret_cast<int*>(_v) = _t->get_glissEaseIn(); break;
        case 134: *reinterpret_cast<int*>(_v) = _t->get_glissEaseOut(); break;
        case 135: *reinterpret_cast<bool*>(_v) = _t->get_diagonal(); break;
        case 136: *reinterpret_cast<QVariant*>(_v) = _t->get_groups(); break;
        case 137: *reinterpret_cast<QVariant*>(_v) = _t->get_lineStyle(); break;
        case 138: *reinterpret_cast<QVariant*>(_v) = _t->get_lineColor(); break;
        case 139: *reinterpret_cast<QVariant*>(_v) = _t->get_lineWidth(); break;
        case 140: *reinterpret_cast<qreal*>(_v) = _t->get_timeStretch(); break;
        case 141: *reinterpret_cast<QVariant*>(_v) = _t->get_ornamentStyle(); break;
        case 142: *reinterpret_cast<QVariant*>(_v) = _t->get_intervalAbove(); break;
        case 143: *reinterpret_cast<QVariant*>(_v) = _t->get_intervalBelow(); break;
        case 144: *reinterpret_cast<int*>(_v) = _t->get_ornamentShowAccidental(); break;
        case 145: *reinterpret_cast<QVariant*>(_v) = _t->get_ornamentShowCueNote(); break;
        case 146: *reinterpret_cast<bool*>(_v) = _t->get_startOnUpperNote(); break;
        case 147: *reinterpret_cast<QVariant*>(_v) = _t->get_timesig(); break;
        case 148: *reinterpret_cast<QVariant*>(_v) = _t->get_timesigStretch(); break;
        case 149: *reinterpret_cast<int*>(_v) = _t->get_timesigType(); break;
        case 150: *reinterpret_cast<QVariant*>(_v) = _t->get_mmRestNumberPos(); break;
        case 151: *reinterpret_cast<QVariant*>(_v) = _t->get_mmRestNumberOffset(); break;
        case 152: *reinterpret_cast<bool*>(_v) = _t->get_mmRestNumberVisible(); break;
        case 153: *reinterpret_cast<QVariant*>(_v) = _t->get_measureRepeatNumberPos(); break;
        case 154: *reinterpret_cast<int*>(_v) = _t->get_verse(); break;
        case 155: *reinterpret_cast<QVariant*>(_v) = _t->get_volta_ending(); break;
        case 156: *reinterpret_cast<bool*>(_v) = _t->get_lineVisible(); break;
        case 157: *reinterpret_cast<qreal*>(_v) = _t->get_mag(); break;
        case 158: *reinterpret_cast<int*>(_v) = _t->get_useDrumset(); break;
        case 159: *reinterpret_cast<QVariant*>(_v) = _t->get_role(); break;
        case 160: *reinterpret_cast<int*>(_v) = _t->get_track(); break;
        case 161: *reinterpret_cast<int*>(_v) = _t->get_fretStrings(); break;
        case 162: *reinterpret_cast<int*>(_v) = _t->get_fretFrets(); break;
        case 163: *reinterpret_cast<bool*>(_v) = _t->get_showNut(); break;
        case 164: *reinterpret_cast<int*>(_v) = _t->get_fretOffset(); break;
        case 165: *reinterpret_cast<int*>(_v) = _t->get_fretNumPos(); break;
        case 166: *reinterpret_cast<QVariant*>(_v) = _t->get_orientation(); break;
        case 167: *reinterpret_cast<bool*>(_v) = _t->get_fretShowFingering(); break;
        case 168: *reinterpret_cast<QVariant*>(_v) = _t->get_fretFingering(); break;
        case 169: *reinterpret_cast<bool*>(_v) = _t->get_harmonyVoiceLiteral(); break;
        case 170: *reinterpret_cast<QVariant*>(_v) = _t->get_harmonyVoicing(); break;
        case 171: *reinterpret_cast<QVariant*>(_v) = _t->get_harmonyDuration(); break;
        case 172: *reinterpret_cast<QVariant*>(_v) = _t->get_harmonyBassScale(); break;
        case 173: *reinterpret_cast<bool*>(_v) = _t->get_harmonyDoNotStackModifiers(); break;
        case 174: *reinterpret_cast<int*>(_v) = _t->get_systemBracket(); break;
        case 175: *reinterpret_cast<bool*>(_v) = _t->get_gap(); break;
        case 176: *reinterpret_cast<bool*>(_v) = _t->get_autoplace(); break;
        case 177: *reinterpret_cast<qreal*>(_v) = _t->get_dashLineLen(); break;
        case 178: *reinterpret_cast<qreal*>(_v) = _t->get_dashGapLen(); break;
        case 179: *reinterpret_cast<QVariant*>(_v) = _t->get_symbol(); break;
        case 180: *reinterpret_cast<bool*>(_v) = _t->get_playRepeats(); break;
        case 181: *reinterpret_cast<bool*>(_v) = _t->get_createSystemHeader(); break;
        case 182: *reinterpret_cast<int*>(_v) = _t->get_staffLines(); break;
        case 183: *reinterpret_cast<QVariant*>(_v) = _t->get_lineDistance(); break;
        case 184: *reinterpret_cast<int*>(_v) = _t->get_stepOffset(); break;
        case 185: *reinterpret_cast<bool*>(_v) = _t->get_staffShowBarlines(); break;
        case 186: *reinterpret_cast<bool*>(_v) = _t->get_staffShowLedgerlines(); break;
        case 187: *reinterpret_cast<bool*>(_v) = _t->get_staffStemless(); break;
        case 188: *reinterpret_cast<bool*>(_v) = _t->get_staffInvisible(); break;
        case 189: *reinterpret_cast<QColor*>(_v) = _t->get_staffColor(); break;
        case 190: *reinterpret_cast<QVariant*>(_v) = _t->get_headScheme(); break;
        case 191: *reinterpret_cast<bool*>(_v) = _t->get_staffGenClef(); break;
        case 192: *reinterpret_cast<bool*>(_v) = _t->get_staffGenTimesig(); break;
        case 193: *reinterpret_cast<bool*>(_v) = _t->get_staffGenKeysig(); break;
        case 194: *reinterpret_cast<QVariant*>(_v) = _t->get_staffYoffset(); break;
        case 195: *reinterpret_cast<int*>(_v) = _t->get_bracketSpan(); break;
        case 196: *reinterpret_cast<int*>(_v) = _t->get_bracketColumn(); break;
        case 197: *reinterpret_cast<int*>(_v) = _t->get_inameLayoutPosition(); break;
        case 198: *reinterpret_cast<QVariant*>(_v) = _t->get_subStyle(); break;
        case 199: *reinterpret_cast<QVariant*>(_v) = _t->get_fontFace(); break;
        case 200: *reinterpret_cast<qreal*>(_v) = _t->get_fontSize(); break;
        case 201: *reinterpret_cast<int*>(_v) = _t->get_fontStyle(); break;
        case 202: *reinterpret_cast<qreal*>(_v) = _t->get_lineSpacing(); break;
        case 203: *reinterpret_cast<int*>(_v) = _t->get_frameType(); break;
        case 204: *reinterpret_cast<QVariant*>(_v) = _t->get_frameWidth(); break;
        case 205: *reinterpret_cast<QVariant*>(_v) = _t->get_framePadding(); break;
        case 206: *reinterpret_cast<int*>(_v) = _t->get_frameRound(); break;
        case 207: *reinterpret_cast<QColor*>(_v) = _t->get_frameFgColor(); break;
        case 208: *reinterpret_cast<QColor*>(_v) = _t->get_frameBgColor(); break;
        case 209: *reinterpret_cast<bool*>(_v) = _t->get_sizeSpatiumDependent(); break;
        case 210: *reinterpret_cast<bool*>(_v) = _t->get_textSizeSpatiumDependent(); break;
        case 211: *reinterpret_cast<qreal*>(_v) = _t->get_musicalSymbolsScale(); break;
        case 212: *reinterpret_cast<QVariant*>(_v) = _t->get_align(); break;
        case 213: *reinterpret_cast<int*>(_v) = _t->get_textScriptAlign(); break;
        case 214: *reinterpret_cast<bool*>(_v) = _t->get_systemFlag(); break;
        case 215: *reinterpret_cast<QVariant*>(_v) = _t->get_beginText(); break;
        case 216: *reinterpret_cast<QVariant*>(_v) = _t->get_beginTextAlign(); break;
        case 217: *reinterpret_cast<QVariant*>(_v) = _t->get_beginTextPosition(); break;
        case 218: *reinterpret_cast<QVariant*>(_v) = _t->get_beginTextPlace(); break;
        case 219: *reinterpret_cast<QVariant*>(_v) = _t->get_beginHookType(); break;
        case 220: *reinterpret_cast<QVariant*>(_v) = _t->get_beginHookHeight(); break;
        case 221: *reinterpret_cast<QVariant*>(_v) = _t->get_beginLineArrowHeight(); break;
        case 222: *reinterpret_cast<QVariant*>(_v) = _t->get_beginLineArrowWidth(); break;
        case 223: *reinterpret_cast<QVariant*>(_v) = _t->get_beginFilledArrowHeight(); break;
        case 224: *reinterpret_cast<QVariant*>(_v) = _t->get_beginFilledArrowWidth(); break;
        case 225: *reinterpret_cast<QVariant*>(_v) = _t->get_beginFontFace(); break;
        case 226: *reinterpret_cast<qreal*>(_v) = _t->get_beginFontSize(); break;
        case 227: *reinterpret_cast<int*>(_v) = _t->get_beginFontStyle(); break;
        case 228: *reinterpret_cast<QPointF*>(_v) = _t->get_beginTextOffset(); break;
        case 229: *reinterpret_cast<QVariant*>(_v) = _t->get_gapBetweenTextAndLine(); break;
        case 230: *reinterpret_cast<QVariant*>(_v) = _t->get_continueText(); break;
        case 231: *reinterpret_cast<QVariant*>(_v) = _t->get_continueTextAlign(); break;
        case 232: *reinterpret_cast<QVariant*>(_v) = _t->get_continueTextPosition(); break;
        case 233: *reinterpret_cast<QVariant*>(_v) = _t->get_continueTextPlace(); break;
        case 234: *reinterpret_cast<QVariant*>(_v) = _t->get_continueFontFace(); break;
        case 235: *reinterpret_cast<qreal*>(_v) = _t->get_continueFontSize(); break;
        case 236: *reinterpret_cast<int*>(_v) = _t->get_continueFontStyle(); break;
        case 237: *reinterpret_cast<QPointF*>(_v) = _t->get_continueTextOffset(); break;
        case 238: *reinterpret_cast<QVariant*>(_v) = _t->get_endText(); break;
        case 239: *reinterpret_cast<QVariant*>(_v) = _t->get_endTextAlign(); break;
        case 240: *reinterpret_cast<QVariant*>(_v) = _t->get_endTextPosition(); break;
        case 241: *reinterpret_cast<QVariant*>(_v) = _t->get_endTextPlace(); break;
        case 242: *reinterpret_cast<QVariant*>(_v) = _t->get_endHookType(); break;
        case 243: *reinterpret_cast<QVariant*>(_v) = _t->get_endHookHeight(); break;
        case 244: *reinterpret_cast<QVariant*>(_v) = _t->get_endLineArrowHeight(); break;
        case 245: *reinterpret_cast<QVariant*>(_v) = _t->get_endLineArrowWidth(); break;
        case 246: *reinterpret_cast<QVariant*>(_v) = _t->get_endFilledArrowHeight(); break;
        case 247: *reinterpret_cast<QVariant*>(_v) = _t->get_endFilledArrowWidth(); break;
        case 248: *reinterpret_cast<QVariant*>(_v) = _t->get_endFontFace(); break;
        case 249: *reinterpret_cast<qreal*>(_v) = _t->get_endFontSize(); break;
        case 250: *reinterpret_cast<int*>(_v) = _t->get_endFontStyle(); break;
        case 251: *reinterpret_cast<QPointF*>(_v) = _t->get_endTextOffset(); break;
        case 252: *reinterpret_cast<QVariant*>(_v) = _t->get_notelinePlacement(); break;
        case 253: *reinterpret_cast<bool*>(_v) = _t->get_avoidBarLines(); break;
        case 254: *reinterpret_cast<qreal*>(_v) = _t->get_dynamicsSize(); break;
        case 255: *reinterpret_cast<bool*>(_v) = _t->get_centerOnNotehead(); break;
        case 256: *reinterpret_cast<bool*>(_v) = _t->get_anchorToEndOfPrevious(); break;
        case 257: *reinterpret_cast<bool*>(_v) = _t->get_snapToDynamics(); break;
        case 258: *reinterpret_cast<bool*>(_v) = _t->get_snapBefore(); break;
        case 259: *reinterpret_cast<bool*>(_v) = _t->get_snapAfter(); break;
        case 260: *reinterpret_cast<QVariant*>(_v) = _t->get_voiceAssignment(); break;
        case 261: *reinterpret_cast<QVariant*>(_v) = _t->get_centerBetweenStaves(); break;
        case 262: *reinterpret_cast<QVariant*>(_v) = _t->get_posAbove(); break;
        case 263: *reinterpret_cast<int*>(_v) = _t->get_locationStaves(); break;
        case 264: *reinterpret_cast<int*>(_v) = _t->get_locationVoices(); break;
        case 265: *reinterpret_cast<int*>(_v) = _t->get_locationMeasures(); break;
        case 266: *reinterpret_cast<QVariant*>(_v) = _t->get_locationFractions(); break;
        case 267: *reinterpret_cast<int*>(_v) = _t->get_locationGrace(); break;
        case 268: *reinterpret_cast<int*>(_v) = _t->get_locationNote(); break;
        case 269: *reinterpret_cast<int*>(_v) = _t->get_voice(); break;
        case 270: *reinterpret_cast<QVariant*>(_v) = _t->get_position(); break;
        case 271: *reinterpret_cast<QVariant*>(_v) = _t->get_concertClefType(); break;
        case 272: *reinterpret_cast<QVariant*>(_v) = _t->get_transposingClefType(); break;
        case 273: *reinterpret_cast<int*>(_v) = _t->get_clefToBarlinePos(); break;
        case 274: *reinterpret_cast<bool*>(_v) = _t->get_isHeader(); break;
        case 275: *reinterpret_cast<int*>(_v) = _t->get_concertKey(); break;
        case 276: *reinterpret_cast<int*>(_v) = _t->get_actualKey(); break;
        case 277: *reinterpret_cast<QVariant*>(_v) = _t->get_action(); break;
        case 278: *reinterpret_cast<QVariant*>(_v) = _t->get_minDistance(); break;
        case 279: *reinterpret_cast<int*>(_v) = _t->get_arpeggioType(); break;
        case 280: *reinterpret_cast<int*>(_v) = _t->get_chordLineType(); break;
        case 281: *reinterpret_cast<bool*>(_v) = _t->get_chordLineStraight(); break;
        case 282: *reinterpret_cast<bool*>(_v) = _t->get_chordLineWavy(); break;
        case 283: *reinterpret_cast<int*>(_v) = _t->get_tremoloType(); break;
        case 284: *reinterpret_cast<int*>(_v) = _t->get_tremoloStrokeStyle(); break;
        case 285: *reinterpret_cast<int*>(_v) = _t->get_harmonyType(); break;
        case 286: *reinterpret_cast<int*>(_v) = _t->get_arpeggioSpan(); break;
        case 287: *reinterpret_cast<QVariant*>(_v) = _t->get_bracketHookLength(); break;
        case 288: *reinterpret_cast<int*>(_v) = _t->get_bracketHookPos(); break;
        case 289: *reinterpret_cast<bool*>(_v) = _t->get_bracketRightSide(); break;
        case 290: *reinterpret_cast<int*>(_v) = _t->get_bendType(); break;
        case 291: *reinterpret_cast<QVariant*>(_v) = _t->get_bendCurve(); break;
        case 292: *reinterpret_cast<QPointF*>(_v) = _t->get_bendVertexOffset(); break;
        case 293: *reinterpret_cast<int*>(_v) = _t->get_bendShowHoldLine(); break;
        case 294: *reinterpret_cast<qreal*>(_v) = _t->get_bendStartTimeFactor(); break;
        case 295: *reinterpret_cast<qreal*>(_v) = _t->get_bendEndTimeFactor(); break;
        case 296: *reinterpret_cast<int*>(_v) = _t->get_guitarDiveTabPos(); break;
        case 297: *reinterpret_cast<int*>(_v) = _t->get_guitarBendAmount(); break;
        case 298: *reinterpret_cast<int*>(_v) = _t->get_vibratoLineType(); break;
        case 299: *reinterpret_cast<bool*>(_v) = _t->get_guitarDiveIsSlack(); break;
        case 300: *reinterpret_cast<int*>(_v) = _t->get_tremoloBarType(); break;
        case 301: *reinterpret_cast<QVariant*>(_v) = _t->get_tremoloBarCurve(); break;
        case 302: *reinterpret_cast<bool*>(_v) = _t->get_startWithLongNames(); break;
        case 303: *reinterpret_cast<bool*>(_v) = _t->get_startWithMeasureOne(); break;
        case 304: *reinterpret_cast<bool*>(_v) = _t->get_firstSystemIndentation(); break;
        case 305: *reinterpret_cast<QVariant*>(_v) = _t->get_path(); break;
        case 306: *reinterpret_cast<int*>(_v) = _t->get_preferSharpFlat(); break;
        case 307: *reinterpret_cast<QVariant*>(_v) = _t->get_playTechType(); break;
        case 308: *reinterpret_cast<QVariant*>(_v) = _t->get_tempoChangeType(); break;
        case 309: *reinterpret_cast<QVariant*>(_v) = _t->get_tempoEasingMethod(); break;
        case 310: *reinterpret_cast<qreal*>(_v) = _t->get_tempoChangeFactor(); break;
        case 311: *reinterpret_cast<bool*>(_v) = _t->get_isDiagram(); break;
        case 312: *reinterpret_cast<bool*>(_v) = _t->get_active(); break;
        case 313: *reinterpret_cast<int*>(_v) = _t->get_fretPosition(); break;
        case 314: *reinterpret_cast<QVariant*>(_v) = _t->get_ignoredStrings(); break;
        case 315: *reinterpret_cast<bool*>(_v) = _t->get_generateText(); break;
        case 316: *reinterpret_cast<int*>(_v) = _t->get_transposeMode(); break;
        case 317: *reinterpret_cast<QVariant*>(_v) = _t->get_tiePlacement(); break;
        case 318: *reinterpret_cast<QVariant*>(_v) = _t->get_minLength(); break;
        case 319: *reinterpret_cast<QVariant*>(_v) = _t->get_partialSpannerDirection(); break;
        case 320: *reinterpret_cast<bool*>(_v) = _t->get_positionLinkedToMaster(); break;
        case 321: *reinterpret_cast<bool*>(_v) = _t->get_appearanceLinkedToMaster(); break;
        case 322: *reinterpret_cast<bool*>(_v) = _t->get_textLinkedToMaster(); break;
        case 323: *reinterpret_cast<bool*>(_v) = _t->get_excludeFromParts(); break;
        case 324: *reinterpret_cast<int*>(_v) = _t->get_stringsCount(); break;
        case 325: *reinterpret_cast<QVariant*>(_v) = _t->get_preset(); break;
        case 326: *reinterpret_cast<QVariant*>(_v) = _t->get_visibleStrings(); break;
        case 327: *reinterpret_cast<QVariant*>(_v) = _t->get_scoreFont(); break;
        case 328: *reinterpret_cast<qreal*>(_v) = _t->get_symbolsSize(); break;
        case 329: *reinterpret_cast<qreal*>(_v) = _t->get_symbolAngle(); break;
        case 330: *reinterpret_cast<bool*>(_v) = _t->get_applyToAllStaves(); break;
        case 331: *reinterpret_cast<bool*>(_v) = _t->get_isCourtesy(); break;
        case 332: *reinterpret_cast<bool*>(_v) = _t->get_excludeVerticalAlign(); break;
        case 333: *reinterpret_cast<QVariant*>(_v) = _t->get_playCountTextSetting(); break;
        case 334: *reinterpret_cast<QVariant*>(_v) = _t->get_playCountText(); break;
        case 335: *reinterpret_cast<bool*>(_v) = _t->get_alignWithOtherRests(); break;
        case 336: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->tick(); break;
        case 337: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->beat(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 5: _t->setOffsetX(*reinterpret_cast<float*>(_v)); break;
        case 6: _t->setOffsetY(*reinterpret_cast<float*>(_v)); break;
        case 20: _t->set_subType(*reinterpret_cast<int*>(_v)); break;
        case 23: _t->set_color(*reinterpret_cast<QColor*>(_v)); break;
        case 24: _t->set_visible(*reinterpret_cast<bool*>(_v)); break;
        case 25: _t->set_z(*reinterpret_cast<int*>(_v)); break;
        case 26: _t->set_small(*reinterpret_cast<bool*>(_v)); break;
        case 27: _t->set_hideWhenEmpty(*reinterpret_cast<QJSValue*>(_v)); break;
        case 28: _t->set_hideStavesWhenIndividuallyEmpty(*reinterpret_cast<bool*>(_v)); break;
        case 29: _t->set_showCourtesy(*reinterpret_cast<bool*>(_v)); break;
        case 30: _t->set_keysig_mode(*reinterpret_cast<QJSValue*>(_v)); break;
        case 31: _t->set_lineType(*reinterpret_cast<QJSValue*>(_v)); break;
        case 32: _t->set_headType(*reinterpret_cast<QJSValue*>(_v)); break;
        case 33: _t->set_headGroup(*reinterpret_cast<QJSValue*>(_v)); break;
        case 34: _t->set_articulationAnchor(*reinterpret_cast<int*>(_v)); break;
        case 35: _t->set_direction(*reinterpret_cast<QJSValue*>(_v)); break;
        case 36: _t->set_horizontalDirection(*reinterpret_cast<QJSValue*>(_v)); break;
        case 37: _t->set_stemDirection(*reinterpret_cast<QJSValue*>(_v)); break;
        case 38: _t->set_slurDirection(*reinterpret_cast<QJSValue*>(_v)); break;
        case 39: _t->set_mirrorHead(*reinterpret_cast<QVariant*>(_v)); break;
        case 40: _t->set_hasParentheses(*reinterpret_cast<QVariant*>(_v)); break;
        case 41: _t->set_pause(*reinterpret_cast<qreal*>(_v)); break;
        case 42: _t->set_barlineType(*reinterpret_cast<QVariant*>(_v)); break;
        case 43: _t->set_barlineSpan(*reinterpret_cast<QVariant*>(_v)); break;
        case 44: _t->set_barlineSpanFrom(*reinterpret_cast<int*>(_v)); break;
        case 45: _t->set_barlineSpanTo(*reinterpret_cast<int*>(_v)); break;
        case 46: _t->set_barlineShowTips(*reinterpret_cast<bool*>(_v)); break;
        case 47: _t->set_offset(*reinterpret_cast<QPointF*>(_v)); break;
        case 48: _t->set_ghost(*reinterpret_cast<bool*>(_v)); break;
        case 49: _t->set_play(*reinterpret_cast<bool*>(_v)); break;
        case 50: _t->set_boxHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 51: _t->set_boxWidth(*reinterpret_cast<QVariant*>(_v)); break;
        case 52: _t->set_boxAutoSize(*reinterpret_cast<bool*>(_v)); break;
        case 53: _t->set_topGap(*reinterpret_cast<QVariant*>(_v)); break;
        case 54: _t->set_bottomGap(*reinterpret_cast<QVariant*>(_v)); break;
        case 55: _t->set_leftMargin(*reinterpret_cast<qreal*>(_v)); break;
        case 56: _t->set_rightMargin(*reinterpret_cast<qreal*>(_v)); break;
        case 57: _t->set_topMargin(*reinterpret_cast<qreal*>(_v)); break;
        case 58: _t->set_bottomMargin(*reinterpret_cast<qreal*>(_v)); break;
        case 59: _t->set_paddingToNotationAbove(*reinterpret_cast<QVariant*>(_v)); break;
        case 60: _t->set_paddingToNotationBelow(*reinterpret_cast<QVariant*>(_v)); break;
        case 61: _t->set_layoutBreakType(*reinterpret_cast<QVariant*>(_v)); break;
        case 62: _t->set_autoscale(*reinterpret_cast<bool*>(_v)); break;
        case 63: _t->set_size(*reinterpret_cast<QVariant*>(_v)); break;
        case 64: _t->set_imageHeight(*reinterpret_cast<qreal*>(_v)); break;
        case 65: _t->set_imageWidth(*reinterpret_cast<qreal*>(_v)); break;
        case 66: _t->set_imageFramed(*reinterpret_cast<bool*>(_v)); break;
        case 67: _t->set_fretFrameTextScale(*reinterpret_cast<qreal*>(_v)); break;
        case 68: _t->set_fretFrameDiagramScale(*reinterpret_cast<qreal*>(_v)); break;
        case 69: _t->set_fretFrameColumnGap(*reinterpret_cast<QVariant*>(_v)); break;
        case 70: _t->set_fretFrameRowGap(*reinterpret_cast<QVariant*>(_v)); break;
        case 71: _t->set_fretFrameChordPerRow(*reinterpret_cast<int*>(_v)); break;
        case 72: _t->set_fretFrameHAlign(*reinterpret_cast<int*>(_v)); break;
        case 73: _t->set_fretFrameDiagramsOrder(*reinterpret_cast<QVariant*>(_v)); break;
        case 74: _t->set_scale(*reinterpret_cast<QVariant*>(_v)); break;
        case 75: _t->set_lockAspectRatio(*reinterpret_cast<bool*>(_v)); break;
        case 76: _t->set_sizeIsSpatium(*reinterpret_cast<bool*>(_v)); break;
        case 77: _t->set_text(*reinterpret_cast<QVariant*>(_v)); break;
        case 78: _t->set_htmlText(*reinterpret_cast<QVariant*>(_v)); break;
        case 79: _t->set_userModified(*reinterpret_cast<bool*>(_v)); break;
        case 80: _t->set_beamPos(*reinterpret_cast<QVariant*>(_v)); break;
        case 81: _t->set_beamNoSlope(*reinterpret_cast<bool*>(_v)); break;
        case 82: _t->set_crossStaffMove(*reinterpret_cast<int*>(_v)); break;
        case 83: _t->set_userLen(*reinterpret_cast<QVariant*>(_v)); break;
        case 84: _t->set_space(*reinterpret_cast<QVariant*>(_v)); break;
        case 85: _t->set_tempo(*reinterpret_cast<QVariant*>(_v)); break;
        case 86: _t->set_tempoFollowText(*reinterpret_cast<bool*>(_v)); break;
        case 87: _t->set_tempoAlignRightOfRehearsalMark(*reinterpret_cast<bool*>(_v)); break;
        case 88: _t->set_accidentalBracket(*reinterpret_cast<int*>(_v)); break;
        case 89: _t->set_accidentalType(*reinterpret_cast<int*>(_v)); break;
        case 90: _t->set_stackingOrderOffset(*reinterpret_cast<int*>(_v)); break;
        case 91: _t->set_numeratorString(*reinterpret_cast<QVariant*>(_v)); break;
        case 92: _t->set_denominatorString(*reinterpret_cast<QVariant*>(_v)); break;
        case 93: _t->set_fbprefix(*reinterpret_cast<int*>(_v)); break;
        case 94: _t->set_fbdigit(*reinterpret_cast<int*>(_v)); break;
        case 95: _t->set_fbsuffix(*reinterpret_cast<int*>(_v)); break;
        case 96: _t->set_fbcontinuationline(*reinterpret_cast<int*>(_v)); break;
        case 97: _t->set_fbparenthesis1(*reinterpret_cast<int*>(_v)); break;
        case 98: _t->set_fbparenthesis2(*reinterpret_cast<int*>(_v)); break;
        case 99: _t->set_fbparenthesis3(*reinterpret_cast<int*>(_v)); break;
        case 100: _t->set_fbparenthesis4(*reinterpret_cast<int*>(_v)); break;
        case 101: _t->set_fbparenthesis5(*reinterpret_cast<int*>(_v)); break;
        case 102: _t->set_ottavaType(*reinterpret_cast<int*>(_v)); break;
        case 103: _t->set_numbersOnly(*reinterpret_cast<bool*>(_v)); break;
        case 104: _t->set_trillType(*reinterpret_cast<int*>(_v)); break;
        case 105: _t->set_vibratoType(*reinterpret_cast<int*>(_v)); break;
        case 106: _t->set_hairpinCircledTip(*reinterpret_cast<bool*>(_v)); break;
        case 107: _t->set_hairpinType(*reinterpret_cast<int*>(_v)); break;
        case 108: _t->set_hairpinHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 109: _t->set_hairpinContHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 110: _t->set_veloChange(*reinterpret_cast<int*>(_v)); break;
        case 111: _t->set_veloChangeMethod(*reinterpret_cast<QVariant*>(_v)); break;
        case 112: _t->set_veloChangeSpeed(*reinterpret_cast<QVariant*>(_v)); break;
        case 113: _t->set_dynamicType(*reinterpret_cast<QVariant*>(_v)); break;
        case 114: _t->set_singleNoteDynamics(*reinterpret_cast<bool*>(_v)); break;
        case 115: _t->set_changeMethod(*reinterpret_cast<QVariant*>(_v)); break;
        case 116: _t->set_placement(*reinterpret_cast<QVariant*>(_v)); break;
        case 117: _t->set_hPlacement(*reinterpret_cast<QVariant*>(_v)); break;
        case 118: _t->set_mmRestRangeBracketType(*reinterpret_cast<int*>(_v)); break;
        case 119: _t->set_velocity(*reinterpret_cast<int*>(_v)); break;
        case 120: _t->set_jumpTo(*reinterpret_cast<QVariant*>(_v)); break;
        case 121: _t->set_playUntil(*reinterpret_cast<QVariant*>(_v)); break;
        case 122: _t->set_continueAt(*reinterpret_cast<QVariant*>(_v)); break;
        case 123: _t->set_label(*reinterpret_cast<QVariant*>(_v)); break;
        case 124: _t->set_markerType(*reinterpret_cast<int*>(_v)); break;
        case 125: _t->set_musicSymbolSize(*reinterpret_cast<qreal*>(_v)); break;
        case 126: _t->set_markerCenterOnSymbol(*reinterpret_cast<bool*>(_v)); break;
        case 127: _t->set_arpUserLen1(*reinterpret_cast<qreal*>(_v)); break;
        case 128: _t->set_arpUserLen2(*reinterpret_cast<qreal*>(_v)); break;
        case 129: _t->set_glissType(*reinterpret_cast<int*>(_v)); break;
        case 130: _t->set_glissText(*reinterpret_cast<QVariant*>(_v)); break;
        case 131: _t->set_glissShowText(*reinterpret_cast<bool*>(_v)); break;
        case 132: _t->set_glissandoStyle(*reinterpret_cast<QVariant*>(_v)); break;
        case 133: _t->set_glissEaseIn(*reinterpret_cast<int*>(_v)); break;
        case 134: _t->set_glissEaseOut(*reinterpret_cast<int*>(_v)); break;
        case 135: _t->set_diagonal(*reinterpret_cast<bool*>(_v)); break;
        case 136: _t->set_groups(*reinterpret_cast<QVariant*>(_v)); break;
        case 137: _t->set_lineStyle(*reinterpret_cast<QVariant*>(_v)); break;
        case 138: _t->set_lineColor(*reinterpret_cast<QVariant*>(_v)); break;
        case 139: _t->set_lineWidth(*reinterpret_cast<QVariant*>(_v)); break;
        case 140: _t->set_timeStretch(*reinterpret_cast<qreal*>(_v)); break;
        case 141: _t->set_ornamentStyle(*reinterpret_cast<QVariant*>(_v)); break;
        case 142: _t->set_intervalAbove(*reinterpret_cast<QVariant*>(_v)); break;
        case 143: _t->set_intervalBelow(*reinterpret_cast<QVariant*>(_v)); break;
        case 144: _t->set_ornamentShowAccidental(*reinterpret_cast<int*>(_v)); break;
        case 145: _t->set_ornamentShowCueNote(*reinterpret_cast<QVariant*>(_v)); break;
        case 146: _t->set_startOnUpperNote(*reinterpret_cast<bool*>(_v)); break;
        case 147: _t->set_timesig(*reinterpret_cast<QVariant*>(_v)); break;
        case 148: _t->set_timesigStretch(*reinterpret_cast<QVariant*>(_v)); break;
        case 149: _t->set_timesigType(*reinterpret_cast<int*>(_v)); break;
        case 150: _t->set_mmRestNumberPos(*reinterpret_cast<QVariant*>(_v)); break;
        case 151: _t->set_mmRestNumberOffset(*reinterpret_cast<QVariant*>(_v)); break;
        case 152: _t->set_mmRestNumberVisible(*reinterpret_cast<bool*>(_v)); break;
        case 153: _t->set_measureRepeatNumberPos(*reinterpret_cast<QVariant*>(_v)); break;
        case 154: _t->set_verse(*reinterpret_cast<int*>(_v)); break;
        case 155: _t->set_volta_ending(*reinterpret_cast<QVariant*>(_v)); break;
        case 156: _t->set_lineVisible(*reinterpret_cast<bool*>(_v)); break;
        case 157: _t->set_mag(*reinterpret_cast<qreal*>(_v)); break;
        case 158: _t->set_useDrumset(*reinterpret_cast<int*>(_v)); break;
        case 159: _t->set_role(*reinterpret_cast<QVariant*>(_v)); break;
        case 160: _t->set_track(*reinterpret_cast<int*>(_v)); break;
        case 161: _t->set_fretStrings(*reinterpret_cast<int*>(_v)); break;
        case 162: _t->set_fretFrets(*reinterpret_cast<int*>(_v)); break;
        case 163: _t->set_showNut(*reinterpret_cast<bool*>(_v)); break;
        case 164: _t->set_fretOffset(*reinterpret_cast<int*>(_v)); break;
        case 165: _t->set_fretNumPos(*reinterpret_cast<int*>(_v)); break;
        case 166: _t->set_orientation(*reinterpret_cast<QVariant*>(_v)); break;
        case 167: _t->set_fretShowFingering(*reinterpret_cast<bool*>(_v)); break;
        case 168: _t->set_fretFingering(*reinterpret_cast<QVariant*>(_v)); break;
        case 169: _t->set_harmonyVoiceLiteral(*reinterpret_cast<bool*>(_v)); break;
        case 170: _t->set_harmonyVoicing(*reinterpret_cast<QVariant*>(_v)); break;
        case 171: _t->set_harmonyDuration(*reinterpret_cast<QVariant*>(_v)); break;
        case 172: _t->set_harmonyBassScale(*reinterpret_cast<QVariant*>(_v)); break;
        case 173: _t->set_harmonyDoNotStackModifiers(*reinterpret_cast<bool*>(_v)); break;
        case 174: _t->set_systemBracket(*reinterpret_cast<int*>(_v)); break;
        case 175: _t->set_gap(*reinterpret_cast<bool*>(_v)); break;
        case 176: _t->set_autoplace(*reinterpret_cast<bool*>(_v)); break;
        case 177: _t->set_dashLineLen(*reinterpret_cast<qreal*>(_v)); break;
        case 178: _t->set_dashGapLen(*reinterpret_cast<qreal*>(_v)); break;
        case 179: _t->set_symbol(*reinterpret_cast<QVariant*>(_v)); break;
        case 180: _t->set_playRepeats(*reinterpret_cast<bool*>(_v)); break;
        case 181: _t->set_createSystemHeader(*reinterpret_cast<bool*>(_v)); break;
        case 182: _t->set_staffLines(*reinterpret_cast<int*>(_v)); break;
        case 183: _t->set_lineDistance(*reinterpret_cast<QVariant*>(_v)); break;
        case 184: _t->set_stepOffset(*reinterpret_cast<int*>(_v)); break;
        case 185: _t->set_staffShowBarlines(*reinterpret_cast<bool*>(_v)); break;
        case 186: _t->set_staffShowLedgerlines(*reinterpret_cast<bool*>(_v)); break;
        case 187: _t->set_staffStemless(*reinterpret_cast<bool*>(_v)); break;
        case 188: _t->set_staffInvisible(*reinterpret_cast<bool*>(_v)); break;
        case 189: _t->set_staffColor(*reinterpret_cast<QColor*>(_v)); break;
        case 190: _t->set_headScheme(*reinterpret_cast<QVariant*>(_v)); break;
        case 191: _t->set_staffGenClef(*reinterpret_cast<bool*>(_v)); break;
        case 192: _t->set_staffGenTimesig(*reinterpret_cast<bool*>(_v)); break;
        case 193: _t->set_staffGenKeysig(*reinterpret_cast<bool*>(_v)); break;
        case 194: _t->set_staffYoffset(*reinterpret_cast<QVariant*>(_v)); break;
        case 195: _t->set_bracketSpan(*reinterpret_cast<int*>(_v)); break;
        case 196: _t->set_bracketColumn(*reinterpret_cast<int*>(_v)); break;
        case 197: _t->set_inameLayoutPosition(*reinterpret_cast<int*>(_v)); break;
        case 198: _t->set_subStyle(*reinterpret_cast<QVariant*>(_v)); break;
        case 199: _t->set_fontFace(*reinterpret_cast<QVariant*>(_v)); break;
        case 200: _t->set_fontSize(*reinterpret_cast<qreal*>(_v)); break;
        case 201: _t->set_fontStyle(*reinterpret_cast<int*>(_v)); break;
        case 202: _t->set_lineSpacing(*reinterpret_cast<qreal*>(_v)); break;
        case 203: _t->set_frameType(*reinterpret_cast<int*>(_v)); break;
        case 204: _t->set_frameWidth(*reinterpret_cast<QVariant*>(_v)); break;
        case 205: _t->set_framePadding(*reinterpret_cast<QVariant*>(_v)); break;
        case 206: _t->set_frameRound(*reinterpret_cast<int*>(_v)); break;
        case 207: _t->set_frameFgColor(*reinterpret_cast<QColor*>(_v)); break;
        case 208: _t->set_frameBgColor(*reinterpret_cast<QColor*>(_v)); break;
        case 209: _t->set_sizeSpatiumDependent(*reinterpret_cast<bool*>(_v)); break;
        case 210: _t->set_textSizeSpatiumDependent(*reinterpret_cast<bool*>(_v)); break;
        case 211: _t->set_musicalSymbolsScale(*reinterpret_cast<qreal*>(_v)); break;
        case 212: _t->set_align(*reinterpret_cast<QVariant*>(_v)); break;
        case 213: _t->set_textScriptAlign(*reinterpret_cast<int*>(_v)); break;
        case 214: _t->set_systemFlag(*reinterpret_cast<bool*>(_v)); break;
        case 215: _t->set_beginText(*reinterpret_cast<QVariant*>(_v)); break;
        case 216: _t->set_beginTextAlign(*reinterpret_cast<QVariant*>(_v)); break;
        case 217: _t->set_beginTextPosition(*reinterpret_cast<QVariant*>(_v)); break;
        case 218: _t->set_beginTextPlace(*reinterpret_cast<QVariant*>(_v)); break;
        case 219: _t->set_beginHookType(*reinterpret_cast<QVariant*>(_v)); break;
        case 220: _t->set_beginHookHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 221: _t->set_beginLineArrowHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 222: _t->set_beginLineArrowWidth(*reinterpret_cast<QVariant*>(_v)); break;
        case 223: _t->set_beginFilledArrowHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 224: _t->set_beginFilledArrowWidth(*reinterpret_cast<QVariant*>(_v)); break;
        case 225: _t->set_beginFontFace(*reinterpret_cast<QVariant*>(_v)); break;
        case 226: _t->set_beginFontSize(*reinterpret_cast<qreal*>(_v)); break;
        case 227: _t->set_beginFontStyle(*reinterpret_cast<int*>(_v)); break;
        case 228: _t->set_beginTextOffset(*reinterpret_cast<QPointF*>(_v)); break;
        case 229: _t->set_gapBetweenTextAndLine(*reinterpret_cast<QVariant*>(_v)); break;
        case 230: _t->set_continueText(*reinterpret_cast<QVariant*>(_v)); break;
        case 231: _t->set_continueTextAlign(*reinterpret_cast<QVariant*>(_v)); break;
        case 232: _t->set_continueTextPosition(*reinterpret_cast<QVariant*>(_v)); break;
        case 233: _t->set_continueTextPlace(*reinterpret_cast<QVariant*>(_v)); break;
        case 234: _t->set_continueFontFace(*reinterpret_cast<QVariant*>(_v)); break;
        case 235: _t->set_continueFontSize(*reinterpret_cast<qreal*>(_v)); break;
        case 236: _t->set_continueFontStyle(*reinterpret_cast<int*>(_v)); break;
        case 237: _t->set_continueTextOffset(*reinterpret_cast<QPointF*>(_v)); break;
        case 238: _t->set_endText(*reinterpret_cast<QVariant*>(_v)); break;
        case 239: _t->set_endTextAlign(*reinterpret_cast<QVariant*>(_v)); break;
        case 240: _t->set_endTextPosition(*reinterpret_cast<QVariant*>(_v)); break;
        case 241: _t->set_endTextPlace(*reinterpret_cast<QVariant*>(_v)); break;
        case 242: _t->set_endHookType(*reinterpret_cast<QVariant*>(_v)); break;
        case 243: _t->set_endHookHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 244: _t->set_endLineArrowHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 245: _t->set_endLineArrowWidth(*reinterpret_cast<QVariant*>(_v)); break;
        case 246: _t->set_endFilledArrowHeight(*reinterpret_cast<QVariant*>(_v)); break;
        case 247: _t->set_endFilledArrowWidth(*reinterpret_cast<QVariant*>(_v)); break;
        case 248: _t->set_endFontFace(*reinterpret_cast<QVariant*>(_v)); break;
        case 249: _t->set_endFontSize(*reinterpret_cast<qreal*>(_v)); break;
        case 250: _t->set_endFontStyle(*reinterpret_cast<int*>(_v)); break;
        case 251: _t->set_endTextOffset(*reinterpret_cast<QPointF*>(_v)); break;
        case 252: _t->set_notelinePlacement(*reinterpret_cast<QVariant*>(_v)); break;
        case 253: _t->set_avoidBarLines(*reinterpret_cast<bool*>(_v)); break;
        case 254: _t->set_dynamicsSize(*reinterpret_cast<qreal*>(_v)); break;
        case 255: _t->set_centerOnNotehead(*reinterpret_cast<bool*>(_v)); break;
        case 256: _t->set_anchorToEndOfPrevious(*reinterpret_cast<bool*>(_v)); break;
        case 257: _t->set_snapToDynamics(*reinterpret_cast<bool*>(_v)); break;
        case 258: _t->set_snapBefore(*reinterpret_cast<bool*>(_v)); break;
        case 259: _t->set_snapAfter(*reinterpret_cast<bool*>(_v)); break;
        case 260: _t->set_voiceAssignment(*reinterpret_cast<QVariant*>(_v)); break;
        case 261: _t->set_centerBetweenStaves(*reinterpret_cast<QVariant*>(_v)); break;
        case 262: _t->set_posAbove(*reinterpret_cast<QVariant*>(_v)); break;
        case 263: _t->set_locationStaves(*reinterpret_cast<int*>(_v)); break;
        case 264: _t->set_locationVoices(*reinterpret_cast<int*>(_v)); break;
        case 265: _t->set_locationMeasures(*reinterpret_cast<int*>(_v)); break;
        case 266: _t->set_locationFractions(*reinterpret_cast<QVariant*>(_v)); break;
        case 267: _t->set_locationGrace(*reinterpret_cast<int*>(_v)); break;
        case 268: _t->set_locationNote(*reinterpret_cast<int*>(_v)); break;
        case 269: _t->set_voice(*reinterpret_cast<int*>(_v)); break;
        case 270: _t->set_position(*reinterpret_cast<QVariant*>(_v)); break;
        case 271: _t->set_concertClefType(*reinterpret_cast<QVariant*>(_v)); break;
        case 272: _t->set_transposingClefType(*reinterpret_cast<QVariant*>(_v)); break;
        case 273: _t->set_clefToBarlinePos(*reinterpret_cast<int*>(_v)); break;
        case 274: _t->set_isHeader(*reinterpret_cast<bool*>(_v)); break;
        case 275: _t->set_concertKey(*reinterpret_cast<int*>(_v)); break;
        case 276: _t->set_actualKey(*reinterpret_cast<int*>(_v)); break;
        case 277: _t->set_action(*reinterpret_cast<QVariant*>(_v)); break;
        case 278: _t->set_minDistance(*reinterpret_cast<QVariant*>(_v)); break;
        case 279: _t->set_arpeggioType(*reinterpret_cast<int*>(_v)); break;
        case 280: _t->set_chordLineType(*reinterpret_cast<int*>(_v)); break;
        case 281: _t->set_chordLineStraight(*reinterpret_cast<bool*>(_v)); break;
        case 282: _t->set_chordLineWavy(*reinterpret_cast<bool*>(_v)); break;
        case 283: _t->set_tremoloType(*reinterpret_cast<int*>(_v)); break;
        case 284: _t->set_tremoloStrokeStyle(*reinterpret_cast<int*>(_v)); break;
        case 285: _t->set_harmonyType(*reinterpret_cast<int*>(_v)); break;
        case 286: _t->set_arpeggioSpan(*reinterpret_cast<int*>(_v)); break;
        case 287: _t->set_bracketHookLength(*reinterpret_cast<QVariant*>(_v)); break;
        case 288: _t->set_bracketHookPos(*reinterpret_cast<int*>(_v)); break;
        case 289: _t->set_bracketRightSide(*reinterpret_cast<bool*>(_v)); break;
        case 290: _t->set_bendType(*reinterpret_cast<int*>(_v)); break;
        case 291: _t->set_bendCurve(*reinterpret_cast<QVariant*>(_v)); break;
        case 292: _t->set_bendVertexOffset(*reinterpret_cast<QPointF*>(_v)); break;
        case 293: _t->set_bendShowHoldLine(*reinterpret_cast<int*>(_v)); break;
        case 294: _t->set_bendStartTimeFactor(*reinterpret_cast<qreal*>(_v)); break;
        case 295: _t->set_bendEndTimeFactor(*reinterpret_cast<qreal*>(_v)); break;
        case 296: _t->set_guitarDiveTabPos(*reinterpret_cast<int*>(_v)); break;
        case 297: _t->set_guitarBendAmount(*reinterpret_cast<int*>(_v)); break;
        case 298: _t->set_vibratoLineType(*reinterpret_cast<int*>(_v)); break;
        case 299: _t->set_guitarDiveIsSlack(*reinterpret_cast<bool*>(_v)); break;
        case 300: _t->set_tremoloBarType(*reinterpret_cast<int*>(_v)); break;
        case 301: _t->set_tremoloBarCurve(*reinterpret_cast<QVariant*>(_v)); break;
        case 302: _t->set_startWithLongNames(*reinterpret_cast<bool*>(_v)); break;
        case 303: _t->set_startWithMeasureOne(*reinterpret_cast<bool*>(_v)); break;
        case 304: _t->set_firstSystemIndentation(*reinterpret_cast<bool*>(_v)); break;
        case 305: _t->set_path(*reinterpret_cast<QVariant*>(_v)); break;
        case 306: _t->set_preferSharpFlat(*reinterpret_cast<int*>(_v)); break;
        case 307: _t->set_playTechType(*reinterpret_cast<QVariant*>(_v)); break;
        case 308: _t->set_tempoChangeType(*reinterpret_cast<QVariant*>(_v)); break;
        case 309: _t->set_tempoEasingMethod(*reinterpret_cast<QVariant*>(_v)); break;
        case 310: _t->set_tempoChangeFactor(*reinterpret_cast<qreal*>(_v)); break;
        case 311: _t->set_isDiagram(*reinterpret_cast<bool*>(_v)); break;
        case 312: _t->set_active(*reinterpret_cast<bool*>(_v)); break;
        case 313: _t->set_fretPosition(*reinterpret_cast<int*>(_v)); break;
        case 314: _t->set_ignoredStrings(*reinterpret_cast<QVariant*>(_v)); break;
        case 315: _t->set_generateText(*reinterpret_cast<bool*>(_v)); break;
        case 316: _t->set_transposeMode(*reinterpret_cast<int*>(_v)); break;
        case 317: _t->set_tiePlacement(*reinterpret_cast<QVariant*>(_v)); break;
        case 318: _t->set_minLength(*reinterpret_cast<QVariant*>(_v)); break;
        case 319: _t->set_partialSpannerDirection(*reinterpret_cast<QVariant*>(_v)); break;
        case 320: _t->set_positionLinkedToMaster(*reinterpret_cast<bool*>(_v)); break;
        case 321: _t->set_appearanceLinkedToMaster(*reinterpret_cast<bool*>(_v)); break;
        case 322: _t->set_textLinkedToMaster(*reinterpret_cast<bool*>(_v)); break;
        case 323: _t->set_excludeFromParts(*reinterpret_cast<bool*>(_v)); break;
        case 324: _t->set_stringsCount(*reinterpret_cast<int*>(_v)); break;
        case 325: _t->set_preset(*reinterpret_cast<QVariant*>(_v)); break;
        case 326: _t->set_visibleStrings(*reinterpret_cast<QVariant*>(_v)); break;
        case 327: _t->set_scoreFont(*reinterpret_cast<QVariant*>(_v)); break;
        case 328: _t->set_symbolsSize(*reinterpret_cast<qreal*>(_v)); break;
        case 329: _t->set_symbolAngle(*reinterpret_cast<qreal*>(_v)); break;
        case 330: _t->set_applyToAllStaves(*reinterpret_cast<bool*>(_v)); break;
        case 331: _t->set_isCourtesy(*reinterpret_cast<bool*>(_v)); break;
        case 332: _t->set_excludeVerticalAlign(*reinterpret_cast<bool*>(_v)); break;
        case 333: _t->set_playCountTextSetting(*reinterpret_cast<QVariant*>(_v)); break;
        case 334: _t->set_playCountText(*reinterpret_cast<QVariant*>(_v)); break;
        case 335: _t->set_alignWithOtherRests(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 20: _t->reset_subType(); break;
        case 23: _t->reset_color(); break;
        case 24: _t->reset_visible(); break;
        case 25: _t->reset_z(); break;
        case 26: _t->reset_small(); break;
        case 27: _t->reset_hideWhenEmpty(); break;
        case 28: _t->reset_hideStavesWhenIndividuallyEmpty(); break;
        case 29: _t->reset_showCourtesy(); break;
        case 30: _t->reset_keysig_mode(); break;
        case 31: _t->reset_lineType(); break;
        case 32: _t->reset_headType(); break;
        case 33: _t->reset_headGroup(); break;
        case 34: _t->reset_articulationAnchor(); break;
        case 35: _t->reset_direction(); break;
        case 36: _t->reset_horizontalDirection(); break;
        case 37: _t->reset_stemDirection(); break;
        case 38: _t->reset_slurDirection(); break;
        case 39: _t->reset_mirrorHead(); break;
        case 40: _t->reset_hasParentheses(); break;
        case 41: _t->reset_pause(); break;
        case 42: _t->reset_barlineType(); break;
        case 43: _t->reset_barlineSpan(); break;
        case 44: _t->reset_barlineSpanFrom(); break;
        case 45: _t->reset_barlineSpanTo(); break;
        case 46: _t->reset_barlineShowTips(); break;
        case 47: _t->reset_offset(); break;
        case 48: _t->reset_ghost(); break;
        case 49: _t->reset_play(); break;
        case 50: _t->reset_boxHeight(); break;
        case 51: _t->reset_boxWidth(); break;
        case 52: _t->reset_boxAutoSize(); break;
        case 53: _t->reset_topGap(); break;
        case 54: _t->reset_bottomGap(); break;
        case 55: _t->reset_leftMargin(); break;
        case 56: _t->reset_rightMargin(); break;
        case 57: _t->reset_topMargin(); break;
        case 58: _t->reset_bottomMargin(); break;
        case 59: _t->reset_paddingToNotationAbove(); break;
        case 60: _t->reset_paddingToNotationBelow(); break;
        case 61: _t->reset_layoutBreakType(); break;
        case 62: _t->reset_autoscale(); break;
        case 63: _t->reset_size(); break;
        case 64: _t->reset_imageHeight(); break;
        case 65: _t->reset_imageWidth(); break;
        case 66: _t->reset_imageFramed(); break;
        case 67: _t->reset_fretFrameTextScale(); break;
        case 68: _t->reset_fretFrameDiagramScale(); break;
        case 69: _t->reset_fretFrameColumnGap(); break;
        case 70: _t->reset_fretFrameRowGap(); break;
        case 71: _t->reset_fretFrameChordPerRow(); break;
        case 72: _t->reset_fretFrameHAlign(); break;
        case 73: _t->reset_fretFrameDiagramsOrder(); break;
        case 74: _t->reset_scale(); break;
        case 75: _t->reset_lockAspectRatio(); break;
        case 76: _t->reset_sizeIsSpatium(); break;
        case 77: _t->reset_text(); break;
        case 78: _t->reset_htmlText(); break;
        case 79: _t->reset_userModified(); break;
        case 80: _t->reset_beamPos(); break;
        case 81: _t->reset_beamNoSlope(); break;
        case 82: _t->reset_crossStaffMove(); break;
        case 83: _t->reset_userLen(); break;
        case 84: _t->reset_space(); break;
        case 85: _t->reset_tempo(); break;
        case 86: _t->reset_tempoFollowText(); break;
        case 87: _t->reset_tempoAlignRightOfRehearsalMark(); break;
        case 88: _t->reset_accidentalBracket(); break;
        case 89: _t->reset_accidentalType(); break;
        case 90: _t->reset_stackingOrderOffset(); break;
        case 91: _t->reset_numeratorString(); break;
        case 92: _t->reset_denominatorString(); break;
        case 93: _t->reset_fbprefix(); break;
        case 94: _t->reset_fbdigit(); break;
        case 95: _t->reset_fbsuffix(); break;
        case 96: _t->reset_fbcontinuationline(); break;
        case 97: _t->reset_fbparenthesis1(); break;
        case 98: _t->reset_fbparenthesis2(); break;
        case 99: _t->reset_fbparenthesis3(); break;
        case 100: _t->reset_fbparenthesis4(); break;
        case 101: _t->reset_fbparenthesis5(); break;
        case 102: _t->reset_ottavaType(); break;
        case 103: _t->reset_numbersOnly(); break;
        case 104: _t->reset_trillType(); break;
        case 105: _t->reset_vibratoType(); break;
        case 106: _t->reset_hairpinCircledTip(); break;
        case 107: _t->reset_hairpinType(); break;
        case 108: _t->reset_hairpinHeight(); break;
        case 109: _t->reset_hairpinContHeight(); break;
        case 110: _t->reset_veloChange(); break;
        case 111: _t->reset_veloChangeMethod(); break;
        case 112: _t->reset_veloChangeSpeed(); break;
        case 113: _t->reset_dynamicType(); break;
        case 114: _t->reset_singleNoteDynamics(); break;
        case 115: _t->reset_changeMethod(); break;
        case 116: _t->reset_placement(); break;
        case 117: _t->reset_hPlacement(); break;
        case 118: _t->reset_mmRestRangeBracketType(); break;
        case 119: _t->reset_velocity(); break;
        case 120: _t->reset_jumpTo(); break;
        case 121: _t->reset_playUntil(); break;
        case 122: _t->reset_continueAt(); break;
        case 123: _t->reset_label(); break;
        case 124: _t->reset_markerType(); break;
        case 125: _t->reset_musicSymbolSize(); break;
        case 126: _t->reset_markerCenterOnSymbol(); break;
        case 127: _t->reset_arpUserLen1(); break;
        case 128: _t->reset_arpUserLen2(); break;
        case 129: _t->reset_glissType(); break;
        case 130: _t->reset_glissText(); break;
        case 131: _t->reset_glissShowText(); break;
        case 132: _t->reset_glissandoStyle(); break;
        case 133: _t->reset_glissEaseIn(); break;
        case 134: _t->reset_glissEaseOut(); break;
        case 135: _t->reset_diagonal(); break;
        case 136: _t->reset_groups(); break;
        case 137: _t->reset_lineStyle(); break;
        case 138: _t->reset_lineColor(); break;
        case 139: _t->reset_lineWidth(); break;
        case 140: _t->reset_timeStretch(); break;
        case 141: _t->reset_ornamentStyle(); break;
        case 142: _t->reset_intervalAbove(); break;
        case 143: _t->reset_intervalBelow(); break;
        case 144: _t->reset_ornamentShowAccidental(); break;
        case 145: _t->reset_ornamentShowCueNote(); break;
        case 146: _t->reset_startOnUpperNote(); break;
        case 147: _t->reset_timesig(); break;
        case 148: _t->reset_timesigStretch(); break;
        case 149: _t->reset_timesigType(); break;
        case 150: _t->reset_mmRestNumberPos(); break;
        case 151: _t->reset_mmRestNumberOffset(); break;
        case 152: _t->reset_mmRestNumberVisible(); break;
        case 153: _t->reset_measureRepeatNumberPos(); break;
        case 154: _t->reset_verse(); break;
        case 155: _t->reset_volta_ending(); break;
        case 156: _t->reset_lineVisible(); break;
        case 157: _t->reset_mag(); break;
        case 158: _t->reset_useDrumset(); break;
        case 159: _t->reset_role(); break;
        case 160: _t->reset_track(); break;
        case 161: _t->reset_fretStrings(); break;
        case 162: _t->reset_fretFrets(); break;
        case 163: _t->reset_showNut(); break;
        case 164: _t->reset_fretOffset(); break;
        case 165: _t->reset_fretNumPos(); break;
        case 166: _t->reset_orientation(); break;
        case 167: _t->reset_fretShowFingering(); break;
        case 168: _t->reset_fretFingering(); break;
        case 169: _t->reset_harmonyVoiceLiteral(); break;
        case 170: _t->reset_harmonyVoicing(); break;
        case 171: _t->reset_harmonyDuration(); break;
        case 172: _t->reset_harmonyBassScale(); break;
        case 173: _t->reset_harmonyDoNotStackModifiers(); break;
        case 174: _t->reset_systemBracket(); break;
        case 175: _t->reset_gap(); break;
        case 176: _t->reset_autoplace(); break;
        case 177: _t->reset_dashLineLen(); break;
        case 178: _t->reset_dashGapLen(); break;
        case 179: _t->reset_symbol(); break;
        case 180: _t->reset_playRepeats(); break;
        case 181: _t->reset_createSystemHeader(); break;
        case 182: _t->reset_staffLines(); break;
        case 183: _t->reset_lineDistance(); break;
        case 184: _t->reset_stepOffset(); break;
        case 185: _t->reset_staffShowBarlines(); break;
        case 186: _t->reset_staffShowLedgerlines(); break;
        case 187: _t->reset_staffStemless(); break;
        case 188: _t->reset_staffInvisible(); break;
        case 189: _t->reset_staffColor(); break;
        case 190: _t->reset_headScheme(); break;
        case 191: _t->reset_staffGenClef(); break;
        case 192: _t->reset_staffGenTimesig(); break;
        case 193: _t->reset_staffGenKeysig(); break;
        case 194: _t->reset_staffYoffset(); break;
        case 195: _t->reset_bracketSpan(); break;
        case 196: _t->reset_bracketColumn(); break;
        case 197: _t->reset_inameLayoutPosition(); break;
        case 198: _t->reset_subStyle(); break;
        case 199: _t->reset_fontFace(); break;
        case 200: _t->reset_fontSize(); break;
        case 201: _t->reset_fontStyle(); break;
        case 202: _t->reset_lineSpacing(); break;
        case 203: _t->reset_frameType(); break;
        case 204: _t->reset_frameWidth(); break;
        case 205: _t->reset_framePadding(); break;
        case 206: _t->reset_frameRound(); break;
        case 207: _t->reset_frameFgColor(); break;
        case 208: _t->reset_frameBgColor(); break;
        case 209: _t->reset_sizeSpatiumDependent(); break;
        case 210: _t->reset_textSizeSpatiumDependent(); break;
        case 211: _t->reset_musicalSymbolsScale(); break;
        case 212: _t->reset_align(); break;
        case 213: _t->reset_textScriptAlign(); break;
        case 214: _t->reset_systemFlag(); break;
        case 215: _t->reset_beginText(); break;
        case 216: _t->reset_beginTextAlign(); break;
        case 217: _t->reset_beginTextPosition(); break;
        case 218: _t->reset_beginTextPlace(); break;
        case 219: _t->reset_beginHookType(); break;
        case 220: _t->reset_beginHookHeight(); break;
        case 221: _t->reset_beginLineArrowHeight(); break;
        case 222: _t->reset_beginLineArrowWidth(); break;
        case 223: _t->reset_beginFilledArrowHeight(); break;
        case 224: _t->reset_beginFilledArrowWidth(); break;
        case 225: _t->reset_beginFontFace(); break;
        case 226: _t->reset_beginFontSize(); break;
        case 227: _t->reset_beginFontStyle(); break;
        case 228: _t->reset_beginTextOffset(); break;
        case 229: _t->reset_gapBetweenTextAndLine(); break;
        case 230: _t->reset_continueText(); break;
        case 231: _t->reset_continueTextAlign(); break;
        case 232: _t->reset_continueTextPosition(); break;
        case 233: _t->reset_continueTextPlace(); break;
        case 234: _t->reset_continueFontFace(); break;
        case 235: _t->reset_continueFontSize(); break;
        case 236: _t->reset_continueFontStyle(); break;
        case 237: _t->reset_continueTextOffset(); break;
        case 238: _t->reset_endText(); break;
        case 239: _t->reset_endTextAlign(); break;
        case 240: _t->reset_endTextPosition(); break;
        case 241: _t->reset_endTextPlace(); break;
        case 242: _t->reset_endHookType(); break;
        case 243: _t->reset_endHookHeight(); break;
        case 244: _t->reset_endLineArrowHeight(); break;
        case 245: _t->reset_endLineArrowWidth(); break;
        case 246: _t->reset_endFilledArrowHeight(); break;
        case 247: _t->reset_endFilledArrowWidth(); break;
        case 248: _t->reset_endFontFace(); break;
        case 249: _t->reset_endFontSize(); break;
        case 250: _t->reset_endFontStyle(); break;
        case 251: _t->reset_endTextOffset(); break;
        case 252: _t->reset_notelinePlacement(); break;
        case 253: _t->reset_avoidBarLines(); break;
        case 254: _t->reset_dynamicsSize(); break;
        case 255: _t->reset_centerOnNotehead(); break;
        case 256: _t->reset_anchorToEndOfPrevious(); break;
        case 257: _t->reset_snapToDynamics(); break;
        case 258: _t->reset_snapBefore(); break;
        case 259: _t->reset_snapAfter(); break;
        case 260: _t->reset_voiceAssignment(); break;
        case 261: _t->reset_centerBetweenStaves(); break;
        case 262: _t->reset_posAbove(); break;
        case 263: _t->reset_locationStaves(); break;
        case 264: _t->reset_locationVoices(); break;
        case 265: _t->reset_locationMeasures(); break;
        case 266: _t->reset_locationFractions(); break;
        case 267: _t->reset_locationGrace(); break;
        case 268: _t->reset_locationNote(); break;
        case 269: _t->reset_voice(); break;
        case 270: _t->reset_position(); break;
        case 271: _t->reset_concertClefType(); break;
        case 272: _t->reset_transposingClefType(); break;
        case 273: _t->reset_clefToBarlinePos(); break;
        case 274: _t->reset_isHeader(); break;
        case 275: _t->reset_concertKey(); break;
        case 276: _t->reset_actualKey(); break;
        case 277: _t->reset_action(); break;
        case 278: _t->reset_minDistance(); break;
        case 279: _t->reset_arpeggioType(); break;
        case 280: _t->reset_chordLineType(); break;
        case 281: _t->reset_chordLineStraight(); break;
        case 282: _t->reset_chordLineWavy(); break;
        case 283: _t->reset_tremoloType(); break;
        case 284: _t->reset_tremoloStrokeStyle(); break;
        case 285: _t->reset_harmonyType(); break;
        case 286: _t->reset_arpeggioSpan(); break;
        case 287: _t->reset_bracketHookLength(); break;
        case 288: _t->reset_bracketHookPos(); break;
        case 289: _t->reset_bracketRightSide(); break;
        case 290: _t->reset_bendType(); break;
        case 291: _t->reset_bendCurve(); break;
        case 292: _t->reset_bendVertexOffset(); break;
        case 293: _t->reset_bendShowHoldLine(); break;
        case 294: _t->reset_bendStartTimeFactor(); break;
        case 295: _t->reset_bendEndTimeFactor(); break;
        case 296: _t->reset_guitarDiveTabPos(); break;
        case 297: _t->reset_guitarBendAmount(); break;
        case 298: _t->reset_vibratoLineType(); break;
        case 299: _t->reset_guitarDiveIsSlack(); break;
        case 300: _t->reset_tremoloBarType(); break;
        case 301: _t->reset_tremoloBarCurve(); break;
        case 302: _t->reset_startWithLongNames(); break;
        case 303: _t->reset_startWithMeasureOne(); break;
        case 304: _t->reset_firstSystemIndentation(); break;
        case 305: _t->reset_path(); break;
        case 306: _t->reset_preferSharpFlat(); break;
        case 307: _t->reset_playTechType(); break;
        case 308: _t->reset_tempoChangeType(); break;
        case 309: _t->reset_tempoEasingMethod(); break;
        case 310: _t->reset_tempoChangeFactor(); break;
        case 311: _t->reset_isDiagram(); break;
        case 312: _t->reset_active(); break;
        case 313: _t->reset_fretPosition(); break;
        case 314: _t->reset_ignoredStrings(); break;
        case 315: _t->reset_generateText(); break;
        case 316: _t->reset_transposeMode(); break;
        case 317: _t->reset_tiePlacement(); break;
        case 318: _t->reset_minLength(); break;
        case 319: _t->reset_partialSpannerDirection(); break;
        case 320: _t->reset_positionLinkedToMaster(); break;
        case 321: _t->reset_appearanceLinkedToMaster(); break;
        case 322: _t->reset_textLinkedToMaster(); break;
        case 323: _t->reset_excludeFromParts(); break;
        case 324: _t->reset_stringsCount(); break;
        case 325: _t->reset_preset(); break;
        case 326: _t->reset_visibleStrings(); break;
        case 327: _t->reset_scoreFont(); break;
        case 328: _t->reset_symbolsSize(); break;
        case 329: _t->reset_symbolAngle(); break;
        case 330: _t->reset_applyToAllStaves(); break;
        case 331: _t->reset_isCourtesy(); break;
        case 332: _t->reset_excludeVerticalAlign(); break;
        case 333: _t->reset_playCountTextSetting(); break;
        case 334: _t->reset_playCountText(); break;
        case 335: _t->reset_alignWithOtherRests(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::EngravingItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::EngravingItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv113EngravingItemE_t>.strings))
        return static_cast<void*>(this);
    return apiv1::ScoreElement::qt_metacast(_clname);
}

int mu::engraving::apiv1::EngravingItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = apiv1::ScoreElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 338;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv14NoteE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Note::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv14NoteE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Note",
        "createPlayEvent",
        "apiv1::PlayEvent*",
        "",
        "add",
        "apiv1::EngravingItem*",
        "wrapped",
        "remove",
        "accidental",
        "accidentalType",
        "mu::engraving::AccidentalType",
        "dots",
        "QQmlListProperty<apiv1::EngravingItem>",
        "elements",
        "playEvents",
        "QQmlListProperty<apiv1::PlayEvent>",
        "spannerForward",
        "spannerBack",
        "tieBack",
        "apiv1::Tie*",
        "tieForward",
        "firstTiedNote",
        "apiv1::Note*",
        "lastTiedNote",
        "noteType",
        "mu::engraving::NoteType",
        "pitch",
        "tpc1",
        "tpc2",
        "tpc",
        "dotPosition",
        "QVariant",
        "veloType",
        "userVelocity",
        "tuning",
        "line",
        "fixed",
        "fixedLine",
        "fret",
        "string",
        "dead",
        "isTrillCueNote"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'createPlayEvent'
        QtMocHelpers::MethodData<apiv1::PlayEvent *()>(1, 3, QMC::AccessPublic, 0x80000000 | 2),
        // Method 'add'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(4, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Method 'remove'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(7, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'accidental'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(8, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'accidentalType'
        QtMocHelpers::PropertyData<mu::engraving::AccidentalType>(9, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'dots'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'elements'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(13, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'playEvents'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::PlayEvent>>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'spannerForward'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(16, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'spannerBack'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(17, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'tieBack'
        QtMocHelpers::PropertyData<apiv1::Tie*>(18, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'tieForward'
        QtMocHelpers::PropertyData<apiv1::Tie*>(20, 0x80000000 | 19, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'firstTiedNote'
        QtMocHelpers::PropertyData<apiv1::Note*>(21, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'lastTiedNote'
        QtMocHelpers::PropertyData<apiv1::Note*>(23, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'noteType'
        QtMocHelpers::PropertyData<mu::engraving::NoteType>(24, 0x80000000 | 25, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'pitch'
        QtMocHelpers::PropertyData<int>(26, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tpc1'
        QtMocHelpers::PropertyData<int>(27, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tpc2'
        QtMocHelpers::PropertyData<int>(28, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tpc'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'dotPosition'
        QtMocHelpers::PropertyData<QVariant>(30, 0x80000000 | 31, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'veloType'
        QtMocHelpers::PropertyData<QVariant>(32, 0x80000000 | 31, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'userVelocity'
        QtMocHelpers::PropertyData<int>(33, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'tuning'
        QtMocHelpers::PropertyData<qreal>(34, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'line'
        QtMocHelpers::PropertyData<int>(35, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fixed'
        QtMocHelpers::PropertyData<bool>(36, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fixedLine'
        QtMocHelpers::PropertyData<int>(37, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'fret'
        QtMocHelpers::PropertyData<int>(38, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'string'
        QtMocHelpers::PropertyData<int>(39, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'dead'
        QtMocHelpers::PropertyData<bool>(40, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'isTrillCueNote'
        QtMocHelpers::PropertyData<bool>(41, QMetaType::Bool, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Note, qt_meta_tag_ZN2mu9engraving5apiv14NoteE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Note::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14NoteE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14NoteE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv14NoteE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Note::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Note *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::PlayEvent* _r = _t->createPlayEvent();
            if (_a[0]) *reinterpret_cast<apiv1::PlayEvent**>(_a[0]) = std::move(_r); }  break;
        case 1: _t->add((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        case 2: _t->remove((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< mu::engraving::AccidentalType >(); break;
        case 11:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< mu::engraving::NoteType >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->accidental(); break;
        case 1: *reinterpret_cast<mu::engraving::AccidentalType*>(_v) = _t->accidentalType(); break;
        case 2: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->dots(); break;
        case 3: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->elements(); break;
        case 4: *reinterpret_cast<QQmlListProperty<apiv1::PlayEvent>*>(_v) = _t->playEvents(); break;
        case 5: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->spannerFor(); break;
        case 6: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->spannerBack(); break;
        case 7: *reinterpret_cast<apiv1::Tie**>(_v) = _t->tieBack(); break;
        case 8: *reinterpret_cast<apiv1::Tie**>(_v) = _t->tieForward(); break;
        case 9: *reinterpret_cast<apiv1::Note**>(_v) = _t->firstTiedNote(); break;
        case 10: *reinterpret_cast<apiv1::Note**>(_v) = _t->lastTiedNote(); break;
        case 11: *reinterpret_cast<mu::engraving::NoteType*>(_v) = _t->noteType(); break;
        case 12: *reinterpret_cast<int*>(_v) = _t->get_pitch(); break;
        case 13: *reinterpret_cast<int*>(_v) = _t->get_tpc1(); break;
        case 14: *reinterpret_cast<int*>(_v) = _t->get_tpc2(); break;
        case 15: *reinterpret_cast<int*>(_v) = _t->tpc(); break;
        case 16: *reinterpret_cast<QVariant*>(_v) = _t->get_dotPosition(); break;
        case 17: *reinterpret_cast<QVariant*>(_v) = _t->get_veloType(); break;
        case 18: *reinterpret_cast<int*>(_v) = _t->get_userVelocity(); break;
        case 19: *reinterpret_cast<qreal*>(_v) = _t->get_tuning(); break;
        case 20: *reinterpret_cast<int*>(_v) = _t->get_line(); break;
        case 21: *reinterpret_cast<bool*>(_v) = _t->get_fixed(); break;
        case 22: *reinterpret_cast<int*>(_v) = _t->get_fixedLine(); break;
        case 23: *reinterpret_cast<int*>(_v) = _t->get_fret(); break;
        case 24: *reinterpret_cast<int*>(_v) = _t->get_string(); break;
        case 25: *reinterpret_cast<bool*>(_v) = _t->get_dead(); break;
        case 26: *reinterpret_cast<bool*>(_v) = _t->isTrillCueNote(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setAccidentalType(*reinterpret_cast<mu::engraving::AccidentalType*>(_v)); break;
        case 12: _t->set_pitch(*reinterpret_cast<int*>(_v)); break;
        case 13: _t->set_tpc1(*reinterpret_cast<int*>(_v)); break;
        case 14: _t->set_tpc2(*reinterpret_cast<int*>(_v)); break;
        case 15: _t->setTpc(*reinterpret_cast<int*>(_v)); break;
        case 16: _t->set_dotPosition(*reinterpret_cast<QVariant*>(_v)); break;
        case 17: _t->set_veloType(*reinterpret_cast<QVariant*>(_v)); break;
        case 18: _t->set_userVelocity(*reinterpret_cast<int*>(_v)); break;
        case 19: _t->set_tuning(*reinterpret_cast<qreal*>(_v)); break;
        case 20: _t->set_line(*reinterpret_cast<int*>(_v)); break;
        case 21: _t->set_fixed(*reinterpret_cast<bool*>(_v)); break;
        case 22: _t->set_fixedLine(*reinterpret_cast<int*>(_v)); break;
        case 23: _t->set_fret(*reinterpret_cast<int*>(_v)); break;
        case 24: _t->set_string(*reinterpret_cast<int*>(_v)); break;
        case 25: _t->set_dead(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 12: _t->reset_pitch(); break;
        case 13: _t->reset_tpc1(); break;
        case 14: _t->reset_tpc2(); break;
        case 16: _t->reset_dotPosition(); break;
        case 17: _t->reset_veloType(); break;
        case 18: _t->reset_userVelocity(); break;
        case 19: _t->reset_tuning(); break;
        case 20: _t->reset_line(); break;
        case 21: _t->reset_fixed(); break;
        case 22: _t->reset_fixedLine(); break;
        case 23: _t->reset_fret(); break;
        case 24: _t->reset_string(); break;
        case 25: _t->reset_dead(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Note::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Note::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14NoteE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::Note::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv115DurationElementE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::DurationElement::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv115DurationElementE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::DurationElement",
        "duration",
        "apiv1::Fraction*",
        "globalDuration",
        "actualDuration",
        "tuplet",
        "apiv1::Tuplet*",
        "topTuplet",
        "measure",
        "apiv1::Measure*"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'duration'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag),
        // property 'globalDuration'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'actualDuration'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(4, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'tuplet'
        QtMocHelpers::PropertyData<apiv1::Tuplet*>(5, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'topTuplet'
        QtMocHelpers::PropertyData<apiv1::Tuplet*>(7, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'measure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DurationElement, qt_meta_tag_ZN2mu9engraving5apiv115DurationElementE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::DurationElement::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv115DurationElementE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv115DurationElementE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv115DurationElementE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::DurationElement::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DurationElement *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->ticks(); break;
        case 1: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->globalDuration(); break;
        case 2: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->actualDuration(); break;
        case 3: *reinterpret_cast<apiv1::Tuplet**>(_v) = _t->parentTuplet(); break;
        case 4: *reinterpret_cast<apiv1::Tuplet**>(_v) = _t->topTuplet(); break;
        case 5: *reinterpret_cast<apiv1::Measure**>(_v) = _t->parentMeasure(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->changeCRlen(*reinterpret_cast<apiv1::Fraction**>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::DurationElement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::DurationElement::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv115DurationElementE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::DurationElement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv16TupletE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Tuplet::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv16TupletE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Tuplet",
        "numberType",
        "bracketType",
        "hasBracket",
        "actualNotes",
        "normalNotes",
        "p1",
        "QPointF",
        "p2",
        "defaultP1",
        "defaultP2",
        "elements",
        "QQmlListProperty<apiv1::EngravingItem>"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'numberType'
        QtMocHelpers::PropertyData<int>(1, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'bracketType'
        QtMocHelpers::PropertyData<int>(2, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'hasBracket'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'actualNotes'
        QtMocHelpers::PropertyData<int>(4, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'normalNotes'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'p1'
        QtMocHelpers::PropertyData<QPointF>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'p2'
        QtMocHelpers::PropertyData<QPointF>(8, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'defaultP1'
        QtMocHelpers::PropertyData<QPointF>(9, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'defaultP2'
        QtMocHelpers::PropertyData<QPointF>(10, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'elements'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Tuplet, qt_meta_tag_ZN2mu9engraving5apiv16TupletE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Tuplet::staticMetaObject = { {
    QMetaObject::SuperData::link<DurationElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16TupletE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16TupletE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv16TupletE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Tuplet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Tuplet *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->get_numberType(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->get_bracketType(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->hasBracket(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->get_actualNotes(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->get_normalNotes(); break;
        case 5: *reinterpret_cast<QPointF*>(_v) = _t->get_p1(); break;
        case 6: *reinterpret_cast<QPointF*>(_v) = _t->get_p2(); break;
        case 7: *reinterpret_cast<QPointF*>(_v) = _t->defaultP1(); break;
        case 8: *reinterpret_cast<QPointF*>(_v) = _t->defaultP2(); break;
        case 9: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->elements(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_numberType(*reinterpret_cast<int*>(_v)); break;
        case 1: _t->set_bracketType(*reinterpret_cast<int*>(_v)); break;
        case 5: _t->set_p1(*reinterpret_cast<QPointF*>(_v)); break;
        case 6: _t->set_p2(*reinterpret_cast<QPointF*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 0: _t->reset_numberType(); break;
        case 1: _t->reset_bracketType(); break;
        case 5: _t->reset_p1(); break;
        case 6: _t->reset_p2(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Tuplet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Tuplet::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16TupletE_t>.strings))
        return static_cast<void*>(this);
    return DurationElement::qt_metacast(_clname);
}

int mu::engraving::apiv1::Tuplet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DurationElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv19ChordRestE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::ChordRest::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv19ChordRestE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::ChordRest",
        "actualBeamMode",
        "",
        "beamRests",
        "lyrics",
        "QQmlListProperty<apiv1::Lyrics>",
        "beam",
        "apiv1::Beam*",
        "isFullMeasureRest",
        "staffMove",
        "durationTypeWithDots",
        "QVariant",
        "beamMode",
        "elements",
        "QQmlListProperty<apiv1::EngravingItem>"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'actualBeamMode'
        QtMocHelpers::MethodData<int(bool)>(1, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Bool, 3 },
        }}),
        // Method 'actualBeamMode'
        QtMocHelpers::MethodData<int()>(1, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Int),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'lyrics'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Lyrics>>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'beam'
        QtMocHelpers::PropertyData<apiv1::Beam*>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'isFullMeasureRest'
        QtMocHelpers::PropertyData<bool>(8, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'staffMove'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'durationTypeWithDots'
        QtMocHelpers::PropertyData<QVariant>(10, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'beamMode'
        QtMocHelpers::PropertyData<QVariant>(12, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'elements'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(13, 0x80000000 | 14, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChordRest, qt_meta_tag_ZN2mu9engraving5apiv19ChordRestE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::ChordRest::staticMetaObject = { {
    QMetaObject::SuperData::link<DurationElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19ChordRestE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19ChordRestE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv19ChordRestE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::ChordRest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChordRest *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = _t->actualBeamMode((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 1: { int _r = _t->actualBeamMode();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QQmlListProperty<apiv1::Lyrics>*>(_v) = _t->lyrics(); break;
        case 1: *reinterpret_cast<apiv1::Beam**>(_v) = _t->beam(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isFullMeasureRest(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->get_staffMove(); break;
        case 4: *reinterpret_cast<QVariant*>(_v) = _t->get_durationTypeWithDots(); break;
        case 5: *reinterpret_cast<QVariant*>(_v) = _t->get_beamMode(); break;
        case 6: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->elements(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->set_staffMove(*reinterpret_cast<int*>(_v)); break;
        case 4: _t->set_durationTypeWithDots(*reinterpret_cast<QVariant*>(_v)); break;
        case 5: _t->set_beamMode(*reinterpret_cast<QVariant*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 3: _t->reset_staffMove(); break;
        case 4: _t->reset_durationTypeWithDots(); break;
        case 5: _t->reset_beamMode(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::ChordRest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::ChordRest::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv19ChordRestE_t>.strings))
        return static_cast<void*>(this);
    return DurationElement::qt_metacast(_clname);
}

int mu::engraving::apiv1::ChordRest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DurationElement::qt_metacall(_c, _id, _a);
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
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv15ChordE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Chord::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv15ChordE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Chord",
        "add",
        "",
        "apiv1::EngravingItem*",
        "wrapped",
        "remove",
        "graceNotes",
        "QQmlListProperty<apiv1::Chord>",
        "graceNotesBefore",
        "graceNotesAfter",
        "notes",
        "QQmlListProperty<apiv1::Note>",
        "articulations",
        "QQmlListProperty<apiv1::EngravingItem>",
        "stem",
        "stemSlash",
        "hook",
        "noteType",
        "mu::engraving::NoteType",
        "playEventType",
        "mu::engraving::PlayEventType",
        "noStem",
        "QVariant",
        "showStemSlash",
        "combineVoice",
        "isTrillCueNote",
        "upNote",
        "apiv1::Note*",
        "downNote",
        "arpeggio",
        "spanArpeggio",
        "tremoloSingleChord",
        "tremoloTwoChord"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'add'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'remove'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'graceNotes'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Chord>>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'graceNotesBefore'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Chord>>(8, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'graceNotesAfter'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Chord>>(9, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'notes'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Note>>(10, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'articulations'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'stem'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(14, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'stemSlash'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(15, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'hook'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(16, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'noteType'
        QtMocHelpers::PropertyData<mu::engraving::NoteType>(17, 0x80000000 | 18, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'playEventType'
        QtMocHelpers::PropertyData<mu::engraving::PlayEventType>(19, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
        // property 'noStem'
        QtMocHelpers::PropertyData<QVariant>(21, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'showStemSlash'
        QtMocHelpers::PropertyData<bool>(23, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'combineVoice'
        QtMocHelpers::PropertyData<QVariant>(24, 0x80000000 | 22, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'isTrillCueNote'
        QtMocHelpers::PropertyData<bool>(25, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'upNote'
        QtMocHelpers::PropertyData<apiv1::Note*>(26, 0x80000000 | 27, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'downNote'
        QtMocHelpers::PropertyData<apiv1::Note*>(28, 0x80000000 | 27, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'arpeggio'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(29, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'spanArpeggio'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(30, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'tremoloSingleChord'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(31, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'tremoloTwoChord'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(32, 0x80000000 | 3, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Chord, qt_meta_tag_ZN2mu9engraving5apiv15ChordE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Chord::staticMetaObject = { {
    QMetaObject::SuperData::link<ChordRest::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15ChordE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15ChordE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv15ChordE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Chord::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Chord *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->add((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        case 1: _t->remove((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< mu::engraving::NoteType >(); break;
        case 9:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< mu::engraving::PlayEventType >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QQmlListProperty<apiv1::Chord>*>(_v) = _t->graceNotes(); break;
        case 1: *reinterpret_cast<QQmlListProperty<apiv1::Chord>*>(_v) = _t->graceNotesBefore(); break;
        case 2: *reinterpret_cast<QQmlListProperty<apiv1::Chord>*>(_v) = _t->graceNotesAfter(); break;
        case 3: *reinterpret_cast<QQmlListProperty<apiv1::Note>*>(_v) = _t->notes(); break;
        case 4: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->articulations(); break;
        case 5: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->stem(); break;
        case 6: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->stemSlash(); break;
        case 7: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->hook(); break;
        case 8: *reinterpret_cast<mu::engraving::NoteType*>(_v) = _t->noteType(); break;
        case 9: *reinterpret_cast<mu::engraving::PlayEventType*>(_v) = _t->playEventType(); break;
        case 10: *reinterpret_cast<QVariant*>(_v) = _t->get_noStem(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->get_showStemSlash(); break;
        case 12: *reinterpret_cast<QVariant*>(_v) = _t->get_combineVoice(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->isTrillCueNote(); break;
        case 14: *reinterpret_cast<apiv1::Note**>(_v) = _t->upNote(); break;
        case 15: *reinterpret_cast<apiv1::Note**>(_v) = _t->downNote(); break;
        case 16: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->arpeggio(); break;
        case 17: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->spanArpeggio(); break;
        case 18: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->tremoloSingleChord(); break;
        case 19: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->tremoloTwoChord(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 9: _t->setPlayEventType(*reinterpret_cast<mu::engraving::PlayEventType*>(_v)); break;
        case 10: _t->set_noStem(*reinterpret_cast<QVariant*>(_v)); break;
        case 11: _t->set_showStemSlash(*reinterpret_cast<bool*>(_v)); break;
        case 12: _t->set_combineVoice(*reinterpret_cast<QVariant*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 10: _t->reset_noStem(); break;
        case 11: _t->reset_showStemSlash(); break;
        case 12: _t->reset_combineVoice(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Chord::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Chord::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15ChordE_t>.strings))
        return static_cast<void*>(this);
    return ChordRest::qt_metacast(_clname);
}

int mu::engraving::apiv1::Chord::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ChordRest::qt_metacall(_c, _id, _a);
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
        _id -= 20;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv14BeamE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Beam::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv14BeamE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Beam",
        "growLeft",
        "growRight",
        "isCrossStaff",
        "isFullCrossStaff",
        "defaultCrossStaffIdx",
        "minCRMove",
        "maxCRMove",
        "elements",
        "QQmlListProperty<apiv1::ChordRest>"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'growLeft'
        QtMocHelpers::PropertyData<qreal>(1, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'growRight'
        QtMocHelpers::PropertyData<qreal>(2, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'isCrossStaff'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'isFullCrossStaff'
        QtMocHelpers::PropertyData<bool>(4, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'defaultCrossStaffIdx'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'minCRMove'
        QtMocHelpers::PropertyData<int>(6, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'maxCRMove'
        QtMocHelpers::PropertyData<int>(7, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'elements'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::ChordRest>>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Beam, qt_meta_tag_ZN2mu9engraving5apiv14BeamE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Beam::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14BeamE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14BeamE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv14BeamE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Beam::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Beam *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<qreal*>(_v) = _t->get_growLeft(); break;
        case 1: *reinterpret_cast<qreal*>(_v) = _t->get_growRight(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->cross(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->fullCross(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->defaultCrossStaffIdx(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->minCRMove(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->maxCRMove(); break;
        case 7: *reinterpret_cast<QQmlListProperty<apiv1::ChordRest>*>(_v) = _t->elements(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_growLeft(*reinterpret_cast<qreal*>(_v)); break;
        case 1: _t->set_growRight(*reinterpret_cast<qreal*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 0: _t->reset_growLeft(); break;
        case 1: _t->reset_growRight(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Beam::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Beam::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14BeamE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::Beam::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv17SegmentE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Segment::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv17SegmentE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Segment",
        "elementAt",
        "apiv1::EngravingItem*",
        "",
        "track",
        "leadingSpace",
        "QVariant",
        "annotations",
        "QQmlListProperty<apiv1::EngravingItem>",
        "nextInMeasure",
        "apiv1::Segment*",
        "next",
        "prevInMeasure",
        "prev",
        "segmentType",
        "tick",
        "fraction",
        "apiv1::Fraction*"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'elementAt'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(int)>(1, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'leadingSpace'
        QtMocHelpers::PropertyData<QVariant>(5, 0x80000000 | 6, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'annotations'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(7, 0x80000000 | 8, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'nextInMeasure'
        QtMocHelpers::PropertyData<apiv1::Segment*>(9, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'next'
        QtMocHelpers::PropertyData<apiv1::Segment*>(11, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'prevInMeasure'
        QtMocHelpers::PropertyData<apiv1::Segment*>(12, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'prev'
        QtMocHelpers::PropertyData<apiv1::Segment*>(13, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'segmentType'
        QtMocHelpers::PropertyData<int>(14, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'tick'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'fraction'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(16, 0x80000000 | 17, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Segment, qt_meta_tag_ZN2mu9engraving5apiv17SegmentE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Segment::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17SegmentE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17SegmentE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv17SegmentE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Segment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Segment *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::EngravingItem* _r = _t->elementAt((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariant*>(_v) = _t->get_leadingSpace(); break;
        case 1: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->annotations(); break;
        case 2: *reinterpret_cast<apiv1::Segment**>(_v) = _t->nextInMeasure(); break;
        case 3: *reinterpret_cast<apiv1::Segment**>(_v) = _t->nextInScore(); break;
        case 4: *reinterpret_cast<apiv1::Segment**>(_v) = _t->prevInMeasure(); break;
        case 5: *reinterpret_cast<apiv1::Segment**>(_v) = _t->prevInScore(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->segmentType(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->tick(); break;
        case 8: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->fraction(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_leadingSpace(*reinterpret_cast<QVariant*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 0: _t->reset_leadingSpace(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Segment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Segment::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17SegmentE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::Segment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
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
        _id -= 9;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv111MeasureBaseE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::MeasureBase::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv111MeasureBaseE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::MeasureBase",
        "add",
        "",
        "apiv1::EngravingItem*",
        "wrapped",
        "remove",
        "repeatEnd",
        "repeatStart",
        "repeatJump",
        "noOffset",
        "irregular",
        "no",
        "tick",
        "apiv1::Fraction*",
        "ticks",
        "elements",
        "QQmlListProperty<apiv1::EngravingItem>",
        "nextMeasure",
        "apiv1::Measure*",
        "nextMeasureMM",
        "prevMeasure",
        "prevMeasureMM",
        "next",
        "apiv1::MeasureBase*",
        "nextMM",
        "prev",
        "prevMM"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'add'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'remove'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'repeatEnd'
        QtMocHelpers::PropertyData<bool>(6, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'repeatStart'
        QtMocHelpers::PropertyData<bool>(7, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'repeatJump'
        QtMocHelpers::PropertyData<bool>(8, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'noOffset'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'irregular'
        QtMocHelpers::PropertyData<bool>(10, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'no'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'tick'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(12, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'ticks'
        QtMocHelpers::PropertyData<apiv1::Fraction*>(14, 0x80000000 | 13, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'elements'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(15, 0x80000000 | 16, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'nextMeasure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(17, 0x80000000 | 18, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'nextMeasureMM'
        QtMocHelpers::PropertyData<apiv1::Measure*>(19, 0x80000000 | 18, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'prevMeasure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(20, 0x80000000 | 18, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'prevMeasureMM'
        QtMocHelpers::PropertyData<apiv1::Measure*>(21, 0x80000000 | 18, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'next'
        QtMocHelpers::PropertyData<apiv1::MeasureBase*>(22, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'nextMM'
        QtMocHelpers::PropertyData<apiv1::MeasureBase*>(24, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'prev'
        QtMocHelpers::PropertyData<apiv1::MeasureBase*>(25, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'prevMM'
        QtMocHelpers::PropertyData<apiv1::MeasureBase*>(26, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MeasureBase, qt_meta_tag_ZN2mu9engraving5apiv111MeasureBaseE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::MeasureBase::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv111MeasureBaseE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv111MeasureBaseE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv111MeasureBaseE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::MeasureBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MeasureBase *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->add((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        case 1: _t->remove((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->get_repeatEnd(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->get_repeatStart(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->get_repeatJump(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->get_noOffset(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->get_irregular(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->no(); break;
        case 6: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->tick(); break;
        case 7: *reinterpret_cast<apiv1::Fraction**>(_v) = _t->ticks(); break;
        case 8: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->elements(); break;
        case 9: *reinterpret_cast<apiv1::Measure**>(_v) = _t->nextMeasure(); break;
        case 10: *reinterpret_cast<apiv1::Measure**>(_v) = _t->nextMeasureMM(); break;
        case 11: *reinterpret_cast<apiv1::Measure**>(_v) = _t->prevMeasure(); break;
        case 12: *reinterpret_cast<apiv1::Measure**>(_v) = _t->prevMeasureMM(); break;
        case 13: *reinterpret_cast<apiv1::MeasureBase**>(_v) = _t->next(); break;
        case 14: *reinterpret_cast<apiv1::MeasureBase**>(_v) = _t->nextMM(); break;
        case 15: *reinterpret_cast<apiv1::MeasureBase**>(_v) = _t->prev(); break;
        case 16: *reinterpret_cast<apiv1::MeasureBase**>(_v) = _t->prevMM(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_repeatEnd(*reinterpret_cast<bool*>(_v)); break;
        case 1: _t->set_repeatStart(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->set_repeatJump(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->set_noOffset(*reinterpret_cast<int*>(_v)); break;
        case 4: _t->set_irregular(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 0: _t->reset_repeatEnd(); break;
        case 1: _t->reset_repeatStart(); break;
        case 2: _t->reset_repeatJump(); break;
        case 3: _t->reset_noOffset(); break;
        case 4: _t->reset_irregular(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::MeasureBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::MeasureBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv111MeasureBaseE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::MeasureBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
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
        _id -= 17;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv17MeasureE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Measure::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv17MeasureE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Measure",
        "vspacerUp",
        "apiv1::EngravingItem*",
        "",
        "staffIdx",
        "vspacerDown",
        "measureNumber",
        "mmRangeText",
        "corrupted",
        "visible",
        "stemless",
        "firstSegment",
        "apiv1::Segment*",
        "lastSegment",
        "timesigNominal",
        "QVariant",
        "timesigActual",
        "measureNumberMode",
        "showsMeasureNumberInAutoMode",
        "breakMmr",
        "repeatCount",
        "userStretch",
        "mmRest",
        "apiv1::Measure*",
        "isMMRestStart",
        "segments",
        "QQmlListProperty<apiv1::Segment>"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'vspacerUp'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(int)>(1, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'vspacerDown'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(int)>(5, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'measureNumber'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(int)>(6, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'mmRangeText'
        QtMocHelpers::MethodData<apiv1::EngravingItem *(int)>(7, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'corrupted'
        QtMocHelpers::MethodData<bool(int)>(8, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'visible'
        QtMocHelpers::MethodData<bool(int)>(9, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'stemless'
        QtMocHelpers::MethodData<bool(int)>(10, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'firstSegment'
        QtMocHelpers::PropertyData<apiv1::Segment*>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'lastSegment'
        QtMocHelpers::PropertyData<apiv1::Segment*>(13, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'timesigNominal'
        QtMocHelpers::PropertyData<QVariant>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'timesigActual'
        QtMocHelpers::PropertyData<QVariant>(16, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'measureNumberMode'
        QtMocHelpers::PropertyData<int>(17, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'showsMeasureNumberInAutoMode'
        QtMocHelpers::PropertyData<bool>(18, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'breakMmr'
        QtMocHelpers::PropertyData<bool>(19, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'repeatCount'
        QtMocHelpers::PropertyData<int>(20, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'userStretch'
        QtMocHelpers::PropertyData<qreal>(21, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'mmRest'
        QtMocHelpers::PropertyData<apiv1::Measure*>(22, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'isMMRestStart'
        QtMocHelpers::PropertyData<bool>(24, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'segments'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Segment>>(25, 0x80000000 | 26, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Measure, qt_meta_tag_ZN2mu9engraving5apiv17MeasureE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Measure::staticMetaObject = { {
    QMetaObject::SuperData::link<MeasureBase::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17MeasureE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17MeasureE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv17MeasureE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Measure::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Measure *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { apiv1::EngravingItem* _r = _t->vspacerUp((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 1: { apiv1::EngravingItem* _r = _t->vspacerDown((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 2: { apiv1::EngravingItem* _r = _t->measureNumber((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 3: { apiv1::EngravingItem* _r = _t->mmRangeText((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->corrupted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->visible((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->stemless((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<apiv1::Segment**>(_v) = _t->firstSegment(); break;
        case 1: *reinterpret_cast<apiv1::Segment**>(_v) = _t->lastSegment(); break;
        case 2: *reinterpret_cast<QVariant*>(_v) = _t->get_timesigNominal(); break;
        case 3: *reinterpret_cast<QVariant*>(_v) = _t->get_timesigActual(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->get_measureNumberMode(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->showsMeasureNumberInAutoMode(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->get_breakMmr(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->get_repeatCount(); break;
        case 8: *reinterpret_cast<qreal*>(_v) = _t->get_userStretch(); break;
        case 9: *reinterpret_cast<apiv1::Measure**>(_v) = _t->mmRest(); break;
        case 10: *reinterpret_cast<bool*>(_v) = _t->isMMRest(); break;
        case 11: *reinterpret_cast<QQmlListProperty<apiv1::Segment>*>(_v) = _t->segments(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->set_timesigNominal(*reinterpret_cast<QVariant*>(_v)); break;
        case 3: _t->set_timesigActual(*reinterpret_cast<QVariant*>(_v)); break;
        case 4: _t->set_measureNumberMode(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->set_breakMmr(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->set_repeatCount(*reinterpret_cast<int*>(_v)); break;
        case 8: _t->set_userStretch(*reinterpret_cast<qreal*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 2: _t->reset_timesigNominal(); break;
        case 3: _t->reset_timesigActual(); break;
        case 4: _t->reset_measureNumberMode(); break;
        case 6: _t->reset_breakMmr(); break;
        case 7: _t->reset_repeatCount(); break;
        case 8: _t->reset_userStretch(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Measure::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Measure::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17MeasureE_t>.strings))
        return static_cast<void*>(this);
    return MeasureBase::qt_metacast(_clname);
}

int mu::engraving::apiv1::Measure::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MeasureBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv16SystemE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::System::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv16SystemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::System",
        "bbox",
        "QRectF",
        "",
        "staffIdx",
        "yOffset",
        "show",
        "setHideStaffIfEmpty",
        "hide",
        "measures",
        "QQmlListProperty<apiv1::MeasureBase>",
        "firstMeasure",
        "apiv1::Measure*",
        "lastMeasure",
        "first",
        "apiv1::MeasureBase*",
        "last",
        "isLocked",
        "pageBreak",
        "systemDividerLeft",
        "apiv1::EngravingItem*",
        "systemDividerRight"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'bbox'
        QtMocHelpers::MethodData<QRectF(int)>(1, 3, QMC::AccessPublic, 0x80000000 | 2, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'yOffset'
        QtMocHelpers::MethodData<qreal(int)>(5, 3, QMC::AccessPublic, QMetaType::QReal, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'show'
        QtMocHelpers::MethodData<bool(int)>(6, 3, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'setHideStaffIfEmpty'
        QtMocHelpers::MethodData<void(int, int)>(7, 3, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 }, { QMetaType::Int, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'measures'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::MeasureBase>>(9, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'firstMeasure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'lastMeasure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(13, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'first'
        QtMocHelpers::PropertyData<apiv1::MeasureBase*>(14, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'last'
        QtMocHelpers::PropertyData<apiv1::MeasureBase*>(16, 0x80000000 | 15, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'isLocked'
        QtMocHelpers::PropertyData<bool>(17, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'pageBreak'
        QtMocHelpers::PropertyData<bool>(18, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'systemDividerLeft'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(19, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'systemDividerRight'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(21, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<System, qt_meta_tag_ZN2mu9engraving5apiv16SystemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::System::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16SystemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16SystemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv16SystemE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::System::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<System *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QRectF _r = _t->bbox((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QRectF*>(_a[0]) = std::move(_r); }  break;
        case 1: { qreal _r = _t->yOffset((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->show((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setHideStaffIfEmpty((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QQmlListProperty<apiv1::MeasureBase>*>(_v) = _t->measures(); break;
        case 1: *reinterpret_cast<apiv1::Measure**>(_v) = _t->firstMeasure(); break;
        case 2: *reinterpret_cast<apiv1::Measure**>(_v) = _t->lastMeasure(); break;
        case 3: *reinterpret_cast<apiv1::MeasureBase**>(_v) = _t->first(); break;
        case 4: *reinterpret_cast<apiv1::MeasureBase**>(_v) = _t->last(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->isLocked(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->pageBreak(); break;
        case 7: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->systemDividerLeft(); break;
        case 8: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->systemDividerRight(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 5: _t->setIsLocked(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::System::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::System::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16SystemE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::System::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv14PageE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Page::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv14PageE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Page",
        "pageNumber",
        "pagenumber",
        "systems",
        "QQmlListProperty<apiv1::System>"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'pageNumber'
        QtMocHelpers::PropertyData<int>(1, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'pagenumber'
        QtMocHelpers::PropertyData<int>(2, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'systems'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::System>>(3, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Page, qt_meta_tag_ZN2mu9engraving5apiv14PageE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Page::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14PageE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14PageE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv14PageE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Page::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Page *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->pageNumber(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->pageNumber(); break;
        case 2: *reinterpret_cast<QQmlListProperty<apiv1::System>*>(_v) = _t->systems(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Page::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Page::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv14PageE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::Page::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv18OrnamentE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Ornament::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv18OrnamentE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Ornament",
        "hasIntervalAbove",
        "hasIntervalBelow",
        "showCueNote",
        "accidentalAbove",
        "apiv1::EngravingItem*",
        "accidentalBelow"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'hasIntervalAbove'
        QtMocHelpers::PropertyData<bool>(1, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'hasIntervalBelow'
        QtMocHelpers::PropertyData<bool>(2, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'showCueNote'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'accidentalAbove'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'accidentalBelow'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Ornament, qt_meta_tag_ZN2mu9engraving5apiv18OrnamentE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Ornament::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv18OrnamentE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv18OrnamentE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv18OrnamentE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Ornament::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Ornament *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->hasIntervalAbove(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->hasIntervalBelow(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->showCueNote(); break;
        case 3: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->accidentalAbove(); break;
        case 4: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->accidentalBelow(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Ornament::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Ornament::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv18OrnamentE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::Ornament::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv15StaffE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Staff::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv15StaffE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Staff",
        "clefType",
        "",
        "apiv1::Fraction*",
        "tick",
        "timeStretch",
        "timeSig",
        "EngravingItem*",
        "key",
        "transpose",
        "apiv1::IntervalWrapper*",
        "swing",
        "QVariantMap",
        "capo",
        "stemless",
        "staffHeight",
        "isPitchedStaff",
        "isTabStaff",
        "isDrumStaff",
        "lines",
        "lineDistance",
        "isLinesInvisible",
        "middleLine",
        "bottomLine",
        "staffMag",
        "spatium",
        "pitchOffset",
        "isVoiceVisible",
        "voice",
        "small",
        "mag",
        "color",
        "QColor",
        "playbackVoice1",
        "playbackVoice2",
        "playbackVoice3",
        "playbackVoice4",
        "showMeasureNumbers",
        "QVariant",
        "showIfEntireSystemEmpty",
        "staffBarlineSpan",
        "staffBarlineSpanFrom",
        "staffBarlineSpanTo",
        "staffInvisible",
        "staffUserdist",
        "part",
        "apiv1::Part*",
        "idx",
        "show",
        "visible",
        "cutaway",
        "hideSystemBarLine",
        "mergeMatchingRests",
        "reflectTranspositionInLinkedTab",
        "primaryStaff",
        "apiv1::Staff*",
        "brackets",
        "QQmlListProperty<apiv1::EngravingItem>"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'clefType'
        QtMocHelpers::MethodData<int(apiv1::Fraction *)>(1, 2, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'timeStretch'
        QtMocHelpers::MethodData<apiv1::Fraction *(apiv1::Fraction *)>(5, 2, QMC::AccessPublic, 0x80000000 | 3, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'timeSig'
        QtMocHelpers::MethodData<EngravingItem *(apiv1::Fraction *)>(6, 2, QMC::AccessPublic, 0x80000000 | 7, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'key'
        QtMocHelpers::MethodData<int(apiv1::Fraction *)>(8, 2, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'transpose'
        QtMocHelpers::MethodData<apiv1::IntervalWrapper *(apiv1::Fraction *)>(9, 2, QMC::AccessPublic, 0x80000000 | 10, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'swing'
        QtMocHelpers::MethodData<QVariantMap(apiv1::Fraction *)>(11, 2, QMC::AccessPublic, 0x80000000 | 12, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'capo'
        QtMocHelpers::MethodData<QVariantMap(apiv1::Fraction *)>(13, 2, QMC::AccessPublic, 0x80000000 | 12, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'stemless'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(14, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'staffHeight'
        QtMocHelpers::MethodData<qreal(apiv1::Fraction *)>(15, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'isPitchedStaff'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(16, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'isTabStaff'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(17, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'isDrumStaff'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(18, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'lines'
        QtMocHelpers::MethodData<int(apiv1::Fraction *)>(19, 2, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'lineDistance'
        QtMocHelpers::MethodData<qreal(apiv1::Fraction *)>(20, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'isLinesInvisible'
        QtMocHelpers::MethodData<bool(apiv1::Fraction *)>(21, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'middleLine'
        QtMocHelpers::MethodData<int(apiv1::Fraction *)>(22, 2, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'bottomLine'
        QtMocHelpers::MethodData<int(apiv1::Fraction *)>(23, 2, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'staffMag'
        QtMocHelpers::MethodData<qreal(apiv1::Fraction *)>(24, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'spatium'
        QtMocHelpers::MethodData<qreal(apiv1::Fraction *)>(25, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'pitchOffset'
        QtMocHelpers::MethodData<int(apiv1::Fraction *)>(26, 2, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Method 'isVoiceVisible'
        QtMocHelpers::MethodData<bool(int)>(27, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 28 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'small'
        QtMocHelpers::PropertyData<bool>(29, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'mag'
        QtMocHelpers::PropertyData<qreal>(30, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'color'
        QtMocHelpers::PropertyData<QColor>(31, 0x80000000 | 32, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'playbackVoice1'
        QtMocHelpers::PropertyData<bool>(33, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'playbackVoice2'
        QtMocHelpers::PropertyData<bool>(34, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'playbackVoice3'
        QtMocHelpers::PropertyData<bool>(35, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'playbackVoice4'
        QtMocHelpers::PropertyData<bool>(36, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'showMeasureNumbers'
        QtMocHelpers::PropertyData<QVariant>(37, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'showIfEntireSystemEmpty'
        QtMocHelpers::PropertyData<bool>(39, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffBarlineSpan'
        QtMocHelpers::PropertyData<int>(40, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffBarlineSpanFrom'
        QtMocHelpers::PropertyData<int>(41, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffBarlineSpanTo'
        QtMocHelpers::PropertyData<int>(42, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffInvisible'
        QtMocHelpers::PropertyData<bool>(43, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'staffUserdist'
        QtMocHelpers::PropertyData<qreal>(44, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'part'
        QtMocHelpers::PropertyData<apiv1::Part*>(45, 0x80000000 | 46, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'idx'
        QtMocHelpers::PropertyData<int>(47, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'show'
        QtMocHelpers::PropertyData<bool>(48, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'visible'
        QtMocHelpers::PropertyData<bool>(49, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'cutaway'
        QtMocHelpers::PropertyData<bool>(50, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'hideSystemBarLine'
        QtMocHelpers::PropertyData<bool>(51, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'mergeMatchingRests'
        QtMocHelpers::PropertyData<QVariant>(52, 0x80000000 | 38, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'reflectTranspositionInLinkedTab'
        QtMocHelpers::PropertyData<bool>(53, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'primaryStaff'
        QtMocHelpers::PropertyData<apiv1::Staff*>(54, 0x80000000 | 55, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'brackets'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::EngravingItem>>(56, 0x80000000 | 57, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Staff, qt_meta_tag_ZN2mu9engraving5apiv15StaffE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Staff::staticMetaObject = { {
    QMetaObject::SuperData::link<ScoreElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15StaffE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15StaffE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv15StaffE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Staff::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Staff *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = _t->clefType((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 1: { apiv1::Fraction* _r = _t->timeStretch((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Fraction**>(_a[0]) = std::move(_r); }  break;
        case 2: { EngravingItem* _r = _t->timeSig((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<EngravingItem**>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->key((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 4: { apiv1::IntervalWrapper* _r = _t->transpose((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::IntervalWrapper**>(_a[0]) = std::move(_r); }  break;
        case 5: { QVariantMap _r = _t->swing((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 6: { QVariantMap _r = _t->capo((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->stemless((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { qreal _r = _t->staffHeight((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->isPitchedStaff((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->isTabStaff((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->isDrumStaff((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->lines((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 13: { qreal _r = _t->lineDistance((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->isLinesInvisible((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { int _r = _t->middleLine((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 16: { int _r = _t->bottomLine((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 17: { qreal _r = _t->staffMag((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 18: { qreal _r = _t->spatium((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 19: { int _r = _t->pitchOffset((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 20: { bool _r = _t->isVoiceVisible((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->get_small(); break;
        case 1: *reinterpret_cast<qreal*>(_v) = _t->get_mag(); break;
        case 2: *reinterpret_cast<QColor*>(_v) = _t->get_color(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->get_playbackVoice1(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->get_playbackVoice2(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->get_playbackVoice3(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->get_playbackVoice4(); break;
        case 7: *reinterpret_cast<QVariant*>(_v) = _t->get_showMeasureNumbers(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->get_showIfEntireSystemEmpty(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->get_staffBarlineSpan(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->get_staffBarlineSpanFrom(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->get_staffBarlineSpanTo(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->get_staffInvisible(); break;
        case 13: *reinterpret_cast<qreal*>(_v) = _t->get_staffUserdist(); break;
        case 14: *reinterpret_cast<apiv1::Part**>(_v) = _t->part(); break;
        case 15: *reinterpret_cast<int*>(_v) = _t->idx(); break;
        case 16: *reinterpret_cast<bool*>(_v) = _t->show(); break;
        case 17: *reinterpret_cast<bool*>(_v) = _t->get_visible(); break;
        case 18: *reinterpret_cast<bool*>(_v) = _t->get_cutaway(); break;
        case 19: *reinterpret_cast<bool*>(_v) = _t->get_hideSystemBarLine(); break;
        case 20: *reinterpret_cast<QVariant*>(_v) = _t->get_mergeMatchingRests(); break;
        case 21: *reinterpret_cast<bool*>(_v) = _t->get_reflectTranspositionInLinkedTab(); break;
        case 22: *reinterpret_cast<apiv1::Staff**>(_v) = _t->primaryStaff(); break;
        case 23: *reinterpret_cast<QQmlListProperty<apiv1::EngravingItem>*>(_v) = _t->brackets(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_small(*reinterpret_cast<bool*>(_v)); break;
        case 1: _t->set_mag(*reinterpret_cast<qreal*>(_v)); break;
        case 2: _t->set_color(*reinterpret_cast<QColor*>(_v)); break;
        case 3: _t->set_playbackVoice1(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->set_playbackVoice2(*reinterpret_cast<bool*>(_v)); break;
        case 5: _t->set_playbackVoice3(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->set_playbackVoice4(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->set_showMeasureNumbers(*reinterpret_cast<QVariant*>(_v)); break;
        case 8: _t->set_showIfEntireSystemEmpty(*reinterpret_cast<bool*>(_v)); break;
        case 9: _t->set_staffBarlineSpan(*reinterpret_cast<int*>(_v)); break;
        case 10: _t->set_staffBarlineSpanFrom(*reinterpret_cast<int*>(_v)); break;
        case 11: _t->set_staffBarlineSpanTo(*reinterpret_cast<int*>(_v)); break;
        case 12: _t->set_staffInvisible(*reinterpret_cast<bool*>(_v)); break;
        case 13: _t->set_staffUserdist(*reinterpret_cast<qreal*>(_v)); break;
        case 17: _t->set_visible(*reinterpret_cast<bool*>(_v)); break;
        case 18: _t->set_cutaway(*reinterpret_cast<bool*>(_v)); break;
        case 19: _t->set_hideSystemBarLine(*reinterpret_cast<bool*>(_v)); break;
        case 20: _t->set_mergeMatchingRests(*reinterpret_cast<QVariant*>(_v)); break;
        case 21: _t->set_reflectTranspositionInLinkedTab(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 0: _t->reset_small(); break;
        case 1: _t->reset_mag(); break;
        case 2: _t->reset_color(); break;
        case 3: _t->reset_playbackVoice1(); break;
        case 4: _t->reset_playbackVoice2(); break;
        case 5: _t->reset_playbackVoice3(); break;
        case 6: _t->reset_playbackVoice4(); break;
        case 7: _t->reset_showMeasureNumbers(); break;
        case 8: _t->reset_showIfEntireSystemEmpty(); break;
        case 9: _t->reset_staffBarlineSpan(); break;
        case 10: _t->reset_staffBarlineSpanFrom(); break;
        case 11: _t->reset_staffBarlineSpanTo(); break;
        case 12: _t->reset_staffInvisible(); break;
        case 13: _t->reset_staffUserdist(); break;
        case 17: _t->reset_visible(); break;
        case 18: _t->reset_cutaway(); break;
        case 19: _t->reset_hideSystemBarLine(); break;
        case 20: _t->reset_mergeMatchingRests(); break;
        case 21: _t->reset_reflectTranspositionInLinkedTab(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Staff::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Staff::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15StaffE_t>.strings))
        return static_cast<void*>(this);
    return ScoreElement::qt_metacast(_clname);
}

int mu::engraving::apiv1::Staff::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ScoreElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv114SpannerSegmentE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::SpannerSegment::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv114SpannerSegmentE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::SpannerSegment",
        "spanner",
        "apiv1::Spanner*",
        "spannerSegmentType",
        "pos2",
        "QPointF",
        "userOff2",
        "slurUoff1",
        "slurUoff2",
        "slurUoff3",
        "slurUoff4"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'spanner'
        QtMocHelpers::PropertyData<apiv1::Spanner*>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'spannerSegmentType'
        QtMocHelpers::PropertyData<int>(3, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'pos2'
        QtMocHelpers::PropertyData<QPointF>(4, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'userOff2'
        QtMocHelpers::PropertyData<QPointF>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'slurUoff1'
        QtMocHelpers::PropertyData<QPointF>(7, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'slurUoff2'
        QtMocHelpers::PropertyData<QPointF>(8, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'slurUoff3'
        QtMocHelpers::PropertyData<QPointF>(9, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'slurUoff4'
        QtMocHelpers::PropertyData<QPointF>(10, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SpannerSegment, qt_meta_tag_ZN2mu9engraving5apiv114SpannerSegmentE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::SpannerSegment::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv114SpannerSegmentE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv114SpannerSegmentE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv114SpannerSegmentE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::SpannerSegment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SpannerSegment *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<apiv1::Spanner**>(_v) = _t->spanner(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->spannerSegmentType(); break;
        case 2: *reinterpret_cast<QPointF*>(_v) = _t->pos2(); break;
        case 3: *reinterpret_cast<QPointF*>(_v) = _t->get_userOff2(); break;
        case 4: *reinterpret_cast<QPointF*>(_v) = _t->get_slurUoff1(); break;
        case 5: *reinterpret_cast<QPointF*>(_v) = _t->get_slurUoff2(); break;
        case 6: *reinterpret_cast<QPointF*>(_v) = _t->get_slurUoff3(); break;
        case 7: *reinterpret_cast<QPointF*>(_v) = _t->get_slurUoff4(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->set_userOff2(*reinterpret_cast<QPointF*>(_v)); break;
        case 4: _t->set_slurUoff1(*reinterpret_cast<QPointF*>(_v)); break;
        case 5: _t->set_slurUoff2(*reinterpret_cast<QPointF*>(_v)); break;
        case 6: _t->set_slurUoff3(*reinterpret_cast<QPointF*>(_v)); break;
        case 7: _t->set_slurUoff4(*reinterpret_cast<QPointF*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 3: _t->reset_userOff2(); break;
        case 4: _t->reset_slurUoff1(); break;
        case 5: _t->reset_slurUoff2(); break;
        case 6: _t->reset_slurUoff3(); break;
        case 7: _t->reset_slurUoff4(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::SpannerSegment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::SpannerSegment::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv114SpannerSegmentE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::SpannerSegment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv17SpannerE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Spanner::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv17SpannerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Spanner",
        "spannerTick",
        "QVariant",
        "spannerTicks",
        "spannerTrack2",
        "anchor",
        "startElement",
        "apiv1::EngravingItem*",
        "endElement",
        "spannerSegments",
        "QQmlListProperty<apiv1::SpannerSegment>",
        "ornament",
        "apiv1::Ornament*"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'spannerTick'
        QtMocHelpers::PropertyData<QVariant>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'spannerTicks'
        QtMocHelpers::PropertyData<QVariant>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
        // property 'spannerTrack2'
        QtMocHelpers::PropertyData<int>(4, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'anchor'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'startElement'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'endElement'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(8, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'spannerSegments'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::SpannerSegment>>(9, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'ornament'
        QtMocHelpers::PropertyData<apiv1::Ornament*>(11, 0x80000000 | 12, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Spanner, qt_meta_tag_ZN2mu9engraving5apiv17SpannerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Spanner::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17SpannerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17SpannerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv17SpannerE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Spanner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Spanner *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariant*>(_v) = _t->get_spannerTick(); break;
        case 1: *reinterpret_cast<QVariant*>(_v) = _t->get_spannerTicks(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->get_spannerTrack2(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->get_anchor(); break;
        case 4: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->startElement(); break;
        case 5: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->endElement(); break;
        case 6: *reinterpret_cast<QQmlListProperty<apiv1::SpannerSegment>*>(_v) = _t->spannerSegments(); break;
        case 7: *reinterpret_cast<apiv1::Ornament**>(_v) = _t->ornament(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_spannerTick(*reinterpret_cast<QVariant*>(_v)); break;
        case 1: _t->set_spannerTicks(*reinterpret_cast<QVariant*>(_v)); break;
        case 2: _t->set_spannerTrack2(*reinterpret_cast<int*>(_v)); break;
        case 3: _t->set_anchor(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 0: _t->reset_spannerTick(); break;
        case 1: _t->reset_spannerTicks(); break;
        case 2: _t->reset_spannerTrack2(); break;
        case 3: _t->reset_anchor(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Spanner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Spanner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv17SpannerE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::Spanner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv13TieE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Tie::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv13TieE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Tie",
        "startNote",
        "apiv1::Note*",
        "endNote",
        "isInside"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'startNote'
        QtMocHelpers::PropertyData<apiv1::Note*>(1, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'endNote'
        QtMocHelpers::PropertyData<apiv1::Note*>(3, 0x80000000 | 2, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'isInside'
        QtMocHelpers::PropertyData<bool>(4, QMetaType::Bool, QMC::DefaultPropertyFlags),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Tie, qt_meta_tag_ZN2mu9engraving5apiv13TieE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Tie::staticMetaObject = { {
    QMetaObject::SuperData::link<Spanner::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv13TieE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv13TieE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv13TieE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Tie::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Tie *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<apiv1::Note**>(_v) = _t->startNote(); break;
        case 1: *reinterpret_cast<apiv1::Note**>(_v) = _t->endNote(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isInside(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Tie::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Tie::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv13TieE_t>.strings))
        return static_cast<void*>(this);
    return Spanner::qt_metacast(_clname);
}

int mu::engraving::apiv1::Tie::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Spanner::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
namespace {
struct qt_meta_tag_ZN2mu9engraving5apiv16LyricsE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Lyrics::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv16LyricsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Lyrics",
        "plainText",
        "isMelisma",
        "separator",
        "apiv1::EngravingItem*",
        "syllabic",
        "lyricTicks",
        "QVariant"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
        // property 'plainText'
        QtMocHelpers::PropertyData<QString>(1, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'isMelisma'
        QtMocHelpers::PropertyData<bool>(2, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'separator'
        QtMocHelpers::PropertyData<apiv1::EngravingItem*>(3, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'syllabic'
        QtMocHelpers::PropertyData<int>(5, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable),
        // property 'lyricTicks'
        QtMocHelpers::PropertyData<QVariant>(6, 0x80000000 | 7, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Resettable | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Lyrics, qt_meta_tag_ZN2mu9engraving5apiv16LyricsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Lyrics::staticMetaObject = { {
    QMetaObject::SuperData::link<EngravingItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16LyricsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16LyricsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv16LyricsE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Lyrics::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Lyrics *>(_o);
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->plainText(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isMelisma(); break;
        case 2: *reinterpret_cast<apiv1::EngravingItem**>(_v) = _t->separator(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->get_syllabic(); break;
        case 4: *reinterpret_cast<QVariant*>(_v) = _t->get_lyricTicks(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->set_syllabic(*reinterpret_cast<int*>(_v)); break;
        case 4: _t->set_lyricTicks(*reinterpret_cast<QVariant*>(_v)); break;
        default: break;
        }
    }
    if (_c == QMetaObject::ResetProperty) {
        switch (_id) {
        case 3: _t->reset_syllabic(); break;
        case 4: _t->reset_lyricTicks(); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Lyrics::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Lyrics::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv16LyricsE_t>.strings))
        return static_cast<void*>(this);
    return EngravingItem::qt_metacast(_clname);
}

int mu::engraving::apiv1::Lyrics::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EngravingItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP

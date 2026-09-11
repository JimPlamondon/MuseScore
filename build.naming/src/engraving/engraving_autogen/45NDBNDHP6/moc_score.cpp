/****************************************************************************
** Meta object code from reading C++ file 'score.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/engraving/api/v1/score.h"
#include "engraving/api/v1/selection.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'score.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2mu9engraving5apiv15ScoreE_t {};
} // unnamed namespace

template <> constexpr inline auto mu::engraving::apiv1::Score::qt_create_metaobjectdata<qt_meta_tag_ZN2mu9engraving5apiv15ScoreE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "mu::engraving::apiv1::Score",
        "metaTag",
        "",
        "tag",
        "setMetaTag",
        "val",
        "appendPart",
        "instrumentId",
        "appendPartByMusicXmlId",
        "instrumentMusicXmlId",
        "addRemoveSystemLocks",
        "interval",
        "lock",
        "makeIntoSystem",
        "apiv1::MeasureBase*",
        "first",
        "last",
        "extractLyrics",
        "appendMeasures",
        "n",
        "tick2measure",
        "apiv1::Measure*",
        "apiv1::Fraction*",
        "tick",
        "firstSegment",
        "apiv1::Segment*",
        "segmentType",
        "findSegmentAtTick",
        "types",
        "addText",
        "type",
        "text",
        "doLayout",
        "startTick",
        "endTick",
        "replaceInstrument",
        "apiv1::Part*",
        "part",
        "setPartVisible",
        "visible",
        "setPartSharpFlat",
        "sharpFlat",
        "setInstrumentName",
        "name",
        "setInstrumentAbbreviature",
        "abbreviature",
        "setStaffType",
        "apiv1::Staff*",
        "staff",
        "staffTypeId",
        "removeParts",
        "QList<apiv1::Part*>",
        "parts",
        "removeStaves",
        "QList<apiv1::Staff*>",
        "staves",
        "moveParts",
        "sourceParts",
        "destinationPart",
        "insertMode",
        "moveStaves",
        "sourceStaves",
        "destinationStaff",
        "addSystemObjects",
        "removeSystemObjects",
        "moveSystemObjects",
        "sourceStaff",
        "appendStaff",
        "appendLinkedStaff",
        "setVoiceVisible",
        "voiceIndex",
        "replaceDrumset",
        "apiv1::Drumset*",
        "drumset",
        "insertPart",
        "index",
        "replacePart",
        "setScoreOrder",
        "orderId",
        "newCursor",
        "apiv1::Cursor*",
        "startCmd",
        "qActionName",
        "endCmd",
        "rollback",
        "showElementInScore",
        "apiv1::EngravingItem*",
        "element",
        "staffIdx",
        "createPlayEvents",
        "scoreName",
        "title",
        "composer",
        "lyricist",
        "duration",
        "mscoreVersion",
        "mscoreRevision",
        "style",
        "apiv1::MStyle*",
        "keysig",
        "npages",
        "pages",
        "QQmlListProperty<apiv1::Page>",
        "pageNumberOffset",
        "QQmlListProperty<apiv1::Part>",
        "nstaves",
        "QQmlListProperty<apiv1::Staff>",
        "ntracks",
        "systems",
        "QQmlListProperty<apiv1::System>",
        "spanners",
        "QQmlListProperty<apiv1::Spanner>",
        "hasHarmonies",
        "harmonyCount",
        "hasLyrics",
        "lyricCount",
        "lyrics",
        "QQmlListProperty<apiv1::Lyrics>",
        "nmeasures",
        "firstMeasure",
        "firstMeasureMM",
        "lastMeasure",
        "lastMeasureMM",
        "lastSegment",
        "layoutMode",
        "showVerticalFrames",
        "showInvisible",
        "showUnprintable",
        "showFrames",
        "showPageborders",
        "showSoundFlags",
        "markIrregularMeasures",
        "showInstrumentNames",
        "selection",
        "apiv1::Selection*",
        "excerpts",
        "QQmlListProperty<apiv1::Excerpt>"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'metaTag'
        QtMocHelpers::MethodData<QString(const QString &) const>(1, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'setMetaTag'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 5 },
        }}),
        // Method 'appendPart'
        QtMocHelpers::MethodData<void(const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Method 'appendPartByMusicXmlId'
        QtMocHelpers::MethodData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'addRemoveSystemLocks'
        QtMocHelpers::MethodData<void(int, bool)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Bool, 12 },
        }}),
        // Method 'makeIntoSystem'
        QtMocHelpers::MethodData<void(apiv1::MeasureBase *, apiv1::MeasureBase *)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 }, { 0x80000000 | 14, 16 },
        }}),
        // Method 'extractLyrics'
        QtMocHelpers::MethodData<QString() const>(17, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'appendMeasures'
        QtMocHelpers::MethodData<void(int)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 19 },
        }}),
        // Method 'tick2measure'
        QtMocHelpers::MethodData<apiv1::Measure *(apiv1::Fraction *)>(20, 2, QMC::AccessPublic, 0x80000000 | 21, {{
            { 0x80000000 | 22, 23 },
        }}),
        // Method 'firstSegment'
        QtMocHelpers::MethodData<apiv1::Segment *(int)>(24, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::Int, 26 },
        }}),
        // Method 'firstSegment'
        QtMocHelpers::MethodData<apiv1::Segment *()>(24, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 25),
        // Method 'findSegmentAtTick'
        QtMocHelpers::MethodData<apiv1::Segment *(int, apiv1::Fraction *)>(27, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::Int, 28 }, { 0x80000000 | 22, 23 },
        }}),
        // Method 'addText'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 30 }, { QMetaType::QString, 31 },
        }}),
        // Method 'doLayout'
        QtMocHelpers::MethodData<void(apiv1::Fraction *, apiv1::Fraction *)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 33 }, { 0x80000000 | 22, 34 },
        }}),
        // Method 'replaceInstrument'
        QtMocHelpers::MethodData<void(apiv1::Part *, const QString &)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { QMetaType::QString, 7 },
        }}),
        // Method 'setPartVisible'
        QtMocHelpers::MethodData<void(apiv1::Part *, bool)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { QMetaType::Bool, 39 },
        }}),
        // Method 'setPartSharpFlat'
        QtMocHelpers::MethodData<void(apiv1::Part *, int)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { QMetaType::Int, 41 },
        }}),
        // Method 'setInstrumentName'
        QtMocHelpers::MethodData<void(apiv1::Part *, apiv1::Fraction *, const QString &)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { 0x80000000 | 22, 23 }, { QMetaType::QString, 43 },
        }}),
        // Method 'setInstrumentAbbreviature'
        QtMocHelpers::MethodData<void(apiv1::Part *, apiv1::Fraction *, const QString &)>(44, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { 0x80000000 | 22, 23 }, { QMetaType::QString, 45 },
        }}),
        // Method 'setStaffType'
        QtMocHelpers::MethodData<void(apiv1::Staff *, int)>(46, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 47, 48 }, { QMetaType::Int, 49 },
        }}),
        // Method 'removeParts'
        QtMocHelpers::MethodData<void(QList<apiv1::Part*>)>(50, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 51, 52 },
        }}),
        // Method 'removeStaves'
        QtMocHelpers::MethodData<void(QList<apiv1::Staff*>)>(53, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 54, 55 },
        }}),
        // Method 'moveParts'
        QtMocHelpers::MethodData<void(QList<apiv1::Part*>, apiv1::Part *, int)>(56, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 51, 57 }, { 0x80000000 | 36, 58 }, { QMetaType::Int, 59 },
        }}),
        // Method 'moveStaves'
        QtMocHelpers::MethodData<void(QList<apiv1::Staff*>, apiv1::Staff *, int)>(60, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 54, 61 }, { 0x80000000 | 47, 62 }, { QMetaType::Int, 59 },
        }}),
        // Method 'addSystemObjects'
        QtMocHelpers::MethodData<void(QList<apiv1::Staff*>)>(63, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 54, 55 },
        }}),
        // Method 'removeSystemObjects'
        QtMocHelpers::MethodData<void(QList<apiv1::Staff*>)>(64, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 54, 55 },
        }}),
        // Method 'moveSystemObjects'
        QtMocHelpers::MethodData<void(apiv1::Staff *, apiv1::Staff *)>(65, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 47, 66 }, { 0x80000000 | 47, 62 },
        }}),
        // Method 'appendStaff'
        QtMocHelpers::MethodData<apiv1::Staff *(apiv1::Part *)>(67, 2, QMC::AccessPublic, 0x80000000 | 47, {{
            { 0x80000000 | 36, 58 },
        }}),
        // Method 'appendLinkedStaff'
        QtMocHelpers::MethodData<apiv1::Staff *(apiv1::Staff *, apiv1::Part *)>(68, 2, QMC::AccessPublic, 0x80000000 | 47, {{
            { 0x80000000 | 47, 66 }, { 0x80000000 | 36, 58 },
        }}),
        // Method 'setVoiceVisible'
        QtMocHelpers::MethodData<bool(apiv1::Staff *, int, bool)>(69, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 47, 48 }, { QMetaType::Int, 70 }, { QMetaType::Bool, 39 },
        }}),
        // Method 'replaceDrumset'
        QtMocHelpers::MethodData<void(apiv1::Part *, apiv1::Fraction *, apiv1::Drumset *)>(71, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { 0x80000000 | 22, 23 }, { 0x80000000 | 72, 73 },
        }}),
        // Method 'insertPart'
        QtMocHelpers::MethodData<void(const QString &, int)>(74, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::Int, 75 },
        }}),
        // Method 'replacePart'
        QtMocHelpers::MethodData<void(apiv1::Part *, const QString &)>(76, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { QMetaType::QString, 7 },
        }}),
        // Method 'setScoreOrder'
        QtMocHelpers::MethodData<void(const QString &)>(77, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 78 },
        }}),
        // Method 'newCursor'
        QtMocHelpers::MethodData<apiv1::Cursor *()>(79, 2, QMC::AccessPublic, 0x80000000 | 80),
        // Method 'startCmd'
        QtMocHelpers::MethodData<void(const QString &)>(81, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 82 },
        }}),
        // Method 'startCmd'
        QtMocHelpers::MethodData<void()>(81, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Method 'endCmd'
        QtMocHelpers::MethodData<void(bool)>(83, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 84 },
        }}),
        // Method 'endCmd'
        QtMocHelpers::MethodData<void()>(83, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Method 'showElementInScore'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *, int)>(85, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 86, 87 }, { QMetaType::Int, 88 },
        }}),
        // Method 'showElementInScore'
        QtMocHelpers::MethodData<void(apiv1::EngravingItem *)>(85, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 86, 87 },
        }}),
        // Method 'createPlayEvents'
        QtMocHelpers::MethodData<void()>(89, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'scoreName'
        QtMocHelpers::PropertyData<QString>(90, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'title'
        QtMocHelpers::PropertyData<QString>(91, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'composer'
        QtMocHelpers::PropertyData<QString>(92, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'lyricist'
        QtMocHelpers::PropertyData<QString>(93, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'duration'
        QtMocHelpers::PropertyData<int>(94, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'mscoreVersion'
        QtMocHelpers::PropertyData<QString>(95, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'mscoreRevision'
        QtMocHelpers::PropertyData<QString>(96, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'style'
        QtMocHelpers::PropertyData<apiv1::MStyle*>(97, 0x80000000 | 98, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'keysig'
        QtMocHelpers::PropertyData<int>(99, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'npages'
        QtMocHelpers::PropertyData<int>(100, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'pages'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Page>>(101, 0x80000000 | 102, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'pageNumberOffset'
        QtMocHelpers::PropertyData<int>(103, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'parts'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Part>>(52, 0x80000000 | 104, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'nstaves'
        QtMocHelpers::PropertyData<int>(105, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'staves'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Staff>>(55, 0x80000000 | 106, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'ntracks'
        QtMocHelpers::PropertyData<int>(107, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'systems'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::System>>(108, 0x80000000 | 109, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'spanners'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Spanner>>(110, 0x80000000 | 111, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'hasHarmonies'
        QtMocHelpers::PropertyData<bool>(112, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'harmonyCount'
        QtMocHelpers::PropertyData<int>(113, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'hasLyrics'
        QtMocHelpers::PropertyData<bool>(114, QMetaType::Bool, QMC::DefaultPropertyFlags),
        // property 'lyricCount'
        QtMocHelpers::PropertyData<int>(115, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'lyrics'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Lyrics>>(116, 0x80000000 | 117, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'nmeasures'
        QtMocHelpers::PropertyData<int>(118, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'firstMeasure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(119, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'firstMeasureMM'
        QtMocHelpers::PropertyData<apiv1::Measure*>(120, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'lastMeasure'
        QtMocHelpers::PropertyData<apiv1::Measure*>(121, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'lastMeasureMM'
        QtMocHelpers::PropertyData<apiv1::Measure*>(122, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'lastSegment'
        QtMocHelpers::PropertyData<apiv1::Segment*>(123, 0x80000000 | 25, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'layoutMode'
        QtMocHelpers::PropertyData<int>(124, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'showVerticalFrames'
        QtMocHelpers::PropertyData<bool>(125, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable),
        // property 'showInvisible'
        QtMocHelpers::PropertyData<bool>(126, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'showUnprintable'
        QtMocHelpers::PropertyData<bool>(127, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'showFrames'
        QtMocHelpers::PropertyData<bool>(128, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'showPageborders'
        QtMocHelpers::PropertyData<bool>(129, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'showSoundFlags'
        QtMocHelpers::PropertyData<bool>(130, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'markIrregularMeasures'
        QtMocHelpers::PropertyData<bool>(131, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'showInstrumentNames'
        QtMocHelpers::PropertyData<bool>(132, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'selection'
        QtMocHelpers::PropertyData<apiv1::Selection*>(133, 0x80000000 | 134, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
        // property 'excerpts'
        QtMocHelpers::PropertyData<QQmlListProperty<apiv1::Excerpt>>(135, 0x80000000 | 136, QMC::DefaultPropertyFlags | QMC::EnumOrFlag),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Score, qt_meta_tag_ZN2mu9engraving5apiv15ScoreE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject mu::engraving::apiv1::Score::staticMetaObject = { {
    QMetaObject::SuperData::link<apiv1::ScoreElement::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15ScoreE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15ScoreE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2mu9engraving5apiv15ScoreE_t>.metaTypes,
    nullptr
} };

void mu::engraving::apiv1::Score::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Score *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = _t->metaTag((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->setMetaTag((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->appendPart((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->appendPartByMusicXmlId((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->addRemoveSystemLocks((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 5: _t->makeIntoSystem((*reinterpret_cast<std::add_pointer_t<apiv1::MeasureBase*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::MeasureBase*>>(_a[2]))); break;
        case 6: { QString _r = _t->extractLyrics();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->appendMeasures((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: { apiv1::Measure* _r = _t->tick2measure((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Measure**>(_a[0]) = std::move(_r); }  break;
        case 9: { apiv1::Segment* _r = _t->firstSegment((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Segment**>(_a[0]) = std::move(_r); }  break;
        case 10: { apiv1::Segment* _r = _t->firstSegment();
            if (_a[0]) *reinterpret_cast<apiv1::Segment**>(_a[0]) = std::move(_r); }  break;
        case 11: { apiv1::Segment* _r = _t->findSegmentAtTick((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::Segment**>(_a[0]) = std::move(_r); }  break;
        case 12: _t->addText((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->doLayout((*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[2]))); break;
        case 14: _t->replaceInstrument((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 15: _t->setPartVisible((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 16: _t->setPartSharpFlat((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 17: _t->setInstrumentName((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 18: _t->setInstrumentAbbreviature((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 19: _t->setStaffType((*reinterpret_cast<std::add_pointer_t<apiv1::Staff*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 20: _t->removeParts((*reinterpret_cast<std::add_pointer_t<QList<apiv1::Part*>>>(_a[1]))); break;
        case 21: _t->removeStaves((*reinterpret_cast<std::add_pointer_t<QList<apiv1::Staff*>>>(_a[1]))); break;
        case 22: _t->moveParts((*reinterpret_cast<std::add_pointer_t<QList<apiv1::Part*>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 23: _t->moveStaves((*reinterpret_cast<std::add_pointer_t<QList<apiv1::Staff*>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Staff*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 24: _t->addSystemObjects((*reinterpret_cast<std::add_pointer_t<QList<apiv1::Staff*>>>(_a[1]))); break;
        case 25: _t->removeSystemObjects((*reinterpret_cast<std::add_pointer_t<QList<apiv1::Staff*>>>(_a[1]))); break;
        case 26: _t->moveSystemObjects((*reinterpret_cast<std::add_pointer_t<apiv1::Staff*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Staff*>>(_a[2]))); break;
        case 27: { apiv1::Staff* _r = _t->appendStaff((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<apiv1::Staff**>(_a[0]) = std::move(_r); }  break;
        case 28: { apiv1::Staff* _r = _t->appendLinkedStaff((*reinterpret_cast<std::add_pointer_t<apiv1::Staff*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[2])));
            if (_a[0]) *reinterpret_cast<apiv1::Staff**>(_a[0]) = std::move(_r); }  break;
        case 29: { bool _r = _t->setVoiceVisible((*reinterpret_cast<std::add_pointer_t<apiv1::Staff*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 30: _t->replaceDrumset((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<apiv1::Fraction*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<apiv1::Drumset*>>(_a[3]))); break;
        case 31: _t->insertPart((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 32: _t->replacePart((*reinterpret_cast<std::add_pointer_t<apiv1::Part*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 33: _t->setScoreOrder((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 34: { apiv1::Cursor* _r = _t->newCursor();
            if (_a[0]) *reinterpret_cast<apiv1::Cursor**>(_a[0]) = std::move(_r); }  break;
        case 35: _t->startCmd((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 36: _t->startCmd(); break;
        case 37: _t->endCmd((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 38: _t->endCmd(); break;
        case 39: _t->showElementInScore((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 40: _t->showElementInScore((*reinterpret_cast<std::add_pointer_t<apiv1::EngravingItem*>>(_a[1]))); break;
        case 41: _t->createPlayEvents(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->title(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->composer(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->lyricist(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->duration(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->mscoreVersion(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->mscoreRevision(); break;
        case 7: *reinterpret_cast<apiv1::MStyle**>(_v) = _t->style(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->keysig(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->npages(); break;
        case 10: *reinterpret_cast<QQmlListProperty<apiv1::Page>*>(_v) = _t->pages(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->pageNumberOffset(); break;
        case 12: *reinterpret_cast<QQmlListProperty<apiv1::Part>*>(_v) = _t->parts(); break;
        case 13: *reinterpret_cast<int*>(_v) = _t->nstaves(); break;
        case 14: *reinterpret_cast<QQmlListProperty<apiv1::Staff>*>(_v) = _t->staves(); break;
        case 15: *reinterpret_cast<int*>(_v) = _t->ntracks(); break;
        case 16: *reinterpret_cast<QQmlListProperty<apiv1::System>*>(_v) = _t->systems(); break;
        case 17: *reinterpret_cast<QQmlListProperty<apiv1::Spanner>*>(_v) = _t->spanners(); break;
        case 18: *reinterpret_cast<bool*>(_v) = _t->hasHarmonies(); break;
        case 19: *reinterpret_cast<int*>(_v) = _t->harmonyCount(); break;
        case 20: *reinterpret_cast<bool*>(_v) = _t->hasLyrics(); break;
        case 21: *reinterpret_cast<int*>(_v) = _t->lyricCount(); break;
        case 22: *reinterpret_cast<QQmlListProperty<apiv1::Lyrics>*>(_v) = _t->lyrics(); break;
        case 23: *reinterpret_cast<int*>(_v) = _t->nmeasures(); break;
        case 24: *reinterpret_cast<apiv1::Measure**>(_v) = _t->firstMeasure(); break;
        case 25: *reinterpret_cast<apiv1::Measure**>(_v) = _t->firstMeasureMM(); break;
        case 26: *reinterpret_cast<apiv1::Measure**>(_v) = _t->lastMeasure(); break;
        case 27: *reinterpret_cast<apiv1::Measure**>(_v) = _t->lastMeasureMM(); break;
        case 28: *reinterpret_cast<apiv1::Segment**>(_v) = _t->lastSegment(); break;
        case 29: *reinterpret_cast<int*>(_v) = _t->layoutMode(); break;
        case 30: *reinterpret_cast<bool*>(_v) = _t->isShowVBox(); break;
        case 31: *reinterpret_cast<bool*>(_v) = _t->isShowInvisible(); break;
        case 32: *reinterpret_cast<bool*>(_v) = _t->showUnprintable(); break;
        case 33: *reinterpret_cast<bool*>(_v) = _t->showFrames(); break;
        case 34: *reinterpret_cast<bool*>(_v) = _t->showPageborders(); break;
        case 35: *reinterpret_cast<bool*>(_v) = _t->showSoundFlags(); break;
        case 36: *reinterpret_cast<bool*>(_v) = _t->showSoundFlags(); break;
        case 37: *reinterpret_cast<bool*>(_v) = _t->showInstrumentNames(); break;
        case 38: *reinterpret_cast<apiv1::Selection**>(_v) = _t->selection(); break;
        case 39: *reinterpret_cast<QQmlListProperty<apiv1::Excerpt>*>(_v) = _t->excerpts(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setName(*reinterpret_cast<QString*>(_v)); break;
        case 11: _t->setPageNumberOffset(*reinterpret_cast<int*>(_v)); break;
        case 29: _t->setLayoutMode(*reinterpret_cast<int*>(_v)); break;
        case 30: _t->setShowVBox(*reinterpret_cast<bool*>(_v)); break;
        case 31: _t->setShowInvisible(*reinterpret_cast<bool*>(_v)); break;
        case 32: _t->setShowUnprintable(*reinterpret_cast<bool*>(_v)); break;
        case 33: _t->setShowFrames(*reinterpret_cast<bool*>(_v)); break;
        case 34: _t->setShowPageborders(*reinterpret_cast<bool*>(_v)); break;
        case 35: _t->setShowSoundFlags(*reinterpret_cast<bool*>(_v)); break;
        case 36: _t->setMarkIrregularMeasures(*reinterpret_cast<bool*>(_v)); break;
        case 37: _t->setShowInstrumentNames(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *mu::engraving::apiv1::Score::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mu::engraving::apiv1::Score::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2mu9engraving5apiv15ScoreE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "muse::Contextable"))
        return static_cast< muse::Contextable*>(this);
    return apiv1::ScoreElement::qt_metacast(_clname);
}

int mu::engraving::apiv1::Score::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = apiv1::ScoreElement::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 42)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 42;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
    return _id;
}
QT_WARNING_POP

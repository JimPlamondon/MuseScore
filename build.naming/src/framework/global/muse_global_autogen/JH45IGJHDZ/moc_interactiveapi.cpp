/****************************************************************************
** Meta object code from reading C++ file 'interactiveapi.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/framework/global/api/interactiveapi.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interactiveapi.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4muse3api14InteractiveApiE_t {};
} // unnamed namespace

template <> constexpr inline auto muse::api::InteractiveApi::qt_create_metaobjectdata<qt_meta_tag_ZN4muse3api14InteractiveApiE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "muse::api::InteractiveApi",
        "info",
        "",
        "contentTitle",
        "text",
        "warning",
        "error",
        "question",
        "QJSValueList",
        "buttons",
        "openUrl",
        "url",
        "ButtonCode",
        "Ok",
        "Continue",
        "RestoreDefaults",
        "Reset",
        "Apply",
        "Help",
        "Discard",
        "Cancel",
        "Close",
        "Ignore",
        "Retry",
        "Abort",
        "NoToAll",
        "No",
        "YesToAll",
        "Yes",
        "Open",
        "DontSave",
        "SaveAll",
        "Save",
        "Next",
        "Back",
        "Select",
        "Clear",
        "Done"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'info'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Method 'warning'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Method 'error'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 },
        }}),
        // Method 'question'
        QtMocHelpers::MethodData<QString(const QString &, const QString &, const QJSValueList &)>(7, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 }, { 0x80000000 | 8, 9 },
        }}),
        // Method 'openUrl'
        QtMocHelpers::MethodData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ButtonCode'
        QtMocHelpers::EnumData<enum ButtonCode>(12, 12, QMC::EnumFlags{}).add({
            {   13, ButtonCode::Ok },
            {   14, ButtonCode::Continue },
            {   15, ButtonCode::RestoreDefaults },
            {   16, ButtonCode::Reset },
            {   17, ButtonCode::Apply },
            {   18, ButtonCode::Help },
            {   19, ButtonCode::Discard },
            {   20, ButtonCode::Cancel },
            {   21, ButtonCode::Close },
            {   22, ButtonCode::Ignore },
            {   23, ButtonCode::Retry },
            {   24, ButtonCode::Abort },
            {   25, ButtonCode::NoToAll },
            {   26, ButtonCode::No },
            {   27, ButtonCode::YesToAll },
            {   28, ButtonCode::Yes },
            {   29, ButtonCode::Open },
            {   30, ButtonCode::DontSave },
            {   31, ButtonCode::SaveAll },
            {   32, ButtonCode::Save },
            {   33, ButtonCode::Next },
            {   34, ButtonCode::Back },
            {   35, ButtonCode::Select },
            {   36, ButtonCode::Clear },
            {   37, ButtonCode::Done },
        }),
    };
    return QtMocHelpers::metaObjectData<InteractiveApi, qt_meta_tag_ZN4muse3api14InteractiveApiE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject muse::api::InteractiveApi::staticMetaObject = { {
    QMetaObject::SuperData::link<ApiObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4muse3api14InteractiveApiE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4muse3api14InteractiveApiE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4muse3api14InteractiveApiE_t>.metaTypes,
    nullptr
} };

void muse::api::InteractiveApi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<InteractiveApi *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->info((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->warning((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->error((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: { QString _r = _t->question((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QJSValueList>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->openUrl((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *muse::api::InteractiveApi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *muse::api::InteractiveApi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4muse3api14InteractiveApiE_t>.strings))
        return static_cast<void*>(this);
    return ApiObject::qt_metacast(_clname);
}

int muse::api::InteractiveApi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ApiObject::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP

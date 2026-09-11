/****************************************************************************
** Meta object code from reading C++ file 'filesystemapi.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/framework/global/api/filesystemapi.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filesystemapi.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4muse3api13FileSystemApiE_t {};
} // unnamed namespace

template <> constexpr inline auto muse::api::FileSystemApi::qt_create_metaobjectdata<qt_meta_tag_ZN4muse3api13FileSystemApiE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "muse::api::FileSystemApi",
        "fileName",
        "",
        "path",
        "baseName",
        "remove",
        "QVariantMap",
        "clear",
        "copy",
        "src",
        "dst",
        "replace",
        "scanFiles",
        "rootDir",
        "filters",
        "mode",
        "writeTextFile",
        "filePath",
        "str",
        "readTextFile"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'fileName'
        QtMocHelpers::MethodData<QString(const QString &) const>(1, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'baseName'
        QtMocHelpers::MethodData<QString(const QString &) const>(4, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'remove'
        QtMocHelpers::MethodData<QVariantMap(const QString &)>(5, 2, QMC::AccessPublic, 0x80000000 | 6, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'clear'
        QtMocHelpers::MethodData<QVariantMap(const QString &)>(7, 2, QMC::AccessPublic, 0x80000000 | 6, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'copy'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QString &, bool)>(8, 2, QMC::AccessPublic, 0x80000000 | 6, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { QMetaType::Bool, 11 },
        }}),
        // Method 'copy'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QString &)>(8, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 6, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Method 'scanFiles'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QStringList &, const QString &) const>(12, 2, QMC::AccessPublic, 0x80000000 | 6, {{
            { QMetaType::QString, 13 }, { QMetaType::QStringList, 14 }, { QMetaType::QString, 15 },
        }}),
        // Method 'scanFiles'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QStringList &) const>(12, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 6, {{
            { QMetaType::QString, 13 }, { QMetaType::QStringList, 14 },
        }}),
        // Method 'writeTextFile'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QString &) const>(16, 2, QMC::AccessPublic, 0x80000000 | 6, {{
            { QMetaType::QString, 17 }, { QMetaType::QString, 18 },
        }}),
        // Method 'readTextFile'
        QtMocHelpers::MethodData<QVariantMap(const QString &) const>(19, 2, QMC::AccessPublic, 0x80000000 | 6, {{
            { QMetaType::QString, 17 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FileSystemApi, qt_meta_tag_ZN4muse3api13FileSystemApiE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject muse::api::FileSystemApi::staticMetaObject = { {
    QMetaObject::SuperData::link<ApiObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4muse3api13FileSystemApiE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4muse3api13FileSystemApiE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4muse3api13FileSystemApiE_t>.metaTypes,
    nullptr
} };

void muse::api::FileSystemApi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FileSystemApi *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = _t->fileName((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 1: { QString _r = _t->baseName((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 2: { QVariantMap _r = _t->remove((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 3: { QVariantMap _r = _t->clear((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 4: { QVariantMap _r = _t->copy((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 5: { QVariantMap _r = _t->copy((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 6: { QVariantMap _r = _t->scanFiles((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 7: { QVariantMap _r = _t->scanFiles((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 8: { QVariantMap _r = _t->writeTextFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 9: { QVariantMap _r = _t->readTextFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *muse::api::FileSystemApi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *muse::api::FileSystemApi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4muse3api13FileSystemApiE_t>.strings))
        return static_cast<void*>(this);
    return ApiObject::qt_metacast(_clname);
}

int muse::api::FileSystemApi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ApiObject::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP

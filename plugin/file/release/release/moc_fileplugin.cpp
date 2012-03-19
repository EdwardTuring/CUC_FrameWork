/****************************************************************************
** Meta object code from reading C++ file 'fileplugin.h'
**
** Created: Fri Mar 2 21:18:19 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fileplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FilePlugin[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      28,   11,   20,   11, 0x0a,
      41,   11,   20,   11, 0x0a,
      57,   11,   20,   11, 0x0a,
      72,   11,   20,   11, 0x0a,
      94,   85,   20,   11, 0x0a,
     115,   85,  110,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FilePlugin[] = {
    "FilePlugin\0\0debug()\0QString\0pluginName()\0"
    "pluginVersion()\0pluginIcoUrl()\0"
    "chooseFile()\0filename\0saveAs(QString)\0"
    "bool\0exists(QString)\0"
};

const QMetaObject FilePlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FilePlugin,
      qt_meta_data_FilePlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FilePlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FilePlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FilePlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FilePlugin))
        return static_cast<void*>(const_cast< FilePlugin*>(this));
    if (!strcmp(_clname, "CUCPluginInterface"))
        return static_cast< CUCPluginInterface*>(const_cast< FilePlugin*>(this));
    if (!strcmp(_clname, "cn.edu.cuc.CUCPluginInterface/0.1"))
        return static_cast< CUCPluginInterface*>(const_cast< FilePlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int FilePlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: debug(); break;
        case 1: { QString _r = pluginName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: { QString _r = pluginVersion();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QString _r = pluginIcoUrl();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { QString _r = chooseFile();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = saveAs((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { bool _r = exists((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

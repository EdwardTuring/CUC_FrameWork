HEADERS += \
    quazip/zlib.h \
    quazip/zip.h \
    quazip/zconf.h \
    quazip/unzip.h \
    quazip/quazipnewinfo.h \
    quazip/quazipfileinfo.h \
    quazip/quazipfile.h \
    quazip/quazip_global.h \
    quazip/quazip.h \
    quazip/quacrc32.h \
    quazip/quachecksum32.h \
    quazip/quaadler32.h \
    quazip/JlCompress.h \
    quazip/ioapi.h \
    quazip/crypt.h

SOURCES += \
    main.cpp
LIBS +=zdll.lib

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

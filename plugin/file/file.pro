TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += .. # "指定特殊的include path"
TARGET += file # "特别注意这里"
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
    fileplugin.h

SOURCES += \
    fileplugin.cpp


QT +=gui core

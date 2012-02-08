TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += .. # "指定特殊的include path"
TARGET += ftp # "特别注意这里"
DEPENDPATH += .
INCLUDEPATH += .
QT          +=network
HEADERS += \
    ftpplugin.h

SOURCES += \
    ftpplugin.cpp

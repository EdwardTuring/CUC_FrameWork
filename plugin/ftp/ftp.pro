TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += .. # "ָ�������include path"
TARGET += ftp # "�ر�ע������"
DEPENDPATH += .
INCLUDEPATH += .
QT          +=network
HEADERS += \
    ftpplugin.h

SOURCES += \
    ftpplugin.cpp

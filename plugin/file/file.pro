TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += .. # "ָ�������include path"
TARGET += file # "�ر�ע������"
DEPENDPATH += .
INCLUDEPATH += .

HEADERS += \
    fileplugin.h

SOURCES += \
    fileplugin.cpp


QT +=gui core

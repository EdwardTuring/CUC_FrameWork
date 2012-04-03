TEMPLATE = lib
CONFIG += plugin
#CONFIG +=console

INCLUDEPATH += .. # "指定特殊的include path"
TARGET += ftp # "特别注意这里"
DEPENDPATH += .
INCLUDEPATH += .
QT          +=network
HEADERS += \
    ftpplugin.h \
    tst_ftpplugin.h \
    tst_ftpdatahelper.h \
    ftpdatahelper.h

SOURCES += \
    ftpplugin.cpp \
    tst_main.cpp \
    tst_ftpplugin.cpp \
    ftpdatahelper.cpp \
    tst_ftpdatahelper.cpp
#DEFINES+=CUC_TEST

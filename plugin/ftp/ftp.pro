TEMPLATE = lib
CONFIG += plugin
#CONFIG +=console

INCLUDEPATH += .. # "ָ�������include path"
TARGET += ftp # "�ر�ע������"
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

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
    tst_ftpplugin.h

SOURCES += \
    ftpplugin.cpp \
    tst_ftp.cpp \
    tst_ftpplugin.cpp
#DEFINES+=CUC_TEST

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
    tst_ftpplugin.h

SOURCES += \
    ftpplugin.cpp \
    tst_ftp.cpp \
    tst_ftpplugin.cpp
#DEFINES+=CUC_TEST

QT +=network
TARGET =CUC_BCont_updater
SOURCES += \
    main.cpp \
    core/updater.cpp \
    ui/updatedialog.cpp

CONFIG +=console

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./bin
INCLUDEPATH +=../../core  ../../ ../pakger/include
LIBS += E:\Works\Git_Works\CUC_FrameWork\tool\pakger\lib\quazip.lib
RCC_DIR = ./rec
UI_DIR = ./ui
HEADERS += \
    core/updater.h \
    ui/updatedialog.h


RC_FILE = \
      icon.rc
DEFINES += _CUC_TEST_

FORMS += \
    ui/updatedialog.ui

RESOURCES += \
    res/updater_res.qrc

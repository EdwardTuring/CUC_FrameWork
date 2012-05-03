QT +=network
TARGET =updater
SOURCES += \
    main.cpp \
    core/updater.cpp \
    ../../core/configparser.cpp
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./bin
INCLUDEPATH +=../../core  ../../ ../pakger/include
LIBS += E:\Works\Git_Works\CUC_FrameWork\tool\pakger\lib\quazip.lib

HEADERS += \
    core/updater.h \
    ../../core/configparser.h \

DEFINES += _CUC_TEST_

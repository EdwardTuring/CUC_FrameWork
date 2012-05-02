QT +=network
TARGET =updater
SOURCES += \
    main.cpp \
    core/updater.cpp \
    ../../core/configparser.cpp
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./bin
INCLUDEPATH +=../../core  ../../
HEADERS += \
    core/updater.h \
    ../../core/configparser.h

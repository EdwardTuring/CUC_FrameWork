QT      +=  webkit network xml
INCLUDEPATH += ./
HEADERS =   \
    qt4.h \
    browser.h \
    gui/popupwindow.h \
    gui/confirmbox.h \
    gui/mainwindow.h \
    web/webpage.h \
    web/webview.h \
    gui/splashscreen.h \
    web/object/jsobjwindowx.h \
    gui/ui/slidewidget.h \
    gui/ui/repodialog.h \
    gui/ui/aboutdialog.h \
    core/configparser.h \
    core/coreerror.h \
    gui/ui/guidedialog.h \
    tool.h \
    core/networkmanager.h \
    test/tst_networkmanager.h \
    test/test.h \
    config.h
SOURCES =   main.cpp \
    browser.cpp \
    gui/popupwindow.cpp \
    gui/confirmbox.cpp \
    gui/mainwindow.cpp \
    web/webview.cpp \
    web/webpage.cpp \
    gui/splashscreen.cpp \
    web/object/jsobjwindowx.cpp \
    gui/ui/slidewidget.cpp \
    gui/ui/repodialog.cpp \
    gui/ui/aboutdialog.cpp \
    core/configparser.cpp \
    gui/ui/guidedialog.cpp \
    core/networkmanager.cpp \
    test/tst_networkmanager.cpp \
    test/test.cpp \
    tool.cpp
RESOURCES = \
    resource/icon.qrc
RC_FILE = \
      icon.rc
TARGET =mm_client
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./bin
RCC_DIR = ./rec
UI_DIR = ./ui
LIBS +=  $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/User32.lib)
FORMS += \
    gui/ui/slidewidget.ui \
    gui/ui/repodialog.ui \
    gui/ui/aboutdialog.ui \
    gui/ui/guidedialog.ui



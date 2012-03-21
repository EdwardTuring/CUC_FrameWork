QT      +=  webkit network
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
    core/coreerror.h
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
    core/configparser.cpp
RESOURCES = \
    resource/icon.qrc
TARGET =UICreator_Dev
# install
target.path = $$[QT_INSTALL_EXAMPLES]/webkit/fancybrowser
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/webkit/fancybrowser
INSTALLS += target sources
CONFIG   += console
symbian {
    TARGET.UID3 = 0xA000CF6C
    include($$PWD/../../symbianpkgrules.pri)
    TARGET.CAPABILITY = NetworkServices
}
maemo5: include($$PWD/../../maemo5pkgrules.pri)

OTHER_FILES += \
    icon.rc \
    README
RC_FILE = \
      icon.rc
CONFIG+=console
FORMS += \
    gui/ui/slidewidget.ui \
    gui/ui/repodialog.ui \
    gui/ui/aboutdialog.ui

DEFINES+=CUC_DEBUG CUC_TEST

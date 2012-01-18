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
    web/object/jsobjwindowx.h
SOURCES =   main.cpp \
    browser.cpp \
    gui/popupwindow.cpp \
    gui/confirmbox.cpp \
    gui/mainwindow.cpp \
    web/webview.cpp \
    web/webpage.cpp \
    gui/splashscreen.cpp \
    web/object/jsobjwindowx.cpp
RESOURCES = \
    resource/icon.qrc
TARGET =UICreator
# install
target.path = $$[QT_INSTALL_EXAMPLES]/webkit/fancybrowser
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/webkit/fancybrowser
INSTALLS += target sources

symbian {
    TARGET.UID3 = 0xA000CF6C
    include($$PWD/../../symbianpkgrules.pri)
    TARGET.CAPABILITY = NetworkServices
}
maemo5: include($$PWD/../../maemo5pkgrules.pri)

OTHER_FILES += \
    icon.rc
RC_FILE = \
      icon.rc

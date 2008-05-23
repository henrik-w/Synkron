TEMPLATE = app
QT += network xml
HEADERS += main_window.h \
    syncpage.h \
    mtfile.h \
    mtadvancedgroupbox.h \
    extendedlineedit.h \
    extendedlistwidget.h \
    extendedtreewidget.h \
    syncfolders.h \
    mtstringset.h
FORMS += main_window.ui \
    about.ui \
    multisync_page.ui \
    sync_view_item.ui
SOURCES += main.cpp \
    sync.cpp \
    restore.cpp \
    blacklist.cpp \
    multisync.cpp \
    scheduler.cpp \
    filters.cpp \
    mtfile.cpp \
    syncfolders.cpp \
    syncview.cpp \
    analyse.cpp
RESOURCES += resources.qrc \
    i18n.qrc
win32:RC_FILE = Synkron.rc
DESTDIR = ./
unix:!macx {
    DEFINES += USE_UNIX_TOUCH_COMMAND
    TARGET = synkron
    OBJECTS_DIR = .tmp.unix/
    MOC_DIR = .tmp.unix/
    RCC_DIR = .tmp.unix/
}
macx {
    # Comment the following line to use the NSFileManager class
    # to perform copy operations instead of QFile + /usr/bin/touch:
    DEFINES += USE_UNIX_TOUCH_COMMAND
    SOURCES += mtfile_macx.mm
    ICON = images/Synkron128.icns
    icons.path = Contents/Resources
    icons.files = images/slist.icns
    QMAKE_BUNDLE_DATA += icons
    CONFIG += x86 ppc
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
    QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
}
win32 {
    # If not commented, the following line ensures that Synkron is compiled
    # as a portable app. More info at portableapps.com
    # DEFINES += PORTABLE_APP
    OBJECTS_DIR = .tmp.win32/
    MOC_DIR = .tmp.win32/
    RCC_DIR = .tmp.win32/
}
TRANSLATIONS += i18n/Synkron-Slovak.ts \
    i18n/Synkron-German.ts \
    i18n/Synkron-Japanese.ts \
    i18n/Synkron-Arabic.ts \
    i18n/Synkron-Russian.ts \
    i18n/Synkron-Spanish.ts \
    i18n/Synkron-Brazilian_Portuguese.ts

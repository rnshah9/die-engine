QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = die

XCONFIG += use_dex
XCONFIG += use_pdf
XCONFIG += use_archive

macx {
    TARGET = DiE
}

#linux {
#    TARGET = dieg
#}

SOURCES += \
    dialogabout.cpp \
    dialogoptions.cpp \
    guimainwindow.cpp \
    main_gui.cpp

HEADERS += \
    dialogabout.h \
    dialogoptions.h \
    guimainwindow.h

FORMS += \
    dialogabout.ui \
    dialogoptions.ui \
    guimainwindow.ui

include(../build.pri)

!contains(XCONFIG, xmimewidget) {
    XCONFIG += xmimewidget
    include(../XMIMEWidget/xmimewidget.pri)
}

!contains(XCONFIG, formatwidgets) {
    XCONFIG += formatwidgets
    include(../FormatWidgets/formatwidgets.pri)
}

!contains(XCONFIG, xsingleapplication) {
    XCONFIG += xsingleapplication
    include(../XSingleApplication/xsingleapplication.pri)
}

!contains(XCONFIG, xaboutwidget) {
    XCONFIG += xaboutwidget
    include(../XAboutWidget/xaboutwidget.pri)
}

RESOURCES += \
    res.qrc

win32 {
    RC_ICONS = ../icons/main.ico
    CONFIG -= embed_manifest_exe
    QMAKE_MANIFEST = windows.manifest.xml
    VERSION = 3.06.0.0
    QMAKE_TARGET_COMPANY = NTInfo
    QMAKE_TARGET_PRODUCT = Detect It Easy
    QMAKE_TARGET_DESCRIPTION = Detect It Easy is a program for determining types of files.
    QMAKE_TARGET_COPYRIGHT = horsicq@gmail.com
}

macx {
    ICON = ../icons/main.icns
}

DISTFILES += \
    ../CMakeLists.txt \
    ../README.md \
    ../release_version.txt \
    CMakeLists.txt

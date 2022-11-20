TEMPLATE = lib

QT += core
QT -= gui

CONFIG += c++11
CONFIG += staticlib

TARGET = diec
CONFIG += console
CONFIG -= app_bundle

include(../build.pri)

XCONFIG += use_capstone_x86

SOURCES += \
        consoleoutput.cpp

HEADERS += \
    consoleoutput.h

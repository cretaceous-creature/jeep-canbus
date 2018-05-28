#-------------------------------------------------
#
# Project created by QtCreator 2017-04-18T23:03:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CanCollector
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    mainwindow.cpp \
    candatastorage.cpp

HEADERS  += mainwindow.h \
    candatastorage.h

FORMS    += mainwindow.ui

win32 {
HEADERS  += controlcan_win32.h
LIBS += $$PWD/ControlCAN.lib
}

unix {
HEADERS  += controlcan_unix.h
LIBS += $$PWD/libcontrolcan.so
}


TRANSLATIONS = zh_CN.ts

RC_ICONS = icon.ico

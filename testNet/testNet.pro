QT += core
QT += network
QT -= gui

TARGET = testNet
CONFIG += console
CONFIG += debug
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    netobject.cpp \
    client.cpp \
    rootserver.cpp

HEADERS += \
    netobject.h \
    client.h \
    rootserver.h


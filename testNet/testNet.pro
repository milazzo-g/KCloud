QT += core
QT += network
QT -= gui

TARGET = testNet
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    netobject.cpp \
    client.cpp

HEADERS += \
    netobject.h \
    client.h


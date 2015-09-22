QT += core
QT += network
QT -= gui

TARGET = clientServer
CONFIG += console
CONFIG += debug
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ClientServer.cpp \
    Console.cpp

HEADERS += \
    ClientServer.h \
    Console.h


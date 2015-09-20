QT += core
QT += network
QT -= gui

TARGET = NetObject_v_1_0
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    NetObject.cpp \
    Resource.cpp \
    FolderCompressor.cpp

HEADERS += \
    NetObject.h \
    Resource.h \
    FolderCompressor.h


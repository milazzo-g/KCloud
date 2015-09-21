QT += core
QT += sql
QT -= gui

TARGET = DatabaseManager
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    DatabaseManager.cpp

HEADERS += \
    DatabaseManager.h


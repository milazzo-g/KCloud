#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:51:31
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = DatabaseManager
TEMPLATE = lib

DEFINES += DATABASEMANAGER_LIBRARY

SOURCES += DatabaseManager.cpp

HEADERS += DatabaseManager.h\
        databasemanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

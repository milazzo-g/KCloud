#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T10:12:52
#
#-------------------------------------------------

QT       -= gui

TARGET = Resources
TEMPLATE = lib

DEFINES += RESOURCES_LIBRARY

SOURCES += resources.cpp

HEADERS += resources.h\
        resources_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

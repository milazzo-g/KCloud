#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T16:39:07
#
#-------------------------------------------------

QT       -= gui

TARGET = Resource
TEMPLATE = lib

DEFINES += RESOURCE_LIBRARY

SOURCES += Resource.cpp

HEADERS += Resource.h\
        resource_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

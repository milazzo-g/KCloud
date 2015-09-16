#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T10:12:52
#
#-------------------------------------------------

QT       -= gui

TARGET = Resources
TEMPLATE = lib

DEFINES += RESOURCES_LIBRARY

SOURCES += resources.cpp \
    FolderCompressor.cpp

HEADERS += resources.h\
        resources_global.h \
    FolderCompressor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

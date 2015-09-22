#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:03:51
#
#-------------------------------------------------

QT       -= gui

TARGET = Exceptions
TEMPLATE = lib

DEFINES += EXCEPTIONS_LIBRARY

SOURCES += Exceptions.cpp

HEADERS += Exceptions.h\
        exceptions_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

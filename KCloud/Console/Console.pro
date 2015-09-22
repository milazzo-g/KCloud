#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:49:09
#
#-------------------------------------------------

QT       -= gui

TARGET = Console
TEMPLATE = lib

DEFINES += CONSOLE_LIBRARY

SOURCES += Console.cpp

HEADERS += Console.h\
        console_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

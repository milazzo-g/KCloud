#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T19:41:11
#
#-------------------------------------------------

QT       -= gui

TARGET = User
TEMPLATE = lib

DEFINES += USER_LIBRARY

SOURCES += user.cpp

HEADERS += user.h\
        user_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

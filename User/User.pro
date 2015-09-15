#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T14:36:34
#
#-------------------------------------------------

QT       -= gui

TARGET = User
TEMPLATE = lib

DEFINES += USER_LIBRARY

SOURCES += User.cpp

HEADERS += User.h\
        user_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

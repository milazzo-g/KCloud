#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T15:54:47
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = NetObject
TEMPLATE = lib

DEFINES += NETOBJECT_LIBRARY

SOURCES += NetObject.cpp

HEADERS += NetObject.h\
        netobject_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/release/ -lExceptions
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/debug/ -lExceptions
else:unix: LIBS += -L$$OUT_PWD/../Exceptions/ -lExceptions

INCLUDEPATH += $$PWD/../Exceptions
DEPENDPATH += $$PWD/../Exceptions

#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T17:05:58
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Client
TEMPLATE = lib

DEFINES += CLIENT_LIBRARY

SOURCES += Client.cpp

HEADERS += Client.h\
        client_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/release/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/debug/ -lEngine
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Console/release/ -lConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Console/debug/ -lConsole
else:unix: LIBS += -L$$OUT_PWD/../Console/ -lConsole

INCLUDEPATH += $$PWD/../Console
DEPENDPATH += $$PWD/../Console

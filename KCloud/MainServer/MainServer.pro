#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T17:03:48
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = MainServer
TEMPLATE = lib

DEFINES += MAINSERVER_LIBRARY

SOURCES += MainServer.cpp

HEADERS += MainServer.h\
        mainserver_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Console/release/ -lConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Console/debug/ -lConsole
else:unix: LIBS += -L$$OUT_PWD/../Console/ -lConsole

INCLUDEPATH += $$PWD/../Console
DEPENDPATH += $$PWD/../Console

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../WorkerServer/release/ -lWorkerServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../WorkerServer/debug/ -lWorkerServer
else:unix: LIBS += -L$$OUT_PWD/../WorkerServer/ -lWorkerServer

INCLUDEPATH += $$PWD/../WorkerServer
DEPENDPATH += $$PWD/../WorkerServer

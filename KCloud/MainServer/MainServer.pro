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
        mainserver_global.h \
    defines.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/release/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/debug/ -lEngine
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Resource/release/ -lResource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Resource/debug/ -lResource
else:unix: LIBS += -L$$OUT_PWD/../Resource/ -lResource

INCLUDEPATH += $$PWD/../Resource
DEPENDPATH += $$PWD/../Resource

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommandPacket/release/ -lCommandPacket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommandPacket/debug/ -lCommandPacket
else:unix: LIBS += -L$$OUT_PWD/../CommandPacket/ -lCommandPacket

INCLUDEPATH += $$PWD/../CommandPacket
DEPENDPATH += $$PWD/../CommandPacket

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../User/release/ -lUser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../User/debug/ -lUser
else:unix: LIBS += -L$$OUT_PWD/../User/ -lUser

INCLUDEPATH += $$PWD/../User
DEPENDPATH += $$PWD/../User

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetObject/release/ -lNetObject
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetObject/debug/ -lNetObject
else:unix: LIBS += -L$$OUT_PWD/../NetObject/ -lNetObject

INCLUDEPATH += $$PWD/../NetObject
DEPENDPATH += $$PWD/../NetObject

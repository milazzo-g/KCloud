#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T17:01:32
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Engine
TEMPLATE = lib

DEFINES += ENGINE_LIBRARY

SOURCES += Engine.cpp

HEADERS += Engine.h\
        engine_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

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

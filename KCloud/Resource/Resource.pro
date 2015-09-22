#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:13:24
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = Resource
TEMPLATE = lib

DEFINES += RESOURCE_LIBRARY

SOURCES += Resource.cpp

HEADERS += Resource.h\
        resource_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetObject/release/ -lNetObject
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetObject/debug/ -lNetObject
else:unix: LIBS += -L$$OUT_PWD/../NetObject/ -lNetObject

INCLUDEPATH += $$PWD/../NetObject
DEPENDPATH += $$PWD/../NetObject

linux{
    message("Utilizzo configurazione Linux")
    LIBS += -L$$PWD/../LIBS/quazip-0.7.1_build/linux/ -lquazip
}

macx{
    message("Utilizzo configurazione MacOSX")
    LIBS += -L$$PWD/../LIBS/quazip-0.7.1_build/mac/ -lquazip
}

INCLUDEPATH += $$PWD/../LIBS/quazip-0.7.1_build/include
DEPENDPATH += $$PWD/../LIBS/quazip-0.7.1_build/include






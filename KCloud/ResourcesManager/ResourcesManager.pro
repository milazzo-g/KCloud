#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:57:42
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = ResourcesManager
TEMPLATE = lib

DEFINES += RESOURCESMANAGER_LIBRARY

SOURCES += ResourcesManager.cpp

HEADERS += ResourcesManager.h\
        resourcesmanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DatabaseManager/release/ -lDatabaseManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DatabaseManager/debug/ -lDatabaseManager
else:unix: LIBS += -L$$OUT_PWD/../DatabaseManager/ -lDatabaseManager

INCLUDEPATH += $$PWD/../DatabaseManager
DEPENDPATH += $$PWD/../DatabaseManager

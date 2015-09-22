#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T17:07:28
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = WorkerServer
TEMPLATE = lib

DEFINES += WORKERSERVER_LIBRARY

SOURCES += WorkerServer.cpp

HEADERS += WorkerServer.h\
        workerserver_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ResourcesManager/release/ -lResourcesManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ResourcesManager/debug/ -lResourcesManager
else:unix: LIBS += -L$$OUT_PWD/../ResourcesManager/ -lResourcesManager

INCLUDEPATH += $$PWD/../ResourcesManager
DEPENDPATH += $$PWD/../ResourcesManager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UsersManager/release/ -lUsersManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UsersManager/debug/ -lUsersManager
else:unix: LIBS += -L$$OUT_PWD/../UsersManager/ -lUsersManager

INCLUDEPATH += $$PWD/../UsersManager
DEPENDPATH += $$PWD/../UsersManager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/release/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/debug/ -lEngine
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine

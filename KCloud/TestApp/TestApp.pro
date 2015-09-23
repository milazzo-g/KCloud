QT += core
QT += sql
QT += network
QT -= gui

TARGET = TestApp
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MainServer/release/ -lMainServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MainServer/debug/ -lMainServer
else:unix: LIBS += -L$$OUT_PWD/../MainServer/ -lMainServer

INCLUDEPATH += $$PWD/../MainServer
DEPENDPATH += $$PWD/../MainServer

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommandPacket/release/ -lCommandPacket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommandPacket/debug/ -lCommandPacket
else:unix: LIBS += -L$$OUT_PWD/../CommandPacket/ -lCommandPacket

INCLUDEPATH += $$PWD/../CommandPacket
DEPENDPATH += $$PWD/../CommandPacket

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DatabaseManager/release/ -lDatabaseManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DatabaseManager/debug/ -lDatabaseManager
else:unix: LIBS += -L$$OUT_PWD/../DatabaseManager/ -lDatabaseManager

INCLUDEPATH += $$PWD/../DatabaseManager
DEPENDPATH += $$PWD/../DatabaseManager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Resource/release/ -lResource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Resource/debug/ -lResource
else:unix: LIBS += -L$$OUT_PWD/../Resource/ -lResource

INCLUDEPATH += $$PWD/../Resource
DEPENDPATH += $$PWD/../Resource

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/release/ -lExceptions
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/debug/ -lExceptions
else:unix: LIBS += -L$$OUT_PWD/../Exceptions/ -lExceptions

INCLUDEPATH += $$PWD/../Exceptions
DEPENDPATH += $$PWD/../Exceptions

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NetObject/release/ -lNetObject
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NetObject/debug/ -lNetObject
else:unix: LIBS += -L$$OUT_PWD/../NetObject/ -lNetObject

INCLUDEPATH += $$PWD/../NetObject
DEPENDPATH += $$PWD/../NetObject

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ResourceHeader/release/ -lResourceHeader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ResourceHeader/debug/ -lResourceHeader
else:unix: LIBS += -L$$OUT_PWD/../ResourceHeader/ -lResourceHeader

INCLUDEPATH += $$PWD/../ResourceHeader
DEPENDPATH += $$PWD/../ResourceHeader

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../User/release/ -lUser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../User/debug/ -lUser
else:unix: LIBS += -L$$OUT_PWD/../User/ -lUser

INCLUDEPATH += $$PWD/../User
DEPENDPATH += $$PWD/../User

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Client/release/ -lClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Client/debug/ -lClient
else:unix: LIBS += -L$$OUT_PWD/../Client/ -lClient

INCLUDEPATH += $$PWD/../Client
DEPENDPATH += $$PWD/../Client

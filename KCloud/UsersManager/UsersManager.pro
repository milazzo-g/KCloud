#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:53:19
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = UsersManager
TEMPLATE = lib

DEFINES += USERSMANAGER_LIBRARY

SOURCES += UsersManager.cpp

HEADERS += UsersManager.h\
        usersmanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../User/release/ -lUser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../User/debug/ -lUser
else:unix: LIBS += -L$$OUT_PWD/../User/ -lUser

INCLUDEPATH += $$PWD/../User
DEPENDPATH += $$PWD/../User

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DatabaseManager/release/ -lDatabaseManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DatabaseManager/debug/ -lDatabaseManager
else:unix: LIBS += -L$$OUT_PWD/../DatabaseManager/ -lDatabaseManager

INCLUDEPATH += $$PWD/../DatabaseManager
DEPENDPATH += $$PWD/../DatabaseManager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/release/ -lExceptions
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/debug/ -lExceptions
else:unix: LIBS += -L$$OUT_PWD/../Exceptions/ -lExceptions

INCLUDEPATH += $$PWD/../Exceptions
DEPENDPATH += $$PWD/../Exceptions

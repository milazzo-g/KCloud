#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:51:31
#
#-------------------------------------------------

QT       += sql
QT       -= gui

TARGET = DatabaseManager
TEMPLATE = lib

DEFINES += DATABASEMANAGER_LIBRARY

SOURCES += DatabaseManager.cpp

HEADERS += DatabaseManager.h\
        databasemanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/release/ -lExceptions
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/debug/ -lExceptions
else:unix: LIBS += -L$$OUT_PWD/../Exceptions/ -lExceptions

INCLUDEPATH += $$PWD/../Exceptions
DEPENDPATH += $$PWD/../Exceptions

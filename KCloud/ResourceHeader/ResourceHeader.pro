#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:25:26
#
#-------------------------------------------------

QT       -= gui

CONFIG   +=c++11

TARGET = ResourceHeader
TEMPLATE = lib

DEFINES += RESOURCEHEADER_LIBRARY

SOURCES += ResourceHeader.cpp

HEADERS += ResourceHeader.h\
        resourceheader_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../User/release/ -lUser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../User/debug/ -lUser
else:unix: LIBS += -L$$OUT_PWD/../User/ -lUser

INCLUDEPATH += $$PWD/../User
DEPENDPATH += $$PWD/../User

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/release/ -lExceptions
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/debug/ -lExceptions
else:unix: LIBS += -L$$OUT_PWD/../Exceptions/ -lExceptions

INCLUDEPATH += $$PWD/../Exceptions
DEPENDPATH += $$PWD/../Exceptions

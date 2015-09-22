#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T16:30:02
#
#-------------------------------------------------

QT       -= gui

CONFIG   +=c++11

TARGET = User
TEMPLATE = lib

DEFINES += USER_LIBRARY

SOURCES += User.cpp

HEADERS += User.h\
        user_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/release/ -lExceptions
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/debug/ -lExceptions
else:unix: LIBS += -L$$OUT_PWD/../Exceptions/ -lExceptions

INCLUDEPATH += $$PWD/../Exceptions
DEPENDPATH += $$PWD/../Exceptions

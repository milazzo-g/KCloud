QT += core
QT += network
QT -= gui

TARGET = NetObject_v_1_0
CONFIG += console
CONFIG += debug
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    NetObject.cpp \
    Resource.cpp \
    clientserver.cpp

HEADERS += \
    NetObject.h \
    Resource.h \
    clientserver.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../LIBS/quazip-0.7.1_build/lib/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../LIBS/quazip-0.7.1_build/lib/debug/ -lquazip
else:unix: LIBS += -L$$PWD/../../LIBS/quazip-0.7.1_build/lib/ -lquazip

INCLUDEPATH += $$PWD/../../LIBS/quazip-0.7.1_build/include
DEPENDPATH += $$PWD/../../LIBS/quazip-0.7.1_build/include

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
    clientserver.cpp \
    Commandpacket.cpp

HEADERS += \
    NetObject.h \
    Resource.h \
    clientserver.h \
    Commandpacket.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Desktop/Quazip/lib/release/ -lquazip.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Desktop/Quazip/lib/debug/ -lquazip.1.0.0
else:unix: LIBS += -L$$PWD/../../../Desktop/Quazip/lib/ -lquazip.1.0.0

INCLUDEPATH += $$PWD/../../../Desktop/Quazip/include
DEPENDPATH += $$PWD/../../../Desktop/Quazip/include

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



linux{
    message("Utilizzo configurazione Linux")
    LIBS += -L$$PWD/../LIBS/quazip-0.7.1_build/linux/lib/ -lquazip
    INCLUDEPATH += $$PWD/../LIBS/quazip-0.7.1_build/linux/include
    DEPENDPATH += $$PWD/../LIBS/quazip-0.7.1_build/linux/include
}
macx{
    message("Utilizzo configurazione MacOSX")
    LIBS += -L$$PWD/../LIBS/quazip-0.7.1_build/mac/lib/ -lquazip
    INCLUDEPATH += $$PWD/../LIBS/quazip-0.7.1_build/mac/include
    DEPENDPATH += $$PWD/../LIBS/quazip-0.7.1_build/mac/include
}


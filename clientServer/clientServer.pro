QT += core
QT += network
QT -= gui

TARGET = clientServer
CONFIG += console
CONFIG += debug
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ClientServer.cpp \
    Console.cpp \
    ../NetObject_v_1_0/clientserver.cpp \
    ../NetObject_v_1_0/Commandpacket.cpp \
    ../NetObject_v_1_0/NetObject.cpp \
    ../NetObject_v_1_0/Resource.cpp \
    ../NetObject_v_1_0/ResourceHeader.cpp \
    ../NetObject_v_1_0/User.cpp \
    ../Exceptions/Exceptions.cpp \
    ../DatabaseManager/DatabaseManager.cpp

HEADERS += \
    ClientServer.h \
    Console.h \
    ../NetObject_v_1_0/clientserver.h \
    ../NetObject_v_1_0/Commandpacket.h \
    ../NetObject_v_1_0/NetObject.h \
    ../NetObject_v_1_0/Resource.h \
    ../NetObject_v_1_0/ResourceHeader.h \
    ../NetObject_v_1_0/User.h \
    ../Exceptions/Exceptions.h \
    ../DatabaseManager/DatabaseManager.h


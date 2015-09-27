#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T10:12:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicClient
TEMPLATE = app


SOURCES += main.cpp\
        GuiClient.cpp \
    LoginForm.cpp \
    FirstConfigForm.cpp

HEADERS  += GuiClient.h \
    LoginForm.h \
    FirstConfigForm.h

FORMS    += GuiClient.ui \
    LoginForm.ui \
    FirstConfigForm.ui

RESOURCES += \
    icons.qrc

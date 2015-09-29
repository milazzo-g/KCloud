#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T10:12:12
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicClient
TEMPLATE = app


SOURCES += main.cpp\
        GuiClient.cpp \
    LoginForm.cpp \
    FirstConfigForm.cpp \
    GraphicResourceHeader.cpp \
    Loader.cpp \
    Waiter.cpp \
    PermSettings.cpp

HEADERS  += GuiClient.h \
    LoginForm.h \
    FirstConfigForm.h \
    GraphicResourceHeader.h \
    Loader.h \
    Waiter.h \
    PermSettings.h

FORMS    += GuiClient.ui \
    LoginForm.ui \
    FirstConfigForm.ui \
    Loader.ui \
    Waiter.ui \
    PermSettings.ui

RESOURCES += \
    projectResources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Client/release/ -lClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Client/debug/ -lClient
else:unix: LIBS += -L$$OUT_PWD/../Client/ -lClient

INCLUDEPATH += $$PWD/../Client
DEPENDPATH += $$PWD/../Client

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommandPacket/release/ -lCommandPacket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommandPacket/debug/ -lCommandPacket
else:unix: LIBS += -L$$OUT_PWD/../CommandPacket/ -lCommandPacket

INCLUDEPATH += $$PWD/../CommandPacket
DEPENDPATH += $$PWD/../CommandPacket

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../User/release/ -lUser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../User/debug/ -lUser
else:unix: LIBS += -L$$OUT_PWD/../User/ -lUser

INCLUDEPATH += $$PWD/../User
DEPENDPATH += $$PWD/../User

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Console/release/ -lConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Console/debug/ -lConsole
else:unix: LIBS += -L$$OUT_PWD/../Console/ -lConsole

INCLUDEPATH += $$PWD/../Console
DEPENDPATH += $$PWD/../Console

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/release/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/debug/ -lEngine
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

INCLUDEPATH += $$PWD/../Engine
DEPENDPATH += $$PWD/../Engine

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/release/ -lExceptions
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Exceptions/debug/ -lExceptions
else:unix: LIBS += -L$$OUT_PWD/../Exceptions/ -lExceptions

INCLUDEPATH += $$PWD/../Exceptions
DEPENDPATH += $$PWD/../Exceptions

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Resource/release/ -lResource
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Resource/debug/ -lResource
else:unix: LIBS += -L$$OUT_PWD/../Resource/ -lResource

INCLUDEPATH += $$PWD/../Resource
DEPENDPATH += $$PWD/../Resource

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LIBS/quazip-0.7.1_build/linux/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LIBS/quazip-0.7.1_build/linux/debug/ -lquazip
else:unix: LIBS += -L$$PWD/../LIBS/quazip-0.7.1_build/linux/ -lquazip

INCLUDEPATH += $$PWD/../LIBS/quazip-0.7.1_build/linux
DEPENDPATH += $$PWD/../LIBS/quazip-0.7.1_build/linux

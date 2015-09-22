QT += core
QT -= gui

TARGET = TestApp
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MainServer/release/ -lMainServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MainServer/debug/ -lMainServer
else:unix: LIBS += -L$$OUT_PWD/../MainServer/ -lMainServer

INCLUDEPATH += $$PWD/../MainServer
DEPENDPATH += $$PWD/../MainServer

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Console/release/ -lConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Console/debug/ -lConsole
else:unix: LIBS += -L$$OUT_PWD/../Console/ -lConsole

INCLUDEPATH += $$PWD/../Console
DEPENDPATH += $$PWD/../Console

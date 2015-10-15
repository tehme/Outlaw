#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T00:50:43
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = Chat
TEMPLATE = app
CONFIG += c++11

include(../../BuiltDir.pri)
DESTDIR = ../../$${BUILTDIR}/Sketches

INCLUDEPATH += ../../Libs
LIBS += -L../../$${BUILTDIR}/Libs -lNetworkClient


SOURCES += main.cpp\
        MainWindow.cpp \
    GameState.cpp

HEADERS  += MainWindow.hpp \
    GameState.hpp

FORMS    += MainWindow.ui

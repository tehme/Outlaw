#-------------------------------------------------
#
# Project created by QtCreator 2015-09-20T02:41:16
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = TestMcClient
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS += \
    TcpClient.hpp \
    MessageBuffer.hpp \
    VarInt.hpp \
    BinaryUtils.hpp \
    BaseGameState.hpp \
    TestGameState.hpp

SOURCES += \
    TestMcClient_Main.cpp \
    TcpClient.cpp \
    MessageBuffer.cpp \
    VarInt.cpp \
    BinaryUtils.cpp \
    BaseGameState.cpp \
    TestGameState.cpp

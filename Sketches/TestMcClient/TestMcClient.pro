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


SOURCES += \
    TestMcClient_Main.cpp \
    MinecraftClient.cpp \
    MessageBuffer.cpp \
    VarInt.cpp

HEADERS += \
    MinecraftClient.hpp \
    MessageBuffer.hpp \
    VarInt.hpp

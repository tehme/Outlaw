#-------------------------------------------------
#
# Project created by QtCreator 2015-09-20T02:41:16
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = TestMcClient
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

include(../../BuiltDir.pri)

DESTDIR = ../../$${BUILTDIR}/Sketches


INCLUDEPATH += ../../Libs
LIBS += -L../../$${BUILTDIR}/Libs -lNetworkClient

HEADERS += \
    BinaryUtils.hpp \
    TestGameState.hpp \
    MobRadarWidget.hpp \
    StatsWidget.hpp

SOURCES += \
    TestMcClient_Main.cpp \
    BinaryUtils.cpp \
    TestGameState.cpp \
    MobRadarWidget.cpp \
    StatsWidget.cpp

FORMS += \
    MobRadarWidget.ui \
    StatsWidget.ui

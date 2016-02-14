#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T02:36:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChunkRenderer
TEMPLATE = app
CONFIG += c++11

include(../../BuiltDir.pri)
DESTDIR = ../../$${BUILTDIR}/Sketches


HEADERS += \
    MainWindow.hpp \
    OpenGLWidget.hpp \
    ChunkData.hpp

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    OpenGLWidget.cpp \
    ChunkData.cpp

FORMS += \
    MainWindow.ui

RESOURCES += \
    res.qrc

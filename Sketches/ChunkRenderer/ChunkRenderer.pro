#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T02:36:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChunkRenderer
TEMPLATE = app


HEADERS  += MainWindow.hpp \
    OpenGLWidget.hpp

SOURCES +=\
    main.cpp \
    MainWindow.cpp \
    OpenGLWidget.cpp

FORMS    += MainWindow.ui

#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

LIBS += -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pro07-vbo
TEMPLATE = app


SOURCES += main.cpp\
        princ.cpp \
        glarea.cpp \
        cylindre.cpp

HEADERS  += princ.h \
        glarea.h \
        cylindre.h

FORMS    += princ.ui

RESOURCES += \
    pro07-vbo.qrc

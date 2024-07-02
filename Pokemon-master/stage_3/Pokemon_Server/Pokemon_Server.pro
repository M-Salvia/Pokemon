#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T15:41:17
#
#-------------------------------------------------

QT       += core gui widgets network sql testlib


TARGET = Pokemon_Server
TEMPLATE = app


SOURCES += main.cpp\
    Enity.cpp \
    kind.cpp \
        mainwindow.cpp \
    pokemon.cpp

HEADERS  += mainwindow.h \
    Enity.h \
    kind.h \
    pokemon.h

FORMS    += mainwindow.ui

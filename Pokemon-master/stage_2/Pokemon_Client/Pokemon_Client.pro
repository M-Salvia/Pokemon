#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T15:41:17
#
#-------------------------------------------------

QT       += core gui widgets network sql testlib

TARGET = Pokemon_Client
TEMPLATE = app


SOURCES += main.cpp\
    Enity.cpp \
    home.cpp \
    kind.cpp \
        mainwindow.cpp \
    pokemon.cpp \
    login.cpp

HEADERS  += mainwindow.h \
    Enity.h \
    home.h \
    kind.h \
    pokemon.h \
    login.h

FORMS    += mainwindow.ui \
    home.ui \
    login.ui

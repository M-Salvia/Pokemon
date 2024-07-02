
QT       += core gui widgets network sql testlib


TARGET = Pokemon_Client
TEMPLATE = app


SOURCES += main.cpp\
    Enity.cpp \
    battlewindow.cpp \
    home.cpp \
    kind.cpp \
        mainwindow.cpp \
    pokemon.cpp \
    login.cpp

HEADERS  += mainwindow.h \
    Enity.h \
    battlewindow.h \
    home.h \
    kind.h \
    pokemon.h \
    login.h

FORMS    += mainwindow.ui \
    battlewindow.ui \
    home.ui \
    login.ui

RESOURCES += \
    image.qrc \
    music.qrc

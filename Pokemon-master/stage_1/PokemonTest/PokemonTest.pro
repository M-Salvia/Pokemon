QT += testlib
QT += widgets
QT += gui

CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

INCLUDEPATH += \
                                ../Pokemon

LIBS += \
                -L../Pokemon \
                -lPokemon



HEADERS += \
    mainwindow.h \
    pokemon.h \
    Enity.h \
    kind.h \
    ui_mainwindow.h

# Define the source files
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pokemon.cpp \
    Enity.cpp \
    kind.cpp \
     Button_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

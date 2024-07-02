# Pokemon.pro
# Define the target application name
TARGET = Pokemon

# Define the Qt modules your application uses
QT += core gui widgets


# Define the header files
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
    kind.cpp

# Define the UI files
FORMS += \
    mainwindow.ui


#-------------------------------------------------
#
# Project created by QtCreator 2018-03-02T22:14:50
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = blockeditor
TEMPLATE = app

OBJECTS += ../*.o

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -std=c++14

SOURCES += \
    ui_block.cpp \
    ui_main.cpp \
    ui_connection.cpp \
    mainwindow.cpp \
    ui_blockscheme.cpp \
    ui_blocklist.cpp \
    ui_compute.cpp \
    ui_port.cpp


HEADERS += \
    ui_block.hpp \
    ui_cursor.hpp \
    ui_connection.hpp \
    mainwindow.hpp \
    ui_blockscheme.hpp \
    ui_blocklist.hpp \
    ui_blocktype.hpp \
    ui_compute.hpp \
    ui_forward_declare.hpp \
    ui_port.hpp

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc

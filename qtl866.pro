#-------------------------------------------------
#
# Project created by QtCreator 2014-05-04T10:50:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtl866
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    optdialog.cpp

HEADERS  += mainwindow.h \
    optdialog.h \
    devices.h

FORMS    += mainwindow.ui \
    optdialog.ui

OTHER_FILES += \
    README.md \
    COPYING

target.path = /usr/bin/
INSTALLS += target

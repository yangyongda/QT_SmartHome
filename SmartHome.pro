#-------------------------------------------------
#
# Project created by QtCreator 2016-10-10T20:35:31
#
#-------------------------------------------------

QT       += core gui
QT += serialport
QT += network
RC_ICONS += ICON.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHome
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialportdialog.cpp \
    tcpdialog.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    serialportdialog.h \
    tcpdialog.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    serialportdialog.ui \
    tcpdialog.ui \
    aboutdialog.ui

RESOURCES += \
    resource.qrc

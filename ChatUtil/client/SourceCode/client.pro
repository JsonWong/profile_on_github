#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T09:14:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlglogin.cpp \
    dlgregister.cpp \
    dlgjoingroup.cpp \
    dlgsendmsg.cpp

HEADERS  += mainwindow.h \
    dlglogin.h \
    dlgregister.h \
    dlgjoingroup.h \
    dlgsendmsg.h

FORMS    += mainwindow.ui \
    dlglogin.ui \
    dlgregister.ui \
    dlgjoingroup.ui \
    dlgsendmsg.ui

QT += network

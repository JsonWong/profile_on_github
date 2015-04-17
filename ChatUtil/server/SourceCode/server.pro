TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    userip.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    userip.h


#-------------------------------------------------
#
# Project created by QtCreator 2021-05-04T14:09:16
#
#-------------------------------------------------

QT       += core gui
QT  +=  core  gui  axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sniff_out
TEMPLATE = app


SOURCES += main.cpp\
        sniffout.cpp \
    mythread.cpp

HEADERS  += sniffout.h \
    mythread.h

FORMS    += sniffout.ui

RESOURCES += \
    res.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T22:28:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calendar
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    daywidget.cpp \
    labelbutton.cpp \
    addeventdialog.cpp \
    abstractevent.cpp \
    continuousevent.cpp \
    recurrentevent.cpp \
    daydetaildialog.cpp \
    colormenu.cpp

HEADERS  += mainwindow.h \
    daywidget.h \
    const.h \
    labelbutton.h \
    addeventdialog.h \
    abstractevent.h \
    continuousevent.h \
    recurrentevent.h \
    daydetaildialog.h \
    colormenu.h

FORMS    += forms/mainwindow.ui \
    forms/addeventdialog.ui \
    forms/weekrepeatwidget.ui \
    forms/monthrepeatwidget.ui \
    forms/yearrepeatwidget.ui \
    forms/daydetaildialog.ui

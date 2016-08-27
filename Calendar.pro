#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T22:28:35
#
#-------------------------------------------------

QT       += core gui widgets xml

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
    colormenu.cpp \
    filelistwidget.cpp \
    dateselectdialog.cpp \
    setting.cpp \
    preferencedialog.cpp \
    translator.cpp

HEADERS  += mainwindow.h \
    daywidget.h \
    const.h \
    labelbutton.h \
    addeventdialog.h \
    abstractevent.h \
    continuousevent.h \
    recurrentevent.h \
    daydetaildialog.h \
    colormenu.h \
    filelistwidget.h \
    dateselectdialog.h \
    setting.h \
    preferencedialog.h \
    translator.h

FORMS    += forms/mainwindow.ui \
    forms/addeventdialog.ui \
    forms/weekrepeatwidget.ui \
    forms/monthrepeatwidget.ui \
    forms/yearrepeatwidget.ui \
    forms/daydetaildialog.ui \
    forms/dateselectdialog.ui \
    forms/preferencedialog.ui

TRANSLATIONS += \
    resource/zh_CN.ts \
    resource/zh_TW.ts

RESOURCES += \
    resource/res.qrc

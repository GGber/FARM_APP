#-------------------------------------------------
#
# Project created by QtCreator 2019-10-05T21:45:03
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += serialport
QT       += sql

RC_ICONS = myico.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FLD
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myfarmer.cpp \
    load_window.cpp \
    setdata.cpp \
    excel_data.cpp

HEADERS += \
        mainwindow.h \
    myfarmer.h \
    load_window.h \
    setdata.h \
    excel_data.h

FORMS += \
        mainwindow.ui \
    myfarmer.ui \
    load_window.ui \
    setdata.ui \
    excel_data.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc

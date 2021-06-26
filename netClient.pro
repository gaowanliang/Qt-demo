#-------------------------------------------------
#
# Project created by QtCreator 2021-06-21T14:18:06
#
#-------------------------------------------------

QT       += core gui network multimedia  multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = netClient
TEMPLATE = app


SOURCES += main.cpp\
        indexwindow.cpp \
        livingroom.cpp \
        loginwidget.cpp \
        rgswidget.cpp

HEADERS  += loginwidget.h \
    indexwindow.h \
    livingroom.h \
    loginProto.h \
    proto.h \
    rgswidget.h

RESOURCES += \
    myimgs.qrc

FORMS += \
    indexwindow.ui \
    livingroom.ui \
    rgswidget.ui

#-------------------------------------------------
#
# Project created by QtCreator 2017-04-20T17:03:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SAM_test
TEMPLATE = app


SOURCES += main.cpp\
        main_sam_test.cpp \
    myserialport.cpp \
    sam.cpp

HEADERS  += main_sam_test.h \
    myserialport.h \
    sam.h

FORMS    += main_sam_test.ui

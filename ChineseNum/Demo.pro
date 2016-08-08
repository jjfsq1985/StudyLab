QT += core
QT -= gui

CONFIG += c++11

TARGET = Demo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    numtochinese.cpp \
    main.cpp \
    chinesetonum.cpp

HEADERS += \
    numtochinese.h \
    stdafx.h \
    chinesetonum.h \
    DefineNum.h

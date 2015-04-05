# Qt project file - qmake uses his to generate a Makefile

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QwtExample

LIBS += -lqwt -lm -lbcm2835 -liir

HEADERS += window.h ADCutilities.h gz_clk.h

SOURCES += main.cpp window.cpp ADCutilities.c gz_clk.c

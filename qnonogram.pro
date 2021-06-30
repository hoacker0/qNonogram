TEMPLATE = app
TARGET = qnonogram
DEPENDPATH += .
INCLUDEPATH += .
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Input
HEADERS += linesolver.h mainwindow.h nonogram.h pushbutton.h solver.h
SOURCES += linesolver.cpp \
           main.cpp \
           mainwindow.cpp \
           nonogram.cpp \
           pushbutton.cpp \
           solver.cpp
RESOURCES += qnonogram.qrc

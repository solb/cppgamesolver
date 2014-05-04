TARGET = blackbox_gui
HEADERS = BoxPuzzle.h \
			../BoxConfig.h \
			BoxWindow.h
SOURCES = BoxPuzzle.cpp \
			../BoxConfig.cpp \
			BoxWindow.cpp \
			blackbox_gui.cpp

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

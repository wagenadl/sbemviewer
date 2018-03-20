# QMake project file for simpleclient            -*- mode: shell-script; -*-
	

TEMPLATE = app
TARGET = ../sbemviewer
INCLUDEPATH += .
QT += gui widgets network

DEFINES += QT_DEPRECATED_WARNINGS

# Input
HEADERS += TiffLoader.h   TileCache.h   TileViewer.h   TileID.h TileCacheData.h
SOURCES += TiffLoader.cpp TileCache.cpp TileViewer.cpp 
SOURCES += simpleclient.cpp
HEADERS += MainWindow.h   Navigation.h   ServerInfo.h   Curves.h
SOURCES += MainWindow.cpp Navigation.cpp ServerInfo.cpp Curves.cpp
FORMS += MainWindow.ui Navigation.ui Curves.ui

LIBS += -lopencv_core -lopencv_imgcodecs

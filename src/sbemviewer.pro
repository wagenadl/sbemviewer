# QMake project file for simpleclient            -*- mode: shell-script; -*-
	

TEMPLATE = app
TARGET = ../sbemviewer
INCLUDEPATH += .
INCLUDEPATH += view
INCLUDEPATH += db
QT += gui widgets network

DEFINES += QT_DEPRECATED_WARNINGS

# Input
HEADERS += view/TiffLoader.h    view/TileCache.h    view/TileViewer.h    
SOURCES += view/TiffLoader.cpp  view/TileCache.cpp  view/TileViewer.cpp
HEADERS += view/TileID.h  view/TileCacheData.h
SOURCES += view/simpleclient.cpp
HEADERS += view/MainWindow.h    view/Navigation.h    view/ServerInfo.h    view/Curves.h
SOURCES += view/MainWindow.cpp  view/Navigation.cpp  view/ServerInfo.cpp  view/Curves.cpp
FORMS += view/MainWindow.ui view/Navigation.ui view/Curves.ui

LIBS += -lopencv_core -lopencv_imgcodecs

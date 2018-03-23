# QMake project file for simpleclient            -*- mode: shell-script; -*-
	

TEMPLATE = app
TARGET = ../sbemviewer
INCLUDEPATH += .
INCLUDEPATH += main
INCLUDEPATH += view
INCLUDEPATH += db
INCLUDEPATH += edit
QT += gui widgets network sql

DEFINES += QT_DEPRECATED_WARNINGS

# Input
HEADERS += view/TiffLoader.h    view/TileCache.h    view/TileViewer.h    
SOURCES += view/TiffLoader.cpp  view/TileCache.cpp  view/TileViewer.cpp
HEADERS += view/TileID.h view/ViewInfo.h view/TileCacheData.h
SOURCES += main/sbemviewer.cpp
HEADERS += main/MainWindow.h    view/Navigation.h    view/Curves.h
SOURCES += main/MainWindow.cpp  view/Navigation.cpp  view/Curves.cpp
FORMS   += main/ModeDlg.ui
FORMS   += main/MainWindow.ui view/Navigation.ui view/Curves.ui main/ModeDlg.ui
HEADERS += view/ServerInfo.h    main/ModeDlg.h    view/Overlay.h
SOURCES += view/ServerInfo.cpp  main/ModeDlg.cpp  view/Overlay.cpp
HEADERS += db/Database.h    db/PDebug.h    db/SqlFile.h  
SOURCES += db/Database.cpp  db/PDebug.cpp  db/SqlFile.cpp
HEADERS += db/SBEMDB.h
SOURCES += db/SBEMDB.cpp
HEADERS +=  edit/TreeModel.h   edit/TreeView.h
SOURCES +=  edit/TreeModel.cpp edit/TreeView.cpp
FORMS   += edit/TreeView.ui

RESOURCES += db/sbemdb.qrc
LIBS += -lopencv_core -lopencv_imgcodecs

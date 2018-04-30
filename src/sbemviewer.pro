# QMake project file for simpleclient            -*- mode: shell-script; -*-
	

TEMPLATE = app
TARGET = ../sbemviewer
INCLUDEPATH += .
INCLUDEPATH += main
INCLUDEPATH += view
INCLUDEPATH += db
INCLUDEPATH += edit
INCLUDEPATH += miniball
QT += gui widgets network sql
CONFIG += debug_and_release

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) { TARGET=$${TARGET}_debug }

# Input
HEADERS += view/TiffLoader.h    view/TileCache.h    view/TileViewer.h    
SOURCES += view/TiffLoader.cpp  view/TileCache.cpp  view/TileViewer.cpp
HEADERS += view/TileID.h view/ViewInfo.h view/TileCacheData.h
SOURCES += main/sbemviewer.cpp
HEADERS += main/MainWindow.h    view/Navigation.h    view/Curves.h
SOURCES += main/MainWindow.cpp  view/Navigation.cpp  view/Curves.cpp
FORMS   += main/MainWindow.ui   view/Navigation.ui   view/Curves.ui 
FORMS   += main/ModeDlg.ui
HEADERS += view/ServerInfo.h    main/ModeDlg.h    view/Overlay.h
SOURCES += view/ServerInfo.cpp  main/ModeDlg.cpp  view/Overlay.cpp
HEADERS += db/Database.h    db/PDebug.h    db/SqlFile.h  
SOURCES += db/Database.cpp  db/PDebug.cpp  db/SqlFile.cpp
HEADERS += db/SBEMDB.h
SOURCES += db/SBEMDB.cpp
HEADERS +=  edit/TreeModel.h   edit/TreeView.h   edit/EditOverlay.h
SOURCES +=  edit/TreeModel.cpp edit/TreeView.cpp edit/EditOverlay.cpp
FORMS   += edit/TreeView.ui
HEADERS += miniball/Miniball.h   miniball/SmallestEnclosingCircle.hpp
SOURCES += miniball/Miniball.cpp miniball/SmallestEnclosingCircle.cpp
HEADERS += main/Settings.h   view/Overview.h
SOURCES += main/Settings.cpp view/Overview.cpp

RESOURCES += db/sbemdb.qrc
LIBS += -lopencv_core -lopencv_imgcodecs

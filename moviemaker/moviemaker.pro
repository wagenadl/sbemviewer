# QMake project file for simpleclient            -*- mode: shell-script; -*-
	

TEMPLATE = app
TARGET = ../sbemmoviemaker
INCLUDEPATH += .
INCLUDEPATH += db view
QT += gui widgets network sql
CONFIG += debug_and_release
LIBS += -lsqlite3

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) { TARGET=$${TARGET}_debug }

# Input
HEADERS += view/ServerInfo.h
SOURCES += view/ServerInfo.cpp
HEADERS += db/Database.h    db/PDebug.h    db/SqlFile.h  
SOURCES += db/Database.cpp  db/PDebug.cpp  db/SqlFile.cpp
HEADERS += db/SBEMDB.h   
SOURCES += db/SBEMDB.cpp 
FORMS   += MMControls.ui  MovieMaker.ui
HEADERS += MMControls.h   MovieMaker.h
SOURCES += MMControls.cpp MovieMaker.cpp
HEADERS += MovieRender.h   MMSettings.h   SomaLabel.h
SOURCES += MovieRender.cpp MMSettings.cpp SomaLabel.cpp
SOURCES += main.cpp
HEADERS += view/Transform3.h   view/LineF.h   view/DistinctColors.h  
SOURCES += view/Transform3.cpp view/LineF.cpp view/DistinctColors.cpp
RESOURCES += db/sbemdb.qrc


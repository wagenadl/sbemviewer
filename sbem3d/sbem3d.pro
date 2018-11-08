# QMake project file for simpleclient            -*- mode: shell-script; -*-
	

TEMPLATE = app
TARGET = ../sbem3dp
INCLUDEPATH += .
INCLUDEPATH += db
QT += gui widgets network sql
CONFIG += debug_and_release

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, debug|release) { TARGET=$${TARGET}_debug }

# Input
HEADERS += db/Database.h    db/PDebug.h    db/SqlFile.h  
SOURCES += db/Database.cpp  db/PDebug.cpp  db/SqlFile.cpp
HEADERS += db/SBEMDB.h
SOURCES += db/SBEMDB.cpp
SOURCES += sbem3d.cpp
HEADERS += ServerInfo.h   ProjectionWidget.h   ProjectionView.h  LineF.h
SOURCES += ServerInfo.cpp ProjectionWidget.cpp ProjectionView.cpp
HEADERS += Transform3.h   DistinctColors.h
SOURCES += Transform3.cpp DistinctColors.cpp

RESOURCES += db/sbemdb.qrc


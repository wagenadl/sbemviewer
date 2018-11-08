/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include "Curves.h"
#include "ModeDlg.h"
#include "Navigation.h"
#include "Overview.h"
#include "TileViewer.h"
#include "TreeView.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionAbout;
    QAction *actionOpen_DB;
    QAction *actionCreate_DB;
    QAction *actionTypeTreeNode;
    QAction *actionTypePresynapticTerminal;
    QAction *actionTypePostsynapticDensity;
    QAction *actionTypeSoma;
    QAction *actionTypeNeuropilExitPoint;
    QAction *actionConnectNodes;
    QAction *actionDeleteNode;
    QAction *actionDisconnectNodes;
    QAction *actionConnectTerminals;
    QAction *actionDissolveSynapse;
    QAction *actionNewTree;
    QAction *actionDeleteTree;
    QAction *actionShowTrees;
    QAction *actionRestoreDialogs;
    QAction *actionHideTrees;
    QAction *actionEditMemo;
    QAction *actionCenterSelectedNode;
    QAction *actionFindNode;
    QAction *action3DProjection;
    QAction *actionGoRS;
    TileViewer *tileviewer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QMenu *menuChange_node_type;
    QMenu *menuView;
    QDockWidget *navdock;
    Navigation *nav;
    QDockWidget *curvedock;
    Curves *curves;
    QDockWidget *modeDock;
    ModeDlg *mode;
    QDockWidget *treeDock;
    TreeView *treeView;
    QDockWidget *overviewDock;
    Overview *overview;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(987, 818);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionQuit->setShortcutContext(Qt::ApplicationShortcut);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionOpen_DB = new QAction(MainWindow);
        actionOpen_DB->setObjectName(QStringLiteral("actionOpen_DB"));
        actionCreate_DB = new QAction(MainWindow);
        actionCreate_DB->setObjectName(QStringLiteral("actionCreate_DB"));
        actionTypeTreeNode = new QAction(MainWindow);
        actionTypeTreeNode->setObjectName(QStringLiteral("actionTypeTreeNode"));
        actionTypePresynapticTerminal = new QAction(MainWindow);
        actionTypePresynapticTerminal->setObjectName(QStringLiteral("actionTypePresynapticTerminal"));
        actionTypePostsynapticDensity = new QAction(MainWindow);
        actionTypePostsynapticDensity->setObjectName(QStringLiteral("actionTypePostsynapticDensity"));
        actionTypeSoma = new QAction(MainWindow);
        actionTypeSoma->setObjectName(QStringLiteral("actionTypeSoma"));
        actionTypeNeuropilExitPoint = new QAction(MainWindow);
        actionTypeNeuropilExitPoint->setObjectName(QStringLiteral("actionTypeNeuropilExitPoint"));
        actionConnectNodes = new QAction(MainWindow);
        actionConnectNodes->setObjectName(QStringLiteral("actionConnectNodes"));
        actionDeleteNode = new QAction(MainWindow);
        actionDeleteNode->setObjectName(QStringLiteral("actionDeleteNode"));
        actionDisconnectNodes = new QAction(MainWindow);
        actionDisconnectNodes->setObjectName(QStringLiteral("actionDisconnectNodes"));
        actionConnectTerminals = new QAction(MainWindow);
        actionConnectTerminals->setObjectName(QStringLiteral("actionConnectTerminals"));
        actionDissolveSynapse = new QAction(MainWindow);
        actionDissolveSynapse->setObjectName(QStringLiteral("actionDissolveSynapse"));
        actionNewTree = new QAction(MainWindow);
        actionNewTree->setObjectName(QStringLiteral("actionNewTree"));
        actionDeleteTree = new QAction(MainWindow);
        actionDeleteTree->setObjectName(QStringLiteral("actionDeleteTree"));
        actionShowTrees = new QAction(MainWindow);
        actionShowTrees->setObjectName(QStringLiteral("actionShowTrees"));
        actionRestoreDialogs = new QAction(MainWindow);
        actionRestoreDialogs->setObjectName(QStringLiteral("actionRestoreDialogs"));
        actionHideTrees = new QAction(MainWindow);
        actionHideTrees->setObjectName(QStringLiteral("actionHideTrees"));
        actionEditMemo = new QAction(MainWindow);
        actionEditMemo->setObjectName(QStringLiteral("actionEditMemo"));
        actionCenterSelectedNode = new QAction(MainWindow);
        actionCenterSelectedNode->setObjectName(QStringLiteral("actionCenterSelectedNode"));
        actionFindNode = new QAction(MainWindow);
        actionFindNode->setObjectName(QStringLiteral("actionFindNode"));
        action3DProjection = new QAction(MainWindow);
        action3DProjection->setObjectName(QStringLiteral("action3DProjection"));
        actionGoRS = new QAction(MainWindow);
        actionGoRS->setObjectName(QStringLiteral("actionGoRS"));
        tileviewer = new TileViewer(MainWindow);
        tileviewer->setObjectName(QStringLiteral("tileviewer"));
        MainWindow->setCentralWidget(tileviewer);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 987, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuChange_node_type = new QMenu(menuEdit);
        menuChange_node_type->setObjectName(QStringLiteral("menuChange_node_type"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        MainWindow->setMenuBar(menubar);
        navdock = new QDockWidget(MainWindow);
        navdock->setObjectName(QStringLiteral("navdock"));
        nav = new Navigation();
        nav->setObjectName(QStringLiteral("nav"));
        navdock->setWidget(nav);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), navdock);
        curvedock = new QDockWidget(MainWindow);
        curvedock->setObjectName(QStringLiteral("curvedock"));
        curves = new Curves();
        curves->setObjectName(QStringLiteral("curves"));
        curvedock->setWidget(curves);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), curvedock);
        modeDock = new QDockWidget(MainWindow);
        modeDock->setObjectName(QStringLiteral("modeDock"));
        mode = new ModeDlg();
        mode->setObjectName(QStringLiteral("mode"));
        modeDock->setWidget(mode);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), modeDock);
        treeDock = new QDockWidget(MainWindow);
        treeDock->setObjectName(QStringLiteral("treeDock"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeDock->sizePolicy().hasHeightForWidth());
        treeDock->setSizePolicy(sizePolicy);
        treeDock->setFloating(false);
        treeView = new TreeView();
        treeView->setObjectName(QStringLiteral("treeView"));
        treeDock->setWidget(treeView);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), treeDock);
        overviewDock = new QDockWidget(MainWindow);
        overviewDock->setObjectName(QStringLiteral("overviewDock"));
        overview = new Overview();
        overview->setObjectName(QStringLiteral("overview"));
        overviewDock->setWidget(overview);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), overviewDock);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionCreate_DB);
        menuFile->addAction(actionOpen_DB);
        menuFile->addAction(actionQuit);
        menuFile->addSeparator();
        menuHelp->addAction(actionAbout);
        menuEdit->addAction(actionNewTree);
        menuEdit->addAction(actionDeleteTree);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFindNode);
        menuEdit->addAction(menuChange_node_type->menuAction());
        menuEdit->addAction(actionDeleteNode);
        menuEdit->addAction(actionConnectNodes);
        menuEdit->addAction(actionDisconnectNodes);
        menuEdit->addAction(actionEditMemo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionConnectTerminals);
        menuEdit->addAction(actionDissolveSynapse);
        menuChange_node_type->addAction(actionTypeTreeNode);
        menuChange_node_type->addAction(actionTypePresynapticTerminal);
        menuChange_node_type->addAction(actionTypePostsynapticDensity);
        menuChange_node_type->addAction(actionTypeSoma);
        menuChange_node_type->addAction(actionTypeNeuropilExitPoint);
        menuView->addAction(actionShowTrees);
        menuView->addAction(actionHideTrees);
        menuView->addSeparator();
        menuView->addAction(actionRestoreDialogs);
        menuView->addSeparator();
        menuView->addAction(actionCenterSelectedNode);
        menuView->addAction(action3DProjection);
        menuView->addSeparator();
        menuView->addAction(actionGoRS);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SBEM Viewer", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("MainWindow", "&Quit", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionAbout->setText(QApplication::translate("MainWindow", "&About", Q_NULLPTR));
        actionOpen_DB->setText(QApplication::translate("MainWindow", "&Open DB...", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionOpen_DB->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionCreate_DB->setText(QApplication::translate("MainWindow", "&Create DB...", Q_NULLPTR));
        actionTypeTreeNode->setText(QApplication::translate("MainWindow", "Tree &node", Q_NULLPTR));
        actionTypePresynapticTerminal->setText(QApplication::translate("MainWindow", "Presynaptic &terminal", Q_NULLPTR));
        actionTypePostsynapticDensity->setText(QApplication::translate("MainWindow", "Postsynaptic &density", Q_NULLPTR));
        actionTypeSoma->setText(QApplication::translate("MainWindow", "&Soma", Q_NULLPTR));
        actionTypeNeuropilExitPoint->setText(QApplication::translate("MainWindow", "Neuropil &exit point", Q_NULLPTR));
        actionConnectNodes->setText(QApplication::translate("MainWindow", "&Join trees (connect nodes)", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionConnectNodes->setShortcut(QApplication::translate("MainWindow", "Ctrl+J", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionDeleteNode->setText(QApplication::translate("MainWindow", "&Delete node", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionDeleteNode->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionDisconnectNodes->setText(QApplication::translate("MainWindow", "&Split trees (disconnect nodes)", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionDisconnectNodes->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+J", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionConnectTerminals->setText(QApplication::translate("MainWindow", "&Connect terminals (create synapse)", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionConnectTerminals->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionDissolveSynapse->setText(QApplication::translate("MainWindow", "D&issolve synapse", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionDissolveSynapse->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionNewTree->setText(QApplication::translate("MainWindow", "&New tree", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionNewTree->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionDeleteTree->setText(QApplication::translate("MainWindow", "Delete tree", Q_NULLPTR));
        actionShowTrees->setText(QApplication::translate("MainWindow", "&Show all trees", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionShowTrees->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionRestoreDialogs->setText(QApplication::translate("MainWindow", "&Restore dialogs", Q_NULLPTR));
        actionHideTrees->setText(QApplication::translate("MainWindow", "&Hide all trees", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionHideTrees->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+V", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionEditMemo->setText(QApplication::translate("MainWindow", "Edit &memo\342\200\246", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionEditMemo->setShortcut(QApplication::translate("MainWindow", "M", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionCenterSelectedNode->setText(QApplication::translate("MainWindow", "&Center on selected node", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionCenterSelectedNode->setShortcut(QApplication::translate("MainWindow", "C", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionFindNode->setText(QApplication::translate("MainWindow", "&Find node\342\200\246", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionFindNode->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        action3DProjection->setText(QApplication::translate("MainWindow", "3D &projection", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        action3DProjection->setShortcut(QApplication::translate("MainWindow", "P", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionGoRS->setText(QApplication::translate("MainWindow", "&Go to (R,S) position\342\200\246", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionGoRS->setShortcut(QApplication::translate("MainWindow", "G", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("MainWindow", "&Edit", Q_NULLPTR));
        menuChange_node_type->setTitle(QApplication::translate("MainWindow", "Change node &type", Q_NULLPTR));
        menuView->setTitle(QApplication::translate("MainWindow", "&View", Q_NULLPTR));
        navdock->setWindowTitle(QApplication::translate("MainWindow", "Navigation", Q_NULLPTR));
        curvedock->setWindowTitle(QApplication::translate("MainWindow", "Curves", Q_NULLPTR));
        modeDock->setWindowTitle(QApplication::translate("MainWindow", "Mode", Q_NULLPTR));
        treeDock->setWindowTitle(QApplication::translate("MainWindow", "Trees", Q_NULLPTR));
        overviewDock->setWindowTitle(QApplication::translate("MainWindow", "Overview", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

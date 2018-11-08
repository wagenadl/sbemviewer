/********************************************************************************
** Form generated from reading UI file 'TreeView.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TREEVIEW_H
#define UI_TREEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TreeView
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *table;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QToolButton *add;
    QToolButton *del;
    QSpacerItem *horizontalSpacer;
    QToolButton *show;
    QToolButton *hide;

    void setupUi(QWidget *TreeView)
    {
        if (TreeView->objectName().isEmpty())
            TreeView->setObjectName(QStringLiteral("TreeView"));
        TreeView->resize(400, 604);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TreeView->sizePolicy().hasHeightForWidth());
        TreeView->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(TreeView);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        table = new QTableView(TreeView);
        table->setObjectName(QStringLiteral("table"));

        verticalLayout->addWidget(table);

        widget = new QWidget(TreeView);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        add = new QToolButton(widget);
        add->setObjectName(QStringLiteral("add"));

        horizontalLayout->addWidget(add);

        del = new QToolButton(widget);
        del->setObjectName(QStringLiteral("del"));
        del->setEnabled(false);

        horizontalLayout->addWidget(del);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        show = new QToolButton(widget);
        show->setObjectName(QStringLiteral("show"));

        horizontalLayout->addWidget(show);

        hide = new QToolButton(widget);
        hide->setObjectName(QStringLiteral("hide"));

        horizontalLayout->addWidget(hide);


        verticalLayout->addWidget(widget);


        retranslateUi(TreeView);

        QMetaObject::connectSlotsByName(TreeView);
    } // setupUi

    void retranslateUi(QWidget *TreeView)
    {
        TreeView->setWindowTitle(QApplication::translate("TreeView", "TreeView", Q_NULLPTR));
        add->setText(QApplication::translate("TreeView", "+", Q_NULLPTR));
        del->setText(QApplication::translate("TreeView", "\342\200\223", Q_NULLPTR));
        show->setText(QApplication::translate("TreeView", "\342\255\225", Q_NULLPTR));
        hide->setText(QApplication::translate("TreeView", "\342\227\215", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TreeView: public Ui_TreeView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREEVIEW_H

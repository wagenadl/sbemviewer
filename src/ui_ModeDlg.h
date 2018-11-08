/********************************************************************************
** Form generated from reading UI file 'ModeDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODEDLG_H
#define UI_MODEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModeDlg
{
public:
    QVBoxLayout *verticalLayout;
    QRadioButton *view;
    QRadioButton *editTrees;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ModeDlg)
    {
        if (ModeDlg->objectName().isEmpty())
            ModeDlg->setObjectName(QStringLiteral("ModeDlg"));
        ModeDlg->resize(455, 300);
        verticalLayout = new QVBoxLayout(ModeDlg);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        view = new QRadioButton(ModeDlg);
        view->setObjectName(QStringLiteral("view"));
        view->setChecked(true);

        verticalLayout->addWidget(view);

        editTrees = new QRadioButton(ModeDlg);
        editTrees->setObjectName(QStringLiteral("editTrees"));
        editTrees->setEnabled(false);

        verticalLayout->addWidget(editTrees);

        verticalSpacer = new QSpacerItem(20, 157, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ModeDlg);

        QMetaObject::connectSlotsByName(ModeDlg);
    } // setupUi

    void retranslateUi(QWidget *ModeDlg)
    {
        ModeDlg->setWindowTitle(QApplication::translate("ModeDlg", "Mode", Q_NULLPTR));
        view->setText(QApplication::translate("ModeDlg", "View", Q_NULLPTR));
        editTrees->setText(QApplication::translate("ModeDlg", "Edit trees", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ModeDlg: public Ui_ModeDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODEDLG_H

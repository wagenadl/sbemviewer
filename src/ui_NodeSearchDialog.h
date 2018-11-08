/********************************************************************************
** Form generated from reading UI file 'NodeSearchDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NODESEARCHDIALOG_H
#define UI_NODESEARCHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NodeSearchDialog
{
public:
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *treeSelected;
    QRadioButton *treeVisible;
    QRadioButton *treeAll;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *nodeType;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QCheckBox *typePresyn;
    QCheckBox *typePostsyn;
    QCheckBox *typePrePartner;
    QCheckBox *typePostPartner;
    QCheckBox *typeBranchpoint;
    QCheckBox *typeEndpoint;
    QCheckBox *typeExit;
    QCheckBox *typeSoma;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *memo;
    QFrame *frame;
    QFormLayout *formLayout;
    QRadioButton *memoContains;
    QLineEdit *memoText;
    QRadioButton *memoEquals;
    QCheckBox *memoAndAlso;
    QLineEdit *memoAndText;
    QCheckBox *memoButNot;
    QLineEdit *memoNotText;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *count;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NodeSearchDialog)
    {
        if (NodeSearchDialog->objectName().isEmpty())
            NodeSearchDialog->setObjectName(QStringLiteral("NodeSearchDialog"));
        NodeSearchDialog->resize(378, 434);
        verticalLayout_6 = new QVBoxLayout(NodeSearchDialog);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(6, 6, 6, 6);
        groupBox = new QGroupBox(NodeSearchDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(16, 0, 0, 0);
        treeSelected = new QRadioButton(groupBox);
        treeSelected->setObjectName(QStringLiteral("treeSelected"));
        treeSelected->setChecked(true);

        verticalLayout_2->addWidget(treeSelected);

        treeVisible = new QRadioButton(groupBox);
        treeVisible->setObjectName(QStringLiteral("treeVisible"));

        verticalLayout_2->addWidget(treeVisible);

        treeAll = new QRadioButton(groupBox);
        treeAll->setObjectName(QStringLiteral("treeAll"));

        verticalLayout_2->addWidget(treeAll);


        verticalLayout_6->addWidget(groupBox);

        widget_5 = new QWidget(NodeSearchDialog);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        verticalLayout_7 = new QVBoxLayout(widget_5);
        verticalLayout_7->setSpacing(2);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        nodeType = new QCheckBox(widget_5);
        nodeType->setObjectName(QStringLiteral("nodeType"));
        nodeType->setChecked(false);

        verticalLayout_7->addWidget(nodeType);

        frame_2 = new QFrame(widget_5);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setEnabled(false);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Sunken);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(2);
        gridLayout->setVerticalSpacing(0);
        gridLayout->setContentsMargins(16, 0, 0, 0);
        typePresyn = new QCheckBox(frame_2);
        typePresyn->setObjectName(QStringLiteral("typePresyn"));

        gridLayout->addWidget(typePresyn, 0, 1, 1, 1);

        typePostsyn = new QCheckBox(frame_2);
        typePostsyn->setObjectName(QStringLiteral("typePostsyn"));

        gridLayout->addWidget(typePostsyn, 1, 1, 1, 1);

        typePrePartner = new QCheckBox(frame_2);
        typePrePartner->setObjectName(QStringLiteral("typePrePartner"));

        gridLayout->addWidget(typePrePartner, 2, 1, 1, 1);

        typePostPartner = new QCheckBox(frame_2);
        typePostPartner->setObjectName(QStringLiteral("typePostPartner"));

        gridLayout->addWidget(typePostPartner, 7, 1, 1, 1);

        typeBranchpoint = new QCheckBox(frame_2);
        typeBranchpoint->setObjectName(QStringLiteral("typeBranchpoint"));
        typeBranchpoint->setChecked(false);

        gridLayout->addWidget(typeBranchpoint, 0, 0, 1, 1);

        typeEndpoint = new QCheckBox(frame_2);
        typeEndpoint->setObjectName(QStringLiteral("typeEndpoint"));
        typeEndpoint->setChecked(false);

        gridLayout->addWidget(typeEndpoint, 1, 0, 1, 1);

        typeExit = new QCheckBox(frame_2);
        typeExit->setObjectName(QStringLiteral("typeExit"));

        gridLayout->addWidget(typeExit, 2, 0, 1, 1);

        typeSoma = new QCheckBox(frame_2);
        typeSoma->setObjectName(QStringLiteral("typeSoma"));

        gridLayout->addWidget(typeSoma, 7, 0, 1, 1);


        verticalLayout_7->addWidget(frame_2);


        verticalLayout_6->addWidget(widget_5);

        widget = new QWidget(NodeSearchDialog);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        memo = new QCheckBox(widget);
        memo->setObjectName(QStringLiteral("memo"));
        memo->setEnabled(true);

        verticalLayout_3->addWidget(memo);

        frame = new QFrame(widget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setEnabled(false);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        formLayout = new QFormLayout(frame);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setHorizontalSpacing(2);
        formLayout->setVerticalSpacing(2);
        formLayout->setContentsMargins(16, 0, 4, 4);
        memoContains = new QRadioButton(frame);
        memoContains->setObjectName(QStringLiteral("memoContains"));
        memoContains->setChecked(true);

        formLayout->setWidget(1, QFormLayout::LabelRole, memoContains);

        memoText = new QLineEdit(frame);
        memoText->setObjectName(QStringLiteral("memoText"));

        formLayout->setWidget(1, QFormLayout::FieldRole, memoText);

        memoEquals = new QRadioButton(frame);
        memoEquals->setObjectName(QStringLiteral("memoEquals"));

        formLayout->setWidget(0, QFormLayout::LabelRole, memoEquals);

        memoAndAlso = new QCheckBox(frame);
        memoAndAlso->setObjectName(QStringLiteral("memoAndAlso"));

        formLayout->setWidget(2, QFormLayout::LabelRole, memoAndAlso);

        memoAndText = new QLineEdit(frame);
        memoAndText->setObjectName(QStringLiteral("memoAndText"));
        memoAndText->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::FieldRole, memoAndText);

        memoButNot = new QCheckBox(frame);
        memoButNot->setObjectName(QStringLiteral("memoButNot"));

        formLayout->setWidget(3, QFormLayout::LabelRole, memoButNot);

        memoNotText = new QLineEdit(frame);
        memoNotText->setObjectName(QStringLiteral("memoNotText"));
        memoNotText->setEnabled(false);

        formLayout->setWidget(3, QFormLayout::FieldRole, memoNotText);


        verticalLayout_3->addWidget(frame);


        verticalLayout_6->addWidget(widget);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);

        widget_2 = new QWidget(NodeSearchDialog);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        count = new QLabel(widget_2);
        count->setObjectName(QStringLiteral("count"));

        horizontalLayout->addWidget(count);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(widget_2);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout_6->addWidget(widget_2);


        retranslateUi(NodeSearchDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), NodeSearchDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NodeSearchDialog, SLOT(reject()));
        QObject::connect(memo, SIGNAL(toggled(bool)), frame, SLOT(setEnabled(bool)));
        QObject::connect(nodeType, SIGNAL(toggled(bool)), frame_2, SLOT(setEnabled(bool)));
        QObject::connect(memoContains, SIGNAL(toggled(bool)), memoAndAlso, SLOT(setEnabled(bool)));
        QObject::connect(memoContains, SIGNAL(toggled(bool)), memoButNot, SLOT(setEnabled(bool)));
        QObject::connect(treeAll, SIGNAL(toggled(bool)), typePrePartner, SLOT(setDisabled(bool)));
        QObject::connect(treeAll, SIGNAL(toggled(bool)), typePostPartner, SLOT(setDisabled(bool)));
        QObject::connect(memoAndAlso, SIGNAL(toggled(bool)), memoAndText, SLOT(setEnabled(bool)));
        QObject::connect(memoButNot, SIGNAL(toggled(bool)), memoNotText, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(NodeSearchDialog);
    } // setupUi

    void retranslateUi(QDialog *NodeSearchDialog)
    {
        NodeSearchDialog->setWindowTitle(QApplication::translate("NodeSearchDialog", "Search for nodes", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("NodeSearchDialog", "Trees", Q_NULLPTR));
        treeSelected->setText(QApplication::translate("NodeSearchDialog", "Selected", Q_NULLPTR));
        treeVisible->setText(QApplication::translate("NodeSearchDialog", "Visible", Q_NULLPTR));
        treeAll->setText(QApplication::translate("NodeSearchDialog", "All", Q_NULLPTR));
        nodeType->setText(QApplication::translate("NodeSearchDialog", "Node type", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typePresyn->setToolTip(QApplication::translate("NodeSearchDialog", "A presynaptic terminal on the target tree", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typePresyn->setText(QApplication::translate("NodeSearchDialog", "Presynaptic terminal", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typePostsyn->setToolTip(QApplication::translate("NodeSearchDialog", "A postsynaptic density on the target tree", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typePostsyn->setText(QApplication::translate("NodeSearchDialog", "Postsynaptic density", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typePrePartner->setToolTip(QApplication::translate("NodeSearchDialog", "A presynaptic terminal on any tree that forms a synapse with a target tree", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typePrePartner->setText(QApplication::translate("NodeSearchDialog", "Presynaptic partner", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typePostPartner->setToolTip(QApplication::translate("NodeSearchDialog", "A postsynaptic density on any tree that forms a synapse with a target tree", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typePostPartner->setText(QApplication::translate("NodeSearchDialog", "Postsynaptic partner", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typeBranchpoint->setToolTip(QApplication::translate("NodeSearchDialog", "A node where several branches join", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typeBranchpoint->setText(QApplication::translate("NodeSearchDialog", "Branch point", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typeEndpoint->setToolTip(QApplication::translate("NodeSearchDialog", "A node at the end of a branch (but not a synaptic node)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typeEndpoint->setText(QApplication::translate("NodeSearchDialog", "Endpoint", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typeExit->setToolTip(QApplication::translate("NodeSearchDialog", "The node where a cell's main process leaves the neuropil", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typeExit->setText(QApplication::translate("NodeSearchDialog", "Neuropil exit point", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        typeSoma->setToolTip(QApplication::translate("NodeSearchDialog", "The cell body", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        typeSoma->setText(QApplication::translate("NodeSearchDialog", "Soma", Q_NULLPTR));
        memo->setText(QApplication::translate("NodeSearchDialog", "Memo", Q_NULLPTR));
        memoContains->setText(QApplication::translate("NodeSearchDialog", "Contains", Q_NULLPTR));
        memoEquals->setText(QApplication::translate("NodeSearchDialog", "Equals", Q_NULLPTR));
        memoAndAlso->setText(QApplication::translate("NodeSearchDialog", "and also", Q_NULLPTR));
        memoButNot->setText(QApplication::translate("NodeSearchDialog", "but not", Q_NULLPTR));
        label->setText(QApplication::translate("NodeSearchDialog", "Count:", Q_NULLPTR));
        count->setText(QApplication::translate("NodeSearchDialog", "\342\200\223", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class NodeSearchDialog: public Ui_NodeSearchDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NODESEARCHDIALOG_H

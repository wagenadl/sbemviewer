/********************************************************************************
** Form generated from reading UI file 'Curves.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CURVES_H
#define UI_CURVES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Curves
{
public:
    QGridLayout *gridLayout;
    QLabel *xgamma;
    QLabel *xblack;
    QLabel *label_5;
    QLabel *xsharp;
    QLabel *label_6;
    QLabel *xwhite;
    QLabel *label_4;
    QLabel *label_7;
    QToolButton *autoBlack;
    QSlider *black;
    QToolButton *autoWhite;
    QSlider *white;
    QToolButton *autoGamma;
    QSlider *gamma;
    QSlider *sharp;

    void setupUi(QWidget *Curves)
    {
        if (Curves->objectName().isEmpty())
            Curves->setObjectName(QStringLiteral("Curves"));
        Curves->resize(397, 397);
        gridLayout = new QGridLayout(Curves);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        xgamma = new QLabel(Curves);
        xgamma->setObjectName(QStringLiteral("xgamma"));
        xgamma->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(xgamma, 4, 2, 1, 1);

        xblack = new QLabel(Curves);
        xblack->setObjectName(QStringLiteral("xblack"));
        xblack->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(xblack, 0, 2, 1, 1);

        label_5 = new QLabel(Curves);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        xsharp = new QLabel(Curves);
        xsharp->setObjectName(QStringLiteral("xsharp"));
        xsharp->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(xsharp, 6, 2, 1, 1);

        label_6 = new QLabel(Curves);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        xwhite = new QLabel(Curves);
        xwhite->setObjectName(QStringLiteral("xwhite"));
        xwhite->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(xwhite, 2, 2, 1, 1);

        label_4 = new QLabel(Curves);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        label_7 = new QLabel(Curves);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        autoBlack = new QToolButton(Curves);
        autoBlack->setObjectName(QStringLiteral("autoBlack"));

        gridLayout->addWidget(autoBlack, 0, 3, 1, 1);

        black = new QSlider(Curves);
        black->setObjectName(QStringLiteral("black"));
        black->setMaximum(255);
        black->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(black, 1, 0, 1, 4);

        autoWhite = new QToolButton(Curves);
        autoWhite->setObjectName(QStringLiteral("autoWhite"));

        gridLayout->addWidget(autoWhite, 2, 3, 1, 1);

        white = new QSlider(Curves);
        white->setObjectName(QStringLiteral("white"));
        white->setMaximum(255);
        white->setValue(255);
        white->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(white, 3, 0, 1, 4);

        autoGamma = new QToolButton(Curves);
        autoGamma->setObjectName(QStringLiteral("autoGamma"));

        gridLayout->addWidget(autoGamma, 4, 3, 1, 1);

        gamma = new QSlider(Curves);
        gamma->setObjectName(QStringLiteral("gamma"));
        gamma->setMinimum(-99);
        gamma->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(gamma, 5, 0, 1, 4);

        sharp = new QSlider(Curves);
        sharp->setObjectName(QStringLiteral("sharp"));
        sharp->setMinimum(0);
        sharp->setMaximum(250);
        sharp->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sharp, 7, 0, 1, 4);


        retranslateUi(Curves);

        QMetaObject::connectSlotsByName(Curves);
    } // setupUi

    void retranslateUi(QWidget *Curves)
    {
        Curves->setWindowTitle(QApplication::translate("Curves", "Curves", Q_NULLPTR));
        xgamma->setText(QApplication::translate("Curves", "1.00", Q_NULLPTR));
        xblack->setText(QApplication::translate("Curves", "0", Q_NULLPTR));
        label_5->setText(QApplication::translate("Curves", "White", Q_NULLPTR));
        xsharp->setText(QApplication::translate("Curves", "0", Q_NULLPTR));
        label_6->setText(QApplication::translate("Curves", "Black", Q_NULLPTR));
        xwhite->setText(QApplication::translate("Curves", "255", Q_NULLPTR));
        label_4->setText(QApplication::translate("Curves", "Gamma", Q_NULLPTR));
        label_7->setText(QApplication::translate("Curves", "Sharpening", Q_NULLPTR));
        autoBlack->setText(QApplication::translate("Curves", "Auto", Q_NULLPTR));
        autoWhite->setText(QApplication::translate("Curves", "Auto", Q_NULLPTR));
        autoGamma->setText(QApplication::translate("Curves", "Auto", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Curves: public Ui_Curves {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CURVES_H

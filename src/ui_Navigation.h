/********************************************************************************
** Form generated from reading UI file 'Navigation.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAVIGATION_H
#define UI_NAVIGATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Navigation
{
public:
    QGridLayout *gridLayout;
    QToolButton *nextslice2;
    QLineEdit *x;
    QLineEdit *z;
    QLabel *label_7;
    QFrame *line_2;
    QLineEdit *y;
    QFrame *line;
    QToolButton *up;
    QToolButton *prevslice;
    QToolButton *nextslice;
    QToolButton *left;
    QToolButton *right;
    QToolButton *down;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QToolButton *nextslice3;
    QToolButton *lastslice;
    QToolButton *prevslice3;
    QToolButton *firstslice;
    QLabel *label_5;
    QLabel *label_6;
    QToolButton *zoomout;
    QToolButton *prevslice2;
    QFrame *line_3;
    QToolButton *zoomin;
    QLabel *zoom;
    QPushButton *autoupdate;
    QLabel *updateinfo;

    void setupUi(QWidget *Navigation)
    {
        if (Navigation->objectName().isEmpty())
            Navigation->setObjectName(QStringLiteral("Navigation"));
        Navigation->resize(400, 325);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Navigation->sizePolicy().hasHeightForWidth());
        Navigation->setSizePolicy(sizePolicy);
        Navigation->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(Navigation);
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(6, 6, 6, 6);
        nextslice2 = new QToolButton(Navigation);
        nextslice2->setObjectName(QStringLiteral("nextslice2"));

        gridLayout->addWidget(nextslice2, 5, 4, 1, 1);

        x = new QLineEdit(Navigation);
        x->setObjectName(QStringLiteral("x"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(x->sizePolicy().hasHeightForWidth());
        x->setSizePolicy(sizePolicy1);
        x->setMinimumSize(QSize(1, 0));
        x->setMaxLength(6);
        x->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(x, 0, 2, 1, 3);

        z = new QLineEdit(Navigation);
        z->setObjectName(QStringLiteral("z"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(z->sizePolicy().hasHeightForWidth());
        z->setSizePolicy(sizePolicy2);
        z->setMinimumSize(QSize(1, 0));
        z->setMaxLength(6);
        z->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(z, 4, 2, 1, 3);

        label_7 = new QLabel(Navigation);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_7, 8, 0, 1, 2);

        line_2 = new QFrame(Navigation);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setLineWidth(2);
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 3, 0, 1, 7);

        y = new QLineEdit(Navigation);
        y->setObjectName(QStringLiteral("y"));
        sizePolicy2.setHeightForWidth(y->sizePolicy().hasHeightForWidth());
        y->setSizePolicy(sizePolicy2);
        y->setMinimumSize(QSize(1, 0));
        y->setMaxLength(6);
        y->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(y, 2, 2, 1, 3);

        line = new QFrame(Navigation);
        line->setObjectName(QStringLiteral("line"));
        line->setLineWidth(2);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 7, 0, 1, 7);

        up = new QToolButton(Navigation);
        up->setObjectName(QStringLiteral("up"));

        gridLayout->addWidget(up, 2, 1, 1, 1);

        prevslice = new QToolButton(Navigation);
        prevslice->setObjectName(QStringLiteral("prevslice"));

        gridLayout->addWidget(prevslice, 4, 1, 1, 1);

        nextslice = new QToolButton(Navigation);
        nextslice->setObjectName(QStringLiteral("nextslice"));

        gridLayout->addWidget(nextslice, 4, 5, 1, 1);

        left = new QToolButton(Navigation);
        left->setObjectName(QStringLiteral("left"));

        gridLayout->addWidget(left, 0, 1, 1, 1);

        right = new QToolButton(Navigation);
        right->setObjectName(QStringLiteral("right"));

        gridLayout->addWidget(right, 0, 5, 1, 1);

        down = new QToolButton(Navigation);
        down->setObjectName(QStringLiteral("down"));

        gridLayout->addWidget(down, 2, 5, 1, 1);

        label = new QLabel(Navigation);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(Navigation);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_3 = new QLabel(Navigation);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 0, 6, 1, 1);

        label_4 = new QLabel(Navigation);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 2, 6, 1, 1);

        nextslice3 = new QToolButton(Navigation);
        nextslice3->setObjectName(QStringLiteral("nextslice3"));

        gridLayout->addWidget(nextslice3, 5, 5, 1, 1);

        lastslice = new QToolButton(Navigation);
        lastslice->setObjectName(QStringLiteral("lastslice"));

        gridLayout->addWidget(lastslice, 5, 6, 1, 1);

        prevslice3 = new QToolButton(Navigation);
        prevslice3->setObjectName(QStringLiteral("prevslice3"));

        gridLayout->addWidget(prevslice3, 5, 1, 1, 1);

        firstslice = new QToolButton(Navigation);
        firstslice->setObjectName(QStringLiteral("firstslice"));

        gridLayout->addWidget(firstslice, 5, 0, 1, 1);

        label_5 = new QLabel(Navigation);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_6 = new QLabel(Navigation);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 4, 6, 1, 1);

        zoomout = new QToolButton(Navigation);
        zoomout->setObjectName(QStringLiteral("zoomout"));

        gridLayout->addWidget(zoomout, 8, 2, 1, 1);

        prevslice2 = new QToolButton(Navigation);
        prevslice2->setObjectName(QStringLiteral("prevslice2"));

        gridLayout->addWidget(prevslice2, 5, 2, 1, 1);

        line_3 = new QFrame(Navigation);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setLineWidth(2);
        line_3->setMidLineWidth(0);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 1, 0, 1, 7);

        zoomin = new QToolButton(Navigation);
        zoomin->setObjectName(QStringLiteral("zoomin"));

        gridLayout->addWidget(zoomin, 8, 5, 1, 1);

        zoom = new QLabel(Navigation);
        zoom->setObjectName(QStringLiteral("zoom"));
        zoom->setTextFormat(Qt::RichText);

        gridLayout->addWidget(zoom, 8, 4, 1, 1);

        autoupdate = new QPushButton(Navigation);
        autoupdate->setObjectName(QStringLiteral("autoupdate"));
        autoupdate->setMinimumSize(QSize(1, 0));
        autoupdate->setCheckable(true);
        autoupdate->setChecked(false);

        gridLayout->addWidget(autoupdate, 6, 6, 1, 1);

        updateinfo = new QLabel(Navigation);
        updateinfo->setObjectName(QStringLiteral("updateinfo"));

        gridLayout->addWidget(updateinfo, 6, 0, 1, 6);

        QWidget::setTabOrder(x, y);
        QWidget::setTabOrder(y, z);
        QWidget::setTabOrder(z, right);
        QWidget::setTabOrder(right, nextslice);
        QWidget::setTabOrder(nextslice, down);
        QWidget::setTabOrder(down, nextslice3);
        QWidget::setTabOrder(nextslice3, lastslice);
        QWidget::setTabOrder(lastslice, prevslice3);
        QWidget::setTabOrder(prevslice3, firstslice);
        QWidget::setTabOrder(firstslice, up);
        QWidget::setTabOrder(up, left);
        QWidget::setTabOrder(left, prevslice);

        retranslateUi(Navigation);

        QMetaObject::connectSlotsByName(Navigation);
    } // setupUi

    void retranslateUi(QWidget *Navigation)
    {
        Navigation->setWindowTitle(QApplication::translate("Navigation", "Navigation", Q_NULLPTR));
        nextslice2->setText(QApplication::translate("Navigation", "\342\211\253", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        x->setToolTip(QApplication::translate("Navigation", "X (\316\274m)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        x->setText(QApplication::translate("Navigation", "999.99", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        z->setToolTip(QApplication::translate("Navigation", "Z (\316\274m)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        z->setText(QApplication::translate("Navigation", "0.00", Q_NULLPTR));
        label_7->setText(QApplication::translate("Navigation", "Zoom", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        y->setToolTip(QApplication::translate("Navigation", "Y (\316\274m)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        y->setText(QApplication::translate("Navigation", "0.00", Q_NULLPTR));
        up->setText(QApplication::translate("Navigation", "<", Q_NULLPTR));
        prevslice->setText(QApplication::translate("Navigation", "<", Q_NULLPTR));
        nextslice->setText(QApplication::translate("Navigation", ">", Q_NULLPTR));
        left->setText(QApplication::translate("Navigation", "<", Q_NULLPTR));
        right->setText(QApplication::translate("Navigation", ">", Q_NULLPTR));
        down->setText(QApplication::translate("Navigation", ">", Q_NULLPTR));
        label->setText(QApplication::translate("Navigation", "X", Q_NULLPTR));
        label_2->setText(QApplication::translate("Navigation", "Y", Q_NULLPTR));
        label_3->setText(QApplication::translate("Navigation", "\316\274m", Q_NULLPTR));
        label_4->setText(QApplication::translate("Navigation", "\316\274m", Q_NULLPTR));
        nextslice3->setText(QApplication::translate("Navigation", "\342\213\231", Q_NULLPTR));
        lastslice->setText(QApplication::translate("Navigation", "\342\207\245", Q_NULLPTR));
        prevslice3->setText(QApplication::translate("Navigation", "\342\213\230", Q_NULLPTR));
        firstslice->setText(QApplication::translate("Navigation", "\342\207\244", Q_NULLPTR));
        label_5->setText(QApplication::translate("Navigation", "Z", Q_NULLPTR));
        label_6->setText(QApplication::translate("Navigation", "\316\274m", Q_NULLPTR));
        zoomout->setText(QApplication::translate("Navigation", "\342\200\223", Q_NULLPTR));
        prevslice2->setText(QApplication::translate("Navigation", "\342\211\252", Q_NULLPTR));
        zoomin->setText(QApplication::translate("Navigation", "+", Q_NULLPTR));
        zoom->setText(QApplication::translate("Navigation", "2<sup>\342\200\2231</sup>", Q_NULLPTR));
        autoupdate->setText(QApplication::translate("Navigation", "\342\237\263", Q_NULLPTR));
        updateinfo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Navigation: public Ui_Navigation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAVIGATION_H

/********************************************************************************
** Form generated from reading UI file 'aboutwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTWINDOW_H
#define UI_ABOUTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_aboutwindow
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *gitHubPageLabel;
    QPushButton *okayButton;

    void setupUi(QDialog *aboutwindow)
    {
        if (aboutwindow->objectName().isEmpty())
            aboutwindow->setObjectName("aboutwindow");
        aboutwindow->resize(400, 229);
        label = new QLabel(aboutwindow);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 211, 20));
        QFont font;
        font.setBold(true);
        label->setFont(font);
        label_2 = new QLabel(aboutwindow);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 40, 91, 20));
        label_3 = new QLabel(aboutwindow);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 70, 131, 20));
        label_4 = new QLabel(aboutwindow);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 100, 91, 20));
        gitHubPageLabel = new QLabel(aboutwindow);
        gitHubPageLabel->setObjectName("gitHubPageLabel");
        gitHubPageLabel->setGeometry(QRect(10, 120, 381, 41));
        gitHubPageLabel->setWordWrap(true);
        okayButton = new QPushButton(aboutwindow);
        okayButton->setObjectName("okayButton");
        okayButton->setGeometry(QRect(280, 190, 93, 29));

        retranslateUi(aboutwindow);

        QMetaObject::connectSlotsByName(aboutwindow);
    } // setupUi

    void retranslateUi(QDialog *aboutwindow)
    {
        aboutwindow->setWindowTitle(QCoreApplication::translate("aboutwindow", "About", nullptr));
        label->setText(QCoreApplication::translate("aboutwindow", "AoE II board game calculator", nullptr));
        label_2->setText(QCoreApplication::translate("aboutwindow", "Version: 1.0", nullptr));
        label_3->setText(QCoreApplication::translate("aboutwindow", "License: Freeware", nullptr));
        label_4->setText(QCoreApplication::translate("aboutwindow", "GitHub page: ", nullptr));
        gitHubPageLabel->setText(QCoreApplication::translate("aboutwindow", "[Link to GitHub repository]", nullptr));
        okayButton->setText(QCoreApplication::translate("aboutwindow", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class aboutwindow: public Ui_aboutwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTWINDOW_H

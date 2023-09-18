/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *okayButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *gitHubPageLabel;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(400, 224);
        okayButton = new QPushButton(Dialog);
        okayButton->setObjectName("okayButton");
        okayButton->setGeometry(QRect(290, 180, 93, 29));
        label = new QLabel(Dialog);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 211, 20));
        QFont font;
        font.setBold(true);
        label->setFont(font);
        label_2 = new QLabel(Dialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 40, 91, 20));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 70, 131, 20));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 100, 91, 20));
        gitHubPageLabel = new QLabel(Dialog);
        gitHubPageLabel->setObjectName("gitHubPageLabel");
        gitHubPageLabel->setGeometry(QRect(10, 120, 381, 41));
        gitHubPageLabel->setWordWrap(true);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        okayButton->setText(QCoreApplication::translate("Dialog", "OK", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "AoE II board game calculator", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Version: 1.0", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "License: Freeware", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "GitHub page: ", nullptr));
        gitHubPageLabel->setText(QCoreApplication::translate("Dialog", "https://github.com/ProjectsKoryHasWorkedOn/AoE_II_Board_Game_Combat_Results_Calculator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H

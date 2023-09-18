/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *closeProgram;
    QAction *actionAbout;
    QAction *actionUser_guide;
    QAction *actionDeveloper_guide;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *programTitleLabel;
    QLabel *player1UnitsLabel;
    QHBoxLayout *horizontalLayout;
    QLineEdit *player1EntityQuantity;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *player1EntityNamesFilter;
    QListWidget *player1EntityNames;
    QLineEdit *player1MonkQuantity;
    QLabel *label_4;
    QLabel *player2UnitsLabel;
    QGridLayout *gridLayout;
    QLabel *gameOutputLabel;
    QTextEdit *gameOutputTextEdit;
    QPushButton *calculateResultsButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuDocumentation;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        closeProgram = new QAction(MainWindow);
        closeProgram->setObjectName("closeProgram");
        closeProgram->setCheckable(false);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionUser_guide = new QAction(MainWindow);
        actionUser_guide->setObjectName("actionUser_guide");
        actionDeveloper_guide = new QAction(MainWindow);
        actionDeveloper_guide->setObjectName("actionDeveloper_guide");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        programTitleLabel = new QLabel(centralwidget);
        programTitleLabel->setObjectName("programTitleLabel");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        programTitleLabel->setFont(font);
        programTitleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(programTitleLabel);

        player1UnitsLabel = new QLabel(centralwidget);
        player1UnitsLabel->setObjectName("player1UnitsLabel");

        verticalLayout->addWidget(player1UnitsLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        player1EntityQuantity = new QLineEdit(centralwidget);
        player1EntityQuantity->setObjectName("player1EntityQuantity");

        horizontalLayout->addWidget(player1EntityQuantity);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        player1EntityNamesFilter = new QLineEdit(centralwidget);
        player1EntityNamesFilter->setObjectName("player1EntityNamesFilter");

        verticalLayout_3->addWidget(player1EntityNamesFilter);

        player1EntityNames = new QListWidget(centralwidget);
        player1EntityNames->setObjectName("player1EntityNames");

        verticalLayout_3->addWidget(player1EntityNames);


        horizontalLayout->addLayout(verticalLayout_3);

        player1MonkQuantity = new QLineEdit(centralwidget);
        player1MonkQuantity->setObjectName("player1MonkQuantity");

        horizontalLayout->addWidget(player1MonkQuantity);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");

        horizontalLayout->addWidget(label_4);


        verticalLayout->addLayout(horizontalLayout);

        player2UnitsLabel = new QLabel(centralwidget);
        player2UnitsLabel->setObjectName("player2UnitsLabel");

        verticalLayout->addWidget(player2UnitsLabel);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");

        verticalLayout->addLayout(gridLayout);

        gameOutputLabel = new QLabel(centralwidget);
        gameOutputLabel->setObjectName("gameOutputLabel");

        verticalLayout->addWidget(gameOutputLabel);

        gameOutputTextEdit = new QTextEdit(centralwidget);
        gameOutputTextEdit->setObjectName("gameOutputTextEdit");
        gameOutputTextEdit->setReadOnly(true);

        verticalLayout->addWidget(gameOutputTextEdit);

        calculateResultsButton = new QPushButton(centralwidget);
        calculateResultsButton->setObjectName("calculateResultsButton");

        verticalLayout->addWidget(calculateResultsButton);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        menuDocumentation = new QMenu(menuHelp);
        menuDocumentation->setObjectName("menuDocumentation");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(closeProgram);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(menuDocumentation->menuAction());
        menuDocumentation->addAction(actionUser_guide);
        menuDocumentation->addAction(actionDeveloper_guide);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Combat results calculator", nullptr));
        closeProgram->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionUser_guide->setText(QCoreApplication::translate("MainWindow", "User guide", nullptr));
        actionDeveloper_guide->setText(QCoreApplication::translate("MainWindow", "Developer guide", nullptr));
        programTitleLabel->setText(QCoreApplication::translate("MainWindow", "AoE II board game combat results calculator", nullptr));
        player1UnitsLabel->setText(QCoreApplication::translate("MainWindow", "Player 1's battle participant", nullptr));
        player1EntityQuantity->setPlaceholderText(QString());
        player1MonkQuantity->setPlaceholderText(QString());
        label_4->setText(QCoreApplication::translate("MainWindow", "Monks", nullptr));
        player2UnitsLabel->setText(QCoreApplication::translate("MainWindow", "Player 2's battle participant", nullptr));
        gameOutputLabel->setText(QCoreApplication::translate("MainWindow", "Outcome of the battle between the battle participants:", nullptr));
        calculateResultsButton->setText(QCoreApplication::translate("MainWindow", "Calculate results", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuDocumentation->setTitle(QCoreApplication::translate("MainWindow", "Documentation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

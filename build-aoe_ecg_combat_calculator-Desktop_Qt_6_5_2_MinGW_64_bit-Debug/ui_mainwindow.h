/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
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
    QAction *actionEnable_Disable_SFX;
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *programTitleLabel;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *player2UnitsLabel;
    QLabel *player1UnitsLabel_3;
    QLabel *player1UnitsLabel_6;
    QListWidget *player1Technologies;
    QLabel *player1UnitsLabel_2;
    QLineEdit *player1EntityNamesFilter;
    QLabel *player1UnitsLabel;
    QLabel *player1UnitsLabel_7;
    QListWidget *player1EntityNames;
    QComboBox *player1BattleAssistantNames;
    QLabel *label_2;
    QLabel *player1UnitsLabel_5;
    QLabel *label;
    QLineEdit *player1EntityQuantity;
    QLabel *player1UnitsLabel_4;
    QLineEdit *player1MonkQuantity;
    QListWidget *player1Events;
    QVBoxLayout *verticalLayout;
    QLabel *gameOutputLabel;
    QTextEdit *gameOutputTextEdit;
    QPushButton *calculateResultsButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuDocumentation;
    QMenu *menuOptions;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(975, 721);
        closeProgram = new QAction(MainWindow);
        closeProgram->setObjectName("closeProgram");
        closeProgram->setCheckable(false);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionUser_guide = new QAction(MainWindow);
        actionUser_guide->setObjectName("actionUser_guide");
        actionDeveloper_guide = new QAction(MainWindow);
        actionDeveloper_guide->setObjectName("actionDeveloper_guide");
        actionEnable_Disable_SFX = new QAction(MainWindow);
        actionEnable_Disable_SFX->setObjectName("actionEnable_Disable_SFX");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 949, 644));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        programTitleLabel = new QLabel(horizontalLayoutWidget);
        programTitleLabel->setObjectName("programTitleLabel");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        programTitleLabel->setFont(font);
        programTitleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(programTitleLabel);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 3, 1, 1, 1);

        label_6 = new QLabel(horizontalLayoutWidget);
        label_6->setObjectName("label_6");

        gridLayout_2->addWidget(label_6, 7, 1, 1, 1);

        player2UnitsLabel = new QLabel(horizontalLayoutWidget);
        player2UnitsLabel->setObjectName("player2UnitsLabel");

        gridLayout_2->addWidget(player2UnitsLabel, 0, 1, 1, 1);

        player1UnitsLabel_3 = new QLabel(horizontalLayoutWidget);
        player1UnitsLabel_3->setObjectName("player1UnitsLabel_3");

        gridLayout_2->addWidget(player1UnitsLabel_3, 11, 1, 1, 1);

        player1UnitsLabel_6 = new QLabel(horizontalLayoutWidget);
        player1UnitsLabel_6->setObjectName("player1UnitsLabel_6");

        gridLayout_2->addWidget(player1UnitsLabel_6, 5, 0, 1, 1);

        player1Technologies = new QListWidget(horizontalLayoutWidget);
        player1Technologies->setObjectName("player1Technologies");

        gridLayout_2->addWidget(player1Technologies, 10, 0, 1, 1);

        player1UnitsLabel_2 = new QLabel(horizontalLayoutWidget);
        player1UnitsLabel_2->setObjectName("player1UnitsLabel_2");

        gridLayout_2->addWidget(player1UnitsLabel_2, 11, 0, 1, 1);

        player1EntityNamesFilter = new QLineEdit(horizontalLayoutWidget);
        player1EntityNamesFilter->setObjectName("player1EntityNamesFilter");

        gridLayout_2->addWidget(player1EntityNamesFilter, 1, 0, 1, 1);

        player1UnitsLabel = new QLabel(horizontalLayoutWidget);
        player1UnitsLabel->setObjectName("player1UnitsLabel");

        gridLayout_2->addWidget(player1UnitsLabel, 0, 0, 1, 1);

        player1UnitsLabel_7 = new QLabel(horizontalLayoutWidget);
        player1UnitsLabel_7->setObjectName("player1UnitsLabel_7");

        gridLayout_2->addWidget(player1UnitsLabel_7, 5, 1, 1, 1);

        player1EntityNames = new QListWidget(horizontalLayoutWidget);
        player1EntityNames->setObjectName("player1EntityNames");
        player1EntityNames->setEnabled(true);

        gridLayout_2->addWidget(player1EntityNames, 2, 0, 1, 1);

        player1BattleAssistantNames = new QComboBox(horizontalLayoutWidget);
        player1BattleAssistantNames->setObjectName("player1BattleAssistantNames");

        gridLayout_2->addWidget(player1BattleAssistantNames, 6, 0, 1, 1);

        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 7, 0, 1, 1);

        player1UnitsLabel_5 = new QLabel(horizontalLayoutWidget);
        player1UnitsLabel_5->setObjectName("player1UnitsLabel_5");

        gridLayout_2->addWidget(player1UnitsLabel_5, 9, 1, 1, 1);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 3, 0, 1, 1);

        player1EntityQuantity = new QLineEdit(horizontalLayoutWidget);
        player1EntityQuantity->setObjectName("player1EntityQuantity");

        gridLayout_2->addWidget(player1EntityQuantity, 8, 0, 1, 1);

        player1UnitsLabel_4 = new QLabel(horizontalLayoutWidget);
        player1UnitsLabel_4->setObjectName("player1UnitsLabel_4");

        gridLayout_2->addWidget(player1UnitsLabel_4, 9, 0, 1, 1);

        player1MonkQuantity = new QLineEdit(horizontalLayoutWidget);
        player1MonkQuantity->setObjectName("player1MonkQuantity");

        gridLayout_2->addWidget(player1MonkQuantity, 4, 0, 1, 1);

        player1Events = new QListWidget(horizontalLayoutWidget);
        player1Events->setObjectName("player1Events");

        gridLayout_2->addWidget(player1Events, 12, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        gameOutputLabel = new QLabel(horizontalLayoutWidget);
        gameOutputLabel->setObjectName("gameOutputLabel");

        verticalLayout->addWidget(gameOutputLabel);

        gameOutputTextEdit = new QTextEdit(horizontalLayoutWidget);
        gameOutputTextEdit->setObjectName("gameOutputTextEdit");
        gameOutputTextEdit->setReadOnly(true);

        verticalLayout->addWidget(gameOutputTextEdit);

        calculateResultsButton = new QPushButton(horizontalLayoutWidget);
        calculateResultsButton->setObjectName("calculateResultsButton");

        verticalLayout->addWidget(calculateResultsButton);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 975, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        menuDocumentation = new QMenu(menuHelp);
        menuDocumentation->setObjectName("menuDocumentation");
        menuOptions = new QMenu(menubar);
        menuOptions->setObjectName("menuOptions");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menubar->addAction(menuOptions->menuAction());
        menuFile->addAction(closeProgram);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(menuDocumentation->menuAction());
        menuDocumentation->addAction(actionUser_guide);
        menuDocumentation->addAction(actionDeveloper_guide);
        menuOptions->addAction(actionEnable_Disable_SFX);

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
        actionEnable_Disable_SFX->setText(QCoreApplication::translate("MainWindow", "Enable / Disable SFX", nullptr));
        programTitleLabel->setText(QCoreApplication::translate("MainWindow", "AoE II board game combat results calculator", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        player2UnitsLabel->setText(QCoreApplication::translate("MainWindow", "Player 2's battle participant", nullptr));
        player1UnitsLabel_3->setText(QCoreApplication::translate("MainWindow", "Player 2's event cards", nullptr));
        player1UnitsLabel_6->setText(QCoreApplication::translate("MainWindow", "Player 1's battle assistant", nullptr));
        player1UnitsLabel_2->setText(QCoreApplication::translate("MainWindow", "Player 1's event cards", nullptr));
        player1UnitsLabel->setText(QCoreApplication::translate("MainWindow", "Player 1's battle participant", nullptr));
        player1UnitsLabel_7->setText(QCoreApplication::translate("MainWindow", "Player 2's battle assistant", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        player1UnitsLabel_5->setText(QCoreApplication::translate("MainWindow", "Player 2's technologies", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        player1EntityQuantity->setPlaceholderText(QString());
        player1UnitsLabel_4->setText(QCoreApplication::translate("MainWindow", "Player 1's technologies", nullptr));
        player1MonkQuantity->setPlaceholderText(QString());
        gameOutputLabel->setText(QCoreApplication::translate("MainWindow", "Outcome of the battle between the battle participants:", nullptr));
        calculateResultsButton->setText(QCoreApplication::translate("MainWindow", "Calculate results", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuDocumentation->setTitle(QCoreApplication::translate("MainWindow", "Documentation", nullptr));
        menuOptions->setTitle(QCoreApplication::translate("MainWindow", "Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

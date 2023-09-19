#include "aboutwindow.h"
#include "ui_aboutwindow.h"

aboutwindow::aboutwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutwindow)
{
    ui->setupUi(this);

    ui->gitHubPageLabel->setText("<a href=\"https://github.com/ProjectsKoryHasWorkedOn/AoE_II_Board_Game_Combat_Results_Calculator_Reworked\">https://github.com/ProjectsKoryHasWorkedOn/AoE_II_Board_Game_Combat_Results_Calculator_Reworked</a>");
    ui->gitHubPageLabel->setTextFormat(Qt::RichText);
    ui->gitHubPageLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->gitHubPageLabel->setOpenExternalLinks(true);
}

aboutwindow::~aboutwindow()
{
    delete ui;
}

void aboutwindow::on_okayButton_clicked()
{
    hide();
}


#include "outputwindow.h"
#include "ui_outputwindow.h"

outputwindow::outputwindow(QTextEdit *textWindow, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::outputwindow)
{
  ui->setupUi(this);

  this->setWindowTitle("Game output text");


  ui->gameOutputWindowText->setText(textWindow->toHtml());
  ui->gameOutputWindowText->showFullScreen();
}

outputwindow::~outputwindow()
{
  delete ui;
}

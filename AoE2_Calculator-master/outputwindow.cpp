#include "outputwindow.h"
#include "ui_outputwindow.h"

outputwindow::outputwindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::outputwindow)
{
  ui->setupUi(this);

  this->setWindowTitle("Game output text");

}

outputwindow::~outputwindow()
{
  *windowIsOpen = false;
  delete ui;
}


void outputwindow::setup(bool &isWindowOpen){
  isWindowOpen = true;
  *windowIsOpen = isWindowOpen;
}

void outputwindow::sendOutputToThisWindow(QTextEdit *textWindow){
  ui->gameOutputWindowText->setText(textWindow->toHtml());
  // ui->gameOutputWindowText->showFullScreen();
}

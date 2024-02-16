#include "gameoutputwindow.h"
#include "ui_gameoutputwindow.h"

gameOutputWindow::gameOutputWindow(QTextEdit *textWindow, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::gameOutputWindow)
{
  ui->setupUi(this);
}

gameOutputWindow::~gameOutputWindow()
{
 // delete ui;
}

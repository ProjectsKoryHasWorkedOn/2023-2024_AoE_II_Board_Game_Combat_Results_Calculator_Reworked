#include "openoutputwindowclass.h"

openOutputWindowClass::openOutputWindowClass()
{
  outputWindow = new outputwindow();
}

void openOutputWindowClass::showOutputWindow()
{
  outputWindow->show(); // Modal window
}

bool openOutputWindowClass::returnIfWindowIsOpen()
{
  return outputWindow->isVisible();
}

void openOutputWindowClass::sendOutputToOutputWindow(QTextEdit* textWindow)
{
  outputWindow->sendOutputToThisWindow(textWindow);
}

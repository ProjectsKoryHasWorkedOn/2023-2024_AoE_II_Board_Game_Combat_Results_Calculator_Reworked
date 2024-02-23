#include <outputwindow.h>

#ifndef OPENOUTPUTWINDOWCLASS_H
#define OPENOUTPUTWINDOWCLASS_H

class openOutputWindowClass {
public:
  openOutputWindowClass();

  void showOutputWindow();

  void sendOutputToOutputWindow(QTextEdit* textWindow);

  bool returnIfWindowIsOpen();

private:
  outputwindow* outputWindow;
};

#endif // OPENOUTPUTWINDOWCLASS_H

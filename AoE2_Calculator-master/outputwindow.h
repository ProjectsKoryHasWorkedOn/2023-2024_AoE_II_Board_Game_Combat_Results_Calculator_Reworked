#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QDialog>
#include <QTextEdit>

namespace Ui {
class outputwindow;
}

class outputwindow : public QDialog
{
  Q_OBJECT

public:
  explicit outputwindow(QWidget *parent = nullptr);
  ~outputwindow();

  void setup(bool &isWindowOpen);

  void sendOutputToThisWindow(QTextEdit *textWindow);

  bool * windowIsOpen;

private:
  Ui::outputwindow *ui;
};

#endif // OUTPUTWINDOW_H

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
  explicit outputwindow(QTextEdit *textWindow, QWidget *parent = nullptr);
  ~outputwindow();

private:
  Ui::outputwindow *ui;
};

#endif // OUTPUTWINDOW_H

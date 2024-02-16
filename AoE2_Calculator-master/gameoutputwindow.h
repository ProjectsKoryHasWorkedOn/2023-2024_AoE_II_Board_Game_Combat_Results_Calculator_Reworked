#ifndef GAMEOUTPUTWINDOW_H
#define GAMEOUTPUTWINDOW_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class gameOutputWindow;
}

class gameOutputWindow : public QWidget
{
  Q_OBJECT

public:
  explicit gameOutputWindow(QTextEdit *textWindow, QWidget *parent = nullptr);
  ~gameOutputWindow();

private:
  Ui::gameOutputWindow *ui;
};

#endif // GAMEOUTPUTWINDOW_H

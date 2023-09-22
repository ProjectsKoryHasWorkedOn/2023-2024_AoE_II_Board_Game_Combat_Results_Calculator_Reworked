#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  MainWindow   w;


  // Set the application icon
  w.setWindowIcon(QIcon("abacus.ico"));





  w.show();
  return a.exec();
}

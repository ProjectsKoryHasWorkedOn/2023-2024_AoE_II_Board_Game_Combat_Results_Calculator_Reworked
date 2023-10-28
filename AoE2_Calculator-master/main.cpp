#include <iostream>

#include <QApplication>
#include <QStyleFactory>

#include "mainwindow.hpp"

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);

  MainWindow w;

  std::cout << std::unitbuf;




  // Set the application icon
  w.setWindowIcon(QIcon("abacus.ico"));

  w.show();
  return application.exec();
}

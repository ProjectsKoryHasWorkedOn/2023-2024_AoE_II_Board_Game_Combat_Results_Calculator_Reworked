#include <iostream>

#include <QApplication>
#include <QStyleFactory>

#include "mainwindow.hpp"

int main(int argc, char* argv[])
{
  std::cout << std::unitbuf;

  QApplication application(argc, argv);

  MainWindow w;

  // Set the application icon
  w.setWindowIcon(QIcon("abacus.ico"));

  // application.setStyle(QStyleFactory::create("Fusion"));

  w.show();
  return application.exec();
}

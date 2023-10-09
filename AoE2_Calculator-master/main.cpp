#include "mainwindow.hpp"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);

  MainWindow   w;


         // Set the application icon
  w.setWindowIcon(QIcon("abacus.ico"));

  // application.setStyle(QStyleFactory::create("Fusion"));

  w.show();
  return application.exec();
}

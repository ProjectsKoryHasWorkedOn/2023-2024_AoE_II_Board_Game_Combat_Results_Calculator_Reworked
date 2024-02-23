#include <iostream>

#include <QApplication>
#include <QStyleFactory>

#include "database.hpp"
#include "mainwindow.hpp"
#include "openoutputwindowclass.h"

static QString getSqlScriptPath()
{
  QDir dir{workingDirectory};
  dir.cd(importFolderDirectoryName);
  dir.cd(databaseFolderDirectoryName);
  return dir.absoluteFilePath(databaseFileName);
}


static QString getProgramIconPath(){
  QDir dir{workingDirectory};
  dir.cd(importFolderDirectoryName);
  dir.cd(imagesFolderDirectoryName);
  return dir.absoluteFilePath(programIconFilename);
}


int main(int argc, char* argv[])
{
  std::cout << std::unitbuf;
  QApplication application(argc, argv);

  // What the working directory is
  workingDirectory = QCoreApplication::applicationDirPath();

  // gets debug folder for some reason so go up a level
  workingDirectory.cdUp();

  const QString sqlScriptPath{getSqlScriptPath()};
  // qDebug() << sqlScriptPath;
  Database database{};
  database.runSqlScript(sqlScriptPath);

  openOutputWindowClass openTheOutputWindow;

  MainWindow w{&database, &openTheOutputWindow};

  // Set the application icon
  w.setWindowIcon(QIcon(getProgramIconPath()));

  w.show();
  return application.exec();
}

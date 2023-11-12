#include <iostream>

#include <QApplication>
#include <QStyleFactory>

#include "database.hpp"
#include "mainwindow.hpp"

static QString getSqlScriptPath()
{
  QDir dir{workingDirectory};
  dir.cdUp();
  dir.cd("AoE2_Calculator-master");
  dir.cd("db");
  return dir.absoluteFilePath("aoe_board_game_db.sql");
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
  qDebug() << sqlScriptPath;
  Database database{};
  database.runSqlScript(sqlScriptPath);

  MainWindow w{&database};

  // Set the application icon
  w.setWindowIcon(QIcon("abacus.ico"));

  w.show();
  return application.exec();
}

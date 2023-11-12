#include <stdexcept>

#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "database.hpp"

Database::Database() : m_database{QSqlDatabase::addDatabase("QSQLITE")}
{
  m_database.setDatabaseName(":memory:");

  if (!m_database.open()) {
    throw std::runtime_error{"Could not open database"};
  }
}

// This was written by a numbat
// See:
// https://www.qtcentre.org/threads/27167-How-can-i-execute-a-database-script-within-Qt
int Database::runSqlScript(const QString& fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return 0;

  QTextStream in(&file);
  QString     sql = in.readAll();
  if (sql.length() == 0)
    return 0;

  QList<int> splitPoints;
  enum { IN_STR, IN_ESC, NORMAL } state = NORMAL;
  int successCount                      = 0;

  for (int i = 0; i < sql.length(); i++) {
    const int character = sql.at(i).unicode();
    switch (state) {
    case IN_STR:
      switch (character) {
      case '\'':
        state = NORMAL;
        break;
      case '\\':
        state = IN_ESC;
        break;
      }
      break;

    case IN_ESC:
      state = IN_STR;
      break;

    case NORMAL:
      switch (character) {
      case ';':
        splitPoints.push_back(i);
        break;

      case '\'':
        state = IN_STR;
        break;
      }
    }
  }

  splitPoints.push_back(sql.length() - 1);

  for (int i = 0, j = 0; i < splitPoints.length(); i++) {
    QString statement = sql.mid(j, splitPoints.at(i) - j + 1);
    j                 = splitPoints.at(i) + 1;

    if (statement.trimmed().length() > 0) {
      QSqlQuery query(m_database);
      if (query.exec(statement)) {
        successCount++;
      }
      else {
        qDebug() << "Failed:" << statement << "\nReason:" << query.lastError();
        throw std::runtime_error{
          "Error while running SQL script (" + fileName.toStdString()
          + ") statement: " + statement.toStdString()
          + "Reason: " + query.lastError().text().toStdString()};
      }
    }
  }

  return successCount;
}

std::vector<QString> Database::getUnitNames()
{
  std::vector<QString> unitNames{};
  QSqlQuery            query{"SELECT * FROM Units", m_database};
  const int            unitNameIndex{query.record().indexOf("unitName")};

  while (query.next()) {
    const QString unitName{query.value(unitNameIndex).toString()};
    unitNames.push_back(unitName);
  }

  return unitNames;
}

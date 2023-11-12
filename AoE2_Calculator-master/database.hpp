#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <QSqlDatabase>
#include <QStringList>

class Database {
public:
  Database();

  int runSqlScript(const QString& fileName);

  QStringList getUnitNames(int ageID);

private:
  QSqlDatabase m_database;
};

#endif // DATABASE_HPP

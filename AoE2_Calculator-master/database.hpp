#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <QSqlDatabase>
#include <QStringList>

class Database {
public:
  Database();

  int runSqlScript(const QString& fileName);

  QStringList getUnitNames(int ageID);

  QStringList getBuildingNames(int ageID);

  QStringList getTechnologyNames();

  QStringList getEventNames();

private:
  QSqlDatabase m_database;
};

#endif // DATABASE_HPP

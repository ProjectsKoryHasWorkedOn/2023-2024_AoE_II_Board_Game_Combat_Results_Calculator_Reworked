#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <vector>

#include <QSqlDatabase>

class Database {
public:
  Database();

  int runSqlScript(const QString& fileName);

  std::vector<QString> getUnitNames();

private:
  QSqlDatabase m_database;
};

#endif // DATABASE_HPP

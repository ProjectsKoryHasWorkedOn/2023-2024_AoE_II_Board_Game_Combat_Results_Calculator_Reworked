#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <unordered_map>

#include <QSqlDatabase>
#include <QStringList>

#include "backend/entity.h"

class Database {
public:
  Database();

  int runSqlScript(const QString& fileName);

  QStringList getUnitNames(int ageID);

  QStringList getBuildingNames(int ageID);

  QStringList getTechnologyNames();

  QStringList getEventNames();

  std::unordered_map<std::string, Entity> getUnitEntities();

  std::unordered_map<std::string, Entity> getBuildingEntities();

  Entity getUnitModifiers(Entity entityToApplyModifiersTo, Entity entityToCheckArmorClassesOf);

private:
  QSqlDatabase m_database;
};

#endif // DATABASE_HPP

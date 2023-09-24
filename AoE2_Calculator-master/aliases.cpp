#include "aliases.h"

Aliases::Aliases() : m_entityNameToAlias{}, m_aliasToEntityName{} {}

void Aliases::add(QString entityName, QString alias)
{
  m_entityNameToAlias.insert(entityName, alias);
  m_aliasToEntityName.insert(alias, entityName);
}

QList<QString> Aliases::aliasesOf(const QString& entityName) const
{
  return m_entityNameToAlias.values(entityName);
}

QList<QString> Aliases::entityOf(const QString& alias) const
{
  return m_aliasToEntityName.values(alias);
}

QList<QString> Aliases::getAllEntityNames() const
{
  return m_entityNameToAlias.keys();
}

QList<QString> Aliases::getAllAliases() const
{
  return m_aliasToEntityName.keys();
}

#ifndef ALIASES_H
#define ALIASES_H
#include <QMultiHash>

class Aliases {
public:
  Aliases();

  void add(QString entityName, QString alias);

  QList<QString> aliasesOf(const QString& entityName) const;

  QList<QString> entityOf(const QString& alias) const;

  QList<QString> getAllEntityNames() const;

  QList<QString> getAllAliases() const;

private:
  QMultiHash<QString, QString> m_entityNameToAlias;
  QMultiHash<QString, QString> m_aliasToEntityName;
};

#endif // ALIASES_H

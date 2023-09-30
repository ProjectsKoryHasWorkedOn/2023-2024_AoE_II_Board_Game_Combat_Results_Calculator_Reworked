#pragma once
#include <QString>

class CsvEntity {
public:
  static CsvEntity fromLine(const QString& line);

  CsvEntity(
    QString entityName,
    int     entityQuantity,
    QString assistantName,
    int     assistantQuantity);

  void setEntityQuantity(int newQuantity);

  const QString& entityName() const;

  int entityQuantity() const;

  const QString& assistantName() const;

  int assistantQuantity() const;

  QString toString() const;

private:
  QString m_entityName;
  int     m_entityQuantity;
  QString m_assistantName;
  int     m_assistantQuantity;
};

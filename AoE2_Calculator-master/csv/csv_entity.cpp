#include <cassert>

#include <stdexcept>
#include <utility>

#include <QString>
#include <QStringList>
#include <QTextStream>

#include "csv/csv_entity.h"

CsvEntity CsvEntity::fromLine(const QString& line)
{
  const QStringList parts{line.split(QStringLiteral(" "))};
  assert((parts.size() == 4) && "Unexpected count of parts");
  const QString& entityName{parts[0]};
  bool           ok{false};
  const int      entityQuantity{parts[1].toInt(&ok)};

  if (!ok) {
    throw std::runtime_error{
      "Unexpected input: " + parts[1].toStdString() + " is not numeric"};
  }

  const QString& assistantName{parts[2]};
  const int      assistantQuantity{parts[3].toInt(&ok)};

  if (!ok) {
    throw std::runtime_error{
      "Unexpected input: " + parts[3].toStdString() + " is not numeric"};
  }

  return CsvEntity{
    entityName, entityQuantity, assistantName, assistantQuantity};
}

CsvEntity::CsvEntity(
  QString entityName,
  int     entityQuantity,
  QString assistantName,
  int     assistantQuantity)
  : m_entityName{std::move(entityName)}
  , m_entityQuantity{entityQuantity}
  , m_assistantName{std::move(assistantName)}
  , m_assistantQuantity{m_assistantQuantity}
{
}

void CsvEntity::setEntityQuantity(int newQuantity)
{
  m_entityQuantity = newQuantity;
}

const QString& CsvEntity::entityName() const { return m_entityName; }

int CsvEntity::entityQuantity() const { return m_entityQuantity; }

const QString& CsvEntity::assistantName() const { return m_assistantName; }

int CsvEntity::assistantQuantity() const { return m_assistantQuantity; }

QString CsvEntity::toString() const
{
  QString     buffer{};
  QTextStream stream{&buffer};

  stream << m_entityName << ' ' << m_entityQuantity << ' ' << m_assistantName
         << ' ' << m_assistantQuantity;
  return buffer;
}

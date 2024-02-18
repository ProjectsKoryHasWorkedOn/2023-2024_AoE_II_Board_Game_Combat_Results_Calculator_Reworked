#include <cassert>

#include <stdexcept>
#include <utility>

#include <QString>
#include <QStringList>
#include <QTextStream>
#include "csv/csv_player_name.h"

CsvPlayerName CsvPlayerName::fromLine(const QString& line)
{
  const QString& playerName{line};

  return CsvPlayerName{playerName};
}

CsvPlayerName::CsvPlayerName(QString playerName)
  : m_player_name{std::move(playerName)}
{
}

void CsvPlayerName::setPlayerName(QString newName)
{
  m_player_name = newName;
}

const QString& CsvPlayerName::playerName() const
{
  return m_player_name;
}

QString CsvPlayerName::toString() const
{
  QString     buffer{};
  QTextStream stream{&buffer};

  stream << m_player_name;
  return buffer;
}

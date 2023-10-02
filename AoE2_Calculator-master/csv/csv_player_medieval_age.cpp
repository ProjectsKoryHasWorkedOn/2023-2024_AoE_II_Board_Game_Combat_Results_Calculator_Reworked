#include <cassert>

#include <stdexcept>
#include <utility>

#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

#include "csv/csv_player_medieval_ages.h"

CsvPlayerMedievalAge CsvPlayerMedievalAge::fromLine(const QString& line)
{
  const QStringList parts{line.split(QStringLiteral(" "))};
  assert((parts.size() == 2) && "Unexpected count of parts");

  bool           ok{false};
  const int      playerMedievalAge{parts[0].toInt(&ok)};

  if (!ok) {
    throw std::runtime_error{
      "Unexpected input: " + parts[0].toStdString() + " is not numeric"};
  }

    const QString& playerTheAgeCorrespondsTo{parts[1]};

  return CsvPlayerMedievalAge{
    playerMedievalAge, playerTheAgeCorrespondsTo};
}

CsvPlayerMedievalAge::CsvPlayerMedievalAge(int     playerMedievalAge, QString playerTheAgeCorrespondsTo)
  : m_player_medieval_age{playerMedievalAge}
  , m_player_the_age_corresponds_to{std::move(playerTheAgeCorrespondsTo)}

{
}

void CsvPlayerMedievalAge::setPlayerMedievalAge(int newAge)
{
  m_player_medieval_age = newAge;
}

int CsvPlayerMedievalAge::PlayerMedievalAge() const { return m_player_medieval_age; }

QString CsvPlayerMedievalAge::toString() const
{
  QString     buffer{};
  QTextStream stream{&buffer};


  stream << m_player_medieval_age << ' ' << m_player_the_age_corresponds_to;
  return buffer;
}

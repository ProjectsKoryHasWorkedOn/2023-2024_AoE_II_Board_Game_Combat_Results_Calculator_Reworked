#include <stdexcept>

#include <QFile>
#include <QTextStream>

#include "csv/player_medieval_ages.h"
#include "mainwindow.hpp"

namespace {
std::vector<CsvPlayerMedievalAge> readMedievalAges()
{
  QFile file{workingDirectory.absolutePath() + playerMedievalAgesFilePath};
  if (!file.open(
        QIODeviceBase::ReadOnly | QIODeviceBase::Text,
        QFileDevice::ReadOther)) {
    throw std::runtime_error{
      "Could not open \"" + playerMedievalAgesFilePath.toStdString() + "\""};
  }

  QTextStream                       stream{&file};
  std::vector<CsvPlayerMedievalAge> playerMedievalAges{};

  for (QString line{}; !(line = stream.readLine()).isNull();) {
    playerMedievalAges.push_back(CsvPlayerMedievalAge::fromLine(line));
  }

  return playerMedievalAges;
}

void writeMedievalAges(
  const std::vector<CsvPlayerMedievalAge>& playerMedievalAges)
{
  QFile file{workingDirectory.absolutePath() + playerMedievalAgesFilePath};
  if (!file.open(
        QIODeviceBase::WriteOnly | QIODeviceBase::Text,
        QFileDevice::WriteOther)) {
    throw std::runtime_error{
      "Could not open \"" + playerMedievalAgesFilePath.toStdString() + "\""};
  }

  QTextStream stream{&file};

  for (const CsvPlayerMedievalAge& csvPlayerAge : playerMedievalAges) {
    stream << csvPlayerAge.toString() << '\n';
  }
}
} // anonymous namespace

playerMedievalAges::playerMedievalAges()
  : m_player_medieval_age{readMedievalAges()}
{
}

void playerMedievalAges::changePlayer1MedievalAge(int newAge)
{
  player1MedievalAge().setPlayerMedievalAge(newAge);
  writeMedievalAges(m_player_medieval_age);
}

void playerMedievalAges::changePlayer2MedievalAge(int newAge)
{
  player2MedievalAge().setPlayerMedievalAge(newAge);
  writeMedievalAges(m_player_medieval_age);
}

CsvPlayerMedievalAge& playerMedievalAges::player1MedievalAge()
{
  return m_player_medieval_age[0];
}

CsvPlayerMedievalAge& playerMedievalAges::player2MedievalAge()
{
  return m_player_medieval_age[1];
}

const CsvPlayerMedievalAge& playerMedievalAges::player1MedievalAge() const
{
  return m_player_medieval_age[0];
}

const CsvPlayerMedievalAge& playerMedievalAges::player2MedievalAge() const
{
  return m_player_medieval_age[1];
}

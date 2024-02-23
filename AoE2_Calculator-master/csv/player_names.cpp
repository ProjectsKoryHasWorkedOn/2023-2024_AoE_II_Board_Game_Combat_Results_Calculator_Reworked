#include <stdexcept>

#include <QFile>
#include <QTextStream>

#include "csv/player_names.h"
#include "mainwindow.hpp"

namespace {
std::vector<CsvPlayerName> readNames()
{
  QFile file{workingDirectory.absolutePath() + playerNamesFilePath};
  if (!file.open(
        QIODeviceBase::ReadOnly | QIODeviceBase::Text,
        QFileDevice::ReadOther)) {
    throw std::runtime_error{
      "Could not open \"" + playerNamesFilePath.toStdString() + "\""};
  }

  QTextStream                stream{&file};
  std::vector<CsvPlayerName> playernames{};

  for (QString line{}; !(line = stream.readLine()).isNull();) {
    playernames.push_back(CsvPlayerName::fromLine(line));
  }

  return playernames;
}

void writeNames(const std::vector<CsvPlayerName>& playernames)
{
  QFile file{workingDirectory.absolutePath() + playerNamesFilePath};
  if (!file.open(
        QIODeviceBase::WriteOnly | QIODeviceBase::Text,
        QFileDevice::WriteOther)) {
    throw std::runtime_error{
      "Could not open \"" + playerNamesFilePath.toStdString() + "\""};
  }

  QTextStream stream{&file};

  for (const CsvPlayerName& CsvPlayerName : playernames) {
    stream << CsvPlayerName.toString() << '\n';
  }
}
} // anonymous namespace

PlayerName::PlayerName() : m_player_names{readNames()}
{
}

void PlayerName::changePlayer1Name(QString newName)
{
  play1Name().setPlayerName(newName);
  writeNames(m_player_names);
}

void PlayerName::changePlayer2Name(QString newName)
{
  play2Name().setPlayerName(newName);
  writeNames(m_player_names);
}

CsvPlayerName& PlayerName::play1Name()
{
  return m_player_names[0];
}

CsvPlayerName& PlayerName::play2Name()
{
  return m_player_names[1];
}

const CsvPlayerName& PlayerName::play1Name() const
{
  return m_player_names[0];
}

const CsvPlayerName& PlayerName::play2Name() const
{
  return m_player_names[1];
}

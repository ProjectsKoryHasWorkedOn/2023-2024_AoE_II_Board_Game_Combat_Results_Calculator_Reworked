#pragma once
#include <QString>

class CsvPlayerName {
public:
  static CsvPlayerName fromLine(const QString& line);

  CsvPlayerName(QString playerName);

  void setPlayerName(QString newName);

  const QString& playerName() const;

  QString toString() const;

private:
  QString m_player_name;
};

#pragma once
#include <QString>

class CsvPlayerMedievalAge {
public:
  static CsvPlayerMedievalAge fromLine(const QString& line);

  CsvPlayerMedievalAge(
    int playerMedievalAge,
    QString playerTheAgeCorrespondsTo);

  void setPlayerMedievalAge(int newAge);

  int PlayerMedievalAge() const;

  QString toString() const;

private:
  int     m_player_medieval_age;
  QString m_player_the_age_corresponds_to;
};



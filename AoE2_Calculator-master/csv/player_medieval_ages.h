#ifndef PLAYER_MEDIEVAL_AGES_H
#define PLAYER_MEDIEVAL_AGES_H
#include <vector>

#include <QString>

#include "csv/csv_player_medieval_ages.h"

class playerMedievalAges {
public:
  playerMedievalAges();

  void changePlayer1MedievalAge(int newAge);

  void changePlayer2MedievalAge(int newAge);

  CsvPlayerMedievalAge& player1MedievalAge();

  CsvPlayerMedievalAge& player2MedievalAge();

  const CsvPlayerMedievalAge& player1MedievalAge() const;

  const CsvPlayerMedievalAge& player2MedievalAge() const;

private:
  std::vector<CsvPlayerMedievalAge> m_player_medieval_age;
};

#endif // PLAYER_MEDIEVAL_AGES_H

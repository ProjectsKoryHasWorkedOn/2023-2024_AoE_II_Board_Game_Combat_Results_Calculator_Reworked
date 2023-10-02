#ifndef PLAYER_NAMES_H
#define PLAYER_NAMES_H
#include <vector>

#include <QString>

#include "csv/csv_player_name.h"

class PlayerName {
public:
  PlayerName();

  void changePlayer1Name(QString newName);

  void changePlayer2Name(QString newName);

  CsvPlayerName& play1Name();

  CsvPlayerName& play2Name();

  const CsvPlayerName& play1Name() const;

  const CsvPlayerName& play2Name() const;

private:
  std::vector<CsvPlayerName> m_player_names;
};

#endif // PLAYER_NAMES_H

#ifndef RUN_GAME_H
#define RUN_GAME_H
#include "entity_output_config.h"

#include "player.h"
#include "player_selection_memory.h"
#include <QDir>

#include <QString>

#include <functional>

class Database;

extern const int technologiesRows;
extern const int eventsRows;
extern const int playerAgeRows;

void runGame(
  Database*                         database,
  std::function<void(Player, bool)> onPlayerEntityDeath,
  int                               distanceBetweenTheBattleParticipants,
  EntityOutputConfig                entityOutputConfig,
  const QString&                    attackMonksQueryingMode,
  PlayerSelectionMemory             resetMemoryOfPlayerSelectionOnDeath,
  std::string directoryWeAreWorkingIn);

#endif // RUN_GAME_H

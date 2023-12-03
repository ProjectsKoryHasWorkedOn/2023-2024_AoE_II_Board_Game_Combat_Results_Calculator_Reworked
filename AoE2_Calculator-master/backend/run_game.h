#ifndef RUN_GAME_H
#define RUN_GAME_H
#include "player.h"
#include <functional>

class Database;

int runGame(
  Database*                         database,
  std::function<void(Player, bool)> onPlayerEntityDeath);

#endif // RUN_GAME_H

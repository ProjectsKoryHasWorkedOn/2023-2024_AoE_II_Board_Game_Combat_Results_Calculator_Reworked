/** The headers  **/
#include "combatCalculator.h" // Using: combat calculator class
#include "dialog_input.h"
#include "entity.h"              // Using: entity class
#include "fileImporter.h"        // Using: fileImporter class
#include "modifiersCalculator.h" // Using: modifiers calculator class
#include <cstdlib>               // Using: exit(EXIT_FAILURE)
#include <iostream>              // Using: cin, cout
#include <memory>
#include <string> // Using: string

/** Output the remaning damage **/
void outputRemainingDamage(
  float inputRemainingDamageP1,
  float inputRemainingDamageP2)
{
  // Behaviour: Check the remaining damage value
  std::cout << "remaning damage value p1: " << inputRemainingDamageP1 << "<br>";
  std::cout << "remaning damage value p2: " << inputRemainingDamageP2 << "<br>";
}

// Ranged
//  + If it is Player1's turn and they have ranged units and Player2 has at
//  least one monk
//    Player1 can choose to attack the monks instead of doing the ranged round.
//  + If it is Player2's turn and they have ranged units and Player1 has at
//  least one monk
//    Player2 can choose to attack the monks instead of doing the ranged round.
//  + If both choose do attack the others monks -> ActivePlayer::Both for
//  FightMonksRound(Ranged)
//    and ActivePlayer::None for Ranged.
//  + If only one chooses to attack the other player's monks use that player for
//    FightMonksRound(Ranged) and the other player will do the regular ranged
//    round.
//  + If a player can't attack monks because they have no ranged units or their
//  opponent has
//    no monks, they automatically choose the regular ranged round.

// Melee
//  + Works like Ranged, except you can always attack the monks if they have
//  some and you have
//    a real unit (not a building).

// * CHANGE NUMBER OF EVENTS AND TECHNOLOGIES HERE
extern const int technologiesRows = 19, eventsRows = 41, playerAgeRows = 2;

static bool queryIfMonksShouldBeFought(
  FightMonksRounds::Kind kind,
  const std::string&     playerName)
{
  std::cout << "Hey " << playerName << ", ";
  std::cout << "do you want to fight the monks";

  switch (kind) {
  case FightMonksRounds::Kind::Ranged:
    std::cout << " with ranged attacks?<br>";
    break;
  case FightMonksRounds::Kind::Melee:
    std::cout << " with melee attacks?<br>";
    break;
  default:
    Q_UNREACHABLE();
  }

  bool shouldFightMonks{false};
  DIN >> shouldFightMonks;
  return shouldFightMonks;
}

static ActivePlayer getActivePlayerForFightMonks(
  bool doesPlayer1WantToAttackMonks,
  bool doesPlayer2WantToAttackMonks)
{
  if (doesPlayer1WantToAttackMonks && doesPlayer2WantToAttackMonks) {
    return ActivePlayer::Both;
  }

  if (!doesPlayer1WantToAttackMonks && !doesPlayer2WantToAttackMonks) {
    return ActivePlayer::None;
  }

  if (doesPlayer1WantToAttackMonks) {
    return ActivePlayer::Player1;
  }

  return ActivePlayer::Player2;
}

static ActivePlayer getActivePlayerForNormalCombatRound(
  bool doesPlayer1WantToAttackMonks,
  bool doesPlayer2WantToAttackMonks)
{
  if (doesPlayer1WantToAttackMonks && doesPlayer2WantToAttackMonks) {
    return ActivePlayer::None;
  }

  if (!doesPlayer1WantToAttackMonks && !doesPlayer2WantToAttackMonks) {
    return ActivePlayer::Both;
  }

  if (doesPlayer1WantToAttackMonks) {
    return ActivePlayer::Player2;
  }

  return ActivePlayer::Player1;
}

/** The main function **/
void runGame(
  Database*                         database,
  std::function<void(Player, bool)> onPlayerEntityDeath)
{
  /** Simple declarations **/
  // Integer: The player numbers
  int player1 = 1, player2 = 2;

  // String: Store player responses
  std::string getResponse = "";

  // Constant integer: The number of words inside the 'entities.csv' file
  const int entitiesWords = 8;

  // Constant integer: The number of rows inside of the technology, event, and
  // player details files

  // Integer: The rounds of combat
  int numberOfMonkCombatRounds        = 1;
  int numberOfArcherCombatRounds      = 1;
  int numberOfBombardmentCombatRounds = 1;
  int numberOfNormalCombatRounds      = 2;

  // Integer: Modifiers to the attack dealt in each round of combat for p1/p2
  int modifyRoundAttackP1 = 0, modifyRoundAttackP2 = 0;

  // Integer: The remaining damage within each round (the non-rounded down
  // value)
  float p1RemainingDamage = 0, p2RemainingDamage = 0;

  // Boolean: Whether event 4 is active
  bool isEvent4Active = false;

  // Integer array: The technologies
  int* p1_technologies_array;
  int* p2_technologies_array;

  // Integer array: The events
  int* p1_events_array;
  int* p2_events_array;

  // Integer array: The player details
  int* player_age_array;

  // String: The player names. Obtained from player_age_array
  std::string* playerNamesArray;

  /** Complex declarations **/
  // Structure: The entities
  Entity p1BattleParticipant;
  Entity p1AssistingMonkBattleParticipant;
  Entity p2BattleParticipant;
  Entity p2AssistingMonkBattleParticipant;

  // Object: The file importing object
  fileImporter importFile{database};

  // Object: The modifiers calculator object
  modifiersCalculator theModifiersCalculator{database};

  // Object: The combat calculator superclass and the combat rounds subclasses
  CombatCalculatorState     combatCalculatorState{};
  CombatCalculatorCallbacks combatCalculatorCallbacks{onPlayerEntityDeath};
  combatCalculator*         theCombatCalculator;
  monkRounds   monkRounds{&combatCalculatorState, &combatCalculatorCallbacks};
  archerRounds rangedRounds{&combatCalculatorState, &combatCalculatorCallbacks};
  bombardmentRounds bombardmentRounds{
    &combatCalculatorState, &combatCalculatorCallbacks};
  standardRounds standardRounds{
    &combatCalculatorState, &combatCalculatorCallbacks};
  FightMonksRounds fightMonksRangedRounds{
    &combatCalculatorState,
    &combatCalculatorCallbacks,
    FightMonksRounds::Kind::Ranged};
  FightMonksRounds fightMonksMeleeRounds{
    &combatCalculatorState,
    &combatCalculatorCallbacks,
    FightMonksRounds::Kind::Melee};

  /** Part 1: Getting basic information about the input entities **/
  // Behaviour: Load "entities.csv" and get information about the input entities
  // Player 1
  p1BattleParticipant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player1, 0);
  p1AssistingMonkBattleParticipant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player1, 1);

  // Player 2
  p2BattleParticipant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player2, 0);
  p2AssistingMonkBattleParticipant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player2, 1);

  // Behaviour: Load "technologies_[p1/p2].csv" and get information about the
  // active technologies Player 1
  p1_technologies_array = importFile.aSplitColumnFile(
    "import/technologies_p1.csv", technologiesRows);
  std::unique_ptr<int[]> p1TechUp(p1_technologies_array);

  // Player 2
  p2_technologies_array = importFile.aSplitColumnFile(
    "import/technologies_p2.csv", technologiesRows);
  std::unique_ptr<int[]> p2TechUp(p2_technologies_array);

  // Behaviour: Load "events_[p1/p2].csv" and get information about the active
  // events Player 1
  p1_events_array
    = importFile.aSplitColumnFile("import/events_p1.csv", eventsRows);
  std::unique_ptr<int[]> p1EventsUp(p1_events_array);

  // Player 2
  p2_events_array
    = importFile.aSplitColumnFile("import/events_p2.csv", eventsRows);
  std::unique_ptr<int[]> p2EventsUp(p2_events_array);

  // Behaviour: Load "players.csv"and store information about the player details
  // for all players
  player_age_array
    = importFile.aSplitColumnFile("import/playerAge.csv", playerAgeRows);
  std::unique_ptr<int[]> playerAgeUp(player_age_array);

  playerNamesArray = importFile.playerNames("import/playerNames.csv", 2);
  std::unique_ptr<std::string[]> playerNamesUp(playerNamesArray);

  // Behavior: Work out what the initial quantity values are
  p1BattleParticipant.initialEntityQuantity
    = p1BattleParticipant.entityQuantity;
  p2BattleParticipant.initialEntityQuantity
    = p2BattleParticipant.entityQuantity;
  p1AssistingMonkBattleParticipant.initialEntityQuantity
    = p1AssistingMonkBattleParticipant.entityQuantity;
  p2AssistingMonkBattleParticipant.initialEntityQuantity
    = p2AssistingMonkBattleParticipant.entityQuantity;

  /** Part 2: Applying modifiers to the input entities **/
  // Behaviour: Set the battle participants
  theModifiersCalculator.setEntities(
    p1BattleParticipant,
    p2BattleParticipant,
    p1AssistingMonkBattleParticipant,
    p2AssistingMonkBattleParticipant);

  // Behaviour: Set the values for player 1
  theModifiersCalculator.setAdditionalValues(
    player1, player_age_array[0], p1_technologies_array, p1_events_array);

  // Behaviour: Run a function to apply all of the modifiers for player 1
  p1BattleParticipant = theModifiersCalculator.applyAllModifiers(0);
  p1AssistingMonkBattleParticipant
    = theModifiersCalculator.applyAllModifiers(1);

  // Set the values for player 2
  theModifiersCalculator.setAdditionalValues(
    player2, player_age_array[1], p2_technologies_array, p2_events_array);

  // Behaviour: Run a function to apply all of the modifiers for player 2
  p2BattleParticipant = theModifiersCalculator.applyAllModifiers(0);
  p2AssistingMonkBattleParticipant
    = theModifiersCalculator.applyAllModifiers(1);

  // Behaviour: Return information about the input entities once they have been
  // modified (before further calculations occur)
  std::cout << "You entered..."
            << "<br>";
  p1BattleParticipant.outputEntity(playerNamesArray[0]);
  if (p1AssistingMonkBattleParticipant.entityQuantity > 0) {
    std::cout << "(Assisting) ";
    p1AssistingMonkBattleParticipant.outputEntity(playerNamesArray[0]);
  }
  p2BattleParticipant.outputEntity(playerNamesArray[1]);
  if (p2AssistingMonkBattleParticipant.entityQuantity > 0) {
    std::cout << "(Assisting) ";
    p2AssistingMonkBattleParticipant.outputEntity(playerNamesArray[1]);
  }

  /** Part 3: Applying further modifiers **/
  // Event [14] Hard to Starboard - Reduce damage dealt to target ship by 10
  // points per round of combat this turn
  if (p1_events_array[14] == 1) {
    if (p1BattleParticipant.armorClass[11] == true) {
      modifyRoundAttackP2 -= 10;
    }
  }

  if (p2_events_array[14] == 1) {
    if (p2BattleParticipant.armorClass[11] == true) {
      modifyRoundAttackP1 -= 10;
    }
  }

  // Event [21] Non-Compos Mentis - Roll a 1d6 when in combat
  // On a: 1: take 10 extra damage per round.
  // 		 2: draw 2 cards
  //		 3: no effect
  //		 4: deal 10 extra damage per round
  //		 5: discard 2 cards
  //		 6: deal 15 extra damage per round.
  if (p1_events_array[21] == 1) {
    // Behaviour: Just ask the user what got rolled
    std::cout << "Enter 1 if a one got rolled. Enter 4 if a four got rolled. "
                 "Enter 6 if a six got rolled. Otherwise enter zero."
              << "<br>";
    DIN >> getResponse;

    // Behaviour: Apply the results
    if (getResponse == "1") {
      modifyRoundAttackP2 += 10;
    }
    else if (getResponse == "4") {
      modifyRoundAttackP1 += 10;
    }
    else if (getResponse == "6") {
      modifyRoundAttackP1 += 15;
    }
    else {
      // No effect
    }
  }

  if (p2_events_array[21] == 1) {
    // Behaviour: Just ask the user what got rolled
    std::cout << "Enter 1 if a one got rolled. Enter 4 if a four got rolled. "
                 "Enter 6 if a six got rolled. Otherwise enter zero."
              << "<br>";
    DIN >> getResponse;

    // Behaviour: Apply the results
    if (getResponse == "1") {
      modifyRoundAttackP1 += 10;
    }
    else if (getResponse == "4") {
      modifyRoundAttackP2 += 10;
    }
    else if (getResponse == "6") {
      modifyRoundAttackP2 += 15;
    }
    else {
      // No effect
    }
  }

  // Event [4] Caught from the Crow's Nest - One extra bombardment round if
  // there is a Galley or Fire Ship
  if ((p1_events_array[4] == 1) || (p2_events_array[4] == 1)) {
    if (
      ((p1BattleParticipant.armorClass[21] == true)
       || (p2BattleParticipant.armorClass[21] == true))
      || ((p1BattleParticipant.armorClass[22] == true) || (p2BattleParticipant.armorClass[22] == true))) {
      // Behaviour: Set event 4 to active
      isEvent4Active = true;
    }
    else {
      isEvent4Active = false;
    }
  }
  else {
    isEvent4Active = false;
  }

  // Event [38] You Will Die! (Saracen) - This battle goes for four rounds of
  // normal combat, instead of 2 rounds. No retreat is allowed without event
  // card effect.
  if ((p1_events_array[38] == 1) || (p2_events_array[38] == 1)) {
    numberOfNormalCombatRounds = 4;
  }

  // Event [13] Gladitorial Games - You and a target player must move one unit
  // to No-Man's-Land. The units will fight until one is destroyed
  if ((p1_events_array[13] == 1) || (p2_events_array[13] == 1)) {
    numberOfNormalCombatRounds = 10;
  }

  /** Part 4: Return the outcome of each round of combat for the input entities
   * **/
  // Behaviour: Output the remaining damage
  // outputRemainingDamage(p1RemainingDamage, p2RemainingDamage);

  /** Part 4.1: Round 1 **/
  // Behaviour: Set the superclass to the monk rounds
  theCombatCalculator = &monkRounds;

  // Set the player names
  theCombatCalculator->setPlayerNames(playerNamesArray[0], playerNamesArray[1]);

  // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1AssistingMonkBattleParticipant,
    p2AssistingMonkBattleParticipant,
    modifyRoundAttackP1,
    modifyRoundAttackP2);

  // Behaviour: Set the remaining damage values for the combat calculator
  theCombatCalculator->setAdditionalValues(
    p1RemainingDamage, p2RemainingDamage);

  // Behaviour: Calculate the damage dealt for numberOfMonkCombatRounds rounds
  // of monk combat
  monkRounds.roundOutcome(
    numberOfMonkCombatRounds,
    p1_events_array,
    p2_events_array,
    p1_technologies_array,
    p2_technologies_array,
    ActivePlayer::Both);

  // Behaviour: Get the results after numberOfMonkCombatRounds rounds of monk
  // combat Player 1
  p1BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player1);
  p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

  // Player 2
  p2BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player2);
  p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);

  /** Part 4.2: Round 2 **/
  // Proceed with archer round of combat so long as archers are not fighting
  // buildings Ranged damage applies only to units and villagers, not to
  // buildings

  // Check if player1 is able to attack player2's monks with ranged attacks.
  bool player1UsesRangedAttacksAgainstMonks{false};
  if (
    (p1BattleParticipant.rangedDamage > 0)
    && (p2AssistingMonkBattleParticipant.entityQuantity > 0)) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Ranged, playerNamesArray[0])};
    player1UsesRangedAttacksAgainstMonks = shouldFightMonks;
  }

  bool player2UsesRangedAttacksAgainstMonks{false};
  if (
    (p2BattleParticipant.rangedDamage > 0)
    && (p1AssistingMonkBattleParticipant.entityQuantity > 0)) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Ranged, playerNamesArray[1])};
    player2UsesRangedAttacksAgainstMonks = shouldFightMonks;
  }

  theCombatCalculator = &fightMonksRangedRounds;
  // Set the player names
  theCombatCalculator->setPlayerNames(playerNamesArray[0], playerNamesArray[1]);

  // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1AssistingMonkBattleParticipant,
    p2AssistingMonkBattleParticipant,
    modifyRoundAttackP1,
    modifyRoundAttackP2);

  // Behaviour: Set the remaining damage values for the combat calculator
  theCombatCalculator->setAdditionalValues(
    p1RemainingDamage, p2RemainingDamage);

  const ActivePlayer fightMonksRangedRoundsActivePlayer{
    getActivePlayerForFightMonks(
      player1UsesRangedAttacksAgainstMonks,
      player2UsesRangedAttacksAgainstMonks)};
  qDebug() << ">>>>>>>> active player for fight monks ranged:"
           << fightMonksRangedRoundsActivePlayer;
  theCombatCalculator->roundOutcome(
    numberOfArcherCombatRounds,
    p1_events_array,
    p2_events_array,
    p1_technologies_array,
    p2_technologies_array,
    fightMonksRangedRoundsActivePlayer);

  // Behaviour: Get the results after numberOfArcherCombatRounds rounds of
  // ranged combat Player 1
  p1BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player1);
  p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

  // Player 2
  p2BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player2);
  p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);

  // +-+-+-+-+-+-+-+-+
  // +-+-+-+-+-+-+-+-+
  // +-+-+-+-+-+-+-+-+
  // +-+-+-+-+-+-+-+-+
  // +-+-+-+-+-+-+-+-+
  // Ranged round
  theCombatCalculator = &rangedRounds;

  // Who knows if we need this?
  // Set the player names
  theCombatCalculator->setPlayerNames(playerNamesArray[0], playerNamesArray[1]);

  // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1AssistingMonkBattleParticipant,
    p2AssistingMonkBattleParticipant,
    modifyRoundAttackP1,
    modifyRoundAttackP2);

  // Behaviour: Set the remaining damage values for the combat calculator
  theCombatCalculator->setAdditionalValues(
    p1RemainingDamage, p2RemainingDamage);

  const ActivePlayer rangedRoundActivePlayer{
    getActivePlayerForNormalCombatRound(
      player1UsesRangedAttacksAgainstMonks,
      player2UsesRangedAttacksAgainstMonks)};
  qDebug() << ">>>>>>>> Active player for ranged rounds:"
           << rangedRoundActivePlayer;
  theCombatCalculator->roundOutcome(
    numberOfArcherCombatRounds,
    p1_events_array,
    p2_events_array,
    p1_technologies_array,
    p2_technologies_array,
    rangedRoundActivePlayer);

  // Behaviour: Get the results after numberOfArcherCombatRounds rounds of
  // ranged combat Player 1
  p1BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player1);
  p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

  // Player 2
  p2BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player2);
  p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);

  // Behaviour: Output the remaining damage
  // outputRemainingDamage(p1RemainingDamage, p2RemainingDamage);

  /** Part 4.3: Bonus round **/
  // Behaviour: Check for the Caught from the Crow's Nest extra bombardment
  // round Reference: I otherwise deal with a single bombardment round within
  // the standardCombat subclass
  if (isEvent4Active == true) {
    // Behaviour: Set the combat calculator to the bombardment rounds
    theCombatCalculator = &bombardmentRounds;

    // Set the player names
    theCombatCalculator->setPlayerNames(
      playerNamesArray[0], playerNamesArray[1]);

    // Behaviour: Set the protected values
    theCombatCalculator->setCombatParticipants(
      p1BattleParticipant,
      p2BattleParticipant,
      p1AssistingMonkBattleParticipant,
      p2AssistingMonkBattleParticipant,
      modifyRoundAttackP1,
      modifyRoundAttackP2);

    // Behaviour: Set the remaining damage values for the combat calculator
    theCombatCalculator->setAdditionalValues(
      p1RemainingDamage, p2RemainingDamage);

    // Behaviour: Calculate the damage dealt for numberOfBombardmentCombatRounds
    // rounds of bombardment combat and display the results
    bombardmentRounds.roundOutcome(
      numberOfBombardmentCombatRounds,
      p1_events_array,
      p2_events_array,
      p1_technologies_array,
      p2_technologies_array,
      ActivePlayer::Player1);

    // Behaviour: Get the results after numberOfBombardmentCombatRounds rounds
    // of standard combat Player 1
    p1BattleParticipant
      = theCombatCalculator->returnModifiedBattleParticipants(player1);
    p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

    // Player 2
    p2BattleParticipant
      = theCombatCalculator->returnModifiedBattleParticipants(player2);
    p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);
  }

  /** Part 4.4: Round 3 & 4 **/

  // @ Phillip: Ask about how we can not ask question on if we're attacking
  // monks or not when there are no monks This seems to stop it from proceeding
  // static_cast ...

  ///
  // This is the fight monks melee round
  ///
  bool player1UsesMeleeAttacksAgainstMonks{false};
  if (p2AssistingMonkBattleParticipant.entityQuantity > 0) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Melee, playerNamesArray[0])};
    player1UsesMeleeAttacksAgainstMonks = shouldFightMonks;
  }

  bool player2UsesMeleeAttacksAgainstMonks{false};
  if (p1AssistingMonkBattleParticipant.entityQuantity > 0) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Melee, playerNamesArray[1])};
    player2UsesMeleeAttacksAgainstMonks = shouldFightMonks;
  }

  theCombatCalculator = &fightMonksMeleeRounds;
  // Set the player names
  theCombatCalculator->setPlayerNames(playerNamesArray[0], playerNamesArray[1]);

  // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1AssistingMonkBattleParticipant,
    p2AssistingMonkBattleParticipant,
    modifyRoundAttackP1,
    modifyRoundAttackP2);

  // Behaviour: Set the remaining damage values for the combat calculator
  theCombatCalculator->setAdditionalValues(
    p1RemainingDamage, p2RemainingDamage);

  const ActivePlayer fightMonksMeleeRoundsActivePlayer{
    getActivePlayerForFightMonks(
      player1UsesMeleeAttacksAgainstMonks,
      player2UsesMeleeAttacksAgainstMonks)};
  qDebug() << ">>>>>>>> active player for fight monks melee:"
           << fightMonksMeleeRoundsActivePlayer;
  theCombatCalculator->roundOutcome(
    numberOfNormalCombatRounds,
    p1_events_array,
    p2_events_array,
    p1_technologies_array,
    p2_technologies_array,
    fightMonksMeleeRoundsActivePlayer);

  // Behaviour: Get the results after numberOfNormalCombatRounds rounds of
  // standard combat Player 1
  p1BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player1);
  p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

  // Player 2
  p2BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player2);
  p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);

  ///
  // This is the regular melee round
  ///
  theCombatCalculator = &standardRounds;
  const ActivePlayer regularMeleeRoundActivePlayer{
    getActivePlayerForNormalCombatRound(
      player1UsesMeleeAttacksAgainstMonks,
      player2UsesMeleeAttacksAgainstMonks)};
  qDebug() << ">>>>>>>> active player for regular melee round:"
           << regularMeleeRoundActivePlayer;
  theCombatCalculator->roundOutcome(
    numberOfNormalCombatRounds,
    p1_events_array,
    p2_events_array,
    p1_technologies_array,
    p2_technologies_array,
    regularMeleeRoundActivePlayer);

  // Behaviour: Get the results after numberOfNormalCombatRounds rounds of
  // standard combat Player 1
  p1BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player1);
  p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

  // Player 2
  p2BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player2);
  p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);
}

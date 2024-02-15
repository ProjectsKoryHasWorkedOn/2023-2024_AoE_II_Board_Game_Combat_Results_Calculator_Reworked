/** The headers  **/
#include "combatCalculator.h" // Using: combat calculator class
#include "dialog_input.h"
#include "entity.h"              // Using: entity class
#include "fileImporter.h"        // Using: fileImporter class
#include "modifiersCalculator.h" // Using: modifiers calculator class
#include "run_game.h"

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
extern const int technologiesRows = 19, eventsRows = 42, playerAgeRows = 2;

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
  std::function<void(Player, bool)> onPlayerEntityDeath,
  int distanceBetweenTheBattleParticipants,
  EntityOutputConfig entityOutputConfig)
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
  bool isThereAStandaloneBombardmentCombatRound = false; // Normally it's like apart of the first archer / normal combat round

         // Integer: Modifiers to the attack dealt in each round of combat for p1/p2
  int modifyRoundAttackP1 = 0, modifyRoundAttackP2 = 0;

         // Integer: The remaining damage within each round (the non-rounded down
         // value)
  float p1RemainingDamage = 0, p2RemainingDamage = 0;

         // Integer array: The technologies
  int* p1Technologies;
  int* p2Technologies;

         // Integer array: The events
  int* p1Events;
  int* p2Events;

         // Integer array: The player details
  int* playerAges;

         // String: The player names
  std::string* playerNames;

  /** Complex declarations **/
  // Structure: The entities
  Entity p1BattleParticipant;
  Entity p1BattleAssistant;
  Entity p2BattleParticipant;
  Entity p2BattleAssistant;


         // Object: The file importing object
  fileImporter importFile{database};


  /** Part 1: Getting basic information about the input entities **/
  // Behaviour: Load "entities.csv" and get information about the input entities
  // Player 1
  p1BattleParticipant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player1, 0);
  p1BattleAssistant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player1, 1);

         // Player 2
  p2BattleParticipant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player2, 0);
  p2BattleAssistant
    = importFile.entitiesFile("import/entities.csv", entitiesWords, player2, 1);

         // Behaviour: Load "technologies_[p1/p2].csv" and get information about the
         // active technologies Player 1
  p1Technologies = importFile.aSplitColumnFile(
    "import/technologies_p1.csv", technologiesRows);
  std::unique_ptr<int[]> p1TechUp(p1Technologies);

         // Player 2
  p2Technologies = importFile.aSplitColumnFile(
    "import/technologies_p2.csv", technologiesRows);
  std::unique_ptr<int[]> p2TechUp(p2Technologies);

         // Behaviour: Load "events_[p1/p2].csv" and get information about the active
         // events Player 1
  p1Events
    = importFile.aSplitColumnFile("import/events_p1.csv", eventsRows);
  std::unique_ptr<int[]> p1EventsUp(p1Events);

         // Player 2
  p2Events
    = importFile.aSplitColumnFile("import/events_p2.csv", eventsRows);
  std::unique_ptr<int[]> p2EventsUp(p2Events);

         // Behaviour: Load "players.csv"and store information about the player details
         // for all players
  playerAges
    = importFile.aSplitColumnFile("import/playerAge.csv", playerAgeRows);
  std::unique_ptr<int[]> playerAgeUp(playerAges);

  playerNames = importFile.playerNames("import/playerNames.csv", 2);
  std::unique_ptr<std::string[]> playerNamesUp(playerNames);

         // Behavior: Work out what the initial quantity values are
  p1BattleParticipant.initialEntityQuantity
    = p1BattleParticipant.entityQuantity;
  p2BattleParticipant.initialEntityQuantity
    = p2BattleParticipant.entityQuantity;
  p1BattleAssistant.initialEntityQuantity
    = p1BattleAssistant.entityQuantity;
  p2BattleAssistant.initialEntityQuantity
    = p2BattleAssistant.entityQuantity;



         // Object: The modifiers calculator object
  modifiersCalculator theModifiersCalculator{database};

         // Object: The combat calculator superclass and the combat rounds subclasses
  CombatCalculatorState     combatCalculatorState{distanceBetweenTheBattleParticipants,
              entityOutputConfig};

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






  /** Part 2: Applying modifiers to the input entities **/
  // Behaviour: Set the battle participants
  theModifiersCalculator.setEntities(
    p1BattleParticipant,
    p2BattleParticipant,
    p1BattleAssistant,
    p2BattleAssistant);

         // Behaviour: Set the values for player 1
  theModifiersCalculator.setAdditionalValues(
    player1, playerAges[0], p1Technologies, p1Events);

         // Behaviour: Run a function to apply all of the modifiers for player 1
  p1BattleParticipant = theModifiersCalculator.applyAllModifiers(0);
  p1BattleAssistant
    = theModifiersCalculator.applyAllModifiers(1);

         // Set the values for player 2
  theModifiersCalculator.setAdditionalValues(
    player2, playerAges[1], p2Technologies, p2Events);

         // Behaviour: Run a function to apply all of the modifiers for player 2
  p2BattleParticipant = theModifiersCalculator.applyAllModifiers(0);
  p2BattleAssistant
    = theModifiersCalculator.applyAllModifiers(1);

         // Behaviour: Return information about the input entities once they have been
         // modified (before further calculations occur)
  std::cout << "You entered..."
            << "<br>";
  p1BattleParticipant.outputEntity(playerNames[0], entityOutputConfig);
  if (p1BattleAssistant.entityQuantity > 0) {
    std::cout << "(Assisting) ";
    p1BattleAssistant.outputEntity(playerNames[0], entityOutputConfig);
  }
  p2BattleParticipant.outputEntity(playerNames[1], entityOutputConfig);
  if (p2BattleAssistant.entityQuantity > 0) {
    std::cout << "(Assisting) ";
    p2BattleAssistant.outputEntity(playerNames[1], entityOutputConfig);
  }

  /** Part 3: Applying further modifiers **/
  // Event [14] Hard to Starboard - Reduce damage dealt to target ship by 10
  // points per round of combat this turn
  if (p1Events[14] == 1) {
    if (p1BattleParticipant.armorClass[11] == true) {
      modifyRoundAttackP2 -= 10;
    }
  }

  if (p2Events[14] == 1) {
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
  if (p1Events[21] == 1) {
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

  if (p2Events[21] == 1) {
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
  if ((p1Events[4] == 1) || (p2Events[4] == 1)) {
    if (
      ((p1BattleParticipant.armorClass[21] == true)
       || (p2BattleParticipant.armorClass[21] == true))
      || ((p1BattleParticipant.armorClass[22] == true) || (p2BattleParticipant.armorClass[22] == true))) {
      // Behaviour: Make it so there's a standalone bombardment round
      isThereAStandaloneBombardmentCombatRound = true;
    }
  }

         // Event [38] You Will Die! (Saracen) - This battle goes for four rounds of
         // normal combat, instead of 2 rounds. No retreat is allowed without event
         // card effect.
  if ((p1Events[38] == 1) || (p2Events[38] == 1)) {
    numberOfNormalCombatRounds = 4;
  }

         // Event [13] Gladitorial Games - You and a target player must move one unit
         // to No-Man's-Land. The units will fight until one is destroyed
  if ((p1Events[13] == 1) || (p2Events[13] == 1)) {
    numberOfNormalCombatRounds = 10;
  }

  /** Part 4: Return the outcome of each round of combat for the input entities
   * **/
  // Behaviour: Output the remaining damage
  // outputRemainingDamage(p1RemainingDamage, p2RemainingDamage);

  /** Part 4.1: Round 1 **/
  // Behaviour: Set the superclass to the monk rounds
  theCombatCalculator = &monkRounds;

  theCombatCalculator->setStartingQuantites();


         // Set the player names
  theCombatCalculator->setPlayerNames(playerNames[0], playerNames[1]);

         // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1BattleAssistant,
    p2BattleAssistant,
    modifyRoundAttackP1,
    modifyRoundAttackP2);

         // Behaviour: Set the remaining damage values for the combat calculator
  theCombatCalculator->setAdditionalValues(
    p1RemainingDamage, p2RemainingDamage);


         // Behaviour: Calculate the damage dealt for numberOfMonkCombatRounds rounds
         // of monk combat
  monkRounds.roundOutcome(
    numberOfMonkCombatRounds,
    p1Events,
    p2Events,
    p1Technologies,
    p2Technologies,
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
    && (p2BattleAssistant.entityQuantity > 0)) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Ranged, playerNames[0])};
    player1UsesRangedAttacksAgainstMonks = shouldFightMonks;
  }

  bool player2UsesRangedAttacksAgainstMonks{false};
  if (
    (p2BattleParticipant.rangedDamage > 0)
    && (p1BattleAssistant.entityQuantity > 0)) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Ranged, playerNames[1])};
    player2UsesRangedAttacksAgainstMonks = shouldFightMonks;
  }

  theCombatCalculator = &fightMonksRangedRounds;

  theCombatCalculator->setStartingQuantites();


         // Set the player names
  theCombatCalculator->setPlayerNames(playerNames[0], playerNames[1]);

         // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1BattleAssistant,
    p2BattleAssistant,
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
    p1Events,
    p2Events,
    p1Technologies,
    p2Technologies,
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

  theCombatCalculator->setStartingQuantites();


         // Set the player names
  theCombatCalculator->setPlayerNames(playerNames[0], playerNames[1]);

         // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1BattleAssistant,
    p2BattleAssistant,
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
    p1Events,
    p2Events,
    p1Technologies,
    p2Technologies,
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
  if(isThereAStandaloneBombardmentCombatRound == true){
  // Bombardment round
  ///

    bool player1UsesBombardmentAgainstMonks{false};
    if (p2BattleAssistant.entityQuantity > 0) {
      const bool shouldFightMonks{queryIfMonksShouldBeFought(
        FightMonksRounds::Kind::Melee, playerNames[0])};
      player1UsesBombardmentAgainstMonks = shouldFightMonks;
    }

    bool player2UsesBombardmentAgainstMonks{false};
    if (p1BattleAssistant.entityQuantity > 0) {
      const bool shouldFightMonks{queryIfMonksShouldBeFought(
        FightMonksRounds::Kind::Melee, playerNames[1])};
      player2UsesBombardmentAgainstMonks = shouldFightMonks;
    }

           // Behaviour: Set the combat calculator to the bombardment rounds
    theCombatCalculator = &fightMonksMeleeRounds;

    theCombatCalculator->setStartingQuantites();


           // Set the player names
    theCombatCalculator->setPlayerNames(playerNames[0], playerNames[1]);

           // Behaviour: Set the battle participants
    theCombatCalculator->setCombatParticipants(
      p1BattleParticipant,
      p2BattleParticipant,
      p1BattleAssistant,
      p2BattleAssistant,
      modifyRoundAttackP1,
      modifyRoundAttackP2);

           // Behaviour: Set the remaining damage values for the combat calculator
    theCombatCalculator->setAdditionalValues(
      p1RemainingDamage, p2RemainingDamage);

    const ActivePlayer bombardMonksActivePlayer{getActivePlayerForFightMonks(
      player1UsesBombardmentAgainstMonks, player2UsesBombardmentAgainstMonks)};
    qDebug() << ">>>>>>>>>> active player for bombard monks:"
             << bombardMonksActivePlayer;
    theCombatCalculator->roundOutcome(
      numberOfBombardmentCombatRounds,
      p1Events,
      p2Events,
      p1Technologies,
      p2Technologies,
      bombardMonksActivePlayer);

           // Behaviour: Get the results after numberOfBombardmentCombatRounds rounds
           // of standard combat Player 1
    p1BattleParticipant
      = theCombatCalculator->returnModifiedBattleParticipants(player1);
    p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

           // Player 2
    p2BattleParticipant
      = theCombatCalculator->returnModifiedBattleParticipants(player2);
    p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);

           ///
           // Normal bombardment round
           ///
           ///

           // Behaviour: Set the combat calculator to the bombardment rounds
    theCombatCalculator = &bombardmentRounds;

    theCombatCalculator->setStartingQuantites();


           // Set the player names
    theCombatCalculator->setPlayerNames(playerNames[0], playerNames[1]);

           // Behaviour: Set the battle participants
    theCombatCalculator->setCombatParticipants(
      p1BattleParticipant,
      p2BattleParticipant,
      p1BattleAssistant,
      p2BattleAssistant,
      modifyRoundAttackP1,
      modifyRoundAttackP2);

           // Behaviour: Set the remaining damage values for the combat calculator
    theCombatCalculator->setAdditionalValues(
      p1RemainingDamage, p2RemainingDamage);


    const ActivePlayer normalBombardmentActivePlayer{
                                                     getActivePlayerForNormalCombatRound(
                                                       player1UsesBombardmentAgainstMonks,
                                                       player2UsesBombardmentAgainstMonks)};
    qDebug() << ">>>>>>>> active player for normal bombardment round:"
             << normalBombardmentActivePlayer;

           // Behaviour: Calculate the damage dealt for numberOfBombardmentCombatRounds
           // rounds of bombardment combat and display the results
    bombardmentRounds.roundOutcome(
      numberOfBombardmentCombatRounds,
      p1Events,
      p2Events,
      p1Technologies,
      p2Technologies,
      normalBombardmentActivePlayer);

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
  if (p2BattleAssistant.entityQuantity > 0) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Melee, playerNames[0])};
    player1UsesMeleeAttacksAgainstMonks = shouldFightMonks;
  }

  bool player2UsesMeleeAttacksAgainstMonks{false};
  if (p1BattleAssistant.entityQuantity > 0) {
    const bool shouldFightMonks{queryIfMonksShouldBeFought(
      FightMonksRounds::Kind::Melee, playerNames[1])};
    player2UsesMeleeAttacksAgainstMonks = shouldFightMonks;
  }




  theCombatCalculator = &fightMonksMeleeRounds;

  theCombatCalculator->setStartingQuantites();


         // Set the player names
  theCombatCalculator->setPlayerNames(playerNames[0], playerNames[1]);

         // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1BattleAssistant,
    p2BattleAssistant,
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
    p1Events,
    p2Events,
    p1Technologies,
    p2Technologies,
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

         ///
         ///
  theCombatCalculator = &standardRounds;

  theCombatCalculator->setStartingQuantites();


         // Set the player names
  theCombatCalculator->setPlayerNames(playerNames[0], playerNames[1]);

         // Behaviour: Set the battle participants
  theCombatCalculator->setCombatParticipants(
    p1BattleParticipant,
    p2BattleParticipant,
    p1BattleAssistant,
    p2BattleAssistant,
    modifyRoundAttackP1,
    modifyRoundAttackP2);

         // Behaviour: Set the remaining damage values for the combat calculator
  theCombatCalculator->setAdditionalValues(
    p1RemainingDamage, p2RemainingDamage);



  const ActivePlayer regularMeleeRoundActivePlayer{
                                                   getActivePlayerForNormalCombatRound(
                                                     player1UsesMeleeAttacksAgainstMonks,
                                                     player2UsesMeleeAttacksAgainstMonks)};
  qDebug() << ">>>>>>>> active player for regular melee round:"
           << regularMeleeRoundActivePlayer;
  theCombatCalculator->roundOutcome(
    numberOfNormalCombatRounds,
    p1Events,
    p2Events,
    p1Technologies,
    p2Technologies,
    regularMeleeRoundActivePlayer);
}

/** Age of Empires II: The Board Game **/
/** Combat Results Calculator **/ /** Coded by: BGG user Karate_Dog **/

/** The libaries **/
#include "combatCalculator.h" // Using: combat calculator class
#include "dialog_input.h"
#include "entity.h"              // Using: entity class
#include "fileImporter.h"        // Using: fileImporter class
#include "modifiersCalculator.h" // Using: modifiers calculator class
#include <cstdlib>               // Using: exit(EXIT_FAILURE)
#include <iostream>              // Using: cin, cout
#include <string>                // Using: string

/** Output the remaning damage **/
void outputRemainingDamage(
  float inputRemainingDamageP1,
  float inputRemainingDamageP2)
{
  // Behaviour: Check the remaining damage value
  std::cout << "remaning damage value p1: " << inputRemainingDamageP1 << "<br>";
  std::cout << "remaning damage value p2: " << inputRemainingDamageP2 << "<br>";
}

/** The main function **/
int runGame(Database* database, std::function<void(Player, bool)> onPlayerEntityDeath)
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

  // * CHANGE NUMBER OF EVENTS AND TECHNOLOGIES HERE
  const int technologiesRows = 18, eventsRows = 41, playerAgeRows = 2;

  // Integer: The rounds of combat
  int monkCombatRounds        = 1;
  int archerCombatRounds      = 1;
  int bombardmentCombatRounds = 1;
  int normalCombatRounds      = 2;

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
  modifiersCalculator theModifiersCalculator;

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

  // Player 2
  p2_technologies_array = importFile.aSplitColumnFile(
    "import/technologies_p2.csv", technologiesRows);

  // Behaviour: Load "events_[p1/p2].csv" and get information about the active
  // events Player 1
  p1_events_array
    = importFile.aSplitColumnFile("import/events_p1.csv", eventsRows);

  // Player 2
  p2_events_array
    = importFile.aSplitColumnFile("import/events_p2.csv", eventsRows);

  // Behaviour: Load "players.csv"and store information about the player details
  // for all players
  player_age_array
    = importFile.aSplitColumnFile("import/playerAge.csv", playerAgeRows);

  playerNamesArray = importFile.playerNames("import/playerNames.csv", 2);

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
    normalCombatRounds = 4;
  }

  // Event [13] Gladitorial Games - You and a target player must move one unit
  // to No-Man's-Land. The units will fight until one is destroyed
  if ((p1_events_array[13] == 1) || (p2_events_array[13] == 1)) {
    normalCombatRounds = 10;
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

  // Behaviour: Calculate the damage dealt for monkCombatRounds rounds of monk
  // combat
  monkRounds.roundOutcome(monkCombatRounds, p1_events_array, p2_events_array);

  // Behaviour: Get the results after monkCombatRounds rounds of monk combat
  // Player 1
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
  if (
    (p1BattleParticipant.armorClass[1] != true)
    && (p2BattleParticipant.armorClass[1] != true)) {
    // Behaviour: Set the combat calculator to the archer rounds
    theCombatCalculator = &rangedRounds;

    // Set the player names
    theCombatCalculator->setPlayerNames(
      playerNamesArray[0], playerNamesArray[1]);

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

    // Behaviour: Calculate the damage dealt for archerCombatRounds rounds of
    // archer combat
    rangedRounds.roundOutcome(
      archerCombatRounds, p1_events_array, p2_events_array);

    // Behaviour: Get the results after archerCombatRounds rounds of ranged
    // combat Player 1
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

      // Behaviour: Calculate the damage dealt for bombardmentCombatRounds
      // rounds of bombardment combat and display the results
      bombardmentRounds.roundOutcome(
        bombardmentCombatRounds, p1_events_array, p2_events_array);

      // Behaviour: Get the results after bombardmentCombatRounds rounds of
      // standard combat Player 1
      p1BattleParticipant
        = theCombatCalculator->returnModifiedBattleParticipants(player1);
      p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

      // Player 2
      p2BattleParticipant
        = theCombatCalculator->returnModifiedBattleParticipants(player2);
      p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);
    }
  }

  /** Part 4.4: Round 3 & 4 **/
  // Behaviour: Set the combat calculator to the standard rounds
  theCombatCalculator = &standardRounds;

  // Set the player names
  theCombatCalculator->setPlayerNames(playerNamesArray[0], playerNamesArray[1]);

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

  // Behaviour: Calculate the damage dealt for normalCombatRounds rounds of
  // standard combat and display the results
  standardRounds.roundOutcome(
    normalCombatRounds, p1_events_array, p2_events_array);

  // Behaviour: Get the results after normalCombatRounds rounds of standard
  // combat Player 1
  p1BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player1);
  p1RemainingDamage += theCombatCalculator->returnRemaningDamage(player1);

  // Player 2
  p2BattleParticipant
    = theCombatCalculator->returnModifiedBattleParticipants(player2);
  p2RemainingDamage += theCombatCalculator->returnRemaningDamage(player2);

  // Behaviour: Output the remaining damage
  // outputRemainingDamage(p1RemainingDamage, p2RemainingDamage);

  // Behaviour: A required return statement for the system in C++
  return 0;
}

/** The headers **/
#include "combatCalculator.h" // Using: calculator class
#include "dialog_input.h"     // Using: DIN
#include "entity.h"           // Using: entity class
#include "soundEffects.h"     // Using: SFXToPlay()
#include <algorithm>
#include <array>
#include <cmath>    // Using: floor
#include <cstdlib>  // Using: exit(EXIT_FAILURE), srand(), rand()
#include <iostream> // Using: cin, cout
#include <random>
#include <stdlib.h> // Using: atoi
#include <string>   // Using: string

std::ostream& operator<<(std::ostream& os, ActivePlayer player)
{
  switch (player) {
  case ActivePlayer::Player1:
    return os << "Player 1";
  case ActivePlayer::Player2:
    return os << "Player 2";
  case ActivePlayer::Both:
    return os << "Both players";
  case ActivePlayer::None:
    return os << "No players";
  default:
    break;
  }
  Q_UNREACHABLE();
}

QDebug operator<<(QDebug d, ActivePlayer player)
{
  switch (player) {
  case ActivePlayer::Player1:
    return d << "Player 1";
  case ActivePlayer::Player2:
    return d << "Player 2";
  case ActivePlayer::Both:
    return d << "Both players";
  case ActivePlayer::None:
    return d << "No players";
  default:
    break;
  }
  Q_UNREACHABLE();
}

CombatCalculatorCallbacks::CombatCalculatorCallbacks(
  std::function<void(Player, bool)> onPlayerEntityDeath)
  : m_onPlayerEntityDeath{std::move(onPlayerEntityDeath)}
{
}

const std::function<void(Player, bool)>&
CombatCalculatorCallbacks::getOnPlayerEntityDeath() const
{
  return m_onPlayerEntityDeath;
}

CombatCalculatorState::CombatCalculatorState(
  int                distanceBetweenTheBattleParticipants,
  EntityOutputConfig entityOutputConfig)
  : distanceBetweenTheBattleParticipants{distanceBetweenTheBattleParticipants}
  , entityOutputConfig{entityOutputConfig}
{
  // Set all of the stored values to their initial values
  startingBattleParticipantQuantityP1 = 0;
  startingBattleParticipantQuantityP2 = 0;
  startingAssistantQuantityP1         = 0;
  startingAssistantQuantityP2         = 0;
  d6DieRoll                           = 0;
  aDeathHasOccured                    = false;
  isRetreating                        = "";
  p1RoundAttackModifiers              = 0;
  p2RoundAttackModifiers              = 0;
  p1RemainingDamage                   = 0;
  p2RemainingDamage                   = 0;
}

// Function: The constructor/deconstructor
combatCalculator::combatCalculator(
  CombatCalculatorState*     state,
  CombatCalculatorCallbacks* callbacks)
  : m_state{state}
  , m_callbacks{callbacks}
  , startingBattleParticipantQuantityP1{m_state
                                          ->startingBattleParticipantQuantityP1}
  , startingBattleParticipantQuantityP2{m_state
                                          ->startingBattleParticipantQuantityP2}
  , startingAssistantQuantityP1{m_state->startingAssistantQuantityP1}
  , startingAssistantQuantityP2{m_state->startingAssistantQuantityP2}
  , d6DieRoll{m_state->d6DieRoll}
  , aDeathHasOccured{m_state->aDeathHasOccured}
  , isRetreating{m_state->isRetreating}
  , player1Name{m_state->player1Name}
  , player2Name{m_state->player2Name}
  , p1BattleParticipant{m_state->p1BattleParticipant}
  , p2BattleParticipant{m_state->p2BattleParticipant}
  , p1BattleAssistant{m_state->p1BattleAssistant}
  , p2BattleAssistant{m_state->p2BattleAssistant}
  , p1RoundAttackModifiers{m_state->p1RoundAttackModifiers}
  , p2RoundAttackModifiers{m_state->p2RoundAttackModifiers}
  , p1RemainingDamage{m_state->p1RemainingDamage}
  , p2RemainingDamage{m_state->p2RemainingDamage}
{
}

combatCalculator::~combatCalculator()
{
}

void combatCalculator::setPlayerNames(
  std::string& inputtedP1Name,
  std::string& inputtedP2Name)
{
  // Set the player names
  player1Name = inputtedP1Name;
  player2Name = inputtedP2Name;
}

void combatCalculator::setCombatParticipants(
  Entity& inputtedP1BattleParticipant,
  Entity& inputtedP2BattleParticipant,
  Entity& inputtedP1BattleAssistant,
  Entity& inputtedP2BattleAssistant,
  int&    inputtedP1RoundAttackModifiers,
  int&    inputtedP2RoundAttackModifiers)
{
  // Set the battle participants for P1
  p1BattleParticipant    = inputtedP1BattleParticipant;
  p1BattleAssistant      = inputtedP1BattleAssistant;
  p1RoundAttackModifiers = inputtedP1RoundAttackModifiers;

  // Set the battle participants for P2
  p2BattleParticipant    = inputtedP2BattleParticipant;
  p2BattleAssistant      = inputtedP2BattleAssistant;
  p2RoundAttackModifiers = inputtedP2RoundAttackModifiers;
}

void combatCalculator::setStartingQuantites()
{
  //@Phillip: How come this is blank
  startingBattleParticipantQuantityP1
    = p1BattleParticipant.initialEntityQuantity;
  startingBattleParticipantQuantityP2
    = p2BattleParticipant.initialEntityQuantity;
  startingAssistantQuantityP1 = p1BattleAssistant.initialEntityQuantity;
  startingAssistantQuantityP2 = p2BattleAssistant.initialEntityQuantity;
}

void combatCalculator::setAdditionalValues(
  float& inputtedRemainingDamageP1,
  float& inputtedRemainingDamageP2)
{
  // Set the remaining damage for both players
  p1RemainingDamage = inputtedRemainingDamageP1;
  p2RemainingDamage = inputtedRemainingDamageP2;
}

float combatCalculator::returnRemaningDamage(int inputPlayerNumber)
{
  // Return the remaining damage based on the player number
  switch (inputPlayerNumber) {
  case 1:
    return p1RemainingDamage;
  case 2:
    return p2RemainingDamage;
  default:
    std::cout << "Error: Input player number not recognized"
              << "<br>";
    std::terminate();
  }
}

int combatCalculator::generateD6DieInput()
{
  int                d6Value = 0; // Store the generated integer value
  std::random_device rd;          // Set a random seed
  std::mt19937       gen(
    rd()); // Initialize the Mersenne Twister pseudo-random number generator
  std::uniform_int_distribution<> dis(
    1,
    6); // Make the pseudo-random numbers uniformly distributed in range (1, 6)
  d6Value = dis(gen); // Generate a random number
  return d6Value;     // Return the generated number
}

void combatCalculator::checkD6DiceSimulator()
{
  // Check if the generateD6DieInput() function is generating random numbers
  for (int i = 0; i < 50; i++) {
    d6DieRoll = generateD6DieInput();
    std::cout << "die roll: " << i << " has a d6 value of: " << d6DieRoll
              << "<br>";
  }
}

Entity combatCalculator::returnModifiedBattleParticipants(int inputPlayerNumber)
{
  // Return the modified battle battle participants based on the play number
  // after the combat results have been applied
  switch (inputPlayerNumber) {
  case 1:
    return p1BattleParticipant;
  case 2:
    return p2BattleParticipant;
  default:
    std::cout << "Error: Input player number not recognized"
              << "<br>";
    std::terminate();
  }
}

std::string combatCalculator::returnWhatIAm(
  Entity&     inputtedEntity,
  std::string inputtedRound)
{
  if (inputtedRound == "Bombardment round") {
    if (
      (inputtedEntity.standardDamage > 0)
      && (inputtedEntity.maximumRange >= 1)) {
      if ((inputtedEntity.armorClass[1] == true)) { // building
        return "Ranged building fighting with melee damage";
      }
      else if (
        (inputtedEntity.armorClass[25] == true)
        || (inputtedEntity.armorClass[11] == true)) { // land unit or ship
        return "Ranged unit fighting with melee damage";
      }
    }
    else {
      if (inputtedEntity.armorClass[1] == true) { // Building
        return "Non-fighting building";
      }
      else if (
        (inputtedEntity.armorClass[25] == true)
        || (inputtedEntity.armorClass[11] == true)) {
        return "Non-fighting unit";
      }
    }
  }
  else if (inputtedRound == "Ranged round") {
    if (
      (inputtedEntity.rangedDamage > 0)
      && (inputtedEntity.armorClass[0] == true)) { // Archer armor class
      return "Ranged unit fighting with ranged damage";
    }
    else {
      if (inputtedEntity.armorClass[1] == true) { // Building
        return "Non-fighting building";
      }
      else if (
        (inputtedEntity.armorClass[25] == true)
        || (inputtedEntity.armorClass[11] == true)) {
        return "Non-fighting unit";
      }
    }
  }
  else if (inputtedRound == "Standard round") {
    if (
      (inputtedEntity.standardDamage > 0)
      && (inputtedEntity.armorClass[25] == true)) { // Land unit
      return "Melee unit fighting with melee damage";
    }
    else {
      if (inputtedEntity.armorClass[1] == true) { // Building
        return "Non-fighting building";
      }
      else if (
        (inputtedEntity.armorClass[25] == true)
        || (inputtedEntity.armorClass[11] == true)) {
        return "Non-fighting unit";
      }
    }
  }

  std::cout
    << "Error: inputtedRound not recognized or inputtedEntity type not found";
  std::terminate();
}

void combatCalculator::checkIfItCanBeHealed()
{
  // If p1BattleParticipant has less units than it started with and there are
  // heals available to it, heal it
  if (
    p1BattleParticipant.entityQuantity < startingBattleParticipantQuantityP1) {
    while (p1BattleParticipant.healsAvailable != 0) {
      p1BattleParticipant.entityQuantity++;
      p1BattleParticipant.healsAvailable--;
      std::cout << player1Name << " has used one of their heals on their "
                << p1BattleParticipant.entityName << "<br>";
    }
  }

  if (
    p2BattleParticipant.entityQuantity < startingBattleParticipantQuantityP2) {
    while (p2BattleParticipant.healsAvailable != 0) {
      p2BattleParticipant.entityQuantity++;
      p2BattleParticipant.healsAvailable--;
      std::cout << player2Name << " has used one of their heals on their "
                << p2BattleParticipant.entityName << "<br>";
    }
  }

  // If p1BattleAssistant has less units than it started with and there
  // are heals available to it, heal it
  if (p1BattleAssistant.entityQuantity < startingAssistantQuantityP1) {
    while (p1BattleAssistant.healsAvailable != 0) {
      p1BattleAssistant.entityQuantity++;
      p1BattleAssistant.healsAvailable--;
      std::cout << player1Name << " has used one of their heals on their "
                << p1BattleAssistant.entityName << "<br>";
    }
  }

  // If p2BattleAssistant has less units than it started with and there
  // are heals available to it, heal it
  if (p2BattleAssistant.entityQuantity < startingAssistantQuantityP2) {
    while (p2BattleAssistant.healsAvailable != 0) {
      p2BattleAssistant.entityQuantity++;
      p2BattleAssistant.healsAvailable--;
      std::cout << player2Name << " has used one of their heals on their "
                << p2BattleAssistant.entityName << "<br>";
    }
  }
}

// Function: Check if any of the entities have died before proceeding to the
// next round of combat
void combatCalculator::resurrectForEachPlayer(
  std::string& givenPlayerName,
  Entity&      givenPlayerBattleParticipant,
  Entity&      givenPlayerBattleAssistant)
{
  // Check if the rule about monks resurrecting units applies
  // "If a military unit loses its last token but still has monks attached,
  // these monks keep the unit alive and in play"
  // So this won't apply to villagers and monks I imagine

  if (
    (givenPlayerBattleParticipant.entityQuantity <= 0)
    && (givenPlayerBattleParticipant.armorClass[1] == false)
    && (givenPlayerBattleParticipant.entityName != "Villager")
    && (givenPlayerBattleParticipant.entityName != "Monk")) {
    if (givenPlayerBattleAssistant.entityQuantity > 0) {
      std::cout
        << givenPlayerName
        << " has a monk that has prevented all of their units from dying<br>";
      givenPlayerBattleParticipant.entityQuantity++;
    }
  }
}

void combatCalculator::deathDetectionForEachPlayer(
  // Given player information
  Player  currentPlayer,
  Entity& givenPlayerBattleParticipant)
{
  if (givenPlayerBattleParticipant.entityQuantity <= 0) {
    aDeathHasOccured = true;

    if (givenPlayerBattleParticipant.entityName == "Monk") {
      m_callbacks->getOnPlayerEntityDeath()(currentPlayer, true);
    }
    else {
      m_callbacks->getOnPlayerEntityDeath()(currentPlayer, false);
    }

    if (givenPlayerBattleParticipant.entityName == "Wonder") {
      SFXToPlay("/sfx/significant_events/wonder_destroyed_sfx.wav");
    }
  }
}

void combatCalculator::checkIfDead()
{
  // Check if what would otherwise die, can be healed
  checkIfItCanBeHealed();

  resurrectForEachPlayer(player1Name, p1BattleParticipant, p1BattleAssistant);
  resurrectForEachPlayer(player2Name, p2BattleParticipant, p2BattleAssistant);

  deathDetectionForEachPlayer(Player::Player1, p1BattleParticipant);
  deathDetectionForEachPlayer(Player::Player2, p2BattleParticipant);
}

// Function: Check if the attacking ranged archer is retreating
void combatCalculator::checkIfRetreating(std::string messageToAsk)
{
  // Behaviour: Ask the attacker if they want to retreat with their archer if
  // they are not versing cavalry or an archer

  // Todo: Hit and run not working how it should

  // Behavior: Do not ask the question if 2 buildings are fighting one another
  if (
    (p1BattleParticipant.armorClass[1] != true)
    && (p2BattleParticipant.armorClass[1] != true)) {
    // Behavior: Do not ask the question if opposing unit is dead
    if (
      (p1BattleParticipant.entityQuantity > 0)
      && (p2BattleParticipant.entityQuantity > 0)) {
      std::cout << messageToAsk;

      const bool bIsRetreating{DIN.queryForIsRetreating()};
      isRetreating = bIsRetreating ? "1" : "0";

      if ((isRetreating != "1") && (isRetreating != "0")) {
        std::cout << "Error: The retreating value can only be a 0 or 1"
                  << "<br>";
        std::terminate();
      }
    }
    else {
      // Set value to 0 if opposing unit is dead
      isRetreating = "0";
    }
  }
  else {
    // Set value to 0 if 2 buildings are fighting one another
    isRetreating = "0";
  }
}

// Function: Output the entity information with a message
void combatCalculator::outputEntityInformation(std::string inputMessage)
{
  // Behaviour: Only display the message if there is a message to be displayed
  std::cout << "<br>";

  if (inputMessage != "") {
    std::cout << inputMessage << "<br>";
  }

  p1BattleParticipant.outputEntity(player1Name, m_state->entityOutputConfig);
  // Do not show monks as being dead if started with 0
  if (
    (p1BattleAssistant.entityQuantity >= 0)
    && (p1BattleAssistant.initialEntityQuantity != 0)) {
    std::cout << "(Assisting) ";
    p1BattleAssistant.outputEntity(player1Name, m_state->entityOutputConfig);
  }
  p2BattleParticipant.outputEntity(player2Name, m_state->entityOutputConfig);

  // Do not show monks as being dead if started with 0
  if (
    (p2BattleAssistant.entityQuantity >= 0)
    && (p2BattleAssistant.initialEntityQuantity != 0))

  {
    std::cout << "(Assisting) ";
    p2BattleAssistant.outputEntity(player2Name, m_state->entityOutputConfig);
  }
}

// Function: Calculate the non-rounded value (the decimal value)
float combatCalculator::calculateRemainingDamage(
  int roundedNumber,
  int inputAttackerDamage,
  int inputAttackerModifier,
  int inputDefenderHealth,
  int inputSetting)
{
  // Float: Store the remaining damage value
  float getTheDecimalValue = 0;

  // Behaviour: Calculate the attack
  getTheDecimalValue = ((inputAttackerDamage) + inputAttackerModifier);

  // Behaviour: Get just the remaining damage value for units
  if (getTheDecimalValue != 0) {
    if (inputSetting == 0) {
      getTheDecimalValue /= inputDefenderHealth;
    }
    else if (inputSetting == 1) {
      getTheDecimalValue -= inputDefenderHealth;
    }
  }

  // Behaviour: Get the difference between the current number and the rounded
  // number
  getTheDecimalValue = (getTheDecimalValue - roundedNumber);

  return getTheDecimalValue;
}

void combatCalculator::checkRemainingDamage( // @Kory todo: check if this is
                                             // working as intended
  int* p1Events,
  int* p2Events)
{
  // Reference: For whatever reason, decreasing the quantity of:
  // [p1/p2]BattleParticipant.entityQuantity by 1 triggers a floating point
  // error in the second round of combat

  // Event [26] Relentless Attack - this battle, any damage remaining in a round
  // after casualties have been taken carries over into the next round.
  // Remaining damage at the end of this battle is lost
  if (p1Events[26] == 1) {
    if (p2BattleParticipant.entityQuantity >= 1) {
      if (p1RemainingDamage >= 1) {
        p2BattleParticipant.entityQuantity -= 1;
        p1RemainingDamage -= 1;
      }
    }
  }

  if (p2Events[26] == 1) {
    if (p1BattleParticipant.entityQuantity >= 1) {
      if (p2RemainingDamage >= 1) {
        p1BattleParticipant.entityQuantity -= 1;
        p2RemainingDamage -= 1;
      }
    }
  }
}

// Function: Show the outcome of a monk battle
void monkRounds::outputtingMonkRoundOutcomeForAnIndividualPlayer(
  ActivePlayer activePlayer,
  std::string& givenPlayerName,
  int&         givenPlayerPointsAwarded,
  bool&        givenPlayerMonkPowersActivated,
  Entity&      givenPlayerBattleParticipant,
  Entity&      givenPlayerBattleAssistant)
{
  // Shows whether or not the given player's monk powers activated if
  // appropriate
  if (activePlayer != ActivePlayer::None) {
    if (givenPlayerMonkPowersActivated == false) {
      std::cout << givenPlayerName << "'s monk powers failed to activate"
                << "<br>";
    }
    else if (givenPlayerMonkPowersActivated == true) {
      std::cout << givenPlayerName << "'s monk powers activated"
                << "<br>";
    }
  }

  // Show many heals the given player has earned on their battle participants if
  // appropriate
  if (givenPlayerBattleParticipant.healsAvailable > 0) {
    std::cout << givenPlayerName << " has ";
    (givenPlayerBattleParticipant.healsAvailable == 1)
      ? std::cout << " a heal"
      : std::cout << givenPlayerBattleParticipant.healsAvailable << " heals";
    std::cout << " available on their "
              << givenPlayerBattleParticipant.entityName << "<br>";
  }

  // Show many heals the given player has earned on their battle assistants if
  // appropriate
  if (givenPlayerBattleAssistant.healsAvailable > 0) {
    std::cout << givenPlayerName << " has ";
    (givenPlayerBattleAssistant.healsAvailable == 1)
      ? std::cout << " a heal"
      : std::cout << givenPlayerBattleAssistant.healsAvailable << " heals";
    std::cout << " available on their " << givenPlayerBattleAssistant.entityName
              << "<br>";
  }

  // Show how many points the given player earned if appropriate
  if (givenPlayerPointsAwarded != 0) {
    std::cout << ">> " << givenPlayerName << " gets "
              << givenPlayerPointsAwarded << " points"
              << "<br>";
  }
}

void monkRounds::calculatingMonkRoundOutcomeForAnIndividualPlayer(
  // Both player names
  std::string& givenPlayerName,
  std::string& opposingPlayerName,
  // Both player battle participants
  Entity& givenPlayerBattleParticipant,
  Entity& givenPlayerBattleAssistant,
  Entity& opposingPlayerBattleParticipant,
  Entity& opposingPlayerBattleAssistant,
  // Both player events and technologies
  int* givenPlayerEvents,
  int* opposingPlayerEvents,
  int* givenPlayerTechnologies,
  // Given player monk related information
  bool&        givenPlayerHasAMonkPresent,
  bool&        givenPlayerMonkPowersActivated,
  std::string& givenPlayerIsHealingOrConverting,
  int&         givenPlayerConversionANDHealingRate,
  bool&        givenPlayerAssistingMonks,
  bool&        givenPlayerStandaloneMonks,
  int&         givenPlayerPointsGained,
  int&         givenPlayerEntitiesHealed)
{
  /* Check if the event "It's A Miracle" is active for the given player */
  if (givenPlayerEvents[18] == 1) {
    // [18] It's A Miracle - A successful Healing attempt saves us up three
    // tokens, instead of one. Play this card before an attempt is made
    givenPlayerEntitiesHealed = 3;
  }

  /* Check if the given player has monks as a battle assistant */
  if (
    (givenPlayerBattleAssistant.armorClass[9] == true)
    && (givenPlayerBattleAssistant.entityQuantity > 0)) {
    givenPlayerAssistingMonks = true;
    givenPlayerConversionANDHealingRate
      = givenPlayerBattleAssistant.entityQuantity;
  }
  else {
    givenPlayerAssistingMonks = false;
  }

  /* Check if the given player has monks as a battle participant */
  if (
    (givenPlayerBattleParticipant.armorClass[9] == true)
    && (givenPlayerBattleParticipant.entityQuantity > 0)) {
    givenPlayerStandaloneMonks = true;
    givenPlayerConversionANDHealingRate
      = givenPlayerBattleParticipant.entityQuantity;
  }
  else {
    givenPlayerStandaloneMonks = false;
  }

  /* Indicate that the odds of a monk healing or converting are 0 if neither
   * player has monks */
  if (
    givenPlayerAssistingMonks == false && givenPlayerStandaloneMonks == false) {
    givenPlayerConversionANDHealingRate = 0;
  }

  /* Throw an error if the given player has monks attached to monks */
  if (givenPlayerAssistingMonks == true && givenPlayerStandaloneMonks == true) {
    std::cout << "Error: Cannot have monks assisting monks";
    std::terminate();
  }

  /* Detect if the given player does in fact have monks, either as a battle
   * participant or battle assistant */
  givenPlayerHasAMonkPresent = ((givenPlayerAssistingMonks == true)
                                || (givenPlayerStandaloneMonks == true))
                                 ? true
                                 : false;

  /* Check if the event "Fat Friars Tavern O'Spirits" is active for the opposing
   * player */
  if (opposingPlayerEvents[7] == 1) {
    // [7] Fat Friars Tavern O'Spirits - Monks on target unit may not have any
    // Conversion or Healing attempts this turn
    givenPlayerHasAMonkPresent = false;
  }

  if (givenPlayerHasAMonkPresent == true) { // Make sure the given player's monk
                                            // is present before proceeding

    const DialogInput::MonkAction monkAction{DIN.queryForMonkAction(
      givenPlayerName)}; // Get the given player's decision on whether they
                         // would like to make a conversion or healing attempt
    switch (monkAction) {
    case DialogInput::MonkAction::Convert:
      givenPlayerIsHealingOrConverting = "Converting";
      break;
    case DialogInput::MonkAction::Heal:
      givenPlayerIsHealingOrConverting = "Healing";
      break;
    default:
      std::cout << "Error: Unexpected input for " << givenPlayerName
                << "'s "
                   "conversion/healing attempt"
                << "<br>";

      std::terminate();
    }

    /* Check if the event "Back From A Foreign Land" (Byzantine) is active for
     * the given player */
    if (givenPlayerEvents[1] == 1) {
      if (givenPlayerIsHealingOrConverting == "Healing") {
        givenPlayerConversionANDHealingRate += 2;
      }
    }

    /* Check if the event "Back From A Foreign Land" (Teuton) is active for the
     * opposing player */
    if (opposingPlayerEvents[40] == 1) {
      if (givenPlayerIsHealingOrConverting == "Converting") {
        givenPlayerConversionANDHealingRate -= 1;
      }
    }

    d6DieRoll = generateD6DieInput(); // Generate, validate, and return d6 dice
                                      // input before proceeding

    /* Check if the event "Zealous Monks" is active for the given player */
    if (
      (givenPlayerEvents[39] == 1)
      && (givenPlayerIsHealingOrConverting == "Converting")) {
      // [39] Zealous Monks - Target Monk unit gets 1 conversion roll at a 3 or
      // less rate for each monk attached. In other words, you reroll the dice
      // if the number is <= 3 You repeat the roll for each monk attached

      int maximumRerolls
        = (givenPlayerAssistingMonks == true)
            ? givenPlayerBattleAssistant.entityQuantity
            : givenPlayerBattleParticipant
                .entityQuantity; // Check if there are just assisting monks or
                                 // if there are just standalone monks and set
                                 // the quantity of what monk type there is to
                                 // be the maximum number of rolls the player
                                 // gets
      for (int dieReroll = 0; dieReroll != maximumRerolls; dieReroll++) {
        if (d6DieRoll <= 3) {
          d6DieRoll = generateD6DieInput();
        }
      }
    }

    /* Check if the event "Piety" is active for the given player */
    if (
      (givenPlayerEvents[23] == 1)
      && (givenPlayerIsHealingOrConverting == "Converting")) {
      // [23] Piety - If you have one monk attached to a unit, your conversion
      // rate is 4. If the attempt fails, the monk dies
      const int pietyConversionRate = 4;

      if (pietyConversionRate <= d6DieRoll) { // Check if the conversion attempt
                                              // failed and if so, reduce the
                                              // quantity of monks by 1
        (givenPlayerAssistingMonks == true)
          ? givenPlayerBattleAssistant.entityQuantity -= 1
          : givenPlayerBattleParticipant.entityQuantity -= 1;
      }
      else { // The conversion attempt succeeded, will have this affect the
             // calculations later on
        givenPlayerConversionANDHealingRate = pietyConversionRate;
      }
    }

    /* Check if the event "Listen To A Story" is active for the given player */
    if (
      (givenPlayerEvents[19] == 1)
      && (givenPlayerIsHealingOrConverting == "Converting")) {
      // [19] Listen To A Story - Attempt 1-3 extra conversions this battle. Pay
      // 2 gold per conversion
      const int numberOfTimesTheDieHasBeenRolledSoFarForTheGivenPlayer
        = 1; // So we've had one roll from earlier in our calculations so we're
             // starting from 1
      const int numberOfExtraConversionAttemptsForTheGivenPlayer = 3;

      int numberOfTimesTheGivenPlayerHasPaidToAttemptAnotherConversion = 0;
      std::string
        getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByTheGivenPlayer
        = "";
      int numberOfIterationsThroughTheWhileLoop = 0;

      while (
        numberOfIterationsThroughTheWhileLoop
        != (numberOfTimesTheDieHasBeenRolledSoFarForTheGivenPlayer + numberOfExtraConversionAttemptsForTheGivenPlayer)) {
        // Check if the conversion attempt is going to fail
        if (givenPlayerConversionANDHealingRate >= d6DieRoll) {
          std::cout << "The conversion attempt for " << givenPlayerName
                    << " failed. Pay 2 gold "
                       "and enter 1 to try again. Otherwise enter 0"
                    << "<br>";
          DIN
            >> getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByTheGivenPlayer;
        }

        if (
          getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByTheGivenPlayer
          == "1") {
          d6DieRoll = generateD6DieInput();
          numberOfTimesTheGivenPlayerHasPaidToAttemptAnotherConversion++;
        }
        else if (
          getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByTheGivenPlayer
          == "0") {
          numberOfIterationsThroughTheWhileLoop
            = numberOfExtraConversionAttemptsForTheGivenPlayer; // Jump to the
                                                                // end of the
                                                                // while loop
        }

        numberOfIterationsThroughTheWhileLoop++;
      }

      // Display how much gold the given player needs to pay for the "Listen To
      // A Story" event card
      if (numberOfTimesTheGivenPlayerHasPaidToAttemptAnotherConversion > 0) {
        int listenToAStoryGoldCostForTheGivenPlayer
          = 2 * numberOfTimesTheGivenPlayerHasPaidToAttemptAnotherConversion;
        std::cout << givenPlayerName << " must pay "
                  << std::to_string(listenToAStoryGoldCostForTheGivenPlayer)
                  << " gold"
                  << "<br>";
      }
    }

    // Determine if the monk powers have activated for the given player
    givenPlayerMonkPowersActivated
      = (givenPlayerConversionANDHealingRate >= d6DieRoll) ? true : false;

    // Act on the success
    if (givenPlayerMonkPowersActivated == true) {
      if (givenPlayerIsHealingOrConverting == "Converting") {
        // Act on the conversion attempt being successful

        // Work out whether the given player is targeting a assistant or
        // non-assistant
        std::string getGivenPlayerMonkConversionTarget = "";

        auto doesGivenPlayerHaveRedemption = [givenPlayerTechnologies] {
          return givenPlayerTechnologies[18] == 1;
        };

        if (
          givenPlayerBattleAssistant.entityQuantity > 0
          && opposingPlayerBattleAssistant.entityQuantity > 0
          && doesGivenPlayerHaveRedemption()) {
          std::cout << "Is " << givenPlayerName
                    << "'s 'assistant monk' converting " << opposingPlayerName
                    << "'s assisting monk? Enter 1 for yes. Enter 0 for no"
                    << "<br>";
          DIN >> getGivenPlayerMonkConversionTarget;

          // Behaviour: Validate the input before proceeding
          if (
            (getGivenPlayerMonkConversionTarget != "0")
            && (getGivenPlayerMonkConversionTarget != "1")) {
            std::cout << "Error: The input must be 0 or 1"
                      << "<br>";
            std::terminate();
          }
        }

        if (
          (getGivenPlayerMonkConversionTarget == "0")
          || (doesGivenPlayerHaveRedemption() == false)) {
          // Behaviour: Make sure that the conversion attempt is not being
          // applied to siege units
          if (opposingPlayerBattleParticipant.armorClass[12] == true) {
            std::cout << "Error: " << givenPlayerName
                      << "'s conversion attempt cannot be "
                      << "applied to " << opposingPlayerName << "'s siege unit"
                      << "<br>";
            std::terminate();
          }

          // Convert the combat participant
          givenPlayerPointsGained = opposingPlayerBattleParticipant.pointValue;
          if (opposingPlayerBattleParticipant.entityQuantity > 0) {
            opposingPlayerBattleParticipant.entityQuantity -= 1;
            checkIfItCanBeHealed(); // Check for healing every time quantity
                                    // goes down
          }
        }
        else if (getGivenPlayerMonkConversionTarget == "1") {
          givenPlayerPointsGained = opposingPlayerBattleAssistant.pointValue;
          if (opposingPlayerBattleAssistant.entityQuantity > 0) {
            opposingPlayerBattleAssistant.entityQuantity -= 1;
            checkIfItCanBeHealed(); // Check for healing every time quantity
                                    // goes down
          }
        }

        // Play a SFX for a successful conversion attempt
        SFXToPlay("/sfx/rng/successful_monk_conversion_attempt_sfx.wav");
      }
      else if (givenPlayerIsHealingOrConverting == "Healing") {
        // Work out whether the given player is targeting a assistant or
        // non-assistant
        std::string getGivenPlayerMonkHealingTarget = "";

        if (givenPlayerBattleAssistant.entityQuantity > 0) {
          std::cout <<

            "Enter 1 if " << givenPlayerName << "'s monk is healing "
                    << givenPlayerName << "'s "
                    << givenPlayerBattleParticipant.entityName
                    << ". Enter 0 if " << givenPlayerName
                    << "'s monk is healing " << givenPlayerName << "'s "
                    << givenPlayerBattleAssistant.entityName << "<br>";
          DIN >> getGivenPlayerMonkHealingTarget;

          // Behaviour: Validate the input before proceeding
          if (
            (getGivenPlayerMonkHealingTarget != "0")
            && (getGivenPlayerMonkHealingTarget != "1")) {
            std::cout << "Error: The input must be 0 or 1"
                      << "<br>";
            std::terminate();
          }
        }
        else {
          getGivenPlayerMonkHealingTarget = "1";
        }

        // Apply the heal to either the assistant or battle participant
        if (getGivenPlayerMonkHealingTarget == "0") {
          for (int addHeals = 0; addHeals < givenPlayerEntitiesHealed;
               addHeals++) {
            givenPlayerBattleAssistant.healsAvailable++;
            givenPlayerEntitiesHealed -= 1;
          }
        }
        else if (getGivenPlayerMonkHealingTarget == "1") {
          for (int addHeals = 0; addHeals < givenPlayerEntitiesHealed;
               addHeals++) {
            givenPlayerBattleParticipant.healsAvailable++;
            givenPlayerEntitiesHealed -= 1;
          }
        }

        // Play a SFX for a successful healing attempt
        SFXToPlay("/sfx/rng/successful_monk_healing_attempt_sfx.wav");
      }
    }
  }
}

void monkRounds::roundOutcome(
  int          roundRunTimes,
  int*         p1Events,
  int*         p2Events,
  int*         p1Technologies,
  int*         p2Technologies,
  ActivePlayer activePlayer)
{
  bool monkPresentP1         = false,
       monkPresentP2         = false; // Store if there are monks from P1 and P2
  bool monkPowersActivatedP1 = false,
       monkPowersActivatedP2
       = false; // Store if the monks from P1 and P2 did something
  int p1PointsGained = 0,
      p2PointsGained
      = 0; // Store the amount of points awarded to P1 and P2 for kills
  int p1EntitiesHealed
    = 1,
    p2EntitiesHealed
    = 1; // Store the amount of entities a heal from a monk, heals for P1 and P2
  std::string player1IsHealingOrConverting
    = "",
    player2IsHealingOrConverting
    = ""; // Store if P1 and P2 is performing a conversion or healing attempt
  int conversionRateANDhealingRateP1 = 0,
      conversionRateANDhealingRateP2
      = 0; //  Stores the likelihood of an attempt to convert or heal a monk
           //  succeeding for P1 and P2
  bool assistingMonksP1 = false,
       assistingMonksP2
       = false; // Store whether monks are attached to a unit for P1 and P2
  bool standaloneMonksP1 = false,
       standaloneMonksP2
       = false; // Store whether monks are on their own for P1 and P2

  /* Run the monk battle round for X times */
  for (int numberOfTimesToRunTheMonkBattleRound = 0;
       numberOfTimesToRunTheMonkBattleRound < roundRunTimes;
       numberOfTimesToRunTheMonkBattleRound++) {
    checkIfDead(); // Check that no deaths have occured

    if (aDeathHasOccured == false) { // Make sure that no deaths have occured
                                     // before proceeding since something that's
                                     // dead cannot fight even though it can
                                     // attack as it dies

      // Determine if we'll be running the calculations for P1
      if (
        (activePlayer & ActivePlayer::Player1)
        != ActivePlayer::None) { // If "Player 1" != "No players"

        // Perform the calculations for P1
        calculatingMonkRoundOutcomeForAnIndividualPlayer(
          player1Name,
          player2Name,
          p1BattleParticipant,
          p1BattleAssistant,
          p2BattleParticipant,
          p2BattleAssistant,
          p1Events,
          p2Events,
          p1Technologies,
          monkPresentP1,
          monkPowersActivatedP1,
          player1IsHealingOrConverting,
          conversionRateANDhealingRateP1,
          assistingMonksP1,
          standaloneMonksP1,
          p1PointsGained,
          p1EntitiesHealed);
      }

      // Determine if we'll be running the calculations for P2
      if ((activePlayer & ActivePlayer::Player2) != ActivePlayer::None) {
        // Perform the calculations for P2
        calculatingMonkRoundOutcomeForAnIndividualPlayer(
          player2Name,
          player1Name,
          p2BattleParticipant,
          p2BattleAssistant,
          p1BattleParticipant,
          p1BattleAssistant,
          p2Events,
          p1Events,
          p2Technologies,
          monkPresentP2,
          monkPowersActivatedP2,
          player2IsHealingOrConverting,
          conversionRateANDhealingRateP2,
          assistingMonksP2,
          standaloneMonksP2,
          p2PointsGained,
          p2EntitiesHealed);
      }

      // Behaviour: Display the outcome of the monk combat round only if monks
      // from P1 or P2 did something
      if ((monkPowersActivatedP1 == true) || (monkPowersActivatedP2 == true)) {
        std::string outputString
          = "Monk powers activatation round "
            + std::to_string(numberOfTimesToRunTheMonkBattleRound + 1)
            + " calculations...";

        // Show information about the battle participants
        outputEntityInformation(outputString);

        // Show information related to the monk round calculations
        outputtingMonkRoundOutcomeForAnIndividualPlayer(
          activePlayer,
          player1Name,
          p1PointsGained,
          monkPowersActivatedP1,
          p1BattleParticipant,
          p1BattleAssistant);
        outputtingMonkRoundOutcomeForAnIndividualPlayer(
          activePlayer,
          player2Name,
          p2PointsGained,
          monkPowersActivatedP2,
          p2BattleParticipant,
          p2BattleAssistant);
      }

    } // End if conditional checking for no deaths

  } // End for loop

} // End monk combat rounds function

// Function: Show the outcome of a archer battle
void archerRounds::outputtingArcherRoundOutcomeForAnIndividualPlayer(
  std::string& givenPlayerName,
  float&       givenPlayerPointsGained,
  bool&        isGivenPlayerFightingBuilding,
  Entity&      opposingPlayerBattleParticipant,
  int&         opposingPlayerDamageDie,
  std::string& opposingPlayerName)
{
  // Behaviour: Display how many damage die to place if appropriate
  if (isGivenPlayerFightingBuilding == true) {
    if (
      (opposingPlayerBattleParticipant.entityHealth > 0)
      && (opposingPlayerDamageDie > 0)) {
      std::cout << ">> Place " << opposingPlayerDamageDie << " damage die onto "
                << opposingPlayerName << "'s "
                << opposingPlayerBattleParticipant.entityName << "<br>";
    }
  }

  // Behaviour: Display how many points were added if appropriate
  if (givenPlayerPointsGained != 0) {
    std::cout << ">> " << givenPlayerName << " gets " << givenPlayerPointsGained
              << " points"
              << "<br>";
  }
}

// Function: Calculate the outcome of a archer battle
void archerRounds::calculatingArcherRoundOutcomeForAnIndividualPlayer(
  // Shared stuff
  const int roundDownBasedOnMultiplesOfThisNumber,
  // Given player stuff
  Entity& givenPlayerBattleParticipant,
  int*    givenPlayerEvents,
  int     givenPlayerRoundAttackModifiers,
  float&  givenPlayerRemainingDamage,
  bool&   givenPlayerHasAArcherActivated,
  bool&   givenPlayerIsFightingBuilding,
  bool&   givenPlayerIsFightingUnit,
  bool&   givenPlayerRangedUnitCanAttackOpposingPlayerBuilding,
  bool&   givenPlayerRangedUnitCanAttackOpposingPlayerCavalry,
  int&    givenPlayerRangedDamageDealt,
  // Opposing player stuff
  Entity& opposingPlayerBattleParticipant,
  int*    opposingPlayerEvents,
  bool&   opposingPlayerTakesNoDamageDueToShotsInTheBack,
  int&    opposingPlayerEntityDeaths,
  int&    opposingPlayerBuildingDamage,
  int&    opposingPlayerDamageDie)
{
  if (
    (opposingPlayerBattleParticipant.armorClass[1] == true)
    && (givenPlayerBattleParticipant.armorClass[0] == true)) { //  Get whether
                                                               //  the archer
    //  entity is fighting a
    //  building
    givenPlayerIsFightingBuilding = true;
  }
  else {
    givenPlayerIsFightingBuilding = false;
  }

  if (
    (givenPlayerIsFightingBuilding == false)
    && (givenPlayerBattleParticipant.armorClass[0] == true)) {
    givenPlayerIsFightingUnit = true;
  }
  else {
    givenPlayerIsFightingUnit = false;
  }

 // Event that allows archers to attack buildigns
  if (givenPlayerEvents[41] == 1) {
    givenPlayerRangedUnitCanAttackOpposingPlayerBuilding = true;
  }
  else {
    givenPlayerRangedUnitCanAttackOpposingPlayerBuilding = false;
  }

  // Apply the effects of event 33
  if (givenPlayerEvents[33] == 1) {
    // [33] Steady Hand - Target Archer unit may do ranged damage to a Cavalry
    // unit this turn
    givenPlayerRangedUnitCanAttackOpposingPlayerCavalry = true;
  }

  // Perform archer vs building combat if an archer is fighting a building
  if (givenPlayerIsFightingBuilding == true) {
    // Perform calculations if the archer is able to fight the building
    if (givenPlayerRangedUnitCanAttackOpposingPlayerBuilding == true) {
      opposingPlayerBuildingDamage
        = (givenPlayerBattleParticipant.rangedDamage * givenPlayerBattleParticipant.entityQuantity);
      opposingPlayerBuildingDamage += givenPlayerRoundAttackModifiers;
      opposingPlayerBuildingDamage
        -= opposingPlayerBuildingDamage % roundDownBasedOnMultiplesOfThisNumber;

      opposingPlayerDamageDie = opposingPlayerBuildingDamage / 10;
      givenPlayerRemainingDamage += calculateRemainingDamage(
        opposingPlayerDamageDie,
        givenPlayerBattleParticipant.rangedDamage,
        givenPlayerRoundAttackModifiers,
        opposingPlayerBattleParticipant.entityHealth,
        1);

      givenPlayerHasAArcherActivated = true;
    }
  }
  // Perform archer vs unit combat if an archer is fighting a unit
  else if (givenPlayerIsFightingUnit == true) {
    // Do not perform archer vs cav combat unless an event card overrides
    // this rule
    if (
      (givenPlayerBattleParticipant.armorClass[0] == true)
      && // Archer armor class
      (opposingPlayerBattleParticipant.armorClass[4] == true)
      && // Cavalry armor class
      (givenPlayerRangedUnitCanAttackOpposingPlayerCavalry == false)) {
      // Do nothing
    }
    else {
      givenPlayerRangedDamageDealt
        = givenPlayerBattleParticipant.rangedDamage
          * givenPlayerBattleParticipant.entityQuantity;
      givenPlayerRangedDamageDealt += givenPlayerRoundAttackModifiers;
      givenPlayerRangedDamageDealt
        /= opposingPlayerBattleParticipant.entityHealth;
      opposingPlayerEntityDeaths = std::floor(givenPlayerRangedDamageDealt);

      givenPlayerRemainingDamage += calculateRemainingDamage(
        opposingPlayerEntityDeaths,
        givenPlayerBattleParticipant.rangedDamage,
        givenPlayerRoundAttackModifiers,
        opposingPlayerBattleParticipant.entityHealth,
        0);

      givenPlayerHasAArcherActivated = true;
    }
  }

  // Behavior: Make ranged combat attacks for P1 ineffective if this event
  // or shots in the back event is active for P2
  if (
    (opposingPlayerEvents[15] == 1)
    || (opposingPlayerTakesNoDamageDueToShotsInTheBack == true)) {
    // [15] Heavy Tree Cover - Negate range combat round in target battle
    // involving one of your units

    // Deal no unit damage
    givenPlayerRangedDamageDealt = 0;
    opposingPlayerEntityDeaths   = 0;

    // Deal no building damage
    opposingPlayerBuildingDamage = 0;
    opposingPlayerDamageDie      = 0;
  }
}

// Function: Calculate the outcome of a archer battle
void archerRounds::applyingArcherRoundOutcomeForAnIndividualPlayer(
  // Given player
  bool&  givenPlayerHasAArcherActivated,
  bool&  givenPlayerIsFightingBuilding,
  bool&  givenPlayerCanAttackOpposingPlayerBuilding,
  bool&  givenPlayerIsFightingUnit,
  float& givenPlayerPointsGained,
  // Opposing player
  Entity& opposingPlayerBattleParticipant,
  int&    opposingPlayerEntityDeaths,
  int&    opposingPlayerBuildingDamage)
{
  //  Store if changes occured to the quantity of units P1 & P2 started with
  int opposingPlayerStartingQuantity
    = opposingPlayerBattleParticipant.entityQuantity;
  int opposingPlayerEndingQuantity
    = opposingPlayerBattleParticipant.entityQuantity;
  int opposingPlayerQuantityDifference
    = (opposingPlayerStartingQuantity - opposingPlayerEndingQuantity);

  // Behaviour: Apply the results for p1 if ranged damage occured
  if (givenPlayerHasAArcherActivated == true) {
    // Behaviour: Apply the results to buildings or non-buildings
    if (
      (givenPlayerIsFightingBuilding == true)
      && (opposingPlayerBuildingDamage != 0)
      && (givenPlayerCanAttackOpposingPlayerBuilding == true)) {
      opposingPlayerBattleParticipant.entityHealth
        -= opposingPlayerBuildingDamage; // Behaviour: Decrease the building HP
      if (opposingPlayerBattleParticipant.entityHealth <= 0) {
        givenPlayerPointsGained
          = opposingPlayerBattleParticipant
              .pointValue; // Behaviour: Set the points lost value to the
                           // buildings point value
      }
      else if (opposingPlayerBattleParticipant.entityHealth > 0) {
        givenPlayerPointsGained
          = 0; // Behaviour: if the HP of the building is > 0
               // then do not keep the set the point value
      }
    }

    if (givenPlayerIsFightingUnit == true) {
      // Behavior: Store the starting quantity
      opposingPlayerStartingQuantity
        = opposingPlayerBattleParticipant.entityQuantity;

      // Behaviour: Now decrease the quantity 1 by 1, applying heals if there
      // any available
      while (opposingPlayerEntityDeaths != 0) {
        opposingPlayerBattleParticipant.entityQuantity -= 1;
        opposingPlayerEntityDeaths -= 1;
        checkIfDead();
      }

      // Behavior: Store the ending quantity and cap the ending quantity
      // at 0
      if (opposingPlayerBattleParticipant.entityQuantity < 0) {
        opposingPlayerEndingQuantity = 0;
      }
      else {
        opposingPlayerEndingQuantity
          = opposingPlayerBattleParticipant.entityQuantity;
      }
      // Behaviour: Update the difference between the two quantities
      opposingPlayerQuantityDifference
        = (opposingPlayerStartingQuantity - opposingPlayerEndingQuantity);

      // Behaviour: Award points if deaths occured
      if (opposingPlayerEntityDeaths >= 0) {
        givenPlayerPointsGained = opposingPlayerBattleParticipant.pointValue
                                  * opposingPlayerQuantityDifference;
      }
      else {
        givenPlayerPointsGained = 0;
      }
    }
  }
}

// Function: Calculate the outcome of a ranged battle
void archerRounds::roundOutcome(
  int          roundRunTimes,
  int*         p1Events,
  int*         p2Events,
  int*         p1Technologies,
  int*         p2Technologies,
  ActivePlayer activePlayer)
{
  const int roundDownMultiple
    = 10; // Store the multiple to round down by for P1 and P2
  bool p1ArcherActivated = false,
       p2ArcherActivated
       = false; //  Store if a ranged unit from P1 or P2 did something
  int p1RangedDamageDealt = 0, p2RangedDamageDealt = 0;
  int p1EntityDeaths = 0,
      p2EntityDeaths
      = 0; //  Declare the number of units P1 & P2 killed in this round
  int p1BuildingDamage = 0,
      p2BuildingDamage
      = 0; //  Store the amount of damage dealt to buildings in this round
  bool p1IsFightingBuilding = false,
       p2IsFightingBuilding
       = false; //  Store whether the archer entity is fighting a building
  bool p1IsFightingUnit = false, p2IsFightingUnit = false;
  int  p1DamageDie = 0,
      p2DamageDie
      = 0; //  Store the amount of damage die to place onto buildings
  float p1PointsGained = 0,
        p2PointsGained = 0; //  Store the amount of points awarded
  bool p1RangedUnitCanAttackP2Building = false,
       p2RangedUnitCanAttackP1Building
       = false; // Store if archers can attack buildings
  bool p1RangedUnitCanAttackP2Cavalry = false,
       p2RangedUnitCanAttackP1Cavalry
       = false; //  Store if archers can attack cavalry
  bool p1TakeNoDamageDueToShotsInTheBack = false,
       p2TakeNoDamageDueToShotsInTheBack = false;

  // Behaviour: Run the ranged battle round for X times
  for (int numberOfTimesToRunTheRangedBattleRound = 0;
       numberOfTimesToRunTheRangedBattleRound < roundRunTimes;
       numberOfTimesToRunTheRangedBattleRound++) {
    // Behaviour: Check if the remaining damage value triggers a death
    checkRemainingDamage(p1Events, p2Events);

    // Behaviour: Check that no deaths have occured
    checkIfDead();

    if (
      (aDeathHasOccured == false)
      && (isRetreating != "1")) { // Proceed if neither player has retreated and
                                  // no deaths have occured

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player1) != ActivePlayer::None) {
        calculatingArcherRoundOutcomeForAnIndividualPlayer(
          roundDownMultiple,
          p1BattleParticipant,
          p1Events,
          p1RoundAttackModifiers,
          p1RemainingDamage,
          p1ArcherActivated,
          p1IsFightingBuilding,
          p1IsFightingUnit,
          p1RangedUnitCanAttackP2Building,
          p1RangedUnitCanAttackP2Cavalry,
          p1RangedDamageDealt,
          p2BattleParticipant,
          p2Events,
          p2TakeNoDamageDueToShotsInTheBack,
          p2EntityDeaths,
          p2BuildingDamage,
          p2DamageDie);

      } // End of p1 stuff conditional

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player2) != ActivePlayer::None) {
        calculatingArcherRoundOutcomeForAnIndividualPlayer(
          roundDownMultiple,
          p2BattleParticipant,
          p2Events,
          p2RoundAttackModifiers,
          p2RemainingDamage,
          p2ArcherActivated,
          p2IsFightingBuilding,
          p2IsFightingUnit,
          p2RangedUnitCanAttackP1Building,
          p2RangedUnitCanAttackP1Cavalry,
          p2RangedDamageDealt,
          p1BattleParticipant,
          p1Events,
          p1TakeNoDamageDueToShotsInTheBack,
          p1EntityDeaths,
          p1BuildingDamage,
          p1DamageDie);

      } // End of p2 stuff conditional

      if (activePlayer != ActivePlayer::None) {
        applyingArcherRoundOutcomeForAnIndividualPlayer(
          p1ArcherActivated,
          p1IsFightingBuilding,
          p1RangedUnitCanAttackP2Building,
          p1IsFightingUnit,
          p1PointsGained,
          p2BattleParticipant,
          p2EntityDeaths,
          p2BuildingDamage);
        applyingArcherRoundOutcomeForAnIndividualPlayer(
          p2ArcherActivated,
          p2IsFightingBuilding,
          p2RangedUnitCanAttackP1Building,
          p2IsFightingUnit,
          p2PointsGained,
          p1BattleParticipant,
          p1EntityDeaths,
          p1BuildingDamage);
      }

      // Behaviour: Check if the remaining damage value triggers a death
      checkRemainingDamage(p1Events, p2Events);

      // Behaviour: Check for deaths
      checkIfDead();

      // Behaviour: Display the outcome of the archer combat round only if
      // changes occured
      if ((p1ArcherActivated == true) || (p2ArcherActivated == true)) {
        std::string p1Has;
        p1Has = returnWhatIAm(p1BattleParticipant, "Ranged round");

        std::string p2Has;
        p2Has = returnWhatIAm(p2BattleParticipant, "Ranged round");

        std::string outputString
          = p1Has + " vs. " + p2Has + " combat round "
            + std::to_string(numberOfTimesToRunTheRangedBattleRound + 1)
            + " calculations...";
        outputEntityInformation(outputString);

        outputtingArcherRoundOutcomeForAnIndividualPlayer(
          player1Name,
          p1PointsGained,
          p1IsFightingBuilding,
          p2BattleParticipant,
          p2DamageDie,
          player2Name);
        outputtingArcherRoundOutcomeForAnIndividualPlayer(
          player2Name,
          p2PointsGained,
          p2IsFightingBuilding,
          p1BattleParticipant,
          p1DamageDie,
          player1Name);

        std::cout << "<br>";

        // Behaviour: Check if a death has occured
        checkIfDead();

        // Behaviour: Check if the attacking archer is retreating
        if (aDeathHasOccured == false) {
          checkIfRetreating("Do you want to hit and run?<br>");
          checkIfRetreating("Do you want to retreat?<br>");

          // @Kory todo: Make sure that this event is working as intended
          if (
            (isRetreating == "1") && (p1Events[29] == 1)
            || (p2Events[29] == 1)) {
            //  [29] Shots In The Back (Briton) - If a unit in combat with your
            //  Archer retreats, your Archer deals an additional round of range
            //  damage without taking any damage

            std::string getAnswer = "";

            std::cout
              << "Did the conditions of \"Shots In The Back (Briton)\" get "
                 "satisfied for player 1 or 2?"
              << "<br>";
            bool isShotsInTheBackSatisfied{};
            DIN >> isShotsInTheBackSatisfied;
            getAnswer = isShotsInTheBackSatisfied ? "0" : "1";

            if (getAnswer == "1") {
              // Behaviour: Have another round of ranged combat
              roundRunTimes++;

              // Behavior: Have it so no ranged damage is taken for the player
              // with the event in question
              if (p1Events[29] == 1) {
                p1TakeNoDamageDueToShotsInTheBack = true;
              }
              else if (p2Events[29] == 1) {
                p2TakeNoDamageDueToShotsInTheBack = true;
              }
            }
          }
        }
      }

    } // Check if dead or retreating conditional end
  }   // For loop end
} // Archer round function end

void bombardmentRounds::outputtingBombardmentRoundOutcomeForAnIndividualPlayer(
  // Given player stuff
  std::string& givenPlayerName,

  float& givenPlayerPointsGained,
  bool&  givenPlayerIsFightingABuilding,

  Entity& givenPlayerBattleParticipant,
  bool& givenPlayerIsInRange,

  // Opposing player stuff
  std::string& opposingPlayerName,
  Entity&      opposingPlayerBattleParticipant,
  int&         opposingPlayerDamageDie)
{
  // Behavior: Display if it wasn't able to fight due to being out of range
  if(givenPlayerIsInRange == false){
    std::cout << givenPlayerBattleParticipant.entityName << " is unable to range " << opposingPlayerBattleParticipant.entityName << "<br>";
  }



  // Behaviour: Display how many damage die to place if appropriate
  if (givenPlayerIsFightingABuilding == true) {
    if (
      (opposingPlayerBattleParticipant.entityHealth > 0)
      && (opposingPlayerDamageDie > 0)) {
      std::cout << ">> Place " << opposingPlayerDamageDie << " damage die onto "
                << opposingPlayerName << "'s "
                << opposingPlayerBattleParticipant.entityName << "<br>";
    }
  }

  // Behaviour: Display how many points were added if appropriate
  if (givenPlayerPointsGained != 0) {
    std::cout << ">> " << givenPlayerName << " gets " << givenPlayerPointsGained
              << " points"
              << "<br>";
  }
}

void bombardmentRounds::applyingBombardmentRoundOutcomeForAnIndividualPlayer(
  // Given player stuff
  bool&  givenPlayerBombardmentEntityActivated,
  bool&  givenPlayerIsFightingBuilding,
  bool&  givenPlayerIsFightingUnit,
  float& givenPlayerPointsGained,
  // Opposing player stuff
  Entity& opposingPlayerBattleParticipant,
  int&    opposingPlayerBuildingDamage,
  int&    opposingPlayerEntityDeaths)
{


  //  Track if changes occured to the quantity
  int opposingPlayerStartingQuantity
    = opposingPlayerBattleParticipant.entityQuantity;
  int opposingPlayerEndingQuantity
    = opposingPlayerBattleParticipant.entityQuantity;
  int opposingPlayerQuantityDifference
    = (opposingPlayerStartingQuantity - opposingPlayerEndingQuantity);

  // Behaviour: Apply the results for p1 if the activation conditions were
  // met
  if (givenPlayerBombardmentEntityActivated == true) {
    // Behaviour: Apply the results to buildings or non-buildings
    if (givenPlayerIsFightingBuilding == true) {
      // Behaviour: Decrease the building HP
      if (opposingPlayerBuildingDamage != 0) {
        // Behaviour: Set the point value to the buildings point
        givenPlayerPointsGained = opposingPlayerBattleParticipant.pointValue;

        opposingPlayerBattleParticipant.entityHealth
          -= opposingPlayerBuildingDamage;

        // Behaviour: if the HP of the building is > 0 then do not keep the
        // set the point value
        if (opposingPlayerBattleParticipant.entityHealth > 0) {
          givenPlayerPointsGained = 0;
        }
      }
    }
    if (givenPlayerIsFightingUnit == true) {
      // Behavior: Store the starting quantity
      opposingPlayerStartingQuantity
        = opposingPlayerBattleParticipant.entityQuantity;

      // Behaviour: Store how many points the entity is individually worth
      givenPlayerPointsGained = opposingPlayerBattleParticipant.pointValue;

      // Behaviour: Now decrease the quantity
      while (opposingPlayerEntityDeaths != 0) {
        opposingPlayerBattleParticipant.entityQuantity -= 1;
        opposingPlayerEntityDeaths -= 1;
        checkIfDead();
      }

      // Behavior: Store the ending quantity and cap the ending quantity at
      // 0
      if (opposingPlayerBattleParticipant.entityQuantity < 0) {
        opposingPlayerEndingQuantity = 0;
      }
      else {
        opposingPlayerEndingQuantity
          = opposingPlayerBattleParticipant.entityQuantity;
      }

      // Behaviour: Update the difference between the two quantities
      opposingPlayerQuantityDifference
        = (opposingPlayerStartingQuantity - opposingPlayerEndingQuantity);

      // Behaviour: Award points if deaths occured
      if (opposingPlayerEntityDeaths >= 0) {
        // Behavior: Calculate the difference between the two quantities
        givenPlayerPointsGained *= opposingPlayerQuantityDifference;
      }
      else {
        givenPlayerPointsGained = 0;
      }
    }
  }
}

void bombardmentRounds::calculatingBombardmentRoundOutcomeForAnIndividualPlayer(
  // Shared stuff
  const int roundDownBasedOnMultiplesOfThisNumber,

  // Given player stuff
  Entity& givenPlayerBattleParticipant,
  bool&   givenPlayerIsFightingBuilding,
  bool&   givenPlayerIsFightingUnit,
  bool&   givenPlayerBombardmentEntityActivated,
  float&  givenPlayerRemainingDamage,
  int&    givenPlayerRoundAttackModifiers,
  bool& givenPlayerIsInRange,

  // Opposing player stuff
  Entity& opposingPlayerBattleParticipant,
  int&    opposingPlayerBuildingDamage,
  int&    opposingPlayerDamageDie,
  int&    opposingPlayerEntityDeaths)
{
  // Behaviour: Check if player 1 is fighting a building
  if (opposingPlayerBattleParticipant.armorClass[1] == true) { // Building p2
    givenPlayerIsFightingBuilding = true;
  }
  else {
    givenPlayerIsFightingBuilding = false;
  }

  // Behaviour: Check if given player is fighting a unit
  if (
    (opposingPlayerBattleParticipant.armorClass[25] == true)     // Land unit p2
    || (opposingPlayerBattleParticipant.armorClass[11] == true)) // Ship p2

  {
    givenPlayerIsFightingUnit = true;
  }
  else {
    givenPlayerIsFightingUnit = false;
  }



         // Behavior: Check if what's fighting can fight / fight back
  bool givenPlayerNotInMinimumRange = false, givenPlayerNotOutOfMaximumRange = false;

  if(m_state->distanceBetweenTheBattleParticipants >= givenPlayerBattleParticipant.minimumRange)
  {
    givenPlayerNotInMinimumRange = true;
  }

  if(m_state->distanceBetweenTheBattleParticipants <= givenPlayerBattleParticipant.maximumRange){
    givenPlayerNotOutOfMaximumRange = true;
  }

  if(givenPlayerNotInMinimumRange == true && givenPlayerNotOutOfMaximumRange == true){
    givenPlayerIsInRange = true;
  }



  // Behaviour: Calculate the damage against buildings if player 1 is
  // fighting a building
  givenPlayerBombardmentEntityActivated = false;

  if (givenPlayerIsFightingBuilding == true && givenPlayerIsInRange == true) {
    opposingPlayerBuildingDamage = ( ((givenPlayerBattleParticipant.standardDamage * givenPlayerBattleParticipant.entityQuantity) + givenPlayerRoundAttackModifiers) - ((opposingPlayerBattleParticipant.entityHealth) % roundDownBasedOnMultiplesOfThisNumber));
    opposingPlayerDamageDie = opposingPlayerBuildingDamage / 10;
    givenPlayerRemainingDamage += calculateRemainingDamage(
      opposingPlayerDamageDie,
      givenPlayerBattleParticipant.standardDamage,
      givenPlayerRoundAttackModifiers,
      opposingPlayerBattleParticipant.entityHealth,
      1);

    givenPlayerBombardmentEntityActivated = true;
  }
  // Behaviour: Calculate the damage against units if player 1 is fighting a
  // unit

  if (givenPlayerIsFightingUnit == true && givenPlayerIsInRange == true) {
    // Behaviour: The damage only applies if p1 has a Galley or Fire Ship
    if (
      (givenPlayerBattleParticipant.armorClass[21] == true)
      || (givenPlayerBattleParticipant.armorClass[21] == true)) {
      opposingPlayerEntityDeaths = std::floor(
        ((givenPlayerBattleParticipant.standardDamage
          * givenPlayerBattleParticipant.entityQuantity)
         + givenPlayerRoundAttackModifiers)
        / (opposingPlayerBattleParticipant.entityHealth));
      givenPlayerRemainingDamage += calculateRemainingDamage(
        opposingPlayerEntityDeaths,
        givenPlayerBattleParticipant.standardDamage,
        givenPlayerRoundAttackModifiers,
        opposingPlayerBattleParticipant.entityHealth,
        0);

      givenPlayerBombardmentEntityActivated = true;
    }
  }
}

// Function: Calculate the outcome of a bombardment round of battle (only for
// the Crow's nest event card)
void bombardmentRounds::roundOutcome(
  int          roundRunTimes,
  int*         p1Events,
  int*         p2Events,
  int*         p1Technologies,
  int*         p2Technologies,
  ActivePlayer activePlayer)
{
  bool p1BombardmentEntityActivated = false,
       p2BombardmentEntityActivated
       = false; //  Track if the bombardment attack round got activated
  int p1EntityDeaths = 0,
      p2EntityDeaths = 0; //  Declare the number of units killed in this round
  int roundDownMultiple = 10;
  int p1BuildingDamage  = 0,
      p2BuildingDamage
      = 0; //  Declare the amount of damage dealt to buildings in this round
  bool p1IsFightingBuilding = false,
       p2IsFightingBuilding
       = false; //  Get whether the bombardment entity is fighting a building
  bool p1IsFightingUnit = false, p2IsFightingUnit = false;
  int  p1DamageDie = 0,
      p2DamageDie
      = 0; //  Declare the amount of damage die to place onto buildings
  float p2PointsGained = 0,
        p1PointsGained = 0; //  Declare the amount of points awarded
  bool p1InRange = false, p2InRange = false; // Declare if the bombardment entity is in range

  // Behaviour: Run the bombardment battle round for X times
  for (int numberOfTimesToRunTheBombardmentRound = 0;
       numberOfTimesToRunTheBombardmentRound < roundRunTimes;
       numberOfTimesToRunTheBombardmentRound++) {
    // Behaviour: Check that no deaths have occured
    checkIfDead();

    // Behaviour: Check if the remaining damage value triggers a death
    checkRemainingDamage(p1Events, p2Events);

    // Behaviour: Make sure that no deaths have occured and that the attacking
    // entity is not retreating before proceeding
    if ((aDeathHasOccured == false) && (isRetreating != "1")) {
      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player1) != ActivePlayer::None) {
        calculatingBombardmentRoundOutcomeForAnIndividualPlayer(
          roundDownMultiple,
          p1BattleParticipant,
          p1IsFightingBuilding,
          p1IsFightingUnit,
          p1BombardmentEntityActivated,
          p1RemainingDamage,
          p1RoundAttackModifiers,
          p1InRange,
          p2BattleParticipant,
          p2BuildingDamage,
          p2DamageDie,
          p2EntityDeaths);

      } // end p1 calc

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player2) != ActivePlayer::None) {
        calculatingBombardmentRoundOutcomeForAnIndividualPlayer(
          roundDownMultiple,
          p2BattleParticipant,
          p2IsFightingBuilding,
          p2IsFightingUnit,
          p2BombardmentEntityActivated,
          p2RemainingDamage,
          p2RoundAttackModifiers,
          p2InRange,
          p1BattleParticipant,
          p1BuildingDamage,
          p1DamageDie,
          p2EntityDeaths);

      } // end p2 calc

      if (activePlayer != ActivePlayer::None) {
        applyingBombardmentRoundOutcomeForAnIndividualPlayer(
          p1BombardmentEntityActivated,
          p1IsFightingBuilding,
          p1IsFightingUnit,
          p1PointsGained,
          p2BattleParticipant,
          p2BuildingDamage,
          p2EntityDeaths);
        applyingBombardmentRoundOutcomeForAnIndividualPlayer(
          p2BombardmentEntityActivated,
          p2IsFightingBuilding,
          p2IsFightingUnit,
          p2PointsGained,
          p1BattleParticipant,
          p1BuildingDamage,
          p1EntityDeaths);
      }

      // Behaviour: Check if the remaining damage value triggers a death
      checkRemainingDamage(p1Events, p2Events);

      // Behaviour: Check for deaths
      checkIfDead();

      // Behaviour: Make sure that no deaths have occured
      if (aDeathHasOccured == false) {
        // Behaviour: Display the outcome of the bombardment combat round only
        // if changes occured
        if (
          (p1BombardmentEntityActivated == true)
          || (p2BombardmentEntityActivated == true)) {
          std::string p1Has;
          p1Has = returnWhatIAm(p1BattleParticipant, "Bombardment round");

          std::string p2Has;
          p2Has = returnWhatIAm(p2BattleParticipant, "Bombardment round");

          std::string outputString
            = p1Has + " vs. " + p2Has + " combat round "
              + std::to_string(numberOfTimesToRunTheBombardmentRound + 1)
              + " calculations...";

          outputEntityInformation(outputString);

          outputtingBombardmentRoundOutcomeForAnIndividualPlayer(
            player1Name,
            p1PointsGained,
            p1IsFightingBuilding,
            p1BattleParticipant,
            p1InRange,
            player2Name,
            p2BattleParticipant,
            p2DamageDie);
          outputtingBombardmentRoundOutcomeForAnIndividualPlayer(
            player2Name,
            p2PointsGained,
            p2IsFightingBuilding,
            p2BattleParticipant,
            p2InRange,
            player1Name,
            p1BattleParticipant,
            p1DamageDie);

          std::cout << "<br>";

          // Behaviour: Check if a death has occured
          checkIfDead();

          // Behaviour: Check if the attacking entity is retreating if this is
          // not the last round
          if (aDeathHasOccured == false) {
            if (numberOfTimesToRunTheBombardmentRound != roundRunTimes - 1) {
              checkIfRetreating("Bombardment");
            }
          }
        }
      }
    }
  }
}

// Function: Calculate the outcome of a standard battle

void standardRounds::calculatingStandardRoundOutcomeForAnIndividualPlayer(
  // Shared stuff
  int&      numberOfTimesToRunTheStandardRound,
  const int roundDownBasedOnMultiplesOfThisNumber,
  // Given player stuff
  Entity& givenPlayerBattleParticipant,
  int*    givenPlayerEvents,
  int     givenPlayerRoundAttackModifiers,
  float&  givenPlayerRemainingDamage,
  bool&   givenPlayerHasAEntityThatActivated,
  bool&   givenPlayerIsFightingBuilding,
  bool&   givenPlayerIsFightingAUnit,
  float&  givenPlayerPointsGained,
  int&    givenPlayerEntityDeaths,
  bool& givenPlayerIsInRange,

  // Opposing player stuff
  Entity& opposingPlayerBattleParticipant,
  int*    opposingPlayerEvents,
  int&    opposingPlayerEntityDeaths,
  int&    opposingPlayerBuildingDamage,
  int&    opposingPlayerDamageDie)
{
  // Behaviour: Check if given player is fighting a building
  if (opposingPlayerBattleParticipant.armorClass[1] == true) { // Building p2
    givenPlayerIsFightingBuilding = true;
  }
  else {
    givenPlayerIsFightingBuilding = false;
  }

  // Behaviour: Check if given player is fighting a unit
  if (
    (opposingPlayerBattleParticipant.armorClass[25] == true)     // Land unit p2
    || (opposingPlayerBattleParticipant.armorClass[11] == true)) // Ship p2

  {
    givenPlayerIsFightingAUnit = true;
  }
  else {
    givenPlayerIsFightingAUnit = false;
  }

  /* Given player event handling */
  // Behaviour: Give the person who played/did not play the event more attack
  // based on a d6 die roll
  if (givenPlayerEvents[11] == 1) {
    // [11] = Fortune Favors The Foolish
    // 1: Player's unit who did not play the card deals double damage
    // 2: Player's unit who did not play the card gains +2 standardDamage per
    // token 3: No effect 4: Player's unit who played the card gains +2
    // standardDamage per token 5: Player's unit who played the card gains +4
    // standardDamage per token 6: Player's unit who played the card gains
    // double damage until the end of the turn
    d6DieRoll = generateD6DieInput();
    switch (d6DieRoll) {
    case 1:
      opposingPlayerBattleParticipant.standardDamage *= 2;
      break;
    case 2:
      opposingPlayerBattleParticipant.standardDamage
        += (2 * opposingPlayerBattleParticipant.entityQuantity);
      break;
    case 3:
      // No effect
      break;
    case 4:
      givenPlayerBattleParticipant.standardDamage
        += (2 * opposingPlayerBattleParticipant.entityQuantity);
      break;
    case 5:
      givenPlayerBattleParticipant.standardDamage
        += (4 * opposingPlayerBattleParticipant.entityQuantity);
      break;
    case 6:
      givenPlayerBattleParticipant.standardDamage *= 2;
      break;
    }
  }

  // Behaviour: Give the person who played the card more attack
  if (givenPlayerEvents[35] == 1) {
    // [35] The Jester Is Dead, Let's Get Em (Celt) = Sacrifice 1 of your
    // villagers. +4 standardDamage to all defending units this turn
    givenPlayerBattleParticipant.standardDamage += 4;
  }

  // Behaviour: Infantry double standard attack vs cavalry
  if (givenPlayerEvents[20] == 1) {
    // [20] Muddy Battlefield - Target infantry gets double standardAttack
    // versus Cavalry this turn
    if (
      (givenPlayerBattleParticipant.armorClass[8])
      && (opposingPlayerBattleParticipant.armorClass[4])) {
      givenPlayerBattleParticipant.standardDamage *= 2;
    }
  }

  // Behaviour: Decrease the standardDamage by two units if event 9 is played
  if (givenPlayerEvents[9] == 1) {
    // [9] First Battle Jitter - Two tokens on target unit have 0 standardDamage
    // for this battle. Target unit must have five tokens on it
    if (opposingPlayerBattleParticipant.entityQuantity >= 5) {
      int getIndividualAttackOpposingPlayer
        = (opposingPlayerBattleParticipant.standardDamage / opposingPlayerBattleParticipant.entityQuantity);
      opposingPlayerBattleParticipant.standardDamage
        -= getIndividualAttackOpposingPlayer;
      opposingPlayerBattleParticipant.standardDamage
        -= getIndividualAttackOpposingPlayer;
    }
  }

  // Behaviour: Multiply the standardDamage by a random number between 1-6 if a
  // Fire Ship
  d6DieRoll = generateD6DieInput();

  // Behaviour: Set the result to 1 if player 2 has Soak_The_Timbers
  if (opposingPlayerEvents[30] == 1) {
    // [30] Soak The Timbers - All enemy Fire Ships get an automatic result of 1
    // for their die roll when calculating damage this turn
    d6DieRoll = 1;
  }

  if (givenPlayerBattleParticipant.armorClass[22] == true) { // Fire Ship or
                                                             // Fast Fire
    // Ship
    givenPlayerBattleParticipant.standardDamage *= d6DieRoll;
  }

  // Give the player who played the card a weakened defender
  // [24] Black Knight: “Play this card when you are the attacking Cavalry
  // unit. Two tokens on the defending unit have 0 AP for the first round of
  // normal combat
  int getHowMuchAttackIndividualUnitsDo            = 0;
  int getHowMuchAttackTheGroupDoesWithTwoLazyUnits = 0;

  if (givenPlayerEvents[24] == 1) {
    // If there's cavalry in play
    if (givenPlayerBattleParticipant.armorClass[4]) {
      // If it's round 1
      if (numberOfTimesToRunTheStandardRound == 0) {
        // Do not re-calculate this value again in the second round or
        // you're going to get a result that doesn't reflect the individual
        // attack of the unit (it will be divided too much)
        getHowMuchAttackIndividualUnitsDo
          = opposingPlayerBattleParticipant.standardDamage;

        if (opposingPlayerBattleParticipant.entityQuantity > 2) {
          getHowMuchAttackTheGroupDoesWithTwoLazyUnits
            = (opposingPlayerBattleParticipant.entityQuantity - 2)
              * getHowMuchAttackIndividualUnitsDo;
          opposingPlayerBattleParticipant.standardDamage
            = getHowMuchAttackTheGroupDoesWithTwoLazyUnits;
        }
        else {
          getHowMuchAttackTheGroupDoesWithTwoLazyUnits = 0;
          opposingPlayerBattleParticipant.standardDamage
            = getHowMuchAttackTheGroupDoesWithTwoLazyUnits;
        }
      }
      // If it's round 2
      else if (numberOfTimesToRunTheStandardRound == 1) {
        opposingPlayerBattleParticipant.standardDamage
          = getHowMuchAttackIndividualUnitsDo
            * opposingPlayerBattleParticipant.entityQuantity;
      }
    }
  }


         // Behavior: Check if what's fighting can fight / fight back
  bool givenPlayerNotInMinimumRange = false, givenPlayerNotOutOfMaximumRange = false;

  if(m_state->distanceBetweenTheBattleParticipants >= givenPlayerBattleParticipant.minimumRange)
  {
    givenPlayerNotInMinimumRange = true;
  }

  if(m_state->distanceBetweenTheBattleParticipants <= givenPlayerBattleParticipant.maximumRange){
    givenPlayerNotOutOfMaximumRange = true;
  }

  if(givenPlayerNotInMinimumRange == true && givenPlayerNotOutOfMaximumRange == true){
    givenPlayerIsInRange = true;
  }

  // Behaviour: Calculate the damage against buildings if player 1 is fighting a
  // building

  if (givenPlayerIsFightingBuilding == true && givenPlayerIsInRange == true) {
    opposingPlayerBuildingDamage = (givenPlayerBattleParticipant.standardDamage * givenPlayerBattleParticipant.entityQuantity);
    opposingPlayerBuildingDamage
      -= (opposingPlayerBuildingDamage % roundDownBasedOnMultiplesOfThisNumber);
    opposingPlayerDamageDie = opposingPlayerBuildingDamage / 10;
    givenPlayerRemainingDamage += calculateRemainingDamage(
      opposingPlayerDamageDie,
      givenPlayerBattleParticipant.standardDamage,
      givenPlayerRoundAttackModifiers,
      opposingPlayerBattleParticipant.entityHealth,
      1);

    givenPlayerHasAEntityThatActivated = true;
  }
  // Behaviour: Calculate the damage against units if player 1 is fighting
  // a unit

  if (givenPlayerIsFightingAUnit == true && givenPlayerIsInRange == true) {
    // Behaviour: Check if player 1 has an area effect unit and if so then
    // apply area effect damage
    if (givenPlayerBattleParticipant.dealsAreaEffectDamage == true) {
      int p1AreaEffectUnitDamage
        = ((givenPlayerBattleParticipant.standardDamage)
           + givenPlayerRoundAttackModifiers)
          / givenPlayerBattleParticipant.entityQuantity;
      opposingPlayerEntityDeaths = std::floor(
        (((opposingPlayerBattleParticipant.entityQuantity)
          * (p1AreaEffectUnitDamage))
         / opposingPlayerBattleParticipant.entityHealth));
      givenPlayerRemainingDamage += calculateRemainingDamage(
        opposingPlayerEntityDeaths,
        givenPlayerBattleParticipant.standardDamage,
        givenPlayerRoundAttackModifiers,
        opposingPlayerBattleParticipant.entityHealth,
        0);

      givenPlayerHasAEntityThatActivated = true;
    }
    else {
      opposingPlayerEntityDeaths = std::floor(
        ((givenPlayerBattleParticipant.standardDamage
          * givenPlayerBattleParticipant.entityQuantity)
         + givenPlayerRoundAttackModifiers)
        / (opposingPlayerBattleParticipant.entityHealth));
      givenPlayerRemainingDamage += calculateRemainingDamage(
        opposingPlayerEntityDeaths,
        givenPlayerBattleParticipant.standardDamage,
        givenPlayerRoundAttackModifiers,
        opposingPlayerBattleParticipant.entityHealth,
        0);

      givenPlayerHasAEntityThatActivated = true;
    }
  }

  // Behaviour: Clear the results if the entity only attacks once (in the
  // first round of combat) and we are not in the 1st round
  if (
    (givenPlayerBattleParticipant.onlyAttacksOnceInTheFirstRoundOfCombat
     == true)
    && (numberOfTimesToRunTheStandardRound + 1 != 1)) {
    opposingPlayerBuildingDamage = 0;
    opposingPlayerDamageDie      = 0;
    opposingPlayerEntityDeaths   = 0;
  }

  // Behaviour: Clear the results if the entity only attacks in the second
  // round of combat and we are not in the 2nd round
  if (
    (givenPlayerBattleParticipant.onlyAttacksOnceInTheSecondRoundOfCombat
     == true)
    && (numberOfTimesToRunTheStandardRound + 1 != 2)) {
    opposingPlayerBuildingDamage = 0;
    opposingPlayerDamageDie      = 0;
    opposingPlayerEntityDeaths   = 0;
  }

  // Behaviour: Clear the results if the entity is an infantry unit for
  // the attacking player and we are in the first round of combat
  if (givenPlayerEvents[37] == 1) {
    // [37] While They're Sleeping - Target attacking Infantry unit takes
    // no damage for the first round of combat. The ranged round does not
    // count as the first round of combat
    if (givenPlayerBattleParticipant.armorClass[8] == true) {
      givenPlayerEntityDeaths = 0;
    }
  }





}

void standardRounds::outputtingStandardRoundOutcomeForAnIndividualPlayer(
  // Given player stuff
  std::string& givenPlayerName,

  float& givenPlayerPointsGained,
  bool&  givenPlayerIsFightingABuilding,


  Entity& givenPlayerBattleParticipant,
  bool& givenPlayerIsInRange,

  // Opposing player stuff
  std::string& opposingPlayerName,
  Entity&      opposingPlayerBattleParticipant,
  int&         opposingPlayerDamageDie)
{
  // Behavior: Display if it wasn't able to fight due to being out of range
  if(givenPlayerIsInRange == false){
    std::cout << givenPlayerBattleParticipant.entityName << " is unable to range " << opposingPlayerBattleParticipant.entityName << "<br>";
  }


  // Behaviour: Display how many damage die to place if appropriate
  if (givenPlayerIsFightingABuilding == true) {
    if (
      (opposingPlayerBattleParticipant.entityHealth > 0)
      && (opposingPlayerDamageDie > 0)) {
      std::cout << ">> Place " << opposingPlayerDamageDie << " damage die onto "
                << opposingPlayerName << "'s "
                << opposingPlayerBattleParticipant.entityName << "<br>";
    }
  }

  // Behaviour: Display how many points were added if appropriate
  if (givenPlayerPointsGained != 0) {
    std::cout << ">> " << givenPlayerName << " gets " << givenPlayerPointsGained
              << " points"
              << "<br>";
  }
}

// Function: Apply the outcome of a standard battle

void standardRounds::applyingStandardRoundOutcomeForAnIndividualPlayer(// Shared stuff
  int& numberOfTimesToRunTheStandardRound,
  // Given player stuff
  Entity& givenPlayerBattleParticipant,
  int*    givenPlayerEvents,
  bool&   givenPlayerHasAEntityThatActivated,
  bool&   givenPlayerIsFightingAUnit,
  float&  givenPlayerPointsGained,
  bool& givenPlayerIsFightingBuilding,

  // Opposing player stuff
  Entity& opposingPlayerBattleParticipant,
  float&  opposingPlayerPointsGained,
  int&    opposingPlayerEntityDeaths,
  int &opposingPlayerBuildingDamage)
{
  //  Track if changes occured to the quantity

  int opposingPlayerStartingQuantity
    = opposingPlayerBattleParticipant.entityQuantity;
  int opposingPlayerEndingQuantity
    = opposingPlayerBattleParticipant.entityQuantity;
  int opposingPlayerQuantityDifference
    = (opposingPlayerStartingQuantity - opposingPlayerEndingQuantity);

  // Behaviour: Apply the results to units



         // Behaviour: Apply the results for p1 if the activation conditions were
         // met
  if (givenPlayerHasAEntityThatActivated == true) {
    // Behaviour: Apply the results to buildings or non-buildings
    if (givenPlayerIsFightingBuilding == true) {
      // Behaviour: Decrease the building HP
      if (opposingPlayerBuildingDamage != 0) {
        // Behaviour: Set the point value to the buildings point
        givenPlayerPointsGained = opposingPlayerBattleParticipant.pointValue;

        opposingPlayerBattleParticipant.entityHealth
          -= opposingPlayerBuildingDamage;

               // Behaviour: if the HP of the building is > 0 then do not keep
               // the set the point value
        if (opposingPlayerBattleParticipant.entityHealth > 0) {
          givenPlayerPointsGained = 0;
        }

               // Update the quantity of the building
        if (opposingPlayerBattleParticipant.entityHealth < 0) {
          opposingPlayerBattleParticipant.entityQuantity -= 1;
        }
      }
    }



  if (givenPlayerIsFightingAUnit == true) {
    // Behavior: Store the starting quantity
    opposingPlayerStartingQuantity
      = opposingPlayerBattleParticipant.entityQuantity;

    // Behaviour: Store how many points the entity is individually worth
    givenPlayerPointsGained = opposingPlayerBattleParticipant.pointValue;

    // Behaviour: Now decrease the quantity
    while (opposingPlayerEntityDeaths != 0) {
      opposingPlayerBattleParticipant.entityQuantity -= 1;
      opposingPlayerEntityDeaths -= 1;
      checkIfDead();
    }

    // Behavior: Store the ending quantity and cap the ending quantity
    // at
    // 0
    if (opposingPlayerBattleParticipant.entityQuantity < 0) {
      opposingPlayerEndingQuantity = 0;
    }
    else {
      opposingPlayerEndingQuantity
        = opposingPlayerBattleParticipant.entityQuantity;
    }

    // Behaviour: Update the difference between the two quantities
    opposingPlayerQuantityDifference
      = (opposingPlayerStartingQuantity - opposingPlayerEndingQuantity);

    // Behaviour: Award points if deaths occured
    if (opposingPlayerEntityDeaths >= 0) {
      // Behavior: Calculate the difference between the two quantities
      givenPlayerPointsGained *= opposingPlayerQuantityDifference;
    }
    else {
      givenPlayerPointsGained = 0;
    }
  }
}


  // Apply the effects of certain entities that automatically die in the
  // second round of combat Make sure that the standard round got
  // activated to avoid an arithmetic exception
  if (givenPlayerHasAEntityThatActivated == true) {
    if (numberOfTimesToRunTheStandardRound + 1 == 2) {
      if (givenPlayerBattleParticipant.isKamikaze == true) {
        opposingPlayerPointsGained
          = (givenPlayerBattleParticipant.pointValue / givenPlayerBattleParticipant.entityQuantity);
        givenPlayerBattleParticipant.entityQuantity = 0;
      }
    }
  }

  // Behaviour: Deal with event 34
  if (givenPlayerEvents[34] == 1) {
    // [34] The Hammer's Cavalry (Franks) - Play when one of your Cavalry
    // units is reduced to 1 token. Add two tokens to the unit at no cost
    if (givenPlayerBattleParticipant.armorClass[4] == true) { // Cavalry
      if (givenPlayerBattleParticipant.entityQuantity == 1) {
        givenPlayerBattleParticipant.entityQuantity += 2;
      }
    }
  }

}

// Function: Calculate the outcome of a standard round of battle
void standardRounds::roundOutcome(
  int          roundRunTimes,
  int*         p1Events,
  int*         p2Events,
  int*         p1Technologies,
  int*         p2Technologies,
  ActivePlayer activePlayer)
{
  const int roundDownMultiple = 10;
  bool      p1EntityActivated = false, p2EntityActivated = false;
  bool      p1IsFightingBuilding = false,
       p2IsFightingBuilding
       = false; //  Get whether the entity is fighting a building
  bool  p1IsFightingUnit = false, p2IsFightingUnit = false;
  float p1PointsGained = 0,
        p2PointsGained = 0; //  Declare the amount of points awarded
  int p1EntityDeaths   = 0,
      p2EntityDeaths   = 0; //  Declare the number of units killed in this round
  int p1BuildingDamage = 0,
      p2BuildingDamage
      = 0; //  Declare the amount of damage dealt to buildings in this round
  int p1DamageDie = 0,
      p2DamageDie
      = 0; //  Declare the amount of damage die to place onto buildings
  bool p1IsInRange = false, p2IsInRange = false; // Declare if it's in range of what it's attacking

  /* Run the standard battle round for X times */
  for (int numberOfTimesToRunTheStandardRound = 0;
       numberOfTimesToRunTheStandardRound < roundRunTimes;
       numberOfTimesToRunTheStandardRound++) {
    checkIfDead(); // Check that no deaths have occured

    if (
      (aDeathHasOccured == false)
      && (isRetreating != "1")) { // Make sure that no deaths have occured
                                  // before proceeding since something that's
                                  // dead cannot fight even though it can attack
                                  // as it dies

      // Behaviour: Check if the remaining damage value triggers a death
      checkRemainingDamage(p1Events, p2Events);

      // Behaviour: Check that no deaths have occured
      checkIfDead();

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player1) != ActivePlayer::None) {
        calculatingStandardRoundOutcomeForAnIndividualPlayer(
          numberOfTimesToRunTheStandardRound,
          roundDownMultiple,
          p1BattleParticipant,
          p1Events,
          p1RoundAttackModifiers,
          p1RemainingDamage,
          p1EntityActivated,
          p1IsFightingBuilding,
          p1IsFightingUnit,
          p1PointsGained,
          p1EntityDeaths,
          p1IsInRange,
          p2BattleParticipant,
          p2Events,
          p2EntityDeaths,
          p2BuildingDamage,
          p2DamageDie);

      } // end p1 calc

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player2) != ActivePlayer::None) {
        calculatingStandardRoundOutcomeForAnIndividualPlayer(
          numberOfTimesToRunTheStandardRound,
          roundDownMultiple,
          p2BattleParticipant,
          p2Events,
          p2RoundAttackModifiers,
          p2RemainingDamage,
          p2EntityActivated,
          p2IsFightingBuilding,
          p2IsFightingUnit,
          p2PointsGained,
          p2EntityDeaths,
          p2IsInRange,
          p1BattleParticipant,
          p1Events,
          p1EntityDeaths,
          p1BuildingDamage,
          p1DamageDie);

      } // end p2 calc

      if (activePlayer != ActivePlayer::None) {
        // Apply outcome of calculations for P1 and P2 here

        applyingStandardRoundOutcomeForAnIndividualPlayer(
          numberOfTimesToRunTheStandardRound,
          p1BattleParticipant,
          p1Events,
          p1EntityActivated,
          p1IsFightingUnit,
          p1PointsGained,
          p1IsFightingBuilding,
          p2BattleParticipant,
          p2PointsGained,
          p2EntityDeaths,
          p2BuildingDamage);

        applyingStandardRoundOutcomeForAnIndividualPlayer(
          numberOfTimesToRunTheStandardRound,
          p2BattleParticipant,
          p2Events,
          p2EntityActivated,
          p2IsFightingUnit,
          p2PointsGained,
          p2IsFightingBuilding,
          p1BattleParticipant,
          p1PointsGained,
          p1EntityDeaths,
          p1BuildingDamage);
      }

      // Behaviour: Check if the remaining damage value triggers a death
      checkRemainingDamage(p1Events, p2Events);

      // Behaviour: Check for deaths
      checkIfDead();

      // Behaviour: Display the outcome of the standard combat round only if
      // changes occured

      // Behaviour: Display the outcome of the standard combat round only if
      // changes occured
      if ((p1EntityActivated == true) || (p2EntityActivated == true)) {
        std::string p1Has;
        p1Has = returnWhatIAm(p1BattleParticipant, "Standard round");

        std::string p2Has;
        p2Has = returnWhatIAm(p2BattleParticipant, "Standard round");

        std::string outputString
          = p1Has + " vs. " + p2Has + " combat round "
            + std::to_string(numberOfTimesToRunTheStandardRound + 1)
            + " calculations...";

        outputEntityInformation(outputString);

        outputtingStandardRoundOutcomeForAnIndividualPlayer(
          player1Name,
          p1PointsGained,
          p1IsFightingBuilding,
          p1BattleParticipant,
          p1IsInRange,
          player2Name,
          p2BattleParticipant,
          p2DamageDie);
        outputtingStandardRoundOutcomeForAnIndividualPlayer(
          player2Name,
          p2PointsGained,
          p2IsFightingBuilding,
          p2BattleParticipant,
          p2IsInRange,
          player1Name,
          p1BattleParticipant,
          p1DamageDie);

        std::cout << "<br>";

        // Behaviour: Check if a death has occured
        checkIfDead();

        // Behaviour: Check if the attacking entity is retreating if this is not
        // the last round
        if (aDeathHasOccured == false) {
          if (numberOfTimesToRunTheStandardRound != roundRunTimes - 1) {
            if ((p1Events[27] == 1) || (p2Events[27] == 1)) {
              // [27] Retreat - Target unit must retreat before the first round
              // of combat. The ranged round does not count as the first round
              // of combat Behaviour: Retreating must occur
              isRetreating = "1";
            }
            else {
              checkIfRetreating("Do you want to retreat?<br>");
            }
          }
        }
      }
    }
  }
}

FightMonksRounds::FightMonksRounds(
  CombatCalculatorState*     state,
  CombatCalculatorCallbacks* callbacks,
  Kind                       kind)
  : combatCalculator{state, callbacks}, m_kind{kind}
{
}

static std::size_t getOtherPlayerIndex(std::size_t playerIndex)
{
  return playerIndex == 0 ? 1 : 0;
}

void FightMonksRounds::roundOutcome(
  int          roundRunTimes,
  int*         p1Events,
  int*         p2Events,
  int*         p1Technologies,
  int*         p2Technologies,
  ActivePlayer activePlayer)
{
  if (activePlayer == ActivePlayer::None) {
    return;
  }

  constexpr std::size_t                  playerCount{2};
  const std::array<Entity*, playerCount> battleParticipants{
    &p1BattleParticipant, &p2BattleParticipant};
  const std::array<Entity*, playerCount> monks{
    &p1BattleAssistant, &p2BattleAssistant};

  float p2PointsGained = 0, p1PointsGained = 0;

  for (int round{0}; round < roundRunTimes; ++round) {
    for (std::size_t player{0}; player < playerCount; ++player) {
      if (
        ((player == 0)
         && ((activePlayer & ActivePlayer::Player1) == ActivePlayer::None))
        || ((player == 1) && ((activePlayer & ActivePlayer::Player2) == ActivePlayer::None))) {
        continue;
      }

      const std::size_t otherPlayerIdx{getOtherPlayerIndex(player)};
      Entity* const     playerBattleParticipant{battleParticipants[player]};
      Entity* const     otherPlayerMonk{monks[otherPlayerIdx]};

      // aka 'die value'.
      const int battleParticipantCount{playerBattleParticipant->entityQuantity};

      // aka 'RA' or 'AP' value.
      const int battleParticipantDamage{
        getDamageForKind(playerBattleParticipant)};

      const int deadMonks{static_cast<int>(std::floor(
        static_cast<double>((battleParticipantCount * battleParticipantDamage))
        / otherPlayerMonk->entityHealth))};
      otherPlayerMonk->entityQuantity = std::clamp(
        otherPlayerMonk->entityQuantity - deadMonks,
        0,
        otherPlayerMonk->entityQuantity);
    }

    std::string outputString = "Fighting some monks round "
                               + std::to_string(round + 1) + " calculations...";
    outputEntityInformation(outputString);
  }

  checkIfDead();

  if (
    p2BattleAssistant.entityQuantity
    < p2BattleAssistant.initialEntityQuantity) {
    int numberOfMonksP2Lost = p2BattleAssistant.initialEntityQuantity
                              - p2BattleAssistant.entityQuantity;
    p1PointsGained
      = (p2BattleAssistant.pointValue / p2BattleAssistant.initialEntityQuantity)
        * numberOfMonksP2Lost;

    std::cout << ">> " << player1Name << " gets " << p1PointsGained << " points"
              << "<br>";
  }

  if (
    p1BattleAssistant.entityQuantity
    < p1BattleAssistant.initialEntityQuantity) {
    int numberOfMonksP1Lost = p1BattleAssistant.initialEntityQuantity
                              - p1BattleAssistant.entityQuantity;
    p2PointsGained
      = (p1BattleAssistant.pointValue / p1BattleAssistant.initialEntityQuantity)
        * numberOfMonksP1Lost;

    std::cout << ">> " << player2Name << " gets " << p2PointsGained << " points"
              << "<br>";
  }
}

FightMonksRounds::Kind FightMonksRounds::getKind() const
{
  return m_kind;
}

int FightMonksRounds::getDamageForKind(const Entity* entity) const
{
  switch (m_kind) {
  case Kind::Ranged:
    return entity->rangedDamage;
  case Kind::Melee:
    return entity->standardDamage;
  default:
    break;
  }

  Q_UNREACHABLE();
}

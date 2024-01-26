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

CombatCalculatorState::CombatCalculatorState()
{
  // Set all of the stored values to their initial values
  startingBattleParticipantQuantityP1 = 0;
  startingBattleParticipantQuantityP2 = 0;
  startingAssistantQuantityP1         = 0;
  startingAssistantQuantityP2         = 0;
  d6DieRoll                           = 0;
  aDeathHasOccured                    = false;
  isRetreating                        = "";
  roundAttackModifiersP1              = 0;
  roundAttackModifiersP2              = 0;
  remainingDamageP1                   = 0;
  remainingDamageP2                   = 0;
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
  , roundAttackModifiersP1{m_state->roundAttackModifiersP1}
  , roundAttackModifiersP2{m_state->roundAttackModifiersP2}
  , remainingDamageP1{m_state->remainingDamageP1}
  , remainingDamageP2{m_state->remainingDamageP2}
{
}

combatCalculator::~combatCalculator()
{
}

void combatCalculator::setPlayerNames(
  std::string& inputP1Name,
  std::string& inputP2Name)
{
  // Set the player names
  player1Name = inputP1Name;
  player2Name = inputP2Name;
}

void combatCalculator::setCombatParticipants(
  Entity& inputP1BattleParticipant,
  Entity& inputP2BattleParticipant,
  Entity& inputP1AssistingMonkBattleParticipant,
  Entity& inputP2AssistingMonkBattleParticipant,
  int&    inputRoundAttackModifiersP1,
  int&    inputRoundAttackModifiersP2)
{
  // Set the battle participants for P1
  p1BattleParticipant        = inputP1BattleParticipant;
  p1BattleAssistant = inputP1AssistingMonkBattleParticipant;
  roundAttackModifiersP1     = inputRoundAttackModifiersP1;

  // Set the battle participants for P2
  p2BattleParticipant        = inputP2BattleParticipant;
  p2BattleAssistant = inputP2AssistingMonkBattleParticipant;
  roundAttackModifiersP2     = inputRoundAttackModifiersP2;
}

void combatCalculator::setAdditionalValues(
  float& inputRemainingDamageP1,
  float& inputRemainingDamageP2)
{
  // Set the remaining damage for both players
  remainingDamageP1 = inputRemainingDamageP1;
  remainingDamageP2 = inputRemainingDamageP2;
}

float combatCalculator::returnRemaningDamage(int inputPlayerNumber)
{
  // Return the remaining damage based on the player number
  switch (inputPlayerNumber) {
  case 1:
    return remainingDamageP1;
  case 2:
    return remainingDamageP2;
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
  // Return the modified battle battle participants based on the play number after the combat results have been applied
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

void combatCalculator::checkIfItCanBeHealed()
{
  // If p1BattleParticipant has less units than it started with and there are heals available to it, heal it
  if (
    p1BattleParticipant.entityQuantity < startingBattleParticipantQuantityP1) {
    while (p1BattleParticipant.healsAvailable != 0) {
      p1BattleParticipant.entityQuantity++;
      p1BattleParticipant.healsAvailable--;
      std::cout << player1Name << " has used one of their heals on their "
                << p1BattleParticipant.entityName << "<br>";
    }
  }
  // If p2BattleParticipant has less units than it started with and there are heals available to it, heal it
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
void combatCalculator::checkIfDead()
{
  // Check if what would otherwise die, can be healed
  checkIfItCanBeHealed();

  // Check if the rule about monks resurrecting units applies
  // "If a military unit loses its last token but still has monks attached,
  // these monks keep the unit alive and in play"

  if (
    (p1BattleParticipant.entityQuantity <= 0)
    && (p1BattleParticipant.armorClass[1] == false)) {
    if (p1BattleAssistant.entityQuantity > 0) {
      std::cout
        << player1Name
        << " has a monk that has prevented all of their units from dying<br>";
      p1BattleParticipant.entityQuantity++;
    }
    else {
      aDeathHasOccured = true;

      if (p1BattleParticipant.entityName == "Monk") {
        m_callbacks->getOnPlayerEntityDeath()(Player::Player1, true);
      }
      else {
        m_callbacks->getOnPlayerEntityDeath()(Player::Player1, false);
      }

      if (p1BattleParticipant.entityName == "Wonder") {
        SFXToPlay("/sfx/significant_events/wonder_destroyed_sfx.wav");
      }
    }
  }

  if (
    (p2BattleParticipant.entityQuantity <= 0)
    && (p2BattleParticipant.armorClass[1] == false)) {
    if (p2BattleAssistant.entityQuantity > 0) {
      std::cout
        << player2Name
        << " has a monk that has prevented all of their units from dying<br>";
      p2BattleParticipant.entityQuantity++;
    }
    else {
      aDeathHasOccured = true;

      if (p2BattleParticipant.entityName == "Monk") {
        m_callbacks->getOnPlayerEntityDeath()(Player::Player2, true);
      }
      else {
        m_callbacks->getOnPlayerEntityDeath()(Player::Player2, false);
      }

      if (p2BattleParticipant.entityName == "Wonder") {
        SFXToPlay("/sfx/significant_events/wonder_destroyed_sfx.wav");
      }
    }
  }
}

// Function: Check if the attacking ranged archer is retreating
void combatCalculator::checkIfRetreating(std::string messageToAsk)
{
  // Behaviour: Ask the attacker if they want to retreat with their archer if they are not versing cavalry or an archer

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

  p1BattleParticipant.outputEntity(player1Name);
  // Do not show monks as being dead if started with 0
  if (
    (p1BattleAssistant.entityQuantity >= 0)
    && (p1BattleAssistant.initialEntityQuantity != 0)) {
    std::cout << "(Assisting) ";
    p1BattleAssistant.outputEntity(player1Name);
  }
  p2BattleParticipant.outputEntity(player2Name);

  // Do not show monks as being dead if started with 0
  if (
    (p2BattleAssistant.entityQuantity >= 0)
    && (p2BattleAssistant.initialEntityQuantity != 0))

  {
    std::cout << "(Assisting) ";
    p2BattleAssistant.outputEntity(player2Name);
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

  // Behaviour: Get the difference between the current number and the rounded number
  getTheDecimalValue = (getTheDecimalValue - roundedNumber);

  return getTheDecimalValue;
}

void combatCalculator::checkRemainingDamage( // @Kory todo: check if this is
                                             // working as intended
  int* inputP1Events,
  int* inputP2Events)
{
  // Reference: For whatever reason, decreasing the quantity of:
  // [p1/p2]BattleParticipant.entityQuantity by 1 triggers a floating point
  // error in the second round of combat

  // Event [26] Relentless Attack - this battle, any damage remaining in a round
  // after casualties have been taken carries over into the next round.
  // Remaining damage at the end of this battle is lost
  if (inputP1Events[26] == 1) {
    if (p2BattleParticipant.entityQuantity >= 1) {
      if (remainingDamageP1 >= 1) {
        p2BattleParticipant.entityQuantity -= 1;
        remainingDamageP1 -= 1;
      }
    }
  }

  if (inputP2Events[26] == 1) {
    if (p1BattleParticipant.entityQuantity >= 1) {
      if (remainingDamageP2 >= 1) {
        p1BattleParticipant.entityQuantity -= 1;
        remainingDamageP2 -= 1;
      }
    }
  }
}




// Function: Calculate the outcome of a monk battle
void combatCalculator::outputtingMonkRoundOutcomeForAnIndividualPlayer(ActivePlayer & activePlayer, std::string & givenPlayerName, int & givenPlayerPointsAwarded, bool & givenPlayerMonkPowersActivated, Entity & givenPlayerBattleParticipant, Entity & givenPlayerBattleAssistant){
  // Shows whether or not the given player's monk powers activated if appropriate
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

  // Show many heals the given player has earned on their battle participants if appropriate
  if(givenPlayerBattleParticipant.healsAvailable > 0){
    std::cout << givenPlayerName << " has ";
    (givenPlayerBattleParticipant.healsAvailable == 1)
      ? std::cout << " a heal"
      : std::cout << givenPlayerBattleParticipant.healsAvailable << " heals";
    std::cout << " available on their " << givenPlayerBattleParticipant.entityName << "<br>";
  }

  // Show many heals the given player has earned on their battle assistants if appropriate
  if(givenPlayerBattleAssistant.healsAvailable > 0){
    std::cout << givenPlayerName << " has ";
    (givenPlayerBattleAssistant.healsAvailable == 1)
      ? std::cout << " a heal"
      : std::cout << givenPlayerBattleAssistant.healsAvailable << " heals";
    std::cout << " available on their " << givenPlayerBattleAssistant.entityName << "<br>";
  }

  // Show how many points the given player earned if appropriate
  if (givenPlayerPointsAwarded != 0) {
    std::cout << ">> " << givenPlayerName << " gets " << givenPlayerPointsAwarded
              << " points"
              << "<br>";
  }

}

void combatCalculator::calculatingMonkRoundOutcomeForAnIndividualPlayer(
  // Both player names
  std::string & givenPlayerName,
  std::string & opposingPlayerName,
  // Both player battle participants
  Entity & givenPlayerBattleParticipant,
  Entity & givenPlayerBattleAssistant,
  Entity & opposingPlayerBattleParticipant,
  Entity & opposingPlayerBattleAssistant,
  // Both player events and technologies
  int *         givenPlayerEvents,
  int *         opposingPlayerEvents,
  int *         givenPlayerTechnologies,
  // Given player monk related information
  bool & givenPlayerHasAMonkPresent,
  bool & givenPlayerMonkPowersActivated,
  std::string & givenPlayerIsHealingOrConverting,
  int & givenPlayerConversionANDHealingRate,
  bool & givenPlayerAssistingMonks,
  bool & givenPlayerStandaloneMonks,
  int & givenPlayerPointsGained,
  int & givenPlayerEntitiesHealed)
{


  /* Check if the event "It's A Miracle" is active for the given player */
  if (givenPlayerEvents[18] == 1) {
    // [18] It's A Miracle - A successful Healing attempt saves us up three
    // tokens, instead of one. Play this card before an attempt is made
    givenPlayerEntitiesHealed = 3;
  }



  /* Check if player 1 has monks assisting a unit or monks on their own */
  if (
    (givenPlayerBattleAssistant.armorClass[9] == true)
    && (givenPlayerBattleAssistant.entityQuantity > 0)) {
    givenPlayerAssistingMonks               = true;
    givenPlayerConversionANDHealingRate = givenPlayerBattleAssistant.entityQuantity;


    qDebug() << "testing this out<br>";

  }
  else {
    givenPlayerAssistingMonks = false;
  }


  if (
    (givenPlayerBattleParticipant.armorClass[9] == true)
    && (givenPlayerBattleParticipant.entityQuantity > 0)) {
    givenPlayerStandaloneMonks              = true;
    givenPlayerConversionANDHealingRate = givenPlayerBattleParticipant.entityQuantity;
  }
  else {
    givenPlayerStandaloneMonks = false;
  }

  if (givenPlayerAssistingMonks == false && givenPlayerStandaloneMonks == false) {
    givenPlayerConversionANDHealingRate = 0;
  }

  /* Throw an error if player 1 has monks attached to monks */
  if (givenPlayerAssistingMonks == true && givenPlayerStandaloneMonks == true) {
    std::cout << "Error: Cannot have monks assisting monks";
    std::terminate();
  }

  /* Detect if the given player does in fact have monks */
  givenPlayerHasAMonkPresent = ((givenPlayerAssistingMonks == true) || (givenPlayerStandaloneMonks == true))
                                 ? true
                                 : false;

  /* Check if the event "Fat Friars Tavern O'Spirits" is active for the opposing player */
  if (opposingPlayerEvents[7] == 1) {
    // [7] Fat Friars Tavern O'Spirits - Monks on target unit may not have
    // any Conversion or Healing attempts this turn
    givenPlayerHasAMonkPresent = false;
  }






  if (givenPlayerHasAMonkPresent == true) { // Make sure player 1's monk is present
    // before proceeding


    const DialogInput::MonkAction monkAction{
                                             DIN.queryForMonkAction(givenPlayerName)}; // Get player 1's decision
    switch (monkAction) {
    case DialogInput::MonkAction::Convert:
      givenPlayerIsHealingOrConverting = "Converting";
      break;
    case DialogInput::MonkAction::Heal:
      givenPlayerIsHealingOrConverting = "Healing";
      break;
    default:
      std::cout << "Error the input must be converting or healing for "
                << givenPlayerName
                << "'s "
                   "conversion/healing attempt"
                << "<br>";

      std::terminate();
    }

    /* Check if the event "Back From A Foreign Land" (Byzantine) is active
     * for the given player */
    if (givenPlayerEvents[1] == 1) {
      if (givenPlayerIsHealingOrConverting == "Healing") {
        givenPlayerConversionANDHealingRate += 2;
      }
    }

    /* Check if the event "Back From A Foreign Land" (Teuton) is active
     * for the opposing player */
    if (opposingPlayerEvents[40] == 1) {
      if (givenPlayerIsHealingOrConverting == "Converting") {
        givenPlayerConversionANDHealingRate -= 1;
      }
    }

    d6DieRoll = generateD6DieInput(); // Generate, validate, and return d6
                                      // dice input before proceeding

    /* Check if the event "Zealous Monks" is active for the given player */
    if (
      (givenPlayerEvents[39] == 1)
      && (givenPlayerIsHealingOrConverting == "Converting")) {
      // [39] Zealous Monks - Target Monk unit gets 1 conversion roll at a
      // 3 or less rate for each monk attached. In other words, you reroll
      // the dice if the number is <= 3 You repeat the roll for each monk
      // attached

      int maximumRerolls
        = (givenPlayerAssistingMonks == true)
            ? givenPlayerBattleAssistant.entityQuantity
            : givenPlayerBattleParticipant
                .entityQuantity; // Check if there are just assisting
                                 // monks are just standalone monks
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
      // [23] Piety - If you have one monk attached to a unit, your
      // conversion rate is 4. If the attempt fails, the monk dies
      const int pietyConversionRate = 4;

      if (pietyConversionRate <= d6DieRoll) { // Check if the conversion
                                              // attempt failed and if so,
                                              // reduce the quantity of
                                              // monks by 1
        (givenPlayerAssistingMonks == true)
          ? givenPlayerBattleAssistant.entityQuantity -= 1
          : givenPlayerBattleParticipant.entityQuantity -= 1;
      }
      else { // The conversion attempt succeeded, will have this affect
             // the calculations later on
        givenPlayerConversionANDHealingRate = pietyConversionRate;
      }
    }

    /* Check if the event "Listen To A Story" is active for the given player */
    if (
      (givenPlayerEvents[19] == 1)
      && (givenPlayerIsHealingOrConverting == "Converting")) {
      // [19] Listen To A Story - Attempt 1-3 extra conversions this
      // battle. Pay 2 gold per conversion
      const int numberOfTimesTheDieHasBeenRolledSoFarForTheGivenPlayer
        = 1; // So we've had one roll from earlier.
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
          std::cout << "The conversion attempt for the given player failed. Pay 2 gold "
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
            = numberOfExtraConversionAttemptsForTheGivenPlayer; // Jump to the end of
                                                                // the while loop
        }

        numberOfIterationsThroughTheWhileLoop++;
      }

             // Display how much gold player 1 needs to pay for the "Listen To A
             // Story" event card
      if (numberOfTimesTheGivenPlayerHasPaidToAttemptAnotherConversion > 0) {
        int listenToAStoryGoldCostForTheGivenPlayer
          = 2 * numberOfTimesTheGivenPlayerHasPaidToAttemptAnotherConversion;
        std::cout << "<< Player 1 must pay "
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

               // Work out whether player 1 is targeting a assistant or
               // non-assistant

        std::string getGivenPlayerMonkConversionTarget = "";

        auto doesGivenPlayerHaveRedemption = [givenPlayerTechnologies] {
          return givenPlayerTechnologies[18] == 1;
        };

        if (
          givenPlayerBattleAssistant.entityQuantity > 0
          && opposingPlayerBattleAssistant.entityQuantity > 0
          && doesGivenPlayerHaveRedemption()) {
          std::cout
            << "Is " << givenPlayerName << "'s 'assistant monk' targeting "
            << opposingPlayerName
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
            checkIfItCanBeHealed(); // Check for healing every time
                                    // quantity goes down
          }
        }
        else if (getGivenPlayerMonkConversionTarget == "1") {
          givenPlayerPointsGained = opposingPlayerBattleAssistant.pointValue;
          if (opposingPlayerBattleAssistant.entityQuantity > 0) {
            opposingPlayerBattleAssistant.entityQuantity -= 1;
            checkIfItCanBeHealed(); // Check for healing every time
                                    // quantity goes down
          }
        }

               // Play a SFX for a successful conversion attempt
        SFXToPlay("/sfx/rng/successful_monk_conversion_attempt_sfx.wav");
      }
      else if (givenPlayerIsHealingOrConverting == "Healing") {
        // @Kory todo: Perhaps the target of the heal could be either the
        // assistant or battle participant
        for (int addHeals = 0; addHeals < givenPlayerEntitiesHealed; addHeals++) {
          givenPlayerBattleParticipant.healsAvailable++;
        }



               // Play a SFX for a successful healing attempt
        SFXToPlay("/sfx/rng/successful_monk_healing_attempt_sfx.wav");
      }
    }
  }
}


void monkRounds::roundOutcome(
  int          roundRunTimes,
  int*         inputP1Events,
  int*         inputP2Events,
  int*         inputP1Technologies,
  int*         inputP2Technologies,
  ActivePlayer activePlayer)
{
  bool monkPresentP1 = false, monkPresentP2 = false; // Store if there are monks from P1 and P2
  bool monkPowersActivatedP1 = false, monkPowersActivatedP2 = false; // Store if the monks from P1 and P2 did something
  int p1PointsGained = 0, p2PointsGained = 0; // Store the amount of points awarded to P1 and P2 for kills
  int p1EntitiesHealed = 1, p2EntitiesHealed = 1; // Store the amount of entities a heal from a monk, heals for P1 and P2
  std::string player1IsHealingOrConverting = "", player2IsHealingOrConverting = ""; // Store if P1 and P2 is performing a conversion or healing attempt
  int conversionRateANDhealingRateP1 = 0, conversionRateANDhealingRateP2 = 0; //  Stores the likelihood of an attempt to convert or heal a monk succeeding for P1 and P2
  bool assistingMonksP1  = false, assistingMonksP2 = false; // Store whether monks are attached to a unit for P1 and P2
  bool standaloneMonksP1 = false, standaloneMonksP2 = false; // Store whether monks are on their own for P1 and P2

  /* Run the monk battle round for X times */
  for (int numberOfTimesToRunTheMonkBattleRound = 0;
       numberOfTimesToRunTheMonkBattleRound < roundRunTimes;
       numberOfTimesToRunTheMonkBattleRound++) {

    checkIfDead(); // Check that no deaths have occured

    if (aDeathHasOccured == false) { // Make sure that no deaths have occured before proceeding since something that's dead cannot fight even though it can attack as it dies

      // Determine if we'll be running the calculations for P1
      if ((activePlayer & ActivePlayer::Player1) != ActivePlayer::None) { // If "Player 1" != "No players"

        // Perform the calculations for P1
        calculatingMonkRoundOutcomeForAnIndividualPlayer(
          player1Name,
          player2Name,
          p1BattleParticipant,
          p1BattleAssistant,
          p2BattleParticipant,
          p2BattleAssistant,
          inputP1Events,
          inputP2Events,
          inputP1Technologies,
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
          inputP2Events,
          inputP1Events,
          inputP2Technologies,
          monkPresentP2,
          monkPowersActivatedP2,
          player2IsHealingOrConverting,
          conversionRateANDhealingRateP2,
          assistingMonksP2,
          standaloneMonksP2,
          p2PointsGained,
          p2EntitiesHealed);
      }

      // Behaviour: Display the outcome of the monk combat round only if monks from P1 or P2 did something
      if (
        (monkPowersActivatedP1 == true) || (monkPowersActivatedP2 == true)
        ) {

        std::string outputString
          = "Monk round "
            + std::to_string(numberOfTimesToRunTheMonkBattleRound + 1)
            + " calculations...";

        // Show information about the battle participants
        outputEntityInformation(outputString);

        // Show information related to the monk round calculations
        outputtingMonkRoundOutcomeForAnIndividualPlayer(
          activePlayer, player1Name, p1PointsGained, monkPowersActivatedP1, p1BattleParticipant, p1BattleAssistant);
        outputtingMonkRoundOutcomeForAnIndividualPlayer(
          activePlayer, player2Name, p2PointsGained, monkPowersActivatedP2, p2BattleParticipant, p2BattleAssistant);

        std::cout << "<br>";
      }

    } // End if conditional checking for no deaths

  } // End for loop

} // End monk combat rounds function

// Function: Calculate the outcome of a ranged battle
void archerRounds::roundOutcome(
  int          roundRunTimes,
  int*         inputP1Events,
  int*         inputP2Events,
  int*         inputP1Technologies,
  int*         inputP2Technologies,
  ActivePlayer activePlayer)
{
  /* Shared stuff */
  const int roundDownMultiple = 10;

  /* P1 stuff */
  bool p1ArcherActivated
    = false; //  Store if a ranged unit from P1 did something
  int p1RangedDamageDealt = 0;
  int p1EntityDeaths = 0; //  Store the number of units killed in this round
  int p1BuildingDamage
    = 0; //  Store the amount of damage dealt to buildings in this round
  bool isP1FightingBuilding
    = false; //  Store whether the archer entity is fighting a building
  if (
    (p2BattleParticipant.armorClass[1] == true)
    && (p1BattleParticipant.armorClass[0] == true)) { //  Get whether the archer
                                                      //  entity is fighting a
                                                      //  building
    isP1FightingBuilding = true;
  }
  else {
    isP1FightingBuilding = false;
  }
  bool isP1FightingUnit = false;
  if (
    (isP1FightingBuilding == false)
    && (p1BattleParticipant.armorClass[0] == true)) {
    isP1FightingUnit = true;
  }
  else {
    isP1FightingUnit = false;
  }

  int p1DamageDie
    = 0; //  Store the amount of damage die to place onto buildings
  float p2PointsGained     = 0; //  Store the amount of points awarded
  int   p1StartingQuantity = 0; //  Store if changes occured to the quantity
  int   p1EndingQuantity   = 0; //  Store if changes occured to the quantity
  bool  p1RangedUnitCanAttackP2Building
    = false; // Store if archers can attack buildings
  // @Kory todo: Add a new event that allows archers to be able to fight
  // buildings
  bool p1RangedUnitCanAttackP2Cavalry
    = false; //  Store if archers can attack cavalry
  // Apply the effects of event 33
  if (inputP1Events[33] == 1) {
    // [33] Steady Hand - Target Archer unit may do ranged damage to a Cavalry
    // unit this turn
    p1RangedUnitCanAttackP2Cavalry = true;
  }
  bool protectP1FromTakingDamageDueToShotsInTheBack = false;

  /* P2 stuff */
  bool p2ArcherActivated
    = false; //  Track if a ranged unit from either players did something
  int p2EntityDeaths = 0; //  Declare the number of units killed in this round
  int p2RangedDamageDealt = 0;
  int p2BuildingDamage
    = 0; //  Declare the amount of damage dealt to buildings in this round
  bool isP2FightingBuilding
    = false; //  Get whether the archer entity is fighting a building
  if (
    (p1BattleParticipant.armorClass[1] == true)
    && (p2BattleParticipant.armorClass[0] == true)) { //  Get whether the archer
                                                      //  entity is fighting a
                                                      //  building
    isP2FightingBuilding = true;
  }
  else {
    isP2FightingBuilding = false;
  }
  bool isP2FightingUnit = false;
  if (
    (isP2FightingBuilding == false)
    && (p2BattleParticipant.armorClass[0] == true)) {
    isP2FightingUnit = true;
  }
  else {
    isP2FightingUnit = false;
  }

  int p2DamageDie
    = 0; //  Declare the amount of damage die to place onto buildings
  float p1PointsGained     = 0; //  Declare the amount of points awarded
  int   p2StartingQuantity = 0; //  Track if changes occured to the quantity
  int   p2EndingQuantity   = 0; //  Track if changes occured to the quantity
  bool  p2RangedUnitCanAttackP1Building
    = false; // Store if archers can attack buildings
  // @Kory todo: Add a new event that allows archers to be able to fight
  // buildings
  bool p2RangedUnitCanAttackP1Cavalry
    = false; //  Negate the non-cavalry requirement
  if (inputP2Events[33] == 1) {
    // [33] Steady Hand - Target Archer unit may do ranged damage to a Cavalry
    // unit this turn
    p2RangedUnitCanAttackP1Cavalry = true;
  }
  bool protectP2FromTakingDamageDueToShotsInTheBack = false;

  // Behaviour: Run the ranged battle round for X times
  for (int i = 0; i < roundRunTimes; i++) {
    // Behaviour: Check if the remaining damage value triggers a death
    checkRemainingDamage(inputP1Events, inputP2Events);

    // Behaviour: Check that no deaths have occured
    checkIfDead();

    if (
      (aDeathHasOccured == false)
      && (isRetreating != "1")) { // Proceed if neither player has retreated and
                                  // no deaths have occured

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player1) != ActivePlayer::Player1) {
        // Perform archer vs building combat if an archer is fighting a building
        if (isP1FightingBuilding == true) {
          // Perform calculations if the archer is able to fight the building
          if (p1RangedUnitCanAttackP2Building == true) {
            p2BuildingDamage
              = (p1BattleParticipant.rangedDamage * p1BattleParticipant.entityQuantity);
            p2BuildingDamage += roundAttackModifiersP1;
            p2BuildingDamage
              -= (p2BattleParticipant.entityHealth % roundDownMultiple);

            p2DamageDie = p2BuildingDamage / 10;
            remainingDamageP1 += calculateRemainingDamage(
              p2DamageDie,
              p1BattleParticipant.rangedDamage,
              roundAttackModifiersP1,
              p2BattleParticipant.entityHealth,
              1);

            p1ArcherActivated = true;
          }
        }
        // Perform archer vs unit combat if an archer is fighting a unit
        else if (isP1FightingUnit == true) {
          // Do not perform archer vs cav combat unless an event card overrides
          // this rule
          if (
            (p1BattleParticipant.armorClass[0] == true) && // Archer armor class
            (p2BattleParticipant.armorClass[4] == true)
            && // Cavalry armor class
            (p1RangedUnitCanAttackP2Cavalry == false)) {
            // Do nothing
          }
          else {
            p1RangedDamageDealt = p1BattleParticipant.rangedDamage
                                  * p1BattleParticipant.entityQuantity;
            p1RangedDamageDealt += roundAttackModifiersP1;
            p1RangedDamageDealt /= p2BattleParticipant.entityHealth;
            p2EntityDeaths = std::floor(p1RangedDamageDealt);

            remainingDamageP1 += calculateRemainingDamage(
              p2EntityDeaths,
              p1BattleParticipant.rangedDamage,
              roundAttackModifiersP1,
              p2BattleParticipant.entityHealth,
              0);

            p1ArcherActivated = true;
          }
        }

        // Behavior: Make ranged combat attacks for P1 ineffective if this event
        // or shots in the back event is active for P2
        if (
          (inputP2Events[15] == 1)
          || (protectP2FromTakingDamageDueToShotsInTheBack == true)) {
          // [15] Heavy Tree Cover - Negate range combat round in target battle
          // involving one of your units

          // Deal no unit damage
          p1RangedDamageDealt = 0;
          p2EntityDeaths      = 0;

          // Deal no building damage
          p2BuildingDamage = 0;
          p2DamageDie      = 0;
        }

        // Behaviour: Apply the results for p1 if ranged damage occured
        if (p1ArcherActivated == true) {
          // Behaviour: Apply the results to buildings or non-buildings
          if (
            (isP1FightingBuilding == true) && (p2BuildingDamage != 0)
            && (p1RangedUnitCanAttackP2Building == true)) {
            p2BattleParticipant.entityHealth
              -= p2BuildingDamage; // Behaviour: Decrease the building HP
            if (p2BattleParticipant.entityHealth <= 0) {
              p1PointsGained
                = p2BattleParticipant
                    .pointValue; // Behaviour: Set the points lost value to the
                                 // buildings point value
            }
            else if (p2BattleParticipant.entityHealth > 0) {
              p1PointsGained = 0; // Behaviour: if the HP of the building is > 0
                                  // then do not keep the set the point value
            }
          }
          else {
            // Behavior: Store the starting quantity
            p2StartingQuantity = p2BattleParticipant.entityQuantity;

            // Behaviour: Now decrease the quantity
            p2BattleParticipant.entityQuantity -= p2EntityDeaths;

            // Behavior: Store the ending quantity and cap the ending quantity
            // at 0
            if (p2BattleParticipant.entityQuantity < 0) {
              p2EndingQuantity = 0;
            }
            else {
              p2EndingQuantity = p2BattleParticipant.entityQuantity;
            }
            // Behaviour: Calculate the difference between the two quantities
            int p2QuantityDifference = (p2StartingQuantity - p2EndingQuantity);

            // Behaviour: Award points if deaths occured
            if (p2EntityDeaths >= 0) {
              p1PointsGained
                = p2BattleParticipant.pointValue * p2QuantityDifference;
            }
            else {
              p1PointsGained = 0;
            }
          }
        }

      } // End of p1 stuff conditional

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player2) != ActivePlayer::None) {
        // Perform archer vs building combat if an archer is fighting a building
        if (isP2FightingBuilding == true) {
          // Perform calculations if the archer is able to fight the building
          if (p2RangedUnitCanAttackP1Building == true) {
            p1BuildingDamage
              = (p2BattleParticipant.rangedDamage * p2BattleParticipant.entityQuantity);
            p1BuildingDamage += roundAttackModifiersP2;
            p1BuildingDamage
              -= (p1BattleParticipant.entityHealth % roundDownMultiple);

            p1DamageDie = p1BuildingDamage / 10;
            remainingDamageP2 += calculateRemainingDamage(
              p1DamageDie,
              p2BattleParticipant.rangedDamage,
              roundAttackModifiersP1,
              p1BattleParticipant.entityHealth,
              1);

            p2ArcherActivated = true;
          }
        }
        // Perform archer vs unit combat if an archer is fighting a unit
        else if (isP2FightingUnit == true) {
          // Do not perform archer vs cav combat unless an event card overrides
          // this rule
          if (
            (p2BattleParticipant.armorClass[0] == true) && // Archer armor class
            (p1BattleParticipant.armorClass[4] == true)
            && // Cavalry armor class
            (p2RangedUnitCanAttackP1Cavalry == false)) {
            // Do nothing
          }
          else {
            p2RangedDamageDealt = p2BattleParticipant.rangedDamage
                                  * p2BattleParticipant.entityQuantity;
            p2RangedDamageDealt += roundAttackModifiersP2;
            p2RangedDamageDealt /= p1BattleParticipant.entityHealth;
            p1EntityDeaths = std::floor(p2RangedDamageDealt);

            remainingDamageP2 += calculateRemainingDamage(
              p1EntityDeaths,
              p2BattleParticipant.rangedDamage,
              roundAttackModifiersP1,
              p1BattleParticipant.entityHealth,
              0);

            p2ArcherActivated = true;
          }
        }

        // Behavior: Make ranged combat attacks for P2 ineffective if this event
        // or shots in the back event is active for P1
        if (
          (inputP1Events[15] == 1)
          || (protectP1FromTakingDamageDueToShotsInTheBack == true)) {
          // [15] Heavy Tree Cover - Negate range combat round in target battle
          // involving one of your units

          // Deal no unit damage
          p2RangedDamageDealt = 0;
          p1EntityDeaths      = 0;

          // Deal no building damage
          p1BuildingDamage = 0;
          p1DamageDie      = 0;
        }

        // Behaviour: Apply the results for p1 if ranged damage occured
        if (p2ArcherActivated == true) {
          // Behaviour: Apply the results to buildings or non-buildings
          if (
            (isP2FightingBuilding == true) && (p1BuildingDamage != 0)
            && (p2RangedUnitCanAttackP1Building == true)) {
            p1BattleParticipant.entityHealth
              -= p1BuildingDamage; // Behaviour: Decrease the building HP
            if (p1BattleParticipant.entityHealth <= 0) {
              p2PointsGained
                = p1BattleParticipant
                    .pointValue; // Behaviour: Set the points lost value to the
                                 // buildings point value
            }
            else if (p1BattleParticipant.entityHealth > 0) {
              p2PointsGained = 0; // Behaviour: if the HP of the building is > 0
                                  // then do not keep the set the point value
            }
          }
          else {
            // Behavior: Store the starting quantity
            p1StartingQuantity = p1BattleParticipant.entityQuantity;

            // Behaviour: Now decrease the quantity
            p1BattleParticipant.entityQuantity -= p1EntityDeaths;

            // Behavior: Store the ending quantity and cap the ending quantity
            // at 0
            if (p1BattleParticipant.entityQuantity < 0) {
              p1EndingQuantity = 0;
            }
            else {
              p1EndingQuantity = p1BattleParticipant.entityQuantity;
            }
            // Behaviour: Calculate the difference between the two quantities
            int p1QuantityDifference = (p1StartingQuantity - p1EndingQuantity);

            // Behaviour: Award points if deaths occured
            if (p1EntityDeaths >= 0) {
              p2PointsGained
                = p1BattleParticipant.pointValue * p1QuantityDifference;
            }
            else {
              p2PointsGained = 0;
            }
          }
        }

      } // End of p2 stuff conditional

      // Behaviour: Display the outcome of the archer combat round only if
      // changes occured
      if ((p1ArcherActivated == true) || (p2ArcherActivated == true)) {
        std::string outputString
          = "Archer round " + std::to_string(i + 1) + " calculations...";
        outputEntityInformation(outputString);

        // Behaviour: Display how many damage die to place if appropriate
        if (isP1FightingBuilding == true) {
          if ((p2BattleParticipant.entityHealth > 0) && (p2DamageDie > 0)) {
            std::cout << ">> Place " << p2DamageDie << " damage die onto "
                      << player2Name << "'s " << p2BattleParticipant.entityName
                      << "<br>";
          }
        }
        else if (isP2FightingBuilding == true) {
          if ((p1BattleParticipant.entityHealth > 0) && (p1DamageDie > 0)) {
            std::cout << ">> Place " << p1DamageDie << " damage die onto "
                      << player1Name << "'s " << p1BattleParticipant.entityName
                      << "<br>";
          }
        }

        // Behaviour: Display how many points were added if appropriate
        if (p1PointsGained != 0) {
          std::cout << ">> " << player1Name << " gets " << p1PointsGained
                    << " points"
                    << "<br>";
        }
        if (p2PointsGained != 0) {
          std::cout << ">> " << player2Name << " gets " << p2PointsGained
                    << " points"
                    << "<br>";
        }

        std::cout << "<br>";

        // Behaviour: Check if a death has occured
        checkIfDead();

        // Behaviour: Check if the attacking archer is retreating
        if (aDeathHasOccured == false) {
          checkIfRetreating("Do you want to hit and run?<br>");
          checkIfRetreating("Do you want to retreat?<br>");

          // @Kory todo: Make sure that this event is working as intended
          if (
            (isRetreating == "1") && (inputP1Events[29] == 1)
            || (inputP2Events[29] == 1)) {
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
              if (inputP1Events[29] == 1) {
                protectP1FromTakingDamageDueToShotsInTheBack = true;
              }
              else if (inputP2Events[29] == 1) {
                protectP2FromTakingDamageDueToShotsInTheBack = true;
              }
            }
          }
        }
      }

    } // Check if dead or retreating conditional end
  }   // For loop end
} // Archer round function end

// Function: Calculate the outcome of a bombardment round of battle (only for
// the Crow's nest event card)
void bombardmentRounds::roundOutcome(
  int          roundRunTimes,
  int*         inputP1Events,
  int*         inputP2Events,
  int*         inputP1Technologies,
  int*         inputP2Technologies,
  ActivePlayer activePlayer)
{
  //  Track if the bombardment attack round got activated
  bool bombardmentRoundActivated = false;

  //  Declare the number of units killed in this round
  int p1EntityDeaths = 0, p2EntityDeaths = 0;

  //  Declare the amount of damage dealt to buildings in this round
  int p1BuildingDamage = 0, p2BuildingDamage = 0, roundDownMultiple = 10;

  //  Get whether the bombardment entity is fighting a building
  bool isP1FightingBuilding = false, isP2FightingBuilding = false;

  //  Declare the amount of damage die to place onto buildings
  int p1DamageDie = 0, p2DamageDie = 0;

  //  Declare the amount of points awarded
  float p2PointsGained = 0, p1PointsGained = 0;

  //  Track if changes occured to the quantity
  int p1StartingQuantity = 0, p1EndingQuantity = 0;
  int p2StartingQuantity = 0, p2EndingQuantity = 0;

  // Behaviour: Run the bombardment battle round for X times
  for (int i = 0; i < roundRunTimes; i++) {
    // Behaviour: Check that no deaths have occured
    checkIfDead();

    // Behaviour: Check if the remaining damage value triggers a death
    checkRemainingDamage(inputP1Events, inputP2Events);

    // Behaviour: Make sure that no deaths have occured and that the attacking
    // entity is not retreating before proceeding
    if ((aDeathHasOccured == false) && (isRetreating != "1")) {
      // Behaviour: Check if player 1 is fighting a building
      if (p2BattleParticipant.armorClass[1] == true) { // Building p2
        isP1FightingBuilding = true;
      }
      else {
        isP1FightingBuilding = false;
      }

      // Behaviour: Calculate the damage against buildings if player 1 is
      // fighting a building
      if (isP1FightingBuilding == true) {
        p2BuildingDamage = ( ((p1BattleParticipant.standardDamage * p1BattleParticipant.entityQuantity) + roundAttackModifiersP1) - ((p2BattleParticipant.entityHealth) % roundDownMultiple));
        p2DamageDie = p2BuildingDamage / 10;
        remainingDamageP1 += calculateRemainingDamage(
          p2DamageDie,
          p1BattleParticipant.standardDamage,
          roundAttackModifiersP1,
          p2BattleParticipant.entityHealth,
          1);
      }
      // Behaviour: Calculate the damage against units if player 1 is fighting a
      // unit
      else {
        // Behaviour: The damage only applies if p1 has a Galley or Fire Ship
        if (
          (p1BattleParticipant.armorClass[21] == true)
          || (p1BattleParticipant.armorClass[21] == true)) {
          p2EntityDeaths = std::floor(
            ((p1BattleParticipant.standardDamage
              * p1BattleParticipant.entityQuantity)
             + roundAttackModifiersP1)
            / (p2BattleParticipant.entityHealth));
          remainingDamageP1 += calculateRemainingDamage(
            p2EntityDeaths,
            p1BattleParticipant.standardDamage,
            roundAttackModifiersP1,
            p2BattleParticipant.entityHealth,
            0);
        }
      }

      // Behaviour: Check if player 1 is fighting a building
      if (p1BattleParticipant.armorClass[1] == true) { // Building p1
        isP2FightingBuilding = true;
      }
      else {
        isP2FightingBuilding = false;
      }

      // Behaviour: Calculate the damage against buildings if player 2 is
      // fighting a building
      if (isP2FightingBuilding == true) {
        p1BuildingDamage = ( ((p2BattleParticipant.standardDamage * p2BattleParticipant.entityQuantity) + roundAttackModifiersP2) - ((p1BattleParticipant.entityHealth) % roundDownMultiple));
        p1DamageDie = p1BuildingDamage / 10;
        remainingDamageP2 += calculateRemainingDamage(
          p1DamageDie,
          p2BattleParticipant.standardDamage,
          roundAttackModifiersP2,
          p1BattleParticipant.entityHealth,
          1);
      }
      // Behaviour: Calculate the damage against units if player 2 is fighting a
      // unit
      else {
        // Behaviour: The damage only applies if p2 has a Galley or Fire Ship
        if (
          (p2BattleParticipant.armorClass[21] == true)
          || (p2BattleParticipant.armorClass[21] == true)) {
          p1EntityDeaths = std::floor(
            ((p2BattleParticipant.standardDamage
              * p2BattleParticipant.entityQuantity)
             + roundAttackModifiersP2)
            / (p1BattleParticipant.entityHealth));
          remainingDamageP2 += calculateRemainingDamage(
            p1EntityDeaths,
            p2BattleParticipant.standardDamage,
            roundAttackModifiersP2,
            p1BattleParticipant.entityHealth,
            0);
        }
      }

      bombardmentRoundActivated = true;
    }
    else {
      bombardmentRoundActivated = false;
    }

    // Behaviour: Check if the remaining damage value triggers a death
    checkRemainingDamage(inputP1Events, inputP2Events);

    // Behaviour: Check for deaths
    checkIfDead();

    // Behaviour: Make sure that no deaths have occured
    if (aDeathHasOccured == false) {
      // Behaviour: Apply the results for p1 if the activation conditions were
      // met
      if (bombardmentRoundActivated == true) {
        // Behaviour: Apply the results to buildings or non-buildings
        if (isP1FightingBuilding == true) {
          // Behaviour: Decrease the building HP
          if (p2BuildingDamage != 0) {
            // Behaviour: Set the point value to the buildings point
            p1PointsGained = p2BattleParticipant.pointValue;

            p2BattleParticipant.entityHealth -= p2BuildingDamage;

            // Behaviour: if the HP of the building is > 0 then do not keep the
            // set the point value
            if (p2BattleParticipant.entityHealth > 0) {
              p1PointsGained = 0;
            }
          }
        }
        else {
          // Behavior: Store the starting quantity
          p2StartingQuantity = p2BattleParticipant.entityQuantity;

          // Behaviour: Store how many points the entity is individually worth
          p1PointsGained
            = (p2BattleParticipant.pointValue / p2BattleParticipant.entityQuantity);

          // Behaviour: Divide the RD, standardDamage and pointValue by the
          // current quantity to get the values for a quantity of 1
          // getIndividualValues();

          // Behaviour: Now decrease the quantity
          p2BattleParticipant.entityQuantity -= p2EntityDeaths;

          // Behaviour: Now multiply the RD, standardDamage and pointValue by
          // the new quantity
          // getTotalValues();

          // Behavior: Store the ending quantity and cap the ending quantity at
          // 0
          if (p2BattleParticipant.entityQuantity < 0) {
            p2EndingQuantity = 0;
          }
          else {
            p2EndingQuantity = p2BattleParticipant.entityQuantity;
          }

          // Behaviour: Calculate the difference between the two quantities
          int p2QuantityDifference = (p2StartingQuantity - p2EndingQuantity);

          // Behaviour: Award points if deaths occured
          if (p2EntityDeaths >= 0) {
            // Behavior: Calculate the difference between the two quantities
            p1PointsGained *= p2QuantityDifference;
          }
          else {
            p1PointsGained = 0;
          }
        }
      }

      // Behaviour: Apply the results for p2 if bombardment damage occured
      if (bombardmentRoundActivated == true) {
        // Behaviour: Apply the results to buildings or non-buildings
        if (isP2FightingBuilding == true) {
          // Behaviour: Decrease the building HP
          if (p1BuildingDamage != 0) {
            // Behaviour: Set the point value to the buildings point
            p2PointsGained = p1BattleParticipant.pointValue;

            p1BattleParticipant.entityHealth -= p1BuildingDamage;

            // Behaviour: if the HP of the building is > 0 then do not keep the
            // set the point value
            if (p1BattleParticipant.entityHealth > 0) {
              p2PointsGained = 0;
            }
          }
        }
        else {
          // Behavior: Store the starting quantity
          p1StartingQuantity = p1BattleParticipant.entityQuantity;

          // Behaviour: Store how many points the entity is individually worth
          p2PointsGained
            = (p1BattleParticipant.pointValue / p1BattleParticipant.entityQuantity);

          // Behaviour: Divide the RD, standardDamage and pointValue by the
          // current quantity to get the values for a quantity of 1
          // getIndividualValues();

          // Behaviour: Now decrease the quantity
          p1BattleParticipant.entityQuantity -= p1EntityDeaths;

          // Behaviour: Now multiply the RD, standardDamage and pointValue by
          // the new quantity
          // getTotalValues();

          // Behavior: Store the ending quantity and cap the ending quantity at
          // 0
          if (p1BattleParticipant.entityQuantity < 0) {
            p1EndingQuantity = 0;
          }
          else {
            p1EndingQuantity = p1BattleParticipant.entityQuantity;
          }

          // Behaviour: Calculate the difference between the two quantities
          int p1QuantityDifference = (p1StartingQuantity - p1EndingQuantity);

          // Behaviour: Award points if deaths occured
          if (p1EntityDeaths >= 0) {
            // Behavior: Calculate the difference between the two quantities
            p2PointsGained *= p1QuantityDifference;
          }
          else {
            p2PointsGained = 0;
          }
        }
      }
    }

    // Behaviour: Display the outcome of the bombardment combat round only if
    // changes occured
    if (bombardmentRoundActivated == true) {
      std::string outputString
        = "Caught from the Crow's Nest Phase) Bombardment round "
          + std::to_string(i + 1) + " calculations...";
      outputEntityInformation(outputString);

      // Behaviour: Display how many damage die to place if appropriate
      if (isP1FightingBuilding == true) {
        if ((p2BattleParticipant.entityHealth > 0) && (p2DamageDie > 0)) {
          std::cout << ">> Place " << p2DamageDie << " damage die onto "
                    << player2Name << "'s " << p2BattleParticipant.entityName
                    << "<br>";
        }
      }
      else if (isP2FightingBuilding == true) {
        if ((p1BattleParticipant.entityHealth > 0) && (p1DamageDie > 0)) {
          std::cout << ">> Place " << p1DamageDie << " damage die onto "
                    << player1Name << "'s " << p1BattleParticipant.entityName
                    << "<br>";
        }
      }

      // Behaviour: Display how many points were added if appropriate
      if (p1PointsGained != 0) {
        std::cout << ">> " << player1Name << " gets " << p1PointsGained
                  << " points"
                  << "<br>";
      }
      if (p2PointsGained != 0) {
        std::cout << ">> " << player2Name << " gets " << p2PointsGained
                  << " points"
                  << "<br>";
      }

      std::cout << "<br>";

      // Behaviour: Check if a death has occured
      checkIfDead();

      // Behaviour: Check if the attacking entity is retreating if this is not
      // the last round
      if (aDeathHasOccured == false) {
        if (i != roundRunTimes - 1) {
          checkIfRetreating("Bombardment");
        }
      }
    }
    else {
      // Behaviour: Stop from this showing up if a unit dies in the monk round
      // of combat
      if ((aDeathHasOccured == false) && (isRetreating != "1")) {
        std::cout
          << "Skipping Caught from the Crow's Nest Phase) Bombardment round "
               + std::to_string(i + 1) + " calculations..."
          << "<br>";
      }
    }
  }
}

// Function: Calculate the outcome of a standard round of battle
void standardRounds::roundOutcome(
  int          roundRunTimes,
  int*         inputP1Events,
  int*         inputP2Events,
  int*         inputP1Technologies,
  int*         inputP2Technologies,
  ActivePlayer activePlayer)
{
  /* Shared stuff */
  const int roundDownMultiple = 10;

  /* P1 stuff */
  bool p1EntityActivated = false;
  int  p1EntityDeaths = 0; //  Declare the number of units killed in this round
  int  p1BuildingDamage
    = 0; //  Declare the amount of damage dealt to buildings in this round
  bool isP1FightingBuilding
    = false; //  Get whether the entity is fighting a building
  // Behaviour: Check if player 1 is fighting a building
  if (p2BattleParticipant.armorClass[1] == true) { // Building p2
    isP1FightingBuilding = true;
  }
  else {
    isP1FightingBuilding = false;
  }

  int p1DamageDie
    = 0; //  Declare the amount of damage die to place onto buildings
  float p1PointsGained = 0; //  Declare the amount of points awarded //  Track
                            //  if changes occured to the quantity
  int p1StartingQuantity = 0, p1EndingQuantity = 0;

  /* P2 stuff */
  bool p2EntityActivated = false;
  int  p2EntityDeaths = 0; //  Declare the number of units killed in this round
  int  p2BuildingDamage
    = 0; //  Declare the amount of damage dealt to buildings in this round
  bool isP2FightingBuilding
    = false; //  Get whether the  entity is fighting a building
  // Behaviour: Check if player 2 is fighting a building
  if (p1BattleParticipant.armorClass[1] == true) { // Building p1
    isP2FightingBuilding = true;
  }
  else {
    isP2FightingBuilding = false;
  }

  int p2DamageDie
    = 0; //  Declare the amount of damage die to place onto buildings
  float p2PointsGained     = 0; //  Declare the amount of points awarded
  int   p2StartingQuantity = 0,
      p2EndingQuantity     = 0; //  Track if changes occured to the quantity

  /* P1 event handling */
  // Behaviour: Give the person who played/did not play the event more attack
  // based on a d6 die roll
  if (inputP1Events[11] == 1) {
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
      p2BattleParticipant.standardDamage *= 2;
      break;
    case 2:
      p2BattleParticipant.standardDamage
        += (2 * p2BattleParticipant.entityQuantity);
      break;
    case 3:
      // No effect
      break;
    case 4:
      p1BattleParticipant.standardDamage
        += (2 * p2BattleParticipant.entityQuantity);
      break;
    case 5:
      p1BattleParticipant.standardDamage
        += (4 * p2BattleParticipant.entityQuantity);
      break;
    case 6:
      p1BattleParticipant.standardDamage *= 2;
      break;
    }
  }

  // Behaviour: Give the person who played the card more attack
  if (inputP1Events[35] == 1) {
    // [35] The Jester Is Dead, Let's Get Em (Celt) = Sacrifice 1 of your
    // villagers. +4 standardDamage to all defending units this turn
    p1BattleParticipant.standardDamage += 4;
  }

  // Behaviour: Infantry double standard attack vs cavalry
  if (inputP1Events[20] == 1) {
    // [20] Muddy Battlefield - Target infantry gets double standardAttack
    // versus Cavalry this turn
    if (
      (p1BattleParticipant.armorClass[8])
      && (p2BattleParticipant.armorClass[4])) {
      p1BattleParticipant.standardDamage *= 2;
    }
  }

  // Behaviour: Decrease the standardDamage by two units if event 9 is played
  if (inputP1Events[9] == 1) {
    // [9] First Battle Jitter - Two tokens on target unit have 0 standardDamage
    // for this battle. Target unit must have five tokens on it
    if (p2BattleParticipant.entityQuantity >= 5) {
      int getIndividualAttackP2
        = (p2BattleParticipant.standardDamage / p2BattleParticipant.entityQuantity);
      p2BattleParticipant.standardDamage -= getIndividualAttackP2;
      p2BattleParticipant.standardDamage -= getIndividualAttackP2;
    }
  }

  /* P2 event handling */
  if (inputP2Events[11] == 1) {
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
      p1BattleParticipant.standardDamage *= 2;
      break;
    case 2:
      p1BattleParticipant.standardDamage
        += (2 * p1BattleParticipant.entityQuantity);
      break;
    case 3:
      // No effect
      break;
    case 4:
      p2BattleParticipant.standardDamage
        += (2 * p2BattleParticipant.entityQuantity);
      break;
    case 5:
      p2BattleParticipant.standardDamage
        += (4 * p2BattleParticipant.entityQuantity);
      break;
    case 6:
      p2BattleParticipant.standardDamage *= 2;
      break;
    }
  }

  if (inputP2Events[35] == 1) {
    // [35] The Jester Is Dead, Let's Get Em (Celt) = Sacrifice 1 of your
    // villagers. +4 standardDamage to all defending units this turn
    p2BattleParticipant.standardDamage += 4;
  }

  if (inputP2Events[20] == 1) {
    // [20] Muddy Battlefield - Target infantry gets double standardAttack
    // versus Cavalry this turn
    if (
      (p2BattleParticipant.armorClass[8])
      && (p1BattleParticipant.armorClass[4])) {
      p2BattleParticipant.standardDamage *= 2;
    }
  }

  if (inputP2Events[9] == 1) {
    // [9] First Battle Jitter - Two tokens on target unit have 0 standardDamage
    // for this battle. Target unit must have five tokens on it
    if (p1BattleParticipant.entityQuantity >= 5) {
      int getIndividualAttackP1
        = (p2BattleParticipant.standardDamage / p2BattleParticipant.entityQuantity);
      p1BattleParticipant.standardDamage -= getIndividualAttackP1;
      p1BattleParticipant.standardDamage -= getIndividualAttackP1;
    }
  }

  /* Intertwined events */
  // Behaviour: Multiply the standardDamage by a random number between 1-6 if a
  // Fire Ship
  d6DieRoll = generateD6DieInput();

  // Behaviour: Set the result to 1 if player 2 has Soak_The_Timbers
  if (inputP2Events[30] == 1) {
    // [30] Soak The Timbers - All enemy Fire Ships get an automatic result of 1
    // for their die roll when calculating damage this turn
    d6DieRoll = 1;
  }

  if (p1BattleParticipant.armorClass[22] == true) { // Fire Ship or Fast Fire
                                                    // Ship
    p1BattleParticipant.standardDamage *= d6DieRoll;
  }

  d6DieRoll = generateD6DieInput();

  // Behaviour: Set the result to 1 if player 1 has Soak_The_Timbers
  if (inputP1Events[30] == 1) {
    // [30] Soak The Timbers - All enemy Fire Ships get an automatic result of 1
    // for their die roll when calculating damage this turn
    d6DieRoll = 1;
  }

  if (p2BattleParticipant.armorClass[22] == true) { // Fire Ship or Fast Fire
                                                    // Ship
    p2BattleParticipant.standardDamage *= d6DieRoll;
  }

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
      checkRemainingDamage(inputP1Events, inputP2Events);

      // Behaviour: Check that no deaths have occured
      checkIfDead();

      // Give the player who played the card a weakened defender
      // [24] Black Knight: “Play this card when you are the attacking Cavalry
      // unit. Two tokens on the defending unit have 0 AP for the first round of
      // normal combat
      if (inputP1Events[24] == 1) {
        int getHowMuchAttackIndividualUnitsDo;
        int getHowMuchAttackTheGroupDoesWithTwoLazyUnits;

        // If there's cavalry in play
        if (p1BattleParticipant.armorClass[4]) {
          // If it's round 1
          if (numberOfTimesToRunTheStandardRound == 0) {
            // Do not re-calculate this value again in the second round or
            // you're going to get a result that doesn't reflect the individual
            // attack of the unit (it will be divided too much)
            getHowMuchAttackIndividualUnitsDo
              = p2BattleParticipant.standardDamage;

            if (p2BattleParticipant.entityQuantity > 2) {
              getHowMuchAttackTheGroupDoesWithTwoLazyUnits
                = (p2BattleParticipant.entityQuantity - 2)
                  * getHowMuchAttackIndividualUnitsDo;
              p2BattleParticipant.standardDamage
                = getHowMuchAttackTheGroupDoesWithTwoLazyUnits;
            }
            else {
              getHowMuchAttackTheGroupDoesWithTwoLazyUnits = 0;
              p2BattleParticipant.standardDamage
                = getHowMuchAttackTheGroupDoesWithTwoLazyUnits;
            }
          }
          // If it's round 2
          else if (numberOfTimesToRunTheStandardRound == 1) {
            p2BattleParticipant.standardDamage
              = getHowMuchAttackIndividualUnitsDo
                * p2BattleParticipant.entityQuantity;
          }
        }
      }

      if (inputP2Events[24] == 1) {
        int getHowMuchAttackIndividualUnitsDo2;
        int getHowMuchAttackTheGroupDoesWithTwoLazyUnits2;

        // If there's cavalry in play
        if (p2BattleParticipant.armorClass[4]) {
          // If it's round 1
          if (numberOfTimesToRunTheStandardRound == 0) {
            // Do not re-calculate this value again in the second round or
            // you're going to get a result that doesn't reflect the individual
            // attack of the unit (it will be divided too much)
            getHowMuchAttackIndividualUnitsDo2
              = p1BattleParticipant.standardDamage;

            if (p1BattleParticipant.entityQuantity > 2) {
              getHowMuchAttackTheGroupDoesWithTwoLazyUnits2
                = (p1BattleParticipant.entityQuantity - 2)
                  * getHowMuchAttackIndividualUnitsDo2;
              p1BattleParticipant.standardDamage
                = getHowMuchAttackTheGroupDoesWithTwoLazyUnits2;
            }
            else {
              getHowMuchAttackTheGroupDoesWithTwoLazyUnits2 = 0;
              p1BattleParticipant.standardDamage
                = getHowMuchAttackTheGroupDoesWithTwoLazyUnits2;
            }
          }
          // If it's round 2
          else if (numberOfTimesToRunTheStandardRound == 1) {
            p1BattleParticipant.standardDamage
              = getHowMuchAttackIndividualUnitsDo2
                * p1BattleParticipant.entityQuantity;
          }
        }
      }

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player1) != ActivePlayer::None) {
        // Behaviour: Calculate the damage against buildings if player 1 is
        // fighting a building
        if (isP1FightingBuilding == true) {
          p2BuildingDamage
            = ((p1BattleParticipant.standardDamage * p1BattleParticipant.entityQuantity) - ((p2BattleParticipant.entityHealth) % roundDownMultiple));
          p2DamageDie = p2BuildingDamage / 10;
          remainingDamageP1 += calculateRemainingDamage(
            p2DamageDie,
            p1BattleParticipant.standardDamage,
            roundAttackModifiersP1,
            p2BattleParticipant.entityHealth,
            1);

          p1EntityActivated = true;
        }
        // Behaviour: Calculate the damage against units if player 1 is fighting
        // a unit
        else if (isP1FightingBuilding == false) {
          // Behaviour: Check if player 1 has an area effect unit and if so then
          // apply area effect damage
          if (p1BattleParticipant.dealsAreaEffectDamage == true) {
            int p1AreaEffectUnitDamage
              = ((p1BattleParticipant.standardDamage) + roundAttackModifiersP1)
                / p1BattleParticipant.entityQuantity;
            p2EntityDeaths = std::floor(
              (((p2BattleParticipant.entityQuantity) * (p1AreaEffectUnitDamage))
               / p2BattleParticipant.entityHealth));
            remainingDamageP1 += calculateRemainingDamage(
              p2EntityDeaths,
              p1BattleParticipant.standardDamage,
              roundAttackModifiersP1,
              p2BattleParticipant.entityHealth,
              0);

            p1EntityActivated = true;
          }
          else {
            p2EntityDeaths = std::floor(
              ((p1BattleParticipant.standardDamage
                * p1BattleParticipant.entityQuantity)
               + roundAttackModifiersP1)
              / (p2BattleParticipant.entityHealth));
            remainingDamageP1 += calculateRemainingDamage(
              p2EntityDeaths,
              p1BattleParticipant.standardDamage,
              roundAttackModifiersP1,
              p2BattleParticipant.entityHealth,
              0);

            p1EntityActivated = true;
          }
        }

        // Behaviour: Clear the results if the entity only attacks once (in the
        // first round of combat) and we are not in the 1st round
        if (
          (p1BattleParticipant.onlyAttacksOnce == true)
          && (numberOfTimesToRunTheStandardRound + 1 != 1)) {
          p2BuildingDamage = 0;
          p2DamageDie      = 0;
          p2EntityDeaths   = 0;
        }

        // Behaviour: Clear the results if the entity only attacks in the second
        // round of combat and we are not in the 2nd round
        if (
          (p1BattleParticipant.onlyAttacksInTheSecondRoundOfStandardCombat
           == true)
          && (numberOfTimesToRunTheStandardRound + 1 != 2)) {
          p2BuildingDamage = 0;
          p2DamageDie      = 0;
          p2EntityDeaths   = 0;
        }

        // Behaviour: Clear the results if the entity is an infantry unit for
        // the attacking player and we are in the first round of combat
        if (inputP1Events[37] == 1) {
          // [37] While They're Sleeping - Target attacking Infantry unit takes
          // no damage for the first round of combat. The ranged round does not
          // count as the first round of combat
          if (p1BattleParticipant.armorClass[8] == true) {
            p1EntityDeaths = 0;
          }
        }

        // Behaviour: Apply the results for p1 if the activation conditions were
        // met
        if (p1EntityActivated == true) {
          // Behaviour: Apply the results to buildings or non-buildings
          if (isP1FightingBuilding == true) {
            // Behaviour: Decrease the building HP
            if (p2BuildingDamage != 0) {
              // Behaviour: Set the point value to the buildings point
              p1PointsGained = p2BattleParticipant.pointValue;

              p2BattleParticipant.entityHealth -= p2BuildingDamage;

              // Behaviour: if the HP of the building is > 0 then do not keep
              // the set the point value
              if (p2BattleParticipant.entityHealth > 0) {
                p1PointsGained = 0;
              }

              // Update the quantity of the building
              if (p2BattleParticipant.entityHealth < 0) {
                p2BattleParticipant.entityQuantity -= 1;
              }
            }
          }
          else {
            // Behavior: Store the starting quantity
            p2StartingQuantity = p2BattleParticipant.entityQuantity;

            // Behaviour: Store how many points the entity is individually worth
            p1PointsGained = p2BattleParticipant.pointValue;

            // Behaviour: Now decrease the quantity
            p2BattleParticipant.entityQuantity -= p2EntityDeaths;
            // Debug: Why is the point value of p2BattleParticipant 2

            // Behavior: Store the ending quantity and cap the ending quantity
            // at
            // 0
            if (p2BattleParticipant.entityQuantity < 0) {
              p2EndingQuantity = 0;
            }
            else {
              p2EndingQuantity = p2BattleParticipant.entityQuantity;
            }

            // Behaviour: Calculate the difference between the two quantities
            int p2QuantityDifference = (p2StartingQuantity - p2EndingQuantity);

            // Behaviour: Award points if deaths occured
            if (p2EntityDeaths >= 0) {
              // Behavior: Calculate the difference between the two quantities
              p1PointsGained *= p2QuantityDifference;
            }
            else {
              p1PointsGained = 0;
            }
          }
        }

        // Apply the effects of certain entities that automatically die in the
        // second round of combat Make sure that the standard round got
        // activated to avoid an arithmetic exception
        if (p1EntityActivated == true) {
          if (numberOfTimesToRunTheStandardRound + 1 == 2) {
            if (p1BattleParticipant.isKamikaze == true) {
              p2PointsGained
                = (p1BattleParticipant.pointValue / p1BattleParticipant.entityQuantity);
              p1BattleParticipant.entityQuantity = 0;
            }
          }
        }

      } // end p1 calc

      // Determine if we'll be running the calculations for P1 and P2
      // simultaneously
      if ((activePlayer & ActivePlayer::Player2) != ActivePlayer::None) {
        // Behaviour: Calculate the damage against buildings if player 2 is
        // fighting a building
        if (isP2FightingBuilding == true) {
          p1BuildingDamage = ( ((p2BattleParticipant.standardDamage * p2BattleParticipant.entityQuantity) + roundAttackModifiersP2) - ((p1BattleParticipant.entityHealth) % roundDownMultiple));
          p1DamageDie = p1BuildingDamage / 10;
          remainingDamageP2 += calculateRemainingDamage(
            p1DamageDie,
            p2BattleParticipant.standardDamage,
            roundAttackModifiersP2,
            p1BattleParticipant.entityHealth,
            1);

          p2EntityActivated = true;
        }
        // Behaviour: Calculate the damage against units if player 2 is fighting
        // a unit
        else if (isP2FightingBuilding == false) {
          // Behaviour: Check if player 2 has an area effect unit and if so then
          // apply area effect damage
          if (p2BattleParticipant.dealsAreaEffectDamage == true) {
            int p2AreaEffectUnitDamage
              = ((p2BattleParticipant.standardDamage) + roundAttackModifiersP2)
                / p2BattleParticipant.entityQuantity;
            p1EntityDeaths = std::floor(
              (((p1BattleParticipant.entityQuantity) * (p2AreaEffectUnitDamage))
               / p1BattleParticipant.entityHealth));
            remainingDamageP2 += calculateRemainingDamage(
              p1EntityDeaths,
              p2BattleParticipant.standardDamage,
              roundAttackModifiersP2,
              p1BattleParticipant.entityHealth,
              0);

            p2EntityActivated = true;
          }
          else {
            p1EntityDeaths = std::floor(
              ((p2BattleParticipant.standardDamage
                * p2BattleParticipant.entityQuantity)
               + roundAttackModifiersP2)
              / (p1BattleParticipant.entityHealth));
            remainingDamageP2 += calculateRemainingDamage(
              p1EntityDeaths,
              p2BattleParticipant.standardDamage,
              roundAttackModifiersP2,
              p1BattleParticipant.entityHealth,
              0);

            p2EntityActivated = true;
          }
        }

        // Behaviour: Clear the results if the entity only attacks once (in the
        // first round of combat) and we are not in the 1st round

        if (
          (p2BattleParticipant.onlyAttacksOnce == true)
          && (numberOfTimesToRunTheStandardRound + 1 != 1)) {
          p1BuildingDamage = 0;
          p1DamageDie      = 0;
          p1EntityDeaths   = 0;
        }

        // Behaviour: Clear the results if the entity only attacks in the second
        // round of combat and we are not in the 2nd round

        if (
          (p2BattleParticipant.onlyAttacksInTheSecondRoundOfStandardCombat
           == true)
          && (numberOfTimesToRunTheStandardRound + 1 != 2)) {
          p1BuildingDamage = 0;
          p1DamageDie      = 0;
          p1EntityDeaths   = 0;
        }

        if (inputP2Events[37] == 1) {
          // [37] While They're Sleeping - Target attacking Infantry unit takes
          // no damage for the first round of combat. The ranged round does not
          // count as the first round of combat
          if (p2BattleParticipant.armorClass[8] == true) {
            p2EntityDeaths = 0;
          }
        }

        // Behaviour: Apply the results for p2 if standard damage occured
        if (p2EntityActivated == true) {
          // Behaviour: Apply the results to buildings or non-buildings
          if (isP2FightingBuilding == true) {
            // Behaviour: Decrease the building HP
            if (p1BuildingDamage != 0) {
              // Behaviour: Set the point value to the buildings point
              p2PointsGained = p1BattleParticipant.pointValue;

              p1BattleParticipant.entityHealth -= p1BuildingDamage;

              // Behaviour: if the HP of the building is > 0 then do not keep
              // the set the point value
              if (p1BattleParticipant.entityHealth > 0) {
                p2PointsGained = 0;
              }

              // Update the quantity of the building
              if (p1BattleParticipant.entityHealth < 0) {
                p1BattleParticipant.entityQuantity -= 1;
              }
            }
          }
          else {
            // Behavior: Store the starting quantity
            p1StartingQuantity = p1BattleParticipant.entityQuantity;

            // Behaviour: Store how many points the entity is individually worth
            p2PointsGained = p1BattleParticipant.pointValue;

            // Behaviour: Now decrease the quantity
            p1BattleParticipant.entityQuantity -= p1EntityDeaths;

            // Behavior: Store the ending quantity and cap the ending quantity
            // at
            // 0
            if (p1BattleParticipant.entityQuantity < 0) {
              p1EndingQuantity = 0;
            }
            else {
              p1EndingQuantity = p1BattleParticipant.entityQuantity;
            }

            // Behaviour: Calculate the difference between the two quantities
            int p1QuantityDifference = (p1StartingQuantity - p1EndingQuantity);

            // Behaviour: Award points if deaths occured
            if (p1EntityDeaths >= 0) {
              // Behavior: Calculate the difference between the two quantities
              p2PointsGained *= p1QuantityDifference;
            }
            else {
              p2PointsGained = 0;
            }
          }
        }

        // Apply the effects of certain entities that automatically die in the
        // second round of combat Make sure that the standard round got
        // activated to avoid an arithmetic exception
        if (p2EntityActivated == true) {
          if (p2BattleParticipant.isKamikaze == true) {
            if (numberOfTimesToRunTheStandardRound + 1 == 2) {
              p1PointsGained
                = (p2BattleParticipant.pointValue / p2BattleParticipant.entityQuantity);
              p2BattleParticipant.entityQuantity = 0;
            }
          }
        }

      } // end p2 calc

      // Behaviour: Display the outcome of the standard combat round only if
      // changes occured

      // Behaviour: Display the outcome of the standard combat round only if
      // changes occured
      if ((p1EntityActivated == true) || (p2EntityActivated == true)) {
        std::string outputString
          = "Standard round "
            + std::to_string(numberOfTimesToRunTheStandardRound + 1)
            + " calculations...";
        outputEntityInformation(outputString);

        // Behaviour: Display how many damage die to place if appropriate
        if (isP1FightingBuilding == true) {
          if ((p2BattleParticipant.entityHealth > 0) && (p2DamageDie > 0)) {
            std::cout << ">> Place " << p2DamageDie << " damage die onto "
                      << player2Name << "'s " << p2BattleParticipant.entityName
                      << "<br>";
          }
        }
        else if (isP2FightingBuilding == true) {
          if ((p1BattleParticipant.entityHealth > 0) && (p1DamageDie > 0)) {
            std::cout << ">> Place " << p1DamageDie << " damage die onto "
                      << player1Name << "'s " << p1BattleParticipant.entityName
                      << "<br>";
          }
        }

        // Behaviour: Display how many points were added if appropriate
        if (p1PointsGained != 0) {
          std::cout << ">> " << player1Name << " gets " << p1PointsGained
                    << " points"
                    << "<br>";
        }
        if (p2PointsGained != 0) {
          std::cout << ">> " << player2Name << " gets " << p2PointsGained
                    << " points"
                    << "<br>";
        }

        std::cout << "<br>";

        // Behaviour: Deal with event 34
        if (inputP1Events[34] == 1) {
          // [34] The Hammer's Cavalry (Franks) - Play when one of your Cavalry
          // units is reduced to 1 token. Add two tokens to the unit at no cost
          if (p1BattleParticipant.armorClass[4] == true) { // Cavalry
            if (p1BattleParticipant.entityQuantity == 1) {
              // getIndividualValues();
              p1BattleParticipant.entityQuantity += 2;
              // getTotalValues();
            }
          }
        }

        if (inputP2Events[34] == 1) {
          // [34] The Hammer's Cavalry (Franks) - Play when one of your Cavalry
          // units is reduced to 1 token. Add two tokens to the unit at no cost
          if (p2BattleParticipant.armorClass[4] == true) { // Cavalry
            if (p2BattleParticipant.entityQuantity == 1) {
              // getIndividualValues();
              p2BattleParticipant.entityQuantity += 2;
              // getTotalValues();
            }
          }
        }

        // Behaviour: Check if a death has occured
        checkIfDead();

        // Behaviour: Check if the attacking entity is retreating if this is not
        // the last round
        if (aDeathHasOccured == false) {
          if (numberOfTimesToRunTheStandardRound != roundRunTimes - 1) {
            if ((inputP1Events[27] == 1) || (inputP2Events[27] == 1)) {
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
  int*         inputP1Events,
  int*         inputP2Events,
  int*         inputP1Technologies,
  int*         inputP2Technologies,
  ActivePlayer activePlayer)
{
  constexpr std::size_t                  playerCount{2};
  const std::array<Entity*, playerCount> battleParticipants{
    &p1BattleParticipant, &p2BattleParticipant};
  const std::array<Entity*, playerCount> monks{
    &p1BattleAssistant, &p2BattleAssistant};

  float p2PointsGained = 0, p1PointsGained = 0;

  for (int round{0}; round < roundRunTimes; ++round) {
    for (std::size_t player{0}; player < playerCount; ++player) {
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
  }

  checkIfDead();
  outputEntityInformation("");

  if (
    p2BattleAssistant.entityQuantity
    < p2BattleAssistant.initialEntityQuantity) {
    int numberOfMonksP2Lost = p2BattleAssistant.initialEntityQuantity
                              - p2BattleAssistant.entityQuantity;
    p1PointsGained = (p2BattleAssistant.pointValue
                      / p2BattleAssistant.initialEntityQuantity)
                     * numberOfMonksP2Lost;

    std::cout << ">> " << player1Name << " gets " << p1PointsGained << " points"
              << "<br>";
  }

  if (
    p1BattleAssistant.entityQuantity
    < p1BattleAssistant.initialEntityQuantity) {
    int numberOfMonksP1Lost = p1BattleAssistant.initialEntityQuantity
                              - p1BattleAssistant.entityQuantity;
    p2PointsGained = (p1BattleAssistant.pointValue
                      / p1BattleAssistant.initialEntityQuantity)
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

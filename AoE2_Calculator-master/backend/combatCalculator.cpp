/** The libaries **/
#include "combatCalculator.h" // Using: calculator class
#include "dialog_input.h"
#include "entity.h" // Using: entity class
#include "soundEffects.h"
#include <algorithm>
#include <array>
#include <cmath>    // Using: floor
#include <cstdlib>  // Using: exit(EXIT_FAILURE), srand(), rand()
#include <iostream> // Using: cin, cout
#include <random>
#include <stdlib.h> // Using: atoi
#include <string>   // Using: string

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
  // Behaviour: Declare the initial values
  startingBattleParticipantQuantityP1     = 0;
  startingBattleParticipantQuantityP2     = 0;

  startingAssistantQuantityP1     = 0;
  startingAssistantQuantityP2     = 0;


  d6DieRoll              = 0;
  aDeathHasOccured       = false;
  isRetreating           = "";
  roundAttackModifiersP1 = 0;
  roundAttackModifiersP2 = 0;
  remainingDamageP1      = 0;
  remainingDamageP2      = 0;
}

// Function: The constructor/deconstructor
combatCalculator::combatCalculator(
  CombatCalculatorState*     state,
  CombatCalculatorCallbacks* callbacks)
  : m_state{state}
  , m_callbacks{callbacks}
  , startingBattleParticipantQuantityP1{m_state->startingBattleParticipantQuantityP1}
  , startingBattleParticipantQuantityP2{m_state->startingBattleParticipantQuantityP2}
  , startingAssistantQuantityP1{m_state->startingAssistantQuantityP1}
  , startingAssistantQuantityP2{m_state->startingAssistantQuantityP2}
  , d6DieRoll{m_state->d6DieRoll}
  , aDeathHasOccured{m_state->aDeathHasOccured}
  , isRetreating{m_state->isRetreating}
  , player1Name{m_state->player1Name}
  , player2Name{m_state->player2Name}
  , p1BattleParticipant{m_state->p1BattleParticipant}
  , p2BattleParticipant{m_state->p2BattleParticipant}
  , p1AssistingMonkParticipant{m_state->p1AssistingMonkParticipant}
  , p2AssistingMonkParticipant{m_state->p2AssistingMonkParticipant}
  , roundAttackModifiersP1{m_state->roundAttackModifiersP1}
  , roundAttackModifiersP2{m_state->roundAttackModifiersP2}
  , remainingDamageP1{m_state->remainingDamageP1}
  , remainingDamageP2{m_state->remainingDamageP2}
{
}

combatCalculator::~combatCalculator()
{
}

// Function: Set the player names
void combatCalculator::setPlayerNames(
  std::string& inputP1Name,
  std::string& inputP2Name)
{
  player1Name = inputP1Name;
  player2Name = inputP2Name;
}

// Function: Set the battle participants
void combatCalculator::setCombatParticipants(
  Entity& inputP1BattleParticipant,
  Entity& inputP2BattleParticipant,
  Entity& inputP1AssistingMonkBattleParticipant,
  Entity& inputP2AssistingMonkBattleParticipant,
  int&    inputRoundAttackModifiersP1,
  int&    inputRoundAttackModifiersP2)
{
  // Player 1
  p1BattleParticipant        = inputP1BattleParticipant;
  p1AssistingMonkParticipant = inputP1AssistingMonkBattleParticipant;
  roundAttackModifiersP1     = inputRoundAttackModifiersP1;

  // Player 2
  p2BattleParticipant        = inputP2BattleParticipant;
  p2AssistingMonkParticipant = inputP2AssistingMonkBattleParticipant;

  roundAttackModifiersP2 = inputRoundAttackModifiersP2;
}

// Function: The constructor with the pointer
void combatCalculator::setAdditionalValues(
  float& inputRemainingDamageP1,
  float& inputRemainingDamageP2)
{
  // Player 1
  remainingDamageP1 = inputRemainingDamageP1;

  // Player 2
  remainingDamageP2 = inputRemainingDamageP2;
}

// Function: Return the remaining damage
float combatCalculator::returnRemaningDamage(int inputPlayerNumber)
{
  if (inputPlayerNumber == 1) {
    return remainingDamageP1;
  }
  else if (inputPlayerNumber == 2) {
    return remainingDamageP2;
  }
  else {
    std::cout << "Error: Input player number not recognized"
              << "<br>";
    std::terminate();
  }
}

int combatCalculator::generateD6DieInput()
{
  // Int: Declare a variable to store the generated integer value
  int d6Value = 0;

  // Reference: https://ideone.com/41fclS
  // Behaviour: Set a random seed
  std::random_device rd;

  // Behaviour Initialize the Mersenne Twister pseudo-random number generator
  std::mt19937 gen(rd());

  // Behaviour: Generate pseudo-random numbers. The numbers are uniformly
  // distributed in range (1, 6)
  std::uniform_int_distribution<> dis(1, 6);

  // Behaviour: Generate 100 pseudo-random numbers
  int randomX = 0;
  for (int i = 0; i < 100; i++) {
    randomX = dis(gen);

    // Behaviour: Make sure that the numbers are actually changing each compile
    // (unlike srand(time(0));) std::cout << "\nRandom X = " << randomX;
  }

  // Behaviour: Go with the last generated number
  d6Value = randomX;

  return d6Value;
}

// Function: Check the randomness of the d6 dice roller
void combatCalculator::checkD6DiceSimulator()
{
  // Behaviour: Check the randomness of the automatic d6 dice roller
  for (int i = 0; i < 50; i++) {
    d6DieRoll = generateD6DieInput();
    std::cout << "d6 roll " << i << ": " << d6DieRoll << "<br>";
  }
}

// Function: Return the modified battle participants based on the input player
// number
Entity combatCalculator::returnModifiedBattleParticipants(int inputPlayerNumber)
{
  // Behaviour: Return the entities with the combat results applied
  if (inputPlayerNumber == 1) {
    return p1BattleParticipant;
  }
  else if (inputPlayerNumber == 2) {
    return p2BattleParticipant;
  }
  else {
    std::cout << "Error: Input player number not recognized"
              << "<br>";
    std::terminate();
  }
}


void combatCalculator::checkIfItCanBeHealed()
{
  if(p1BattleParticipant.entityQuantity < startingBattleParticipantQuantityP1) {
    if(p1BattleParticipant.healsAvailable > 0){
      p1BattleParticipant.entityQuantity++;
      p1BattleParticipant.healsAvailable--;
      std::cout << player1Name << " has used one of their heals on their " << p1BattleParticipant.entityName << "<br>";
    }
  }
  if(p2BattleParticipant.entityQuantity < startingBattleParticipantQuantityP2){
    if(p2BattleParticipant.healsAvailable > 0){
      p2BattleParticipant.entityQuantity++;
      p2BattleParticipant.healsAvailable--;
      std::cout << player2Name << " has used one of their heals on their " << p2BattleParticipant.entityName << "<br>";
    }
  }

  if(p1AssistingMonkParticipant.entityQuantity < startingAssistantQuantityP1) {
    if(p1AssistingMonkParticipant.healsAvailable > 0){
      p1AssistingMonkParticipant.entityQuantity++;
      p1AssistingMonkParticipant.healsAvailable--;
      std::cout << player1Name << " has used one of their heals on their " << p1AssistingMonkParticipant.entityName << "<br>";
    }
  }
  if(p2AssistingMonkParticipant.entityQuantity < startingAssistantQuantityP2){
    if(p2AssistingMonkParticipant.healsAvailable > 0){
      p2AssistingMonkParticipant.entityQuantity++;
      p2AssistingMonkParticipant.healsAvailable--;
      std::cout << player2Name << " has used one of their heals on their " << p2AssistingMonkParticipant.entityName << "<br>";
    }
  }

}











// Function: Check if any of the entities have died before proceeding to the next round of combat
void combatCalculator::checkIfDead()
{
  // Check if what would otherwise die, can be healed
  checkIfItCanBeHealed();

  // Check if the rule about monks resurrection rule applies
  // "If a military unit loses its last token but still has monks attached,
  // these monks keep the unit alive and in play"

  if (p1BattleParticipant.entityQuantity <= 0) {
    if (p1AssistingMonkParticipant.entityQuantity > 0) {
      std::cout << player1Name << " has resurrected one of their units with their monks<br>";
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
  else if (p2BattleParticipant.entityQuantity <= 0) {
    if (p2AssistingMonkParticipant.entityQuantity > 0) {
      std::cout << player2Name << " has resurrected one of their units with their monks<br>";
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
void combatCalculator::checkIfRetreating(std::string roundType)
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
      if ((roundType == "Archer")) {
        std::cout << "Do you want to hit and run?<br>";
      }
      else {
        std::cout << "Do you want to retreat?<br>";
      }

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
    (p1AssistingMonkParticipant.entityQuantity >= 0)
    && (p1AssistingMonkParticipant.initialEntityQuantity != 0)) {
    std::cout << "(Assisting) ";
    p1AssistingMonkParticipant.outputEntity(player1Name);
  }
  p2BattleParticipant.outputEntity(player2Name);

  // Do not show monks as being dead if started with 0
  if (
    (p2AssistingMonkParticipant.entityQuantity >= 0)
    && (p2AssistingMonkParticipant.initialEntityQuantity != 0))

  {
    std::cout << "(Assisting) ";
    p2AssistingMonkParticipant.outputEntity(player2Name);
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

void combatCalculator::checkRemainingDamage(
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
        // getIndividualValues();
        p2BattleParticipant.entityQuantity -= 1;
        // getTotalValues();
        remainingDamageP1 -= 1;
      }
    }
  }

  if (inputP2Events[26] == 1) {
    if (p1BattleParticipant.entityQuantity >= 1) {
      if (remainingDamageP2 >= 1) {
        // getIndividualValues();
        p1BattleParticipant.entityQuantity -= 1;
        // getTotalValues();
        remainingDamageP2 -= 1;
      }
    }
  }
}


// Function: Calculate the outcome of a monk battle
void monkRounds::roundOutcome(
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events,
  int* inputP1Technologies,
  int* inputP2Technologies,
  std::string inputRunConditions)
{
  // Track if a monk from either players did something
  bool monkPowersActivatedP1 = false;
  bool monkPowersActivatedP2 = false;

  // Track the amount of points awarded to each player
  int p2PointsLost = 0; // P1 points
  int p1PointsLost = 0; // P2 points

  /* Run the monk battle round for X times */
  for(int numberOfTimesToRunTheMonkBattleRound = 0; numberOfTimesToRunTheMonkBattleRound < inputRunTimes; numberOfTimesToRunTheMonkBattleRound++){
    checkIfDead(); // Check that no deaths have occured
    if (aDeathHasOccured == false) { // Make sure that no deaths have occured before proceeding since something that's dead cannot fight even though it can attack as it dies

  // Determine if we'll be running the calculations for P1 and P2 simultaneously
  if((inputRunConditions == "p1_calculations") || (inputRunConditions == "p1_and_p2_calculations")){
    /* Player 1 stuff */
    bool monkPresentP1 = false; // Track if player has a monk to begin with
    int p1EntitiesHealed = 1; // Declare the amount of entities saved (by healing)
    /* Check if the event "It's A Miracle" is active for P1 */
    if (inputP1Events[18] == 1) {
      // [18] It's A Miracle - A successful Healing attempt saves us up three
      // tokens, instead of one. Play this card before an attempt is made
      p1EntitiesHealed = 3;
    }
    int conversionRateANDhealingRateP1 = 0; //  Stores the conversion rates for the monks
    bool assistingMonksP1 = false; // Store whether monks are attached to a unit
    bool standaloneMonksP1 = false; // Store whether monks are on their own
    /* Check if player 1 has monks assisting a unit or monks on their own */
    if ((p1AssistingMonkParticipant.armorClass[9] == true)
        && (p1AssistingMonkParticipant.entityQuantity > 0)){
      assistingMonksP1 = true;
      conversionRateANDhealingRateP1 = p1AssistingMonkParticipant.entityQuantity;

    }
    else{
      assistingMonksP1 = false;
    }
    if((p1BattleParticipant.armorClass[9] == true) && (p1BattleParticipant.entityQuantity > 0) ){
      standaloneMonksP1 = true;
      conversionRateANDhealingRateP1 = p1BattleParticipant.entityQuantity;
    }
    else{
      standaloneMonksP1 = false;
    }

    if(assistingMonksP1 == false && standaloneMonksP1 == false){
      conversionRateANDhealingRateP1 = 0;
    }

    /* Throw an error if player 1 has monks attached to monks */
    if(assistingMonksP1 == true && standaloneMonksP1 == true){
      std::cout << "Error: Cannot have monks assisting monks";
      std::terminate();
    }
    /* Detect if P1 does in fact have monks */
    monkPresentP1 = ((assistingMonksP1 == true) || (standaloneMonksP1 == true)) ? true : false;
    /* Check if the event "Fat Friars Tavern O'Spirits" is active for P2 */
    if (inputP2Events[7] == 1) {
      // [7] Fat Friars Tavern O'Spirits - Monks on target unit may not have
      // any Conversion or Healing attempts this turn
      monkPresentP1 = false;
    }


        if(monkPresentP1 == true){ // Make sure player 1's monk is present before proceeding
          std::string player1IsHealingOrConverting = "0"; // Store if player 1 is performing a conversion or healing attempt

          const DialogInput::MonkAction monkAction{DIN.queryForMonkAction(player1Name)}; // Get player 1's decision
          switch (monkAction) {
          case DialogInput::MonkAction::Convert:
            player1IsHealingOrConverting = "Converting";
            break;
          case DialogInput::MonkAction::Heal:
            player1IsHealingOrConverting = "Healing";
            break;
          default:
            std::cout << "Error the input must be converting or healing for " << player1Name
                      << "'s "
                         "conversion/healing attempt"
                      << "<br>";

            std::terminate();
          }


          /* Check if the event "Back From A Foreign Land" (Byzantine) is active for P1 */
          if (inputP1Events[1] == 1) {
            if (player1IsHealingOrConverting == "Healing") {
              conversionRateANDhealingRateP1 += 2;
            }
          }

          /* Check if the event "Back From A Foreign Land" (Teuton) is active for P2 */
          if (inputP2Events[40] == 1) {
            if (player1IsHealingOrConverting == "Converting") {
              conversionRateANDhealingRateP1 -= 1;
            }
          }

          d6DieRoll = generateD6DieInput(); // Generate, validate, and return d6 dice input before proceeding

          /* Check if the event "Zealous Monks" is active for P1 */
          if ( (inputP1Events[39] == 1) && (player1IsHealingOrConverting == "Converting") ) {
            // [39] Zealous Monks - Target Monk unit gets 1 conversion roll at a 3
            // or less rate for each monk attached. In other words, you reroll the
            // dice if the number is <= 3 You repeat the roll for each monk attached

            int maximumRerolls = (assistingMonksP1 == true) ? p1AssistingMonkParticipant.entityQuantity : p1BattleParticipant.entityQuantity; // Check if there are just assisting monks are just standalone monks
            for(int dieReroll = 0; dieReroll != maximumRerolls; dieReroll ++){
              if(d6DieRoll <= 3){
                d6DieRoll = generateD6DieInput();
              }
            }
          }


          /* Check if the event "Piety" is active for P1 */
          if( (inputP1Events[23] == 1) && (player1IsHealingOrConverting == "Converting") ) {
            // [23] Piety - If you have one monk attached to a unit, your
            // conversion rate is 4. If the attempt fails, the monk dies
            const int pietyConversionRate = 4;

            if(pietyConversionRate <= d6DieRoll){ // Check if the conversion attempt failed and if so, reduce the quantity of monks by 1
              (assistingMonksP1 == true) ? p1AssistingMonkParticipant.entityQuantity -= 1 : p1BattleParticipant.entityQuantity -= 1;
            }
            else{ // The conversion attempt succeeded, will have this affect the calculations later on
              conversionRateANDhealingRateP1 = pietyConversionRate;
            }
          }

          /* Check if the event "Listen To A Story" is active for P1 */
          if( (inputP1Events[19] == 1) && (player1IsHealingOrConverting == "Converting") ) {
            // [19] Listen To A Story - Attempt 1-3 extra conversions this battle.
            // Pay 2 gold per conversion
            const int numberOfTimesTheDieHasBeenRolledSoFarForP1 = 1;  // So we've had one roll from earlier.
            const int numberOfExtraConversionAttemptsForP1 = 3;

            int numberOfTimesPlayer1HasPaidToAttemptAnotherConversion = 0;
            std::string getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer1 = "";
            int numberOfIterationsThroughTheWhileLoop = 0;


            while(numberOfIterationsThroughTheWhileLoop != (numberOfTimesTheDieHasBeenRolledSoFarForP1 + numberOfExtraConversionAttemptsForP1) ){

              // Check if the conversion attempt is going to fail
              if(conversionRateANDhealingRateP1 >= d6DieRoll){
                std::cout << "The conversion attempt for p1 failed. Pay 2 gold "
                             "and enter 1 to try again. Otherwise enter 0"
                          << "<br>";
                DIN >> getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer1;
              }

              if(getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer1 == "1"){
                d6DieRoll = generateD6DieInput();
                numberOfTimesPlayer1HasPaidToAttemptAnotherConversion++;
              }
              else if(getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer1 == "0"){
                numberOfIterationsThroughTheWhileLoop = numberOfExtraConversionAttemptsForP1; // Jump to the end of the while loop
              }

            numberOfIterationsThroughTheWhileLoop++;
            }

          // Display how much gold player 1 needs to pay for the "Listen To A Story" event card
          if(numberOfTimesPlayer1HasPaidToAttemptAnotherConversion > 0){
            int listenToAStoryGoldCostForPlayer1 = 2 * numberOfTimesPlayer1HasPaidToAttemptAnotherConversion;
            std::cout << "<< Player 1 must pay "
                      << std::to_string(listenToAStoryGoldCostForPlayer1) << " gold"
                      << "<br>";
          }
          }

          // Determine if the monk powers have activated for P1
          monkPowersActivatedP1 = (conversionRateANDhealingRateP1 >= d6DieRoll) ? true : false;

          if(monkPowersActivatedP1 == false){
            std::cout << player1Name << "'s monk powers failed to activate"
                    << "<br>";
          }
          // Act on the success
          else if(monkPowersActivatedP1 == true){
            std::cout << player1Name << "'s monk powers activated"
                      << "<br>";


            if(player1IsHealingOrConverting == "Converting"){


            // Act on the conversion attempt being successful

              // Work out whether player 1 is targeting a assistant or non-assistant

              std::string getPlayer1MonkConversionTarget = "";

              auto doesPlayer1HaveRedemption
                = [inputP1Technologies] { return inputP1Technologies[18] == 1; };


              if (
                p1AssistingMonkParticipant.entityQuantity > 0 && p2AssistingMonkParticipant.entityQuantity > 0
                && doesPlayer1HaveRedemption()) {
                  std::cout << "Is " << player1Name
                            << "'s 'assistant monk' targeting " << player2Name
                            << "'s assisting monk? Enter 1 for yes. Enter 0 for no"
                            << "<br>";
                  DIN >> getPlayer1MonkConversionTarget;

                  // Behaviour: Validate the input before proceeding
                  if (
                    (getPlayer1MonkConversionTarget != "0")
                    && (getPlayer1MonkConversionTarget != "1")) {
                    std::cout << "Error: The input must be 0 or 1"
                              << "<br>";
                    std::terminate();
                  }
              }

             if ( (getPlayer1MonkConversionTarget == "0") || (doesPlayer1HaveRedemption() == false) ) {
                     // Behaviour: Make sure that the conversion attempt is not being applied to siege units
              if (p2BattleParticipant.armorClass[12] == true) {
                  std::cout << "Error: " << player1Name
                            << "'s conversion attempt cannot be "
                            << "applied to " << player2Name << "'s siege unit"
                            << "<br>";
                  std::terminate();
              }

              // Convert the combat participant

              p2PointsLost = p2BattleParticipant.pointValue;
              if(p2BattleParticipant.entityQuantity > 0){
                  p2BattleParticipant.entityQuantity -= 1;
                  checkIfItCanBeHealed(); // Check for healing every time quantity goes down
              }


              }
              else if (getPlayer1MonkConversionTarget == "1") {
              p2PointsLost = p2AssistingMonkParticipant.pointValue;
              if(p2AssistingMonkParticipant.entityQuantity > 0){
                  p2AssistingMonkParticipant.entityQuantity -= 1;
                  checkIfItCanBeHealed(); // Check for healing every time quantity goes down
              }
              }




            // Play a SFX for a successful conversion attempt
            SFXToPlay("/sfx/rng/successful_monk_conversion_attempt_sfx.wav");

            }
            else if(player1IsHealingOrConverting == "Healing"){

            // @Kory: Todo: Perhaps the target of the heal could be either the assistant or battle participant
            for(int addHeals = 0; addHeals < p1EntitiesHealed ; addHeals++){
              p1BattleParticipant.healsAvailable ++;
            }

            std::cout << player1Name << " has ";
            (p1BattleParticipant.healsAvailable == 1) ? std::cout << " a heal" : std::cout << p1BattleParticipant.healsAvailable << " heals";
            std::cout << " available on their " << p1BattleParticipant.entityName << "<br>";


            // Play a SFX for a successful healing attempt
            SFXToPlay("/sfx/rng/successful_monk_healing_attempt_sfx.wav");

            }
          }









        }
      }

  if((inputRunConditions == "p2_calculations") || (inputRunConditions == "p1_and_p2_calculations")) {
    /* Player 2 stuff */
    bool monkPresentP2 = false; // Track if player has a monk to begin with
    int p2EntitiesHealed = 1; //  Declare the amount of entities saved (by healing)
    /* Check if the event "It's A Miracle" is active for P2 */
    if (inputP2Events[18] == 1) {
      // [18] It's A Miracle - A successful Healing attempt saves us up three
      // tokens, instead of one. Play this card before an attempt is made
      p2EntitiesHealed = 3;
    }
    int conversionRateANDhealingRateP2 = 0; //  Stores the conversion rates for the monks
    bool assistingMonksP2 = false;  // Store whether monks are attached to a unit
    bool standaloneMonksP2 = false;  // Store whether monks are on their own




    /* Check if player 2 has monks assisting a unit or monks on their own */
    if ((p2AssistingMonkParticipant.armorClass[9] == true)
        && (p2AssistingMonkParticipant.entityQuantity > 0)){
      assistingMonksP2 = true;
      conversionRateANDhealingRateP2 = p2AssistingMonkParticipant.entityQuantity;
    }
    else{
      assistingMonksP2 = false;
    }
    if((p2BattleParticipant.armorClass[9] == true) && (p2BattleParticipant.entityQuantity > 0) ){
      standaloneMonksP2 = true;
      conversionRateANDhealingRateP2 = p2BattleParticipant.entityQuantity;
    }
    else{
      standaloneMonksP2 = false;
    }

    if(assistingMonksP2 == false && standaloneMonksP2 == false){
      conversionRateANDhealingRateP2 = 0;
    }

    /* Throw an error if player 2 has monks attached to monks */
    if(assistingMonksP2 == true && standaloneMonksP2 == true){
      std::cout << "Error: Cannot have monks assisting monks";
      std::terminate();
    }
    /* Detect if P2 does in fact have monks */
    monkPresentP2 = ((assistingMonksP2 == true) || (standaloneMonksP2 == true)) ? true : false;
    /* Check if the event "Fat Friars Tavern O'Spirits" is active for P1 */
    if (inputP1Events[7] == 1) {
      // [7] Fat Friars Tavern O'Spirits - Monks on target unit may not have
      // any Conversion or Healing attempts this turn
      monkPresentP2 = false;
    }


    if(monkPresentP2 == true){ // Make sure player 2's monk is present before proceeding
      std::string player2IsHealingOrConverting = "0"; // Store if player 2 is performing a conversion or healing attempt

      const DialogInput::MonkAction monkAction{DIN.queryForMonkAction(player2Name)}; // Get player 2's decision
      switch (monkAction) {
      case DialogInput::MonkAction::Convert:
            player2IsHealingOrConverting = "Converting";
            break;
      case DialogInput::MonkAction::Heal:
            player2IsHealingOrConverting = "Healing";
            break;
      default:
            std::cout << "Error the input must be converting or healing for " << player2Name
                      << "'s "
                         "conversion/healing attempt"
                      << "<br>";

            std::terminate();
      }


      /* Check if the event "Back From A Foreign Land" (Byzantine) is active for P2 */
      if (inputP2Events[1] == 1) {
            if (player2IsHealingOrConverting == "Healing") {
            conversionRateANDhealingRateP2 += 2;
            }
      }

      /* Check if the event "Back From A Foreign Land" (Teuton) is active for P1 */
      if (inputP1Events[40] == 1) {
            if (player2IsHealingOrConverting == "Converting") {
            conversionRateANDhealingRateP2 -= 1;
            }
      }

      d6DieRoll = generateD6DieInput(); // Generate, validate, and return d6 dice input before proceeding
      /* Check if the event "Zealous Monks" is active for P2 */
      if ( (inputP2Events[39] == 1) && (player2IsHealingOrConverting == "Converting") ) {
            // [39] Zealous Monks - Target Monk unit gets 1 conversion roll at a 3
            // or less rate for each monk attached. In other words, you reroll the
            // dice if the number is <= 3 You repeat the roll for each monk attached

            int maximumRerolls = (assistingMonksP2 == true) ? p2AssistingMonkParticipant.entityQuantity : p2BattleParticipant.entityQuantity; // Check if there are just assisting monks are just standalone monks
            for(int dieReroll = 0; dieReroll != maximumRerolls; dieReroll ++){
            if(d6DieRoll <= 3){
              d6DieRoll = generateD6DieInput();
            }
            }
      }


      /* Check if the event "Piety" is active for P2 */
      if( (inputP2Events[23] == 1) && (player2IsHealingOrConverting == "Converting") ) {
            // [23] Piety - If you have one monk attached to a unit, your
            // conversion rate is 4. If the attempt fails, the monk dies
            const int pietyConversionRate = 4;

            if(pietyConversionRate <= d6DieRoll){ // Check if the conversion attempt failed and if so, reduce the quantity of monks by 1
            (assistingMonksP2 == true) ? p2AssistingMonkParticipant.entityQuantity -= 1 : p2BattleParticipant.entityQuantity -= 1;
            }
            else{ // The conversion attempt succeeded, will have this affect the calculations later on
            conversionRateANDhealingRateP2 = pietyConversionRate;
            }
      }

      /* Check if the event "Listen To A Story" is active for P2 */
      if( (inputP2Events[19] == 1) && (player2IsHealingOrConverting == "Converting") ) {
            // [19] Listen To A Story - Attempt 1-3 extra conversions this battle.
            // Pay 2 gold per conversion
            const int numberOfTimesTheDieHasBeenRolledSoFarForP2 = 1;  // So we've had one roll from earlier.
            const int numberOfExtraConversionAttemptsForP2 = 3;

            int numberOfTimesPlayer2HasPaidToAttemptAnotherConversion = 0;
            std::string getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer2 = "";
            int numOfIterationsThroughTheWhileLoop = 0;


            while(numOfIterationsThroughTheWhileLoop != (numberOfTimesTheDieHasBeenRolledSoFarForP2 + numberOfExtraConversionAttemptsForP2) ){

                   // Check if the conversion attempt is going to fail
            if(conversionRateANDhealingRateP2 >= d6DieRoll){
              std::cout << "The conversion attempt for p1 failed. Pay 2 gold "
                           "and enter 1 to try again. Otherwise enter 0"
                        << "<br>";
              DIN >> getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer2;
            }

            if(getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer2 == "1"){
              d6DieRoll = generateD6DieInput();
              numberOfTimesPlayer2HasPaidToAttemptAnotherConversion++;
            }
            else if(getUserInputOnIfAnotherConversionAttemptWillBeAttemptedByPlayer2 == "0"){
              numOfIterationsThroughTheWhileLoop = numberOfExtraConversionAttemptsForP2; // Jump to the end of the while loop
            }

            numOfIterationsThroughTheWhileLoop++;
            }

                   // Display how much gold player 2 needs to pay for the "Listen To A Story" event card
            if(numberOfTimesPlayer2HasPaidToAttemptAnotherConversion > 0){
            int listenToAStoryGoldCostForPlayer2 = 2 * numberOfTimesPlayer2HasPaidToAttemptAnotherConversion;
            std::cout << "<< Player 2 must pay "
                      << std::to_string(listenToAStoryGoldCostForPlayer2) << " gold"
                      << "<br>";
            }
      }

             // Determine if the monk powers have activated for P2
      monkPowersActivatedP2 = (conversionRateANDhealingRateP2 >= d6DieRoll) ? true : false;
      if(monkPowersActivatedP2 == false){
            std::cout << player2Name << "'s monk powers failed to activate"
                      << "<br>";
      }
      // Act on the success
      else if(monkPowersActivatedP2 == true){
            std::cout << player2Name << "'s monk powers activated"
                      << "<br>";


            if(player2IsHealingOrConverting == "Converting"){


                   // Act on the conversion attempt being successful

                   // Work out whether player 2 is targeting a assistant or non-assistant

            std::string getPlayer2MonkConversionTarget = "";

            auto doesPlayer2HaveRedemption
              = [inputP2Technologies] { return inputP2Technologies[18] == 1; };


            if (
              p2AssistingMonkParticipant.entityQuantity > 0 && p1AssistingMonkParticipant.entityQuantity > 0
              && doesPlayer2HaveRedemption()) {
              std::cout << "Is " << player2Name
                        << "'s 'assistant monk' targeting " << player1Name
                        << "'s assisting monk? Enter 1 for yes. Enter 0 for no"
                        << "<br>";
              DIN >> getPlayer2MonkConversionTarget;

                     // Behaviour: Validate the input before proceeding
              if (
                (getPlayer2MonkConversionTarget != "0")
                && (getPlayer2MonkConversionTarget != "1")) {
                  std::cout << "Error: The input must be 0 or 1"
                            << "<br>";
                  std::terminate();
              }
            }

            if ( (getPlayer2MonkConversionTarget == "0") || (doesPlayer2HaveRedemption() == false) ) {
              // Behaviour: Make sure that the conversion attempt is not being applied to siege units
              if (p1BattleParticipant.armorClass[12] == true) {
                  std::cout << "Error: " << player2Name
                            << "'s conversion attempt cannot be "
                            << "applied to " << player1Name << "'s siege unit"
                            << "<br>";
                  std::terminate();
              }

                     // Convert the combat participant

              p1PointsLost = p1BattleParticipant.pointValue;
              if(p1BattleParticipant.entityQuantity > 0){
                  p1BattleParticipant.entityQuantity -= 1;
                  checkIfItCanBeHealed(); // Check for healing every time quantity goes down
              }


            }
            else if (getPlayer2MonkConversionTarget == "1") {
              p1PointsLost = p1AssistingMonkParticipant.pointValue;
              if(p1AssistingMonkParticipant.entityQuantity > 0){
                  p1AssistingMonkParticipant.entityQuantity -= 1;
                  checkIfItCanBeHealed(); // Check for healing every time quantity goes down
              }
            }




            // Play a SFX for a successful conversion attempt
            SFXToPlay("/sfx/rng/successful_monk_conversion_attempt_sfx.wav");

            }
            else if(player2IsHealingOrConverting == "Healing"){

                   // @Kory: Todo: Perhaps the target of the heal could be either the assistant or battle participant
            for(int numOfHeals = 0; numOfHeals < p2EntitiesHealed ; numOfHeals++){
              p2BattleParticipant.healsAvailable ++;
            }


            std::cout << player2Name << " has ";
            (p2BattleParticipant.healsAvailable == 1) ? std::cout << " a heal" : std::cout << p2BattleParticipant.healsAvailable << " heals";
            std::cout << " available on their " << p2BattleParticipant.entityName << "<br>";



                   // Play a SFX for a successful healing attempt
            SFXToPlay("/sfx/rng/successful_monk_healing_attempt_sfx.wav");

            }







      }


  }
  }


   // Behaviour: Display the outcome of the monk combat round only if changes occured
  if ((monkPowersActivatedP1 == true) || (monkPowersActivatedP2 == true)) {
    std::string outputString
      = "Monk round " + std::to_string(numberOfTimesToRunTheMonkBattleRound + 1) + " calculations...";
    outputEntityInformation(outputString);

           // Behaviour: Display how many points were added if appropriate
    if (p2PointsLost != 0) {
      std::cout << ">> " << player1Name << " gets " << p2PointsLost
                << " points"
                << "<br>";
    }
    if (p1PointsLost != 0) {
      std::cout << ">> " << player2Name << " gets " << p1PointsLost
                << " points"
                << "<br>";
    }

    std::cout << "<br>";
  }










    } // End if conditional checking for no deaths


  } // End for loop

} // End monk combat rounds function

// Function: Calculate the outcome of a ranged battle
void archerRounds::roundOutcome(
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events,
  int* inputP1Technologies,
  int* inputP2Technologies,
  std::string inputRunConditions)
{
  //  Track if a ranged unit from either players did something
  bool p1ArcherPresent = false, p2ArcherPresent = false;

  //  Declare the number of units killed in this round
  int p1EntityDeaths = 0, p2EntityDeaths = 0;

  //  Declare the amount of damage dealt to buildings in this round
  int p1BuildingDamage = 0, p2BuildingDamage = 0, roundDownMultiple = 10;

  //  Get whether the archer entity is fighting a building
  bool isP1FightingBuilding = false, isP2FightingBuilding = false;

  //  Declare the amount of damage die to place onto buildings
  int p1DamageDie = 0, p2DamageDie = 0;

  //  Declare the amount of points awarded
  float p1PointsLost = 0, p2PointsLost = 0;

  //  Track if changes occured to the quantity
  int p1StartingQuantity = 0, p1EndingQuantity = 0;
  int p2StartingQuantity = 0, p2EndingQuantity = 0;

  //  Negate the non-cavalry requirement
  bool rangedUnitAttacksCavalryP1 = false;
  bool rangedUnitAttacksCavalryP2 = false;

  // Apply the effects of event 33
  if (inputP1Events[33] == 1) {
    // [33] Steady Hand - Target Archer unit may do ranged damage to a Cavalry
    // unit this turn
    rangedUnitAttacksCavalryP1 = true;
  }

  if (inputP2Events[33] == 1) {
    // [33] Steady Hand - Target Archer unit may do ranged damage to a Cavalry
    // unit this turn
    rangedUnitAttacksCavalryP2 = true;
  }

  // Behaviour: Run the ranged battle round for X times
  for (int i = 0; i < inputRunTimes; i++) {
    // Behaviour: Check that no deaths have occured
    checkIfDead();

    // Behaviour: Make sure that no deaths have occured and that the attacking
    // entity is not retreating before proceeding
    if ((aDeathHasOccured == false) && (isRetreating != "1")) {
      // Behaviour: Check if player 1 has an archer and if they are not fighting
      // cavalry
      if (
        (p1BattleParticipant.armorClass[0] == true)
        && ((p2BattleParticipant.armorClass[4] != true) || (rangedUnitAttacksCavalryP1 == true))) {
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
          p2BuildingDamage = ( ((p1BattleParticipant.rangedDamage) + roundAttackModifiersP1) - ((p2BattleParticipant.entityHealth) % roundDownMultiple));
          p2DamageDie = p2BuildingDamage / 10;
          remainingDamageP1 += calculateRemainingDamage(
            p2DamageDie,
            p1BattleParticipant.rangedDamage,
            roundAttackModifiersP1,
            p2BattleParticipant.entityHealth,
            1);
        }
        // Behaviour: Calculate the damage against units if player 1 is fighting
        // a unit
        else {
          p2EntityDeaths = std::floor(
            ((p1BattleParticipant.rangedDamage) + roundAttackModifiersP1)
            / (p2BattleParticipant.entityHealth));
          remainingDamageP1 += calculateRemainingDamage(
            p2EntityDeaths,
            p1BattleParticipant.rangedDamage,
            roundAttackModifiersP1,
            p2BattleParticipant.entityHealth,
            0);
        }

        p1ArcherPresent = true;
      }
      else {
        p1ArcherPresent = false;
      }

      // Behaviour: Check if player 2 has an archer and if they are not fighting
      // cavalry
      if (
        (p2BattleParticipant.armorClass[0] == true)
        && ((p1BattleParticipant.armorClass[4] != true) || (rangedUnitAttacksCavalryP2 == true))) {
        // Behaviour: Check if player 2 is fighting a building
        if (p1BattleParticipant.armorClass[1] == true) { // Building p1
          isP2FightingBuilding = true;
        }
        else {
          isP2FightingBuilding = false;
        }

        // Behaviour: Calculate the damage against buildings if player 2 is
        // fighting a building
        if (isP2FightingBuilding == true) {
          p1BuildingDamage = ( ((p2BattleParticipant.rangedDamage) + roundAttackModifiersP2) - ((p1BattleParticipant.entityHealth) % roundDownMultiple));
          p1DamageDie = p1BuildingDamage / 10;
          remainingDamageP2 += calculateRemainingDamage(
            p1DamageDie,
            p2BattleParticipant.rangedDamage,
            roundAttackModifiersP2,
            p1BattleParticipant.entityHealth,
            1);
        }
        // Behaviour: Calculate the damage against units if player 2 is fighting
        // a unit
        else {
          p1EntityDeaths = std::floor(
            ((p2BattleParticipant.rangedDamage) + roundAttackModifiersP2)
            / (p1BattleParticipant.entityHealth));
          remainingDamageP2 += calculateRemainingDamage(
            p1EntityDeaths,
            p2BattleParticipant.rangedDamage,
            roundAttackModifiersP2,
            p1BattleParticipant.entityHealth,
            0);
        }

        p2ArcherPresent = true;
      }
      else {
        p2ArcherPresent = false;
      }
    }

    // Behaviour: Check if the remaining damage value triggers a death
    checkRemainingDamage(inputP1Events, inputP2Events);

    // Behaviour: Check for deaths again
    checkIfDead();

    // Behaviour: Make sure that no entities have died again
    if (aDeathHasOccured == false) {
      // Behaviour: Negate the ranged attack round for p2 if p2 has heavy tree
      // cover
      if (inputP2Events[15] == 1) {
        // [15] Heavy Tree Cover - Negate range combat round in target battle
        // involving one of your units
        p2EntityDeaths   = 0;
        p2BuildingDamage = 0;
        p2DamageDie      = 0;
      }

      // Behaviour: Negate the ranged attack round for p1 if p1 has heavy tree
      // cover
      if (inputP1Events[15] == 1) {
        // [15] Heavy Tree Cover - Negate range combat round in target battle
        // involving one of your units
        p1EntityDeaths   = 0;
        p1BuildingDamage = 0;
        p1DamageDie      = 0;
      }

      // Behaviour: Apply the results for p1 if ranged damage occured
      if (p1ArcherPresent == true) {
        // Behaviour: Apply the results to buildings or non-buildings
        if (isP1FightingBuilding == true) {
          // Behaviour: Decrease the building HP
          if (p2BuildingDamage != 0) {
            // Behaviour: Set the point value to the buildings point
            p2PointsLost = p2BattleParticipant.pointValue;
            p2BattleParticipant.entityHealth -= p2BuildingDamage;

            // Behaviour: if the HP of the building is > 0 then do not keep the
            // set the point value
            if (p2BattleParticipant.entityHealth > 0) {
              p2PointsLost = 0;
            }
          }
        }
        else {
          // Behavior: Store the starting quantity
          p2StartingQuantity = p2BattleParticipant.entityQuantity;

          // Behaviour: Store how many points the entity is individually worth
          p2PointsLost
            = (p2BattleParticipant.pointValue / p2BattleParticipant.entityQuantity);

          // Behaviour: Divide the rangedDamagem SD and pointValue by the
          // current quantity to get the values by 1
          // getIndividualValues();

          // Behaviour: Now decrease the quantity
          p2BattleParticipant.entityQuantity -= p2EntityDeaths;

          // Behaviour: Now multiply the rangedDamage, SD and pointValue by the
          // new quantity
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
            p2PointsLost *= p2QuantityDifference;
          }
          else {
            p2PointsLost = 0;
          }
        }
      }

      // Behaviour: Apply the results for p2 if ranged damage occured
      if (p2ArcherPresent == true) {
        // Behaviour: Apply the results to buildings or non-buildings
        if (isP2FightingBuilding == true) {
          // Behaviour: Decrease the building HP
          if (p1BuildingDamage != 0) {
            // Behaviour: Set the point value to the buildings point
            p1PointsLost = p1BattleParticipant.pointValue;

            p1BattleParticipant.entityHealth -= p1BuildingDamage;

            // Behaviour: if the HP of the building is > 0 then do not keep the
            // set the point value
            if (p1BattleParticipant.entityHealth > 0) {
              p1PointsLost = 0;
            }
          }
        }
        else {
          // Behavior: Store the starting quantity
          p1StartingQuantity = p1BattleParticipant.entityQuantity;

          // Behaviour: Store how many points the entity is individually worth
          p1PointsLost
            = (p1BattleParticipant.pointValue / p1BattleParticipant.entityQuantity);

          // Behaviour: Divide the rangedDamage, SD and pointValue by the
          // current quantity to get the values by 1
          // getIndividualValues();

          // Behaviour: Now decrease the quantity
          p1BattleParticipant.entityQuantity -= p1EntityDeaths;

          // Behaviour: Now multiply the rangedDamage, SD and pointValue by the
          // new quantity
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
            p1PointsLost *= p1QuantityDifference;
          }
          else {
            p1PointsLost = 0;
          }
        }
      }
    }

    // Behaviour: Display the outcome of the archer combat round only if changes
    // occured
    if ((p1ArcherPresent == true) || (p2ArcherPresent == true)) {
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
      if (p2PointsLost != 0) {
        std::cout << ">> " << player1Name << " gets " << p2PointsLost
                  << " points"
                  << "<br>";
      }
      if (p1PointsLost != 0) {
        std::cout << ">> " << player2Name << " gets " << p1PointsLost
                  << " points"
                  << "<br>";
      }

      std::cout << "<br>";

      // Behaviour: Check if a death has occured
      checkIfDead();

      // Behaviour: Check if the attacking archer is retreating
      if (aDeathHasOccured == false) {
        checkIfRetreating("Archer");

        if ((inputP1Events[29] == 1) || (inputP2Events[29] == 1)) {
          //  [29] Shots In The Back (Briton) - If a unit in combat with your
          //  Archer retreats, your Archer deals an additional round of range
          //  damage without taking any damage
          std::string getAnswer = "";

          std::cout << "Did the conditions of Shots_In_The_Back_(Briton) get "
                       "satisfied for player 1 or 2?"
                    << "<br>";
          bool isShotsInTheBackSatisfied{};
          DIN >> isShotsInTheBackSatisfied;
          getAnswer = isShotsInTheBackSatisfied ? "0" : "1";

          if (getAnswer == "1") {
            // Behaviour: Have another round of ranged combat
            inputRunTimes++;
          }
        }
      }
    }
    else {
      /* No longer show if a phase is skipped to conserve space on the window
    std::cout << "<br>"
              << "Skipping Archer round " + std::to_string(i + 1)
                   + " calculations..."
              << "<br>";
*/
    }
  }
}

// Function: Calculate the outcome of a bombardment round of battle (only for
// the Crow's nest event card)
void bombardmentRounds::roundOutcome(
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events,
  int* inputP1Technologies,
  int* inputP2Technologies,
  std::string inputRunConditions)
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
  float p1PointsLost = 0, p2PointsLost = 0;

  //  Track if changes occured to the quantity
  int p1StartingQuantity = 0, p1EndingQuantity = 0;
  int p2StartingQuantity = 0, p2EndingQuantity = 0;

  // Behaviour: Run the bombardment battle round for X times
  for (int i = 0; i < inputRunTimes; i++) {
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
        p2BuildingDamage = ( ((p1BattleParticipant.standardDamage) + roundAttackModifiersP1) - ((p1BattleParticipant.standardDamage) % roundDownMultiple));
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
            ((p1BattleParticipant.standardDamage) + roundAttackModifiersP1)
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
        p1BuildingDamage = ( ((p2BattleParticipant.standardDamage) + roundAttackModifiersP2) - ((p2BattleParticipant.standardDamage) % roundDownMultiple));
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
            ((p2BattleParticipant.standardDamage) + roundAttackModifiersP2)
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
            p2PointsLost = p2BattleParticipant.pointValue;

            p2BattleParticipant.entityHealth -= p2BuildingDamage;

            // Behaviour: if the HP of the building is > 0 then do not keep the
            // set the point value
            if (p2BattleParticipant.entityHealth > 0) {
              p2PointsLost = 0;
            }
          }
        }
        else {
          // Behavior: Store the starting quantity
          p2StartingQuantity = p2BattleParticipant.entityQuantity;

          // Behaviour: Store how many points the entity is individually worth
          p2PointsLost
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
            p2PointsLost *= p2QuantityDifference;
          }
          else {
            p2PointsLost = 0;
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
            p1PointsLost = p1BattleParticipant.pointValue;

            p1BattleParticipant.entityHealth -= p1BuildingDamage;

            // Behaviour: if the HP of the building is > 0 then do not keep the
            // set the point value
            if (p1BattleParticipant.entityHealth > 0) {
              p1PointsLost = 0;
            }
          }
        }
        else {
          // Behavior: Store the starting quantity
          p1StartingQuantity = p1BattleParticipant.entityQuantity;

          // Behaviour: Store how many points the entity is individually worth
          p1PointsLost
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
            p1PointsLost *= p1QuantityDifference;
          }
          else {
            p1PointsLost = 0;
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
      if (p2PointsLost != 0) {
        std::cout << ">> " << player1Name << " gets " << p2PointsLost
                  << " points"
                  << "<br>";
      }
      if (p1PointsLost != 0) {
        std::cout << ">> " << player2Name << " gets " << p1PointsLost
                  << " points"
                  << "<br>";
      }

      std::cout << "<br>";

      // Behaviour: Check if a death has occured
      checkIfDead();

      // Behaviour: Check if the attacking entity is retreating if this is not
      // the last round
      if (aDeathHasOccured == false) {
        if (i != inputRunTimes - 1) {
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
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events,
  int* inputP1Technologies,
  int* inputP2Technologies,
  std::string inputRunConditions)
{
  //  Track if the standard attack round got activated
  bool standardRoundActivated = false;

  //  Declare the number of units killed in this round
  int p1EntityDeaths = 0, p2EntityDeaths = 0;

  //  Declare the amount of damage dealt to buildings in this round
  int p1BuildingDamage = 0, p2BuildingDamage = 0, roundDownMultiple = 10;

  //  Get whether the archer entity is fighting a building
  bool isP1FightingBuilding = false, isP2FightingBuilding = false;

  //  Declare the amount of damage die to place onto buildings
  int p1DamageDie = 0, p2DamageDie = 0;

  //  Declare the amount of points awarded
  float p1PointsLost = 0, p2PointsLost = 0;

  //  Track if changes occured to the quantity
  int p1StartingQuantity = 0, p1EndingQuantity = 0;
  int p2StartingQuantity = 0, p2EndingQuantity = 0;

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

  // Behaviour: Give the person who played the card more attack
  if (inputP1Events[35] == 1) {
    // [35] The Jester Is Dead, Let's Get Em (Celt) = Sacrifice 1 of your
    // villagers. +4 standardDamage to all defending units this turn
    p1BattleParticipant.standardDamage += 4;
  }

  if (inputP2Events[35] == 1) {
    // [35] The Jester Is Dead, Let's Get Em (Celt) = Sacrifice 1 of your
    // villagers. +4 standardDamage to all defending units this turn
    p2BattleParticipant.standardDamage += 4;
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

  if (inputP2Events[20] == 1) {
    // [20] Muddy Battlefield - Target infantry gets double standardAttack
    // versus Cavalry this turn
    if (
      (p2BattleParticipant.armorClass[8])
      && (p1BattleParticipant.armorClass[4])) {
      p2BattleParticipant.standardDamage *= 2;
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

  // Behaviour: Multiply the standardDamage by a random number between 1-6 if a
  // Fire Ship
  d6DieRoll = generateD6DieInput();

  // Behaviour: Set the result to 0 if player 2 has Soak_The_Timbers
  if (inputP2Events[30] == 1) {
    // [30] Soak The Timbers - All enemy Fire Ships get an automatic result of 1
    // for their die roll when calculating damage this turn
    d6DieRoll = 1;
  }

  if (
    (p1BattleParticipant.armorClass[22] == true)
    || (p1BattleParticipant.armorClass[22] == true)) { // Fire Ship or Fast Fire
                                                       // Ship
    p1BattleParticipant.standardDamage *= d6DieRoll;
  }

  d6DieRoll = generateD6DieInput();

  // Behaviour: Set the result to 0 if player 1 has Soak_The_Timbers
  if (inputP1Events[30] == 1) {
    // [30] Soak The Timbers - All enemy Fire Ships get an automatic result of 1
    // for their die roll when calculating damage this turn
    d6DieRoll = 1;
  }

  if (
    (p2BattleParticipant.armorClass[22] == true)
    || (p2BattleParticipant.armorClass[22] == true)) { // Fire Ship or Fast Fire
                                                       // Ship
    p2BattleParticipant.standardDamage *= d6DieRoll;
  }

  // Behaviour: Run the standard battle round for X times
  for (int i = 0; i < inputRunTimes; i++) {
    if ((aDeathHasOccured == false) && (isRetreating != "1")) {
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
          if (i == 0) {
            // Do not re-calculate this value again in the second round or
            // you're going to get a result that doesn't reflect the individual
            // attack of the unit (it will be divided too much)
            getHowMuchAttackIndividualUnitsDo
              = (p2BattleParticipant.standardDamage / p2BattleParticipant.entityQuantity);

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
          else if (i == 1) {
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
          if (i == 0) {
            // Do not re-calculate this value again in the second round or
            // you're going to get a result that doesn't reflect the individual
            // attack of the unit (it will be divided too much)
            getHowMuchAttackIndividualUnitsDo2
              = (p1BattleParticipant.standardDamage / p1BattleParticipant.entityQuantity);

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
          else if (i == 1) {
            p1BattleParticipant.standardDamage
              = getHowMuchAttackIndividualUnitsDo2
                * p1BattleParticipant.entityQuantity;
          }
        }
      }

      // Behaviour: Check if the remaining damage value triggers a death
      checkRemainingDamage(inputP1Events, inputP2Events);

      // Behaviour: Check that no deaths have occured
      checkIfDead();

      // Set the activation conditions to false
      standardRoundActivated = false;
      isP1FightingBuilding   = false;
      isP2FightingBuilding   = false;

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
          p2BuildingDamage
            = ((p1BattleParticipant.standardDamage) - ((p1BattleParticipant.standardDamage) % roundDownMultiple));
          p2DamageDie = p2BuildingDamage / 10;
          remainingDamageP1 += calculateRemainingDamage(
            p2DamageDie,
            p1BattleParticipant.standardDamage,
            roundAttackModifiersP1,
            p2BattleParticipant.entityHealth,
            1);
        }
        // Behaviour: Calculate the damage against units if player 1 is fighting
        // a unit
        else {
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
          }
          else {
            p2EntityDeaths = std::floor(
              ((p1BattleParticipant.standardDamage) + roundAttackModifiersP1)
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
          p1BuildingDamage = ( ((p2BattleParticipant.standardDamage) + roundAttackModifiersP2) - ((p1BattleParticipant.entityHealth) % roundDownMultiple));
          p1DamageDie = p1BuildingDamage / 10;
          remainingDamageP2 += calculateRemainingDamage(
            p1DamageDie,
            p2BattleParticipant.standardDamage,
            roundAttackModifiersP2,
            p1BattleParticipant.entityHealth,
            1);
        }
        // Behaviour: Calculate the damage against units if player 2 is fighting
        // a unit
        else {
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
          }
          else {
            p1EntityDeaths = std::floor(
              ((p2BattleParticipant.standardDamage) + roundAttackModifiersP2)
              / (p1BattleParticipant.entityHealth));
            remainingDamageP2 += calculateRemainingDamage(
              p1EntityDeaths,
              p2BattleParticipant.standardDamage,
              roundAttackModifiersP2,
              p1BattleParticipant.entityHealth,
              0);
          }
        }

        standardRoundActivated = true;
      }
      else {
        standardRoundActivated = false;
      }

      // Behaviour: Clear the results if the entity only attacks once (in the
      // first round of combat) and we are not in the 1st round
      if ((p1BattleParticipant.onlyAttacksOnce == true) && (i + 1 != 1)) {
        p2BuildingDamage = 0;
        p2DamageDie      = 0;
        p2EntityDeaths   = 0;
      }

      if ((p2BattleParticipant.onlyAttacksOnce == true) && (i + 1 != 1)) {
        p1BuildingDamage = 0;
        p1DamageDie      = 0;
        p1EntityDeaths   = 0;
      }

      // Behaviour: Clear the results if the entity only attacks in the second
      // round of combat and we are not in the 2nd round
      if (
        (p1BattleParticipant.onlyAttacksInTheSecondRoundOfStandardCombat
         == true)
        && (i + 1 != 2)) {
        p2BuildingDamage = 0;
        p2DamageDie      = 0;
        p2EntityDeaths   = 0;
      }

      if (
        (p2BattleParticipant.onlyAttacksInTheSecondRoundOfStandardCombat
         == true)
        && (i + 1 != 2)) {
        p1BuildingDamage = 0;
        p1DamageDie      = 0;
        p1EntityDeaths   = 0;
      }

      // Behaviour: Clear the results if the entity is an infantry unit for the
      // attacking player and we are in the first round of combat
      if (inputP1Events[37] == 1) {
        // [37] While They're Sleeping - Target attacking Infantry unit takes no
        // damage for the first round of combat. The ranged round does not count
        // as the first round of combat
        if (p1BattleParticipant.armorClass[8] == true) {
          p1EntityDeaths = 0;
        }
      }

      if (inputP2Events[37] == 1) {
        // [37] While They're Sleeping - Target attacking Infantry unit takes no
        // damage for the first round of combat. The ranged round does not count
        // as the first round of combat
        if (p2BattleParticipant.armorClass[8] == true) {
          p2EntityDeaths = 0;
        }
      }

      // Behaviour: Check if the remaining damage value triggers a death
      checkRemainingDamage(inputP1Events, inputP2Events);

      // Behaviour: Check for the deaths
      checkIfDead();

      // Behaviour: Make sure that there have been no deaths before proceeding
      if (aDeathHasOccured == false) {
        // Behaviour: Apply the results for p1 if the activation conditions were
        // met
        if (standardRoundActivated == true) {
          // Behaviour: Apply the results to buildings or non-buildings
          if (isP1FightingBuilding == true) {
            // Behaviour: Decrease the building HP
            if (p2BuildingDamage != 0) {
              // Behaviour: Set the point value to the buildings point
              p2PointsLost = p2BattleParticipant.pointValue;

              p2BattleParticipant.entityHealth -= p2BuildingDamage;

              // Behaviour: if the HP of the building is > 0 then do not keep
              // the set the point value
              if (p2BattleParticipant.entityHealth > 0) {
                p2PointsLost = 0;
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
            p2PointsLost
              = (p2BattleParticipant.pointValue / p2BattleParticipant.entityQuantity);

            // Behaviour: Divide the RD, standardDamage and pointValue by the
            // current quantity to get the values for a quantity of 1
            // getIndividualValues();

            // Behaviour: Now decrease the quantity
            p2BattleParticipant.entityQuantity -= p2EntityDeaths;

            // Behaviour: Now multiply the RD, standardDamage and pointValue by
            // the new quantity
            // getTotalValues();

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
              p2PointsLost *= p2QuantityDifference;
            }
            else {
              p2PointsLost = 0;
            }
          }
        }

        // Behaviour: Apply the results for p2 if standard damage occured
        if (standardRoundActivated == true) {
          // Behaviour: Apply the results to buildings or non-buildings
          if (isP2FightingBuilding == true) {
            // Behaviour: Decrease the building HP
            if (p1BuildingDamage != 0) {
              // Behaviour: Set the point value to the buildings point
              p1PointsLost = p1BattleParticipant.pointValue;

              p1BattleParticipant.entityHealth -= p1BuildingDamage;

              // Behaviour: if the HP of the building is > 0 then do not keep
              // the set the point value
              if (p1BattleParticipant.entityHealth > 0) {
                p1PointsLost = 0;
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
            p1PointsLost
              = (p1BattleParticipant.pointValue / p1BattleParticipant.entityQuantity);

            // Behaviour: Divide the RD, standardDamage and pointValue by the
            // current quantity to get the values for a quantity of 1
            // getIndividualValues();

            // Behaviour: Now decrease the quantity
            p1BattleParticipant.entityQuantity -= p1EntityDeaths;

            // Behaviour: Now multiply the RD, standardDamage and pointValue by
            // the new quantity
            // getTotalValues();

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
              p1PointsLost *= p1QuantityDifference;
            }
            else {
              p1PointsLost = 0;
            }
          }
        }

        // Apply the effects of certain entities that automatically die in the
        // second round of combat Make sure that the standard round got
        // activated to avoid an arithmetic exception
        if (standardRoundActivated == true) {
          if (i + 1 == 2) {
            if (p1BattleParticipant.isKamikaze == true) {
              p1PointsLost
                = (p1BattleParticipant.pointValue / p1BattleParticipant.entityQuantity);
              p1BattleParticipant.entityQuantity = 0;
            }
            else if (p1BattleParticipant.isKamikaze == true) {
              p2PointsLost
                = (p2BattleParticipant.pointValue / p2BattleParticipant.entityQuantity);
              p2BattleParticipant.entityQuantity = 0;
            }
          }
        }
      }

      // Behaviour: Display the outcome of the archer combat round only if
      // changes occured
      if (standardRoundActivated == true) {
        std::string outputString
          = "Standard round " + std::to_string(i + 1) + " calculations...";
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
        if (p2PointsLost != 0) {
          std::cout << ">> " << player1Name << " gets " << p2PointsLost
                    << " points"
                    << "<br>";
        }
        if (p1PointsLost != 0) {
          std::cout << ">> " << player2Name << " gets " << p1PointsLost
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
          if (i != inputRunTimes - 1) {
            if ((inputP1Events[27] == 1) || (inputP2Events[27] == 1)) {
              // [27] Retreat - Target unit must retreat before the first round
              // of combat. The ranged round does not count as the first round
              // of combat Behaviour: Retreating must occur
              isRetreating = "1";
            }
            else {
              checkIfRetreating("Standard");
            }
          }
        }
      }
      else {
        // Behaviour: Stop from this showing up if a unit dies in the monk round
        // of combat
        if ((aDeathHasOccured == false) && (isRetreating != "1")) {
          std::cout << "Skipping Standard round " + std::to_string(i + 1)
                         + " calculations..."
                    << "<br>";
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
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events,
  int* inputP1Technologies,
  int* inputP2Technologies,
  std::string inputRunConditions)
{
  constexpr std::size_t                  playerCount{2};
  const std::array<Entity*, playerCount> battleParticipants{
    &p1BattleParticipant, &p2BattleParticipant};
  const std::array<Entity*, playerCount> monks{
    &p1AssistingMonkParticipant, &p2AssistingMonkParticipant};

  float p1PointsLost = 0, p2PointsLost = 0;

  for (int round{0}; round < inputRunTimes; ++round) {
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
    p2AssistingMonkParticipant.entityQuantity
    < p2AssistingMonkParticipant.initialEntityQuantity) {
    int numberOfMonksP2Lost = p2AssistingMonkParticipant.initialEntityQuantity
                              - p2AssistingMonkParticipant.entityQuantity;
    p2PointsLost = (p2AssistingMonkParticipant.pointValue
                    / p2AssistingMonkParticipant.initialEntityQuantity)
                   * numberOfMonksP2Lost;

    std::cout << ">> " << player1Name << " gets " << p2PointsLost << " points"
              << "<br>";
  }

  if (
    p1AssistingMonkParticipant.entityQuantity
    < p1AssistingMonkParticipant.initialEntityQuantity) {
    int numberOfMonksP1Lost = p1AssistingMonkParticipant.initialEntityQuantity
                              - p1AssistingMonkParticipant.entityQuantity;
    p1PointsLost = (p1AssistingMonkParticipant.pointValue
                    / p1AssistingMonkParticipant.initialEntityQuantity)
                   * numberOfMonksP1Lost;

    std::cout << ">> " << player2Name << " gets " << p1PointsLost << " points"
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

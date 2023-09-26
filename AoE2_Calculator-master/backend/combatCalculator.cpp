/** The libaries **/
#include "combatCalculator.h" // Using: calculator class
#include "entity.h"           // Using: entity class
#include <cmath>              // Using: floor
#include <cstdlib>            // Using: exit(EXIT_FAILURE), srand(), rand()
#include <iostream>           // Using: cin, cout
#include <random>
#include <stdlib.h> // Using: atoi
#include <string>   // Using: string

// Function: The constructor/deconstructor
combatCalculator::combatCalculator()
{
  // Behaviour: Declare the initial values
  healingEffectP1        = false;
  startingQuantityP1     = 0;
  healingEffectP2        = false;
  startingQuantityP2     = 0;
  d6DieRoll              = 0;
  aDeathHasOccured       = false;
  isRetreating           = "";
  roundAttackModifiersP1 = 0;
  roundAttackModifiersP2 = 0;
  remainingDamageP1      = 0;
  remainingDamageP2      = 0;
}
combatCalculator::~combatCalculator()
{
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
              << "\n";
    exit(EXIT_FAILURE);
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
    std::cout << "d6 roll " << i << ": " << d6DieRoll << "\n";
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
              << "\n";
    exit(EXIT_FAILURE);
  }
}

// Function: Check if any of the entities have died before proceeding to the
// next round of combat
void combatCalculator::checkIfDead()
{
  if (p1BattleParticipant.entityQuantity <= 0) {
    aDeathHasOccured = true;
  }
  else if (p2BattleParticipant.entityQuantity <= 0) {
    aDeathHasOccured = true;
  }
}

// Function: Check if the attacking ranged archer is retreating
void combatCalculator::checkIfRetreating()
{
  // Behaviour: Ask the attacker if they want to retreat with their archer if
  // they are not versing cavalry or an archer
  std::cout << "For entities that can retreat: Enter 1 if retreating or 0 if "
               "fighting in the next round"
            << "\n";
  isRetreating = "0";
  // **Phillip** std::cin >> isRetreating;

  if ((isRetreating != "1") && (isRetreating != "0")) {
    std::cout << "Error: The retreating value can only be a 0 or 1"
              << "\n";
    exit(EXIT_FAILURE);
  }
}

// Function: Output the entity information with a message
void combatCalculator::outputEntityInformation(std::string inputMessage)
{
  // Behaviour: Only display the message if there is a message to be displayed
  std::cout << "\n";

  if (inputMessage != "") {
    std::cout << inputMessage << "\n";
  }

  // Behaviour: Run a function to output the entity information
  p1BattleParticipant.outputEntity(1);
  if (p1AssistingMonkParticipant.entityQuantity > 0) {
    std::cout << "(Assisting) ";
    p1AssistingMonkParticipant.outputEntity(1);
  }
  p2BattleParticipant.outputEntity(2);
  if (p2AssistingMonkParticipant.entityQuantity > 0) {
    std::cout << "(Assisting) ";
    p2AssistingMonkParticipant.outputEntity(2);
  }
}

// Function: Divide the values by the quantity (to get the individual values)
void combatCalculator::getIndividualValues()
{
  // Behaviour: Get the individual values for player 1
  if (p1BattleParticipant.entityQuantity >= 1) {
    p1BattleParticipant.rangedDamage /= p1BattleParticipant.entityQuantity;
    p1BattleParticipant.standardDamage /= p1BattleParticipant.entityQuantity;
    p1BattleParticipant.pointValue /= p1BattleParticipant.entityQuantity;
  }
  else {
    p1BattleParticipant.entityQuantity = 0;
    p1BattleParticipant.rangedDamage   = 0;
    p1BattleParticipant.standardDamage = 0;
    p1BattleParticipant.pointValue     = 0;
  }

  if (p1AssistingMonkParticipant.entityQuantity >= 1) {
    p1AssistingMonkParticipant.pointValue
      /= p1AssistingMonkParticipant.entityQuantity;
  }
  else {
    p1AssistingMonkParticipant.entityQuantity = 0;
    p1AssistingMonkParticipant.rangedDamage   = 0;
    p1AssistingMonkParticipant.standardDamage = 0;
    p1AssistingMonkParticipant.pointValue     = 0;
  }

  // Behaviour: Get the individual values for player 2
  if (p2BattleParticipant.entityQuantity >= 1) {
    p2BattleParticipant.rangedDamage /= p2BattleParticipant.entityQuantity;
    p2BattleParticipant.standardDamage /= p2BattleParticipant.entityQuantity;
    p2BattleParticipant.pointValue /= p2BattleParticipant.entityQuantity;
  }
  else {
    p2BattleParticipant.entityQuantity = 0;
    p2BattleParticipant.rangedDamage   = 0;
    p2BattleParticipant.standardDamage = 0;
    p2BattleParticipant.pointValue     = 0;
  }

  if (p2AssistingMonkParticipant.entityQuantity >= 1) {
    p2AssistingMonkParticipant.pointValue
      /= p2AssistingMonkParticipant.entityQuantity;
  }
  else {
    p2AssistingMonkParticipant.entityQuantity = 0;
    p2AssistingMonkParticipant.rangedDamage   = 0;
    p2AssistingMonkParticipant.standardDamage = 0;
    p2AssistingMonkParticipant.pointValue     = 0;
  }
}

// Function: Times the values by the quantity (to get the total values)
void combatCalculator::getTotalValues()
{
  // Behaviour: Get the total values for player 1
  if (p1BattleParticipant.entityQuantity >= 1) {
    p1BattleParticipant.rangedDamage *= p1BattleParticipant.entityQuantity;
    p1BattleParticipant.standardDamage *= p1BattleParticipant.entityQuantity;
    p1BattleParticipant.pointValue *= p1BattleParticipant.entityQuantity;
  }
  else {
    p1BattleParticipant.entityQuantity = 0;
    p1BattleParticipant.rangedDamage   = 0;
    p1BattleParticipant.standardDamage = 0;
    p1BattleParticipant.pointValue     = 0;
  }

  if (p1AssistingMonkParticipant.entityQuantity >= 1) {
    p1AssistingMonkParticipant.pointValue
      *= p1AssistingMonkParticipant.entityQuantity;
  }
  else {
    p2AssistingMonkParticipant.entityQuantity = 0;
    p2AssistingMonkParticipant.rangedDamage   = 0;
    p2AssistingMonkParticipant.standardDamage = 0;
    p2AssistingMonkParticipant.pointValue     = 0;
  }

  // Behaviour: Get the total values for player 2
  if (p2BattleParticipant.entityQuantity >= 1) {
    p2BattleParticipant.rangedDamage *= p2BattleParticipant.entityQuantity;
    p2BattleParticipant.standardDamage *= p2BattleParticipant.entityQuantity;
    p2BattleParticipant.pointValue *= p2BattleParticipant.entityQuantity;
  }
  else {
    p2BattleParticipant.entityQuantity = 0;
    p2BattleParticipant.rangedDamage   = 0;
    p2BattleParticipant.standardDamage = 0;
    p2BattleParticipant.pointValue     = 0;
  }

  if (p2AssistingMonkParticipant.entityQuantity >= 1) {
    p2AssistingMonkParticipant.pointValue
      *= p2AssistingMonkParticipant.entityQuantity;
  }
  else {
    p2AssistingMonkParticipant.entityQuantity = 0;
    p2AssistingMonkParticipant.rangedDamage   = 0;
    p2AssistingMonkParticipant.standardDamage = 0;
    p2AssistingMonkParticipant.pointValue     = 0;
  }
}

// Function: Make some final checks (after the end of the rounds of combat)
void combatCalculator::finalChecks()
{
  // Behaviour: Check if the extra unit from the monk healing power ought to die
  if (p1BattleParticipant.entityQuantity == startingQuantityP1) {
    if (healingEffectP1 == true) {
      p1BattleParticipant.entityQuantity -= 1;
    }
  }
  if (p2BattleParticipant.entityQuantity == startingQuantityP2) {
    if (healingEffectP2 == true) {
      p2BattleParticipant.entityQuantity -= 1;
    }
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
        getIndividualValues();
        p2BattleParticipant.entityQuantity -= 1;
        getTotalValues();
        remainingDamageP1 -= 1;
      }
    }
  }

  if (inputP2Events[26] == 1) {
    if (p1BattleParticipant.entityQuantity >= 1) {
      if (remainingDamageP2 >= 1) {
        getIndividualValues();
        p1BattleParticipant.entityQuantity -= 1;
        getTotalValues();
        remainingDamageP2 -= 1;
      }
    }
  }
}

// Function: The constructor / deconstructor
monkRounds::monkRounds()
{
}
monkRounds::~monkRounds()
{
}

// Function: Calculate the outcome of a monk battle
void monkRounds::roundOutcome(
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events)
{
  /* How the monk calculation process works:
    conversionRatehealingRate = Number of Monks (up to 5)
    Success if(d6DieRoll (a d6ConversionRoll) <= conversionRatehealingRate){}
  */

  // Bool: Track if either player has a monk to begin with
  bool monkPresentP1 = false, monkPresentP2 = false;

  // Bool: Track if a monk from either players did something
  bool monkPowersActivatedP1 = false, monkPowersActivatedP2 = false;

  // Integer: Declare the amount of points awarded
  int p1PointsLost = 0, p2PointsLost = 0;

  // Integer: Declare the amount of entities saved (by healing)
  int p1EntitiesHealed = 1;
  int p2EntitiesHealed = 1;

  // Behaviour: Increase the number of entities saved (by healing) if event 18
  // is active
  if (inputP1Events[18] == 1) {
    // [18] It's A Miracle - A successful Healing attempt saves us up three
    // tokens, instead of one. Play this card before an attempt is made
    p1EntitiesHealed = 3;
  }

  if (inputP2Events[18] == 1) {
    // [18] It's A Miracle - A successful Healing attempt saves us up three
    // tokens, instead of one. Play this card before an attempt is made
    p2EntitiesHealed = 3;
  }

  // Integer: Store the conversion rates for either the assisting or primary
  // monks (makes things a bit easier to understand)
  int  conversionRatehealingRateP1 = 0, conversionRatehealingRateP2 = 0;


  bool assistingMonksP1 = false, assistingMonksP2 = false;
  bool justMonksP1 = false, justMonksP2 = false;

  // Behaviour: Run the monk battle round for X times
  for (int i = 0; i < inputRunTimes; i++) {
    // Behaviour: Check that no deaths have occured
    checkIfDead();

    // Behaviour: Make sure that no deaths have occured before proceeding
    if (aDeathHasOccured == false) {
      // Behaviour: See if player 1 has a monk
      if (
        ((p1AssistingMonkParticipant.armorClass[9] == true)
         && (p1AssistingMonkParticipant.entityQuantity > 0))
        || (p1BattleParticipant.armorClass[9] == true)) {
        // Behaviour: See what monk is in the battle (a primary or assisting
        // monk)
        if (p1BattleParticipant.entityQuantity > 0) {
          conversionRatehealingRateP1 = p1BattleParticipant.entityQuantity;
          assistingMonksP1 = false;
          justMonksP1      = true;
        }
        else if (p1AssistingMonkParticipant.entityQuantity > 0) {
          conversionRatehealingRateP1 = p1AssistingMonkParticipant.entityQuantity;
          assistingMonksP1 = true;
          justMonksP1      = false;
        }
        monkPresentP1 = true;
      }
      else {
        monkPresentP1 = false;
      }

      // Behaviour: See if player 2 has a monk
      if (
        ((p2AssistingMonkParticipant.armorClass[9] == true)
         && (p2AssistingMonkParticipant.entityQuantity > 0))
        || (p2BattleParticipant.armorClass[9] == true)) {
        // Behaviour: See what monk is in the battle (a primary or assisting
        // monk)
        if (p2BattleParticipant.entityQuantity > 0) {
          conversionRatehealingRateP2 = p2BattleParticipant.entityQuantity;
          assistingMonksP2 = false;
          justMonksP2      = true;
        }
        if (p2AssistingMonkParticipant.entityQuantity > 0) {
          conversionRatehealingRateP2 = p2AssistingMonkParticipant.entityQuantity;
          assistingMonksP2 = true;
          justMonksP2      = false;
        }
        monkPresentP2 = true;
      }
      else {
        monkPresentP2 = false;
      }

      // Behaviour: If player 1 plays event 7 then player 2's monk is virtually
      // non-present
      if (inputP1Events[7] == 1) {
        // [7] Fat Friars Tavern O'Spirits - Monks on target unit may not have
        // any Conversion or Healing attempts this turn
        monkPresentP2 = false;
      }

      // Behaviour: If player 2 plays event 7 then player 1's monk is virtually
      // non-present
      if (inputP2Events[7] == 1) {
        // [7] Fat Friars Tavern O'Spirits - Monks on target unit may not have
        // any Conversion or Healing attempts this turn
        monkPresentP1 = false;
      }

      // Behaviour: If player 1's monk is present then proceed
      if (monkPresentP1 == true) {
        // Behaviour: Ask the user if they are performing a conversion or
        // healing attempt and store the answer
        std::string calculationModeP1 = "0";
        std::cout << "\n"
                  << "Is player 1's monk performing a conversion (enter 0) or "
                     "healing attempt (enter 1)?"
                  << "\n";
        std::cin >> calculationModeP1;

        // Behaviour: Validate the input before proceeding
        if ((calculationModeP1 != "0") && (calculationModeP1 != "1")) {
          std::cout << "Error: The input must be 0 or 1 for player 1's "
                       "conversion/healing attempt"
                    << "\n";
          exit(EXIT_FAILURE);
        }

        // Behaviour: Generate, validate, and return d6 dice input before
        // proceeding
        d6DieRoll = generateD6DieInput();

        if (inputP1Events[39] == 1) {
          // [39] Zealous Monks - Target Monk unit gets 1 conversion roll at a 3
          // or less rate for each monk attached In other words, you reroll the
          // dice if the number is <= 3 You repeat the roll for each monk
          // attached

          // Behaviour: Check if the are 'assisting monks'
          if (assistingMonksP1 == true) {
            for (int i = 0; i != p1AssistingMonkParticipant.entityQuantity;
                 i++) {
              if (d6DieRoll <= 3) {
                d6DieRoll = generateD6DieInput();
              }
            }
          }

          // Behaviour: Check if there are just monks
          if (justMonksP1 == true) {
            for (int i = 0; i != p1BattleParticipant.entityQuantity; i++) {
              if (d6DieRoll <= 3) {
                d6DieRoll = generateD6DieInput();
              }
            }
          }
        }


        // Behaviour: Apply the effects of Back From A Foreign Land (Byzantine civ bonus: +2 healing rate modifier)
        if(inputP1Events[1] == 1){
          if(calculationModeP1 == "1"){
            conversionRatehealingRateP1 += 2;
          }
        }



        // Behaviour: Apply the effects of Back From A Foreign Land (Teuton civ bonus: Conversion rate modifier is -1)



        if(inputP2Events[40] == 1){
          if(calculationModeP1 == "0"){
            conversionRatehealingRateP1 -= 1;
          }
        }


        // Behaviour: Apply the effects of event 23
        if (inputP1Events[23] == 1) {
          // [23] Piety - If you have one monk attached to a unit, your
          // conversion rate is 4. If the attempt fails, the monk dies Reference
          // "Conversion Rate" = "Quantity of the unit"
          const int pietyConversionRate = 4;

          // Behaviour: The case where there are 'assisting monks'
          if (assistingMonksP1 == true) {
            // Behaviour: Check if the conversion attempt failed
            if (pietyConversionRate <= d6DieRoll) {
              getIndividualValues();
              p1AssistingMonkParticipant.entityQuantity -= 1;
              getTotalValues();
            }
            else {
              conversionRatehealingRateP1 = pietyConversionRate;
            }
          }
          // Behaviour: The case where there are just monks
          else if (justMonksP1 == true) {
            // Behaviour: Check if the conversion attempt failed
            if (pietyConversionRate <= d6DieRoll) {
              getIndividualValues();
              p1BattleParticipant.entityQuantity -= 1;
              getTotalValues();
            }
            else {
              conversionRatehealingRateP1 = pietyConversionRate;
            }
          }
        }

        // Behaviour: Give the player the option to roll the die again if they
        // have event 19
        if (inputP1Events[19] == 1) {
          // [19] Listen To A Story - Attempt 1-3 extra conversions this battle.
          // Pay 2 gold per conversion

          // Integer: Store the number of times that the while loop runs
          int timesRunP1
            = 1; // Already rolled the dice once (so timesRunP1 = 1)

          // String: Store the response on whether to keep running the while
          // loop
          std::string getP1Response = "";

          // Integer: Store the number of conversion attempts
          int extraConversionAttemptsP1 = 0;

          while (timesRunP1 != 4) { // 1 + 3 (extra conversions) = 4
            // Behaviour: Check if there are 'assisting monks'
            if (assistingMonksP1 == true) {
              // Behaviour: Check if the conversion attempt failed
              if (conversionRatehealingRateP1 <= d6DieRoll) {
                std::cout << "The conversion attempt for p1 failed. Pay 2 gold "
                             "and enter 1 to try again. Otherwise enter 0"
                          << "\n";
                std::cin >> getP1Response;
              }
            }
            // Behaviour: Check if there are just monks
            else if (justMonksP1 == true) {
              // Behaviour: Check if the conversion attempt failed
              if (conversionRatehealingRateP1 <= d6DieRoll) {
                std::cout << "The conversion attempt for p1 failed. Pay 2 gold "
                             "and enter 1 to try again. Otherwise enter 0"
                          << "\n";
                std::cin >> getP1Response;
              }
            }

            // Behaviour: Act on the user response
            if (getP1Response == "1") {
              d6DieRoll = generateD6DieInput();
              extraConversionAttemptsP1++;
            }
            else if (getP1Response == "0") {
              timesRunP1 = 3;
            }

            timesRunP1 = timesRunP1 + 1;
          }

          // Behaviour: Calculate the cost of the extra conversion attempts if
          // appropriate
          if (extraConversionAttemptsP1 > 0) {
            int extraConversionAttemptCostP1 = 2 * extraConversionAttemptsP1;
            std::cout << "<< Player 1 must pay "
                      << std::to_string(extraConversionAttemptCostP1) << " gold"
                      << "\n";
          }
        }

        /* Main calculations start  here */
        // Behaviour: Cover the case where there are no 'assisting' monks
        if (justMonksP1 == true) {
          if (conversionRatehealingRateP1 <= d6DieRoll) {
            // Behaviour: Return the fact that the attempt was successful
            monkPowersActivatedP1 = true;


          }
          else {
            // Behaviour: Return the fact that the attempt was unsuccessful
            monkPowersActivatedP1 = false;
            std::cout << "Player 1's monk powers failed to activate"
                      << "\n";
          }

          // Behaviour: Act on the success odds
          if ((monkPowersActivatedP1 == true) && (calculationModeP1 == "0")) {
            // Behaviour: Make sure that the conversion attempt is not being
            // applied to siege units
            if (p2BattleParticipant.armorClass[12] == true) {
              std::cout << "Error: Player 1's conversion attempt cannot be "
                           "applied to player 2's siege unit"
                        << "\n";
              exit(EXIT_FAILURE);
            }

            // Behaviour: Act on the conversion attempt being successful
            p2PointsLost
              = (p2BattleParticipant.pointValue / p2BattleParticipant.entityQuantity);
            getIndividualValues();
            p2BattleParticipant.entityQuantity -= 1;
            getTotalValues();

            // Play a SFX for a successful conversion attempt
              // SFXToPlay("/sfx/rng/successful_monk_conversion_attempt_sfx.wav");
          }
          else if (
            (monkPowersActivatedP1 == true) && (calculationModeP1 == "1")) {
            // Behaviour: Act on the healing attempt being successful
            healingEffectP1    = true;
            startingQuantityP1 = p1BattleParticipant.entityQuantity;
            getIndividualValues();
            p1BattleParticipant.entityQuantity += p1EntitiesHealed;
            getTotalValues();

            // Play a SFX for a successful healing attempt
            // SFXToPlay("/sfx/rng/successful_monk_healing_attempt_sfx.wav");
          }
        }
        // Behaviour: Cover the case where there are 'assisting monks'
        else if (assistingMonksP1 == true) {
          if (conversionRatehealingRateP1 <= d6DieRoll) {
            // Behaviour: Return the fact that the attempt was successful
            monkPowersActivatedP1 = true;
          }
          else {
            // Behaviour: Return the fact that the attempt was unsusscessful
            monkPowersActivatedP1 = false;
            std::cout << "Player 1's 'assisting' monk powers failed to activate"
                      << "\n";
          }

          // Behaviour: Act on the success odds
          if ((monkPowersActivatedP1 == true) && (calculationModeP1 == "0")) {
            // Behaviour: Ask if player 1's monk is targeting player 2's
            // non-monk or monk Behaviour: Only ask this question if the
            // 'assisting monk' for player 2 is > 1
            std::string p1AssistingMonkTarget = "0";

            if (assistingMonksP1 == true) {
              std::cout << "Is player 1's 'assistant monk' targeting player "
                           "2's assisting monk? Enter 1 for yes. Enter 0 for no"
                        << "\n";
              std::cin >> p1AssistingMonkTarget;
            }

            // Behaviour: Validate the input before proceeding
            if (
              (p1AssistingMonkTarget != "0")
              && (p1AssistingMonkTarget != "1")) {
              std::cout << "Error: The input must be 0 or 1"
                        << "\n";
              exit(EXIT_FAILURE);
            }

            // Behaviour: Act on the conversion attempt being successful based
            // on the p1AssistingMonkTarget input
            if (p1AssistingMonkTarget == "0") {
              // Behaviour: Make sure that the conversion attempt is not being
              // applied to siege units
              if (p2BattleParticipant.armorClass[12] == true) {
                std::cout << "Error: Player 1's conversion attempt cannot be "
                             "applied to player 2's siege unit"
                          << "\n";
                exit(EXIT_FAILURE);
              }

              // Behaviour: Convert the combat participant
              p2PointsLost
                = (p2BattleParticipant.pointValue / p2BattleParticipant.entityQuantity);
              getIndividualValues();
              p2BattleParticipant.entityQuantity -= 1;
              getTotalValues();
            }
            else if (p1AssistingMonkTarget == "1") {
              // Behaviour: Convert the monk participant
              p2PointsLost = (p2AssistingMonkParticipant.pointValue / p2AssistingMonkParticipant.entityQuantity);
              getIndividualValues();
              p2AssistingMonkParticipant.entityQuantity -= 1;
              getTotalValues();
            }
          }
          else if (
            (monkPowersActivatedP1 == true) && (calculationModeP1 == "1")) {
            // Behaviour: Act on the healing attempt being successful
            healingEffectP1    = true;
            startingQuantityP1 = p1BattleParticipant.entityQuantity;
            getIndividualValues();
            p1BattleParticipant.entityQuantity += p1EntitiesHealed;
            getTotalValues();
          }
        }
      }

      // Behaviour: If player 2's monk is present then proceed
      if (monkPresentP2 == true) {
        // Behaviour: Ask the user if they are performing a conversion or
        // healing attempt and store the answer
        std::string calculationModeP2 = "0";
        std::cout << "Is player 2's monk performing a conversion (enter 0) or "
                     "healing attempt (enter 1)?"
                  << "\n";
        std::cin >> calculationModeP2;

        // Behaviour: Validate the input before proceeding
        if ((calculationModeP2 != "0") && (calculationModeP2 != "1")) {
          std::cout << "Error: The input must be 0 or 1 for player 2's "
                       "conversion/healing attempt"
                    << "\n";
          exit(EXIT_FAILURE);
        }

        // Behaviour: Generate, validate, and return d6 dice input before
        // proceeding
        d6DieRoll = generateD6DieInput();


        // Behaviour: Apply the effects of Back From A Foreign Land (Teuton civ bonus: Conversion rate modifier is -1)

        if(inputP1Events[40] == 1){
          if(calculationModeP2 == "0"){
            conversionRatehealingRateP2 -= 1;
          }
        }

        // Behaviour: Apply the effects of Back From A Foreign Land (Byzantine civ bonus: +2 healing rate modifier)
        if(inputP2Events[1] == 1){
          if(calculationModeP2 == "1"){
            conversionRatehealingRateP2 += 2;
          }
        }



        if (inputP2Events[39] == 1) {
          // [39] Zealous Monks - Target Monk unit gets 1 conversion roll at a 3
          // or less rate for each monk attached In other words, you reroll the
          // dice if the number is <= 3 You repeat the roll for each monk
          // attached

          // Behaviour: Check if the are 'assisting monks'
          if (assistingMonksP2 == true) {
            for (int i = 0; i != p2AssistingMonkParticipant.entityQuantity;
                 i++) {
              if (d6DieRoll <= 3) {
                d6DieRoll = generateD6DieInput();
              }
            }
          }

          // Behaviour: Check if there are just monks
          if (justMonksP2 == true) {
            for (int i = 0; i != p2BattleParticipant.entityQuantity; i++) {
              if (d6DieRoll <= 3) {
                d6DieRoll = generateD6DieInput();
              }
            }
          }
        }

        if (inputP2Events[23] == 1) {
          // [23] Piety - If you have one monk attached to a unit, your
          // conversion rate is 4. If the attempt fails, the monk dies Reference
          // "Conversion Rate" = "Quantity of the unit"
          int pietyconversionRatehealingRateP2 = 4;

          // Behaviour: Check if there are 'assisting monks'
          if (assistingMonksP2 == true) {
            // Behaviour: Check if the conversion attempt failed
            if (pietyconversionRatehealingRateP2 <= d6DieRoll) {
              getIndividualValues();
              p2AssistingMonkParticipant.entityQuantity -= 1;
              getTotalValues();
            }
            else {
              conversionRatehealingRateP2 = pietyconversionRatehealingRateP2;
            }
          }
          // Behaviour: Check if there are just monks
          else if (justMonksP2 == true) {
            // Behaviour: Check if the conversion attempt failed
            if (pietyconversionRatehealingRateP2 <= d6DieRoll) {
              getIndividualValues();
              p2BattleParticipant.entityQuantity -= 1;
              getTotalValues();
            }
            else {
              conversionRatehealingRateP2 = pietyconversionRatehealingRateP2;
            }
          }
        }

        // Behaviour: Give the player the option to roll the die again if player
        // 2 has event 19
        if (inputP2Events[19] == 1) {
          // [19] Listen To A Story - Attempt 1-3 extra conversions this battle.
          // Pay 2 gold per conversion

          // Integer: Store the number of times that the while loop runs
          int timesRunP2
            = 1; // Already rolled the dice once (so timesRunP2 = 1)

          // String: Store the response on whether to keep running the while
          // loop
          std::string getP2Response = "";

          // Integer: Store the number of conversion attempts
          int extraConversionAttemptsP2 = 0;

          while (timesRunP2 != 4) { // 1 + 3 (extra conversions) = 4
            // Behaviour: Check if there are 'assisting monks'
            if (assistingMonksP2 == true) {
              // Behaviour: Check if the conversion attempt failed
              if (conversionRatehealingRateP2 <= d6DieRoll) {
                std::cout << "The conversion attempt for p2 failed. Pay 2 gold "
                             "and enter 1 to try again. Otherwise enter 0"
                          << "\n";
                std::cin >> getP2Response;
              }
            }
            // Behaviour: Check if there are just monks
            else if (justMonksP2 == true) {
              // Behaviour: Check if the conversion attempt failed
              if (conversionRatehealingRateP2 < d6DieRoll) {
                std::cout
                  << "The conversion attempt for p2 failed failed. Pay 2 gold "
                     "and enter 1 to try again. Otherwise enter 0"
                  << "\n";
                std::cin >> getP2Response;
              }
            }

            // Behaviour: Act on the user response
            if (getP2Response == "1") {
              d6DieRoll = generateD6DieInput();
              extraConversionAttemptsP2++;
            }
            else if (getP2Response == "0") {
              timesRunP2 = 3;
            }

            timesRunP2 = timesRunP2 + 1;
          }

          // Behaviour: Calculate the cost of the extra conversion attempts if
          // appropriate
          if (extraConversionAttemptsP2 > 0) {
            int extraConversionAttemptCostP2 = 2 * extraConversionAttemptsP2;
            std::cout << "<< Player 2 must pay "
                      << std::to_string(extraConversionAttemptCostP2) << " gold"
                      << "\n";
          }
        }





        // Behaviour: Cover the case where there are just monks

        if (justMonksP2 == true) {
          if (conversionRatehealingRateP2 <= d6DieRoll) {
            // Behaviour: Return the fact that the attempt was successful
            monkPowersActivatedP2 = true;
          }
          else {
            // Behaviour: Return the fact that the attempt was unsuccessful
            monkPowersActivatedP2 = false;
            std::cout << "Player 2's monk powers failed to activate"
                      << "\n";
          }

          // Behaviour: Act on the success odds
          if ((monkPowersActivatedP2 == true) && (calculationModeP2 == "0")) {
            // Behaviour: Make sure that the conversion attempt is not being
            // applied to siege units
            if (p1BattleParticipant.armorClass[12] == true) {
              std::cout << "Error: Player 2's conversion attempt cannot be "
                           "applied to player 1's siege unit"
                        << "\n";
              exit(EXIT_FAILURE);
            }

            // Behaviour: Act on the conversion attempt being successful
            p1PointsLost
              = (p1BattleParticipant.pointValue / p1BattleParticipant.entityQuantity);
            getIndividualValues();
            p1BattleParticipant.entityQuantity -= 1;
            getTotalValues();

            // Play a SFX for a successful conversion attempt
            // SFXToPlay("/sfx/rng/successful_monk_conversion_attempt_sfx.wav");
          }
          else if (
            (monkPowersActivatedP2 == true) && (calculationModeP2 == "1")) {
            // Behaviour: Act on the healing attempt being successful
            healingEffectP2    = true;
            startingQuantityP2 = p2BattleParticipant.entityQuantity;
            getIndividualValues();
            p2BattleParticipant.entityQuantity += p2EntitiesHealed;
            getTotalValues();

            // Play a SFX for a successful healing attempt
            // SFXToPlay("/sfx/rng/successful_monk_healing_attempt_sfx.wav");
          }
        }
        // Behaviour: Cover the case where there are 'assisting monks'
        else if (assistingMonksP2 == true) {
          if (conversionRatehealingRateP2 <= d6DieRoll) {
            // Behaviour: Return the fact that the attempt was successful
            monkPowersActivatedP2 = true;
          }
          else {
            // Behaviour: Return the fact that the attempt was unsusscessful
            monkPowersActivatedP2 = false;
            std::cout << "Player 2's 'assisting' monk powers failed to activate"
                      << "\n";
          }

          // Behaviour: Act on the success odds
          if ((monkPowersActivatedP2 == true) && (calculationModeP2 == "0")) {
            // Behaviour: Ask if player 2's monk is targeting player 1's
            // non-monk or monk Behaviour: Only ask this question if the
            // 'assisting monk' for player 1 is > 1 quantity
            std::string p2AssistingMonkTarget = "0";

            if (assistingMonksP2 == true) {
              std::cout << "Is player 2's 'assistant monk' targeting player "
                           "1's assisting monk? Enter 1 for yes. Enter 0 for no"
                        << "\n";
              std::cin >> p2AssistingMonkTarget;
            }

            // Behaviour: Validate the input before proceeding
            if (
              (p2AssistingMonkTarget != "0")
              && (p2AssistingMonkTarget != "1")) {
              std::cout << "Error: The input must be 0 or 1"
                        << "\n";
              exit(EXIT_FAILURE);
            }

            // Behaviour: Act on the conversion attempt being successful based
            // on the p2AssistingMonkTarget input
            if (p2AssistingMonkTarget == "0") {
              // Behaviour: Make sure that the conversion attempt is not being
              // applied to siege units
              if (p1BattleParticipant.armorClass[12] == true) {
                std::cout << "Error: Player 2's conversion attempt cannot be "
                             "applied to player 1's siege unit"
                          << "\n";
                exit(EXIT_FAILURE);
              }

              // Behaviour: Convert the combat participant
              p1PointsLost
                = (p1BattleParticipant.pointValue / p1BattleParticipant.entityQuantity);
              getIndividualValues();
              p1BattleParticipant.entityQuantity -= 1;
              getTotalValues();
            }
            else if (p2AssistingMonkTarget == "1") {
              // Behaviour: Convert the monk participant
              p1PointsLost = (p1AssistingMonkParticipant.pointValue / p1AssistingMonkParticipant.entityQuantity);
              getIndividualValues();
              p1AssistingMonkParticipant.entityQuantity -= 1;
              getTotalValues();
            }
          }
          else if (
            (monkPowersActivatedP2 == true) && (calculationModeP2 == "1")) {
            // Behaviour: Act on the healing attempt being successful
            healingEffectP2    = true;
            startingQuantityP2 = p2BattleParticipant.entityQuantity;
            p2BattleParticipant.entityQuantity += p2EntitiesHealed;
          }
        }
      }

      // Behaviour: Display the outcome of the monk combat round only if changes
      // occured
      if ((monkPowersActivatedP1 == true) || (monkPowersActivatedP2 == true)) {
        std::string outputString
          = "Monk round " + std::to_string(i + 1) + " calculations...";
        outputEntityInformation(outputString);

        // Behaviour: Display how many points were added if appropriate
        if (p2PointsLost != 0) {
          std::cout << ">> Player 1 gets " << p2PointsLost << " points"
                    << "\n";
        }
        if (p1PointsLost != 0) {
          std::cout << ">> Player 2 gets " << p1PointsLost << " points"
                    << "\n";
        }

        std::cout << "\n";
      }
      else {
        /* No longer show if a phase is skipped to conserve space on the window
        std::cout << "\n"
                  << "Skipping Monk round " + std::to_string(i + 1)
                       + " calculations..."
                  << "\n";
      */
      }

      // Behaviour: Check if a death has occured
      checkIfDead();
    } // End if conditional checking for no deaths
  }   // End for loop
} // End monk function

// Functions: The constructor and deconstructor
archerRounds::archerRounds()
{
}

archerRounds::~archerRounds()
{
}

// Function: Calculate the outcome of a ranged battle
void archerRounds::roundOutcome(
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events)
{
  // Bool: Track if a ranged unit from either players did something
  bool p1ArcherPresent = false, p2ArcherPresent = false;

  // Integer: Declare the number of units killed in this round
  int p1EntityDeaths = 0, p2EntityDeaths = 0;

  // Integer: Declare the amount of damage dealt to buildings in this round
  int p1BuildingDamage = 0, p2BuildingDamage = 0, roundDownMultiple = 10;

  // Integer: Get whether the archer entity is fighting a building
  bool isP1FightingBuilding = false, isP2FightingBuilding = false;

  // Integer: Declare the amount of damage die to place onto buildings
  int p1DamageDie = 0, p2DamageDie = 0;

  // Integer: Declare the amount of points awarded
  float p1PointsLost = 0, p2PointsLost = 0;

  // Integer: Track if changes occured to the quantity
  int p1StartingQuantity = 0, p1EndingQuantity = 0;
  int p2StartingQuantity = 0, p2EndingQuantity = 0;

  // Bool: Negate the non-cavalry requirement
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
          getIndividualValues();

          // Behaviour: Now decrease the quantity
          p2BattleParticipant.entityQuantity -= p2EntityDeaths;

          // Behaviour: Now multiply the rangedDamage, SD and pointValue by the
          // new quantity
          getTotalValues();

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
          getIndividualValues();

          // Behaviour: Now decrease the quantity
          p1BattleParticipant.entityQuantity -= p1EntityDeaths;

          // Behaviour: Now multiply the rangedDamage, SD and pointValue by the
          // new quantity
          getTotalValues();

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
        std::cout << ">> Place " << p2DamageDie
                  << " damage die onto player 2's "
                  << p2BattleParticipant.entityName << "\n";
      }
      else if (isP2FightingBuilding == true) {
        std::cout << ">> Place " << p1DamageDie
                  << " damage die onto player 1's "
                  << p1BattleParticipant.entityName << "\n";
      }

      // Behaviour: Display how many points were added if appropriate
      if (p2PointsLost != 0) {
        std::cout << ">> Player 1 gets " << p2PointsLost << " points"
                  << "\n";
      }
      if (p1PointsLost != 0) {
        std::cout << ">> Player 2 gets " << p1PointsLost << " points"
                  << "\n";
      }

      std::cout << "\n";

      // Behaviour: Check if a death has occured
      checkIfDead();

      // Behaviour: Check if the attacking archer is retreating
      if (aDeathHasOccured == false) {
        checkIfRetreating();

        if ((inputP1Events[29] == 1) || (inputP2Events[29] == 1)) {
          //  [29] Shots In The Back (Briton) - If a unit in combat with your
          //  Archer retreats, your Archer deals an additional round of range
          //  damage without taking any damage
          std::string getAnswer = "";

          std::cout << "Did the conditions of Shots_In_The_Back_(Briton) get "
                       "satisfied for player 1 or 2? Enter 1 for yes. 0 for no"
                    << "\n";
          std::cin >> getAnswer;

          if (getAnswer == "1") {
            // Behaviour: Have another round of ranged combat
            inputRunTimes++;
          }
        }
      }
    }
    else {
        /* No longer show if a phase is skipped to conserve space on the window
      std::cout << "\n"
                << "Skipping Archer round " + std::to_string(i + 1)
                     + " calculations..."
                << "\n";
*/
    }
  }
}

// Functions: The constructor and deconstructor
bombardmentRounds::bombardmentRounds()
{
}

bombardmentRounds::~bombardmentRounds()
{
}

// Function: Calculate the outcome of a bombardment round of battle (only for
// the Crow's nest event card)
void bombardmentRounds::roundOutcome(
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events)
{
  // Bool: Track if the bombardment attack round got activated
  bool bombardmentRoundActivated = false;

  // Integer: Declare the number of units killed in this round
  int p1EntityDeaths = 0, p2EntityDeaths = 0;

  // Integer: Declare the amount of damage dealt to buildings in this round
  int p1BuildingDamage = 0, p2BuildingDamage = 0, roundDownMultiple = 10;

  // Integer: Get whether the bombardment entity is fighting a building
  bool isP1FightingBuilding = false, isP2FightingBuilding = false;

  // Integer: Declare the amount of damage die to place onto buildings
  int p1DamageDie = 0, p2DamageDie = 0;

  // Integer: Declare the amount of points awarded
  float p1PointsLost = 0, p2PointsLost = 0;

  // Integer: Track if changes occured to the quantity
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
          (p1BattleParticipant.armorClass[22] == true)
          || (p1BattleParticipant.armorClass[22] == true)) {
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
          (p2BattleParticipant.armorClass[22] == true)
          || (p2BattleParticipant.armorClass[22] == true)) {
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
          getIndividualValues();

          // Behaviour: Now decrease the quantity
          p2BattleParticipant.entityQuantity -= p2EntityDeaths;

          // Behaviour: Now multiply the RD, standardDamage and pointValue by
          // the new quantity
          getTotalValues();

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
          getIndividualValues();

          // Behaviour: Now decrease the quantity
          p1BattleParticipant.entityQuantity -= p1EntityDeaths;

          // Behaviour: Now multiply the RD, standardDamage and pointValue by
          // the new quantity
          getTotalValues();

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
        std::cout << ">> Place " << p2DamageDie
                  << " damage die onto player 2's "
                  << p2BattleParticipant.entityName << "\n";
      }
      else if (isP2FightingBuilding == true) {
        std::cout << ">> Place " << p1DamageDie
                  << " damage die onto player 1's "
                  << p1BattleParticipant.entityName << "\n";
      }

      // Behaviour: Display how many points were added if appropriate
      if (p2PointsLost != 0) {
        std::cout << ">> Player 1 gets " << p2PointsLost << " points"
                  << "\n";
      }
      if (p1PointsLost != 0) {
        std::cout << ">> Player 2 gets " << p1PointsLost << " points"
                  << "\n";
      }

      std::cout << "\n";

      // Behaviour: Check if a death has occured
      checkIfDead();

      // Behaviour: Check if the attacking entity is retreating if this is not
      // the last round
      if (aDeathHasOccured == false) {
        if (i != inputRunTimes - 1) {
          checkIfRetreating();
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
          << "\n";
      }
    }
  }
}

// Functions: The constructor and deconstructor
standardRounds::standardRounds()
{
}

standardRounds::~standardRounds()
{
}

// Function: Calculate the outcome of a standard round of battle
void standardRounds::roundOutcome(
  int  inputRunTimes,
  int* inputP1Events,
  int* inputP2Events)
{
  // Bool: Track if the standard attack round got activated
  bool standardRoundActivated = false;

  // Integer: Declare the number of units killed in this round
  int p1EntityDeaths = 0, p2EntityDeaths = 0;

  // Integer: Declare the amount of damage dealt to buildings in this round
  int p1BuildingDamage = 0, p2BuildingDamage = 0, roundDownMultiple = 10;

  // Integer: Get whether the archer entity is fighting a building
  bool isP1FightingBuilding = false, isP2FightingBuilding = false;

  // Integer: Declare the amount of damage die to place onto buildings
  int p1DamageDie = 0, p2DamageDie = 0;

  // Integer: Declare the amount of points awarded
  float p1PointsLost = 0, p2PointsLost = 0;

  // Integer: Track if changes occured to the quantity
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
    (p1BattleParticipant.armorClass[23] == true)
    || (p1BattleParticipant.armorClass[23] == true)) { // Fire Ship or Fast Fire
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
    (p2BattleParticipant.armorClass[23] == true)
    || (p2BattleParticipant.armorClass[23] == true)) { // Fire Ship or Fast Fire
                                                       // Ship
    p2BattleParticipant.standardDamage *= d6DieRoll;
  }

  // Behaviour: Run the standard battle round for X times
  for (int i = 0; i < inputRunTimes; i++) {
     // Give the player who played the card a weakened defender
    // [24] Black Knight: “Play this card when you are the attacking Cavalry unit.
    // Two tokens on the defending unit have 0 AP for the first round of normal combat
    if (inputP1Events[24] == 1){
      int getHowMuchAttackIndividualUnitsDo;
      int getHowMuchAttackTheGroupDoesWithTwoLazyUnits;

      // If there's cavalry in play
      if(p1BattleParticipant.armorClass[4]){
        // If it's round 1
        if(i == 0){
          // Do not re-calculate this value again in the second round or you're going to get a result that doesn't reflect the individual attack of the unit (it will be divided too much)
          getHowMuchAttackIndividualUnitsDo = (p2BattleParticipant.standardDamage / p2BattleParticipant.entityQuantity);

          if(p2BattleParticipant.entityQuantity > 2){
            getHowMuchAttackTheGroupDoesWithTwoLazyUnits = (p2BattleParticipant.entityQuantity - 2) * getHowMuchAttackIndividualUnitsDo;
            p2BattleParticipant.standardDamage = getHowMuchAttackTheGroupDoesWithTwoLazyUnits;
          }
          else{
            getHowMuchAttackTheGroupDoesWithTwoLazyUnits = 0;
            p2BattleParticipant.standardDamage = getHowMuchAttackTheGroupDoesWithTwoLazyUnits;
          }
        }
        // If it's round 2
        else if(i == 1){
          p2BattleParticipant.standardDamage = getHowMuchAttackIndividualUnitsDo * p2BattleParticipant.entityQuantity;
        }
      }
    }

    if (inputP2Events[24] == 1){
      int getHowMuchAttackIndividualUnitsDo2;
      int getHowMuchAttackTheGroupDoesWithTwoLazyUnits2;

             // If there's cavalry in play
      if(p2BattleParticipant.armorClass[4]){
        // If it's round 1
        if(i == 0){
          // Do not re-calculate this value again in the second round or you're going to get a result that doesn't reflect the individual attack of the unit (it will be divided too much)
          getHowMuchAttackIndividualUnitsDo2 = (p1BattleParticipant.standardDamage / p1BattleParticipant.entityQuantity);

          if(p1BattleParticipant.entityQuantity > 2){
            getHowMuchAttackTheGroupDoesWithTwoLazyUnits2 = (p1BattleParticipant.entityQuantity - 2) * getHowMuchAttackIndividualUnitsDo2;
            p1BattleParticipant.standardDamage = getHowMuchAttackTheGroupDoesWithTwoLazyUnits2;
          }
          else{
            getHowMuchAttackTheGroupDoesWithTwoLazyUnits2 = 0;
            p1BattleParticipant.standardDamage = getHowMuchAttackTheGroupDoesWithTwoLazyUnits2;
          }
        }
        // If it's round 2
        else if(i == 1){
          p1BattleParticipant.standardDamage = getHowMuchAttackIndividualUnitsDo2 * p1BattleParticipant.entityQuantity;
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
      // Behaviour: Calculate the damage against units if player 1 is fighting a
      // unit
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
      // Behaviour: Calculate the damage against units if player 2 is fighting a
      // unit
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
      (p1BattleParticipant.onlyAttacksInTheSecondRoundOfStandardCombat == true)
      && (i + 1 != 2)) {
      p2BuildingDamage = 0;
      p2DamageDie      = 0;
      p2EntityDeaths   = 0;
    }

    if (
      (p2BattleParticipant.onlyAttacksInTheSecondRoundOfStandardCombat == true)
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
          getIndividualValues();

          // Behaviour: Now decrease the quantity
          p2BattleParticipant.entityQuantity -= p2EntityDeaths;

          // Behaviour: Now multiply the RD, standardDamage and pointValue by
          // the new quantity
          getTotalValues();

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
      if (standardRoundActivated == true) {
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
          getIndividualValues();

          // Behaviour: Now decrease the quantity
          p1BattleParticipant.entityQuantity -= p1EntityDeaths;

          // Behaviour: Now multiply the RD, standardDamage and pointValue by
          // the new quantity
          getTotalValues();

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

      // Apply the effects of certain entities that automatically die in the
      // second round of combat Make sure that the standard round got activated
      // to avoid an arithmetic exception
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

    // Behaviour: Display the outcome of the archer combat round only if changes
    // occured
    if (standardRoundActivated == true) {
      std::string outputString = "Standard round "
                                 + std::to_string(i + 1) + " calculations...";
      outputEntityInformation(outputString);

      // Behaviour: Display how many damage die to place if appropriate
      if (isP1FightingBuilding == true) {
        std::cout << ">> Place " << p2DamageDie
                  << " damage die onto player 2's "
                  << p2BattleParticipant.entityName << "\n";
      }
      else if (isP2FightingBuilding == true) {
        std::cout << ">> Place " << p1DamageDie
                  << " damage die onto player 1's "
                  << p1BattleParticipant.entityName << "\n";
      }

      // Behaviour: Display how many points were added if appropriate
      if (p2PointsLost != 0) {
        std::cout << ">> Player 1 gets " << p2PointsLost << " points"
                  << "\n";
      }
      if (p1PointsLost != 0) {
        std::cout << ">> Player 2 gets " << p1PointsLost << " points"
                  << "\n";
      }

      std::cout << "\n";

      // Behaviour: Deal with event 34
      if (inputP1Events[34] == 1) {
        // [34] The Hammer's Cavalry (Franks) - Play when one of your Cavalry
        // units is reduced to 1 token. Add two tokens to the unit at no cost
        if (p1BattleParticipant.armorClass[4] == true) { // Cavalry
          if (p1BattleParticipant.entityQuantity == 1) {
            getIndividualValues();
            p1BattleParticipant.entityQuantity += 2;
            getTotalValues();
          }
        }
      }

      if (inputP2Events[34] == 1) {
        // [34] The Hammer's Cavalry (Franks) - Play when one of your Cavalry
        // units is reduced to 1 token. Add two tokens to the unit at no cost
        if (p2BattleParticipant.armorClass[4] == true) { // Cavalry
          if (p2BattleParticipant.entityQuantity == 1) {
            getIndividualValues();
            p2BattleParticipant.entityQuantity += 2;
            getTotalValues();
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
            // [27] Retreat - Target unit must retreat before the first round of
            // combat. The ranged round does not count as the first round of
            // combat Behaviour: Retreating must occur
            isRetreating = "1";
          }
          else {
            checkIfRetreating();
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
                  << "\n";
      }
    }
  }

  // Behaviour: Make some final checks in regards to the healing modifiers
  finalChecks();
}

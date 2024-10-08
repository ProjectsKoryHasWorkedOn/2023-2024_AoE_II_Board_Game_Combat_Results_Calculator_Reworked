/** The headers **/
#include "modifiersCalculator.h" // Using: calculator class
#include "database.hpp"
#include "dialog_input.h"
#include "entity.h" // Using: entity class
#include "run_game.h"
#include <cstdlib>  // Using: exit(EXIT_FAILURE)
#include <iostream> // Using: cin, cout
#include <string>   // Using: string

// Functions: The constructor and deconstructor
modifiersCalculator::modifiersCalculator(Database* database)
  : m_database{database}
{
  // Referencing: Storing the default values is unnecessary because I set them
  // via a function
}

modifiersCalculator::~modifiersCalculator()
{
}

// Function: Set the battle participants
void modifiersCalculator::setEntities(
  Entity& inputBattleParticipantP1,
  Entity& inputBattleParticipantP2,
  Entity& inputAssistingMonkParticipantP1,
  Entity& inputAssistingMonkParticipantP2)
{
  p1BattleParticipant = inputBattleParticipantP1;
  p2BattleParticipant = inputBattleParticipantP2;
  p1BattleAssistant   = inputAssistingMonkParticipantP1;
  p2BattleAssistant   = inputAssistingMonkParticipantP2;
}

// Function: Set the additional values
void modifiersCalculator::setAdditionalValues(
  int& inputPlayerNumber,
  int& inputPlayerAge,
  int* inputTechnologies,
  int* inputEvents)
{
  playerNumber       = inputPlayerNumber;
  playerAge          = inputPlayerAge;
  playerTechnologies = inputTechnologies;
  playerEvents       = inputEvents;
}

// Function: Apply the effects of just the rock/paper/scissors system
// Replaced else if for if statements to fix unexpected output

void modifiersCalculator::applyRockPaperScissorsEffects()
{
  Entity applyRockPaperScissorsModifiers;
  Entity comparisonEntity;

  // Behaviour: Set the entity and the comparison entity (to make things clear)
  if (playerNumber == 1) {
    applyRockPaperScissorsModifiers = p1BattleParticipant;
    comparisonEntity                = p2BattleParticipant;
  }
  else if (playerNumber == 2) {
    applyRockPaperScissorsModifiers = p2BattleParticipant;
    comparisonEntity                = p1BattleParticipant;
  }

  applyRockPaperScissorsModifiers = m_database->getUnitModifiers(
    applyRockPaperScissorsModifiers, comparisonEntity);

  // Behaviour: Update the entity values
  if (playerNumber == 1) {
    p1BattleParticipant = applyRockPaperScissorsModifiers;
  }
  else if (playerNumber == 2) {
    p2BattleParticipant = applyRockPaperScissorsModifiers;
  }
}

// Function: Apply the effects of just the technologies
void modifiersCalculator::applyTechnologyEffects()
{
  // Object: Declare an entity to make what is happening clearer
  Entity applyTechnologyModifiers;

  if (playerNumber == 1) {
    applyTechnologyModifiers = p1BattleParticipant;
  }
  else if (playerNumber == 2) {
    applyTechnologyModifiers = p2BattleParticipant;
  }

  applyTechnologyModifiers = m_database->applyTechnologyEffects(
    applyTechnologyModifiers,
    std::span<int>(playerTechnologies, technologiesRows));

  // Behaviour: Update the Entity values
  if (playerNumber == 1) {
    p1BattleParticipant = applyTechnologyModifiers;
  }
  else if (playerNumber == 2) {
    p2BattleParticipant = applyTechnologyModifiers;
  }
}

void modifiersCalculator::applyEventCardEffects()
{
  // Object: Declare an entity to make what is happening clearer
  Entity applyEventCardModifiers;

  // Behaviour: Set the entity that is being modified
  if (playerNumber == 1) {
    applyEventCardModifiers = p1BattleParticipant;
  }
  else if (playerNumber == 2) {
    applyEventCardModifiers = p2BattleParticipant;
  }

  // String: Store the result from certain event card questions
  std::string isThisRelevant = "";

  if (playerEvents[0] == 1) {
    // A Just Case
    applyEventCardModifiers.standardDamage += 1;
  }
  // [1] Back From A Foreign Land: Byzantine civ bonus: +2 healing rate modifier
  // (done elsewhere)
  if (playerEvents[2] == 1) {
    // [2] Barrel of Grog - If target unit is Celt, they get +2 HP (ought to
    // calculate now)
    std::cout << "Is " << applyEventCardModifiers.entityName << " Celt?"
              << "<br>";
    DIN >> isThisRelevant;

    if (isThisRelevant == "1") {
      applyEventCardModifiers.entityHealth += 2;
    }
  }
  if (playerEvents[3] == 1) {
    // [3] Bone_Shaft_Arrows_(Mongol)
    if (applyEventCardModifiers.armorClass[5] == true) { // Cavalry Archer
      applyEventCardModifiers.standardDamage += 4;
    }
  }
  // [4] Caught from the Crow's Nest (done elsewhere)
  if (playerEvents[5] == 1) {
    // [5] Celtic_Battle_Cry_(Celt)
    if (applyEventCardModifiers.armorClass[8] == true) { // Infantry
      applyEventCardModifiers.standardDamage += 2;
    }
  }
  if (playerEvents[6] == 1) {
    // [6] Dangerous_Times - Attach to a military building. The building has +5
    // garrison until destroyed (ought to calculate now)
    if (applyEventCardModifiers.armorClass[1] == true) { // Building
      std::cout << "Is Dangerous_Times attached to "
                << applyEventCardModifiers.entityName << "? Enter 1 if so"
                << "<br>";
      DIN >> isThisRelevant;

      if (isThisRelevant == "1") {
        applyEventCardModifiers.garrisonValue += 5;
      }
    }
  }
  // [7] Fat Friars Tavern O'Spirits (done elsewhere)
  if (playerEvents[8] == 1) {
    // [8] Field Testing
    if (applyEventCardModifiers.armorClass[8] == true) { // Infantry
      applyEventCardModifiers.entityHealth += 2;
      applyEventCardModifiers.standardDamage += 1;
    }
  }
  // [9] First Battle Jitter (done elsewhere)
  if (playerEvents[10] == 1) {
    // [10] Flaming_Arrows - Attach to an Archer unit. This unit now gets +2
    // standardDamage vs Buildings (ought to calculate now)
    if (applyEventCardModifiers.armorClass[0] == true) { // Archer
      std::cout << "Is Flaming_Arrows attached to "
                << applyEventCardModifiers.entityName << "? Enter 1 if so"
                << "<br>";
      DIN >> isThisRelevant;

      if (isThisRelevant == "1") {
        applyEventCardModifiers.standardDamage += 2;
      }
    }
  }
  // [11] Fortune_Favors_The_Foolish (done elsewhere)
  if (playerEvents[13] == 1) {
    // [12] Gatherin' A Rowdy Bunch
    applyEventCardModifiers.standardDamage -= 1;
  }
  // [13] Gladitorial_Games (done elsewhere)
  // [14] Hard to Starboard (done elsewhere)
  // [15] Heavy Tree Cover (done elsewhere)
  if (playerEvents[16] == 1) {
    // [16] High Ground - Target Archer gains range bonus for this battle. Age
    // II-+1. Age III+2. Age IV+3 (ought to calculate now)
    if (applyEventCardModifiers.armorClass[0] == true) { // Archer
      std::cout << "Is High_Ground attached to "
                << applyEventCardModifiers.entityName << "? Enter 1 if so"
                << "<br>";
      DIN >> isThisRelevant;

      if (isThisRelevant == "1") {
        switch (playerAge) {
        case 1:
          break;
        case 2:
          applyEventCardModifiers.rangedDamage += 1;
          break;
        case 3:
          applyEventCardModifiers.rangedDamage += 2;
          break;
        case 4:
          applyEventCardModifiers.rangedDamage += 3;
          break;
        }
      }
    }
  }

  if (playerEvents[17] == 1) {
    // [17] Husbandry - Attach to a Cavalry Unit. That unit gains +2 HP (ought
    // to calculate now)
    if (applyEventCardModifiers.armorClass[4] == true) { // Cavalry
      std::cout << "Is Husbandry attached to "
                << applyEventCardModifiers.entityName << "? Enter 1 if so"
                << "<br>";
      DIN >> isThisRelevant;

      if (isThisRelevant == "1") {
        applyEventCardModifiers.entityHealth += 2;
      }
    }
  }
  // [18] It's A Miracle (done elsewhere)
  // [19] Listen To A Story (done elsewhere)
  // [20] Muddy Battlefield (done elsewhere)
  // [21] Non-Compos Mentis (done elsewhere)
  // [22] Back From A Foreign Land (Byzantine civ bonus: All buildings get a HP
  // bonus)
  if (playerEvents[22] == 1) {
    if (applyEventCardModifiers.armorClass[1] == true) { // Building
      if (playerAge == 1) {
        applyEventCardModifiers.entityHealth += 10;
      }
      else if (playerAge == 2) {
        applyEventCardModifiers.entityHealth += 20;
      }
      else if (playerAge == 3) {
        applyEventCardModifiers.entityHealth += 30;
      }
      else if (playerAge == 4) {
        applyEventCardModifiers.entityHealth += 40;
      }
      else {
        std::cout << "Error: Player Age not recognized";
      }
    }
  }

  // [23] Piety (done elsewhere)
  // [24] Black Knight (done elsewhere)
  if (playerEvents[25] == 1) {
    // [25] Rally The Workers
    if (applyEventCardModifiers.entityName == "Villager") {
      applyEventCardModifiers.standardDamage += 3;
    }
  }
  // [26] Relentless Attack (done elsewhere)
  // [27] Retreat (done elsewhere)
  // [28] Holy War
  if (playerEvents[28] == 1) {
    applyEventCardModifiers.standardDamage += 4;
  }
  // [29] Shots In The Back (Briton) (done elsewhere)
  // [30] Soak The Timbers (done elsewhere)
  if (playerEvents[31] == 1) {
    // [31] Spirits of the Ancestors - Target unit may not attack or move this
    // turn. For unit owner's next turn, the unit get +3 standardDamage (skip
    // for now)
    std::cout
      << "Is it player " << playerNumber
      << "'s next turn in regards to Spirits of the Ancestors? Enter 1 if so"
      << "<br>";
    DIN >> isThisRelevant;

    if (isThisRelevant == "1") {
      applyEventCardModifiers.standardDamage += 3;
    }
  }

  if (playerEvents[32] == 1) {
    // [32] Squires - Attach to an Infantry Unit. That unit gets +2 HP (ought to
    // calculate now)
    if (applyEventCardModifiers.armorClass[8] == true) { // Infantry
      std::cout << "Is Squires attached to "
                << applyEventCardModifiers.entityName << "? Enter 1 if so"
                << "<br>";
      DIN >> isThisRelevant;

      if (isThisRelevant == "1") {
        applyEventCardModifiers.entityHealth += 2;
      }
    }
  }
  // [33] Steady Hand (done elsewhere)
  // [34] The Hammer's Cavalry (Franks)(done elsewhere)
  // [35] The Jester Is Dead, Let's Get Em (Celt) (done elsewhere)
  if (playerEvents[36] == 1) {
    // [36] Vengeance Is Mine! - Target unit gets +3 HP +3 AP this turn, when
    // defending in your village. Play anytime.(ought to calculate now)
    std::cout << "Is Vengeance_Is_Mine! attached to "
              << applyEventCardModifiers.entityName << "? Enter 1 if so"
              << "<br>";
    DIN >> isThisRelevant;

    if (isThisRelevant == "1") {
      applyEventCardModifiers.standardDamage += 2;
      applyEventCardModifiers.entityHealth += 3;
    }
  }
  // [37] While They're Sleeping  (done elsewhere)
  // [38] You Will Die! (Done elsewhere)
  // [39] Zealous Monks (Done elsewhere)
  // [40] Back From A Foreign Land (Teuton civ bonus: Conversion rate modifier
  // is -1) (done elsewhere)


  // [42] Good winds
  if(playerEvents[42] == 1){
    if(applyEventCardModifiers.armorClass[12] == true){
      applyEventCardModifiers.maximumRange += 1;
    }
  }

  // Behaviour: Update the Entity values
  if (playerNumber == 1) {
    p1BattleParticipant = applyEventCardModifiers;
  }
  else if (playerNumber == 2) {
    p2BattleParticipant = applyEventCardModifiers;
  }
}

// Function: Apply the effects of 1) attack bonuses, 2) technologies, 3) the
// relevant event cards, and 4) quantity for player 1's battle participant
Entity modifiersCalculator::applyAllModifiers(int inputReturnMode)
{
  // Struct: The modified battle participant to return
  Entity modifiedBattleParticipant;

  // Behaviour: Set the return mode
  returnMode = inputReturnMode;

  // Behaviour: Apply the rock/paper/scissors effects
  applyRockPaperScissorsEffects();

  // Behaviour: Apply the technology effects
  applyTechnologyEffects();

  // Behaviour: Apply the event card effects
  applyEventCardEffects();

  // Behaviour: Determine which entity ought to be returned
  if ((returnMode == 0) && (playerNumber == 1)) {
    modifiedBattleParticipant = p1BattleParticipant;
  }
  else if ((returnMode == 1) && (playerNumber == 1)) {
    modifiedBattleParticipant = p1BattleAssistant;
  }
  else if ((returnMode == 0) && (playerNumber == 2)) {
    modifiedBattleParticipant = p2BattleParticipant;
  }
  else if ((returnMode == 1) && (playerNumber == 2)) {
    modifiedBattleParticipant = p2BattleAssistant;
  }

  // Behaviour: Return the modified entity
  return modifiedBattleParticipant;
}

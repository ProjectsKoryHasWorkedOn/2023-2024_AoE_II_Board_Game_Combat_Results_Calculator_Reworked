/** The libaries **/
#include "modifiersCalculator.h" // Using: calculator class
#include "entity.h"              // Using: entity class
#include <cstdlib>               // Using: exit(EXIT_FAILURE)
#include <iostream>              // Using: cin, cout
#include <string>                // Using: string

// Functions: The constructor and deconstructor
modifiersCalculator::modifiersCalculator()
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
  p1BattleParticipant        = inputBattleParticipantP1;
  p2BattleParticipant        = inputBattleParticipantP2;
  p1AssistingMonkParticipant = inputAssistingMonkParticipantP1;
  p2AssistingMonkParticipant = inputAssistingMonkParticipantP2;
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

  // Archer_(Saracen) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Archer_(Saracen)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 5;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Crossbowman_(Saracen)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 6;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Arbalest_(Saracen)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 8;
  }

  // Battering_Ram line
  if (
    (applyRockPaperScissorsModifiers.armorClass[27] == true) && // Battering Ram
    (comparisonEntity.armorClass[1] == true)                    // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 60;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[28] == true) && // Capped ram
    (comparisonEntity.armorClass[1] == true)                    // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 110;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[29] == true) && // Siege ram
    (comparisonEntity.armorClass[1] == true)                    // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 220;
  }

  // Battering_Ram_(Celt) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Battering_Ram_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 70;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Capped_Ram_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 120;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Siege_Ram_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 230;
  }

  // Knight_(Persian) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Knight_(Persian)")
    && (comparisonEntity.armorClass[0] == true) // Archer
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Cavalier_(Persian)")
    && (comparisonEntity.armorClass[0] == true) // Archer
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Paladin_(Persian)")
    && (comparisonEntity.armorClass[0] == true) // Archer
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }

  // Camel line
  if (
    (applyRockPaperScissorsModifiers.armorClass[2] == true) && // Camel
    (comparisonEntity.armorClass[4] == true)                    // Cavalry
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 20;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[31] == true)
    &&                                       // Heavy camel armor class
    (comparisonEntity.armorClass[4] == true) // Cavalry
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 30;
  }

  // Cavalry_Archer_(Saracen) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Cavalry_Archer_(Saracen)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 9;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName
     == "Heavy_Cavalry_Archer_(Saracen)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 10;
  }

  // Militia_(Goth) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Militia_(Goth)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Man_At_Arms_(Goth)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Long_Swordsman_(Goth)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Champion_(Goth)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName
     == "Two_Handed_Swordsman_(Goth)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }

  // Militia_(Japanese) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Champion_(Japanese)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage += 2;
  }

  // Berserk_(Viking) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Elite_Berserk_(Viking)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 17;
  }

  // Huskarl_(Goth) line
  if(
	(applyRockPaperScissorsModifiers.entityName == "Huskarl_(Goth)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[0] == true) // Archer
	)){
    if (comparisonEntity.armorClass[1] == true) {
      applyRockPaperScissorsModifiers.standardDamage = 12;
    }
    else if (comparisonEntity.armorClass[0] == true) {
      applyRockPaperScissorsModifiers.standardDamage = 12;
    }
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Elite_Huskarl_(Goth)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 15;
  }

  // Longboat_(Viking) line
  if(
	(applyRockPaperScissorsModifiers.entityName == "Longboat_(Viking)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[21] == true) || // Demolition Ship
		(comparisonEntity.armorClass[22] == true) || // Galley
		(comparisonEntity.entityName == "Longboat_(Viking)") ||
		(comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
	)){
    applyRockPaperScissorsModifiers.standardDamage = 40;
  }
  if(
	(applyRockPaperScissorsModifiers.entityName == "Elite_Longboat_(Viking)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[21] == true) || // Demolition Ship
		(comparisonEntity.armorClass[22] == true) || // Galley
		(comparisonEntity.entityName == "Longboat_(Viking)") ||
		(comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
	)){
    applyRockPaperScissorsModifiers.standardDamage = 50;
  }

  // Mangudai_(Mongol) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Mangudai_(Mongol)")
    && (comparisonEntity.armorClass[12] == true) // Siege
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 14;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Elite_Mangudai_(Mongol)")
    && (comparisonEntity.armorClass[12] == true) // Siege
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 16;
  }

  // Mameluke_(Saracen) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Mameluke_(Saracen)")
    && (comparisonEntity.armorClass[4] == true) // Cavalry
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 16;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Elite_Mameluke_(Saracen)")
    && (comparisonEntity.armorClass[4] == true) // Cavalry
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 20;
  }

  // Samurai_(Japanese) line
  if(
	(applyRockPaperScissorsModifiers.entityName == "Samurai_(Japanese)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[17] == true) // Unique Unit
	)){
    if (comparisonEntity.armorClass[1] == true) {
      applyRockPaperScissorsModifiers.standardDamage = 10;
    }
    else if (comparisonEntity.armorClass[17] == true) {
      applyRockPaperScissorsModifiers.standardDamage = 12;
    }
  }
  if(
	(applyRockPaperScissorsModifiers.entityName == "Elite_Samurai_(Japanese)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[17] == true) // Unique Unit
	)){
    if (comparisonEntity.armorClass[1] == true) {
      applyRockPaperScissorsModifiers.standardDamage = 15;
    }
    else if (comparisonEntity.armorClass[17] == true) {
      applyRockPaperScissorsModifiers.standardDamage = 18;
    }
  }

  // Skirmisher line
  if (
    (applyRockPaperScissorsModifiers.armorClass[32] == true)
    && (                                          // Skirmisher
      (comparisonEntity.armorClass[0] == true) || // Archer
      (comparisonEntity.armorClass[5] == true)    // Cavalry Archer
      )) {
    applyRockPaperScissorsModifiers.standardDamage = 12;
    applyRockPaperScissorsModifiers.rangedDamage   = 13;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[33] == true)
    && (                                          // Elite Skirmisher
      (comparisonEntity.armorClass[0] == true) || // Archer
      (comparisonEntity.armorClass[5] == true)    // Cavalry Archer
      )) {
    applyRockPaperScissorsModifiers.standardDamage = 16;
  }

  // Throwing_Axeman_(Frank) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Throwing_Axeman_(Frank)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 9;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName
     == "Elite_Throwing_Axeman_(Frank)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 12;
  }

  // War_Elephant_(Persian) line
  if (
    (applyRockPaperScissorsModifiers.entityName
     == "Elite_War_Elephant_(Persian)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 60;
  }

  // Woad_Raider_(Celt) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Woad_Raider_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 12;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Elite_Woad_Raider_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 17;
  }

  // Fire_Ship line
  if (
    (applyRockPaperScissorsModifiers.armorClass[23] == true)
    && (                                           // Fire Ship
      (comparisonEntity.armorClass[1] == true) ||  // Building
      (comparisonEntity.armorClass[23] == true) || // Fire Ship
      (comparisonEntity.armorClass[22] == true) || // Galley
      (comparisonEntity.entityName == "Longboat_(Viking)")
      || (comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
      )) {
    applyRockPaperScissorsModifiers.standardDamage = 10;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[34] == true)
    && (                                           // Fast Fire Ship
      (comparisonEntity.armorClass[1] == true) ||  // Building
      (comparisonEntity.armorClass[23] == true) || // Fire Ship
      (comparisonEntity.armorClass[22] == true) || // Galley
      (comparisonEntity.entityName == "Longboat_(Viking)")
      || (comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
      )) {
    applyRockPaperScissorsModifiers.standardDamage = 12;
  }

  // Galley line
  if (
    (applyRockPaperScissorsModifiers.armorClass[22] == true)
    && (                                           // Galley
      (comparisonEntity.armorClass[1] == true) ||  // Building
      (comparisonEntity.armorClass[22] == true) || // Galley
      (comparisonEntity.entityName == "Longboat_(Viking)")
      || (comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
      )) {
    applyRockPaperScissorsModifiers.standardDamage = 30;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[35] == true)
    && (                                           // War Galley
      (comparisonEntity.armorClass[1] == true) ||  // Building
      (comparisonEntity.armorClass[22] == true) || // Galley
      (comparisonEntity.entityName == "Longboat_(Viking)")
      || (comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
      )) {
    applyRockPaperScissorsModifiers.standardDamage = 40;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[36] == true)
    && (                                           // Galleon
      (comparisonEntity.armorClass[1] == true) ||  // Building
      (comparisonEntity.armorClass[22] == true) || // Galley
      (comparisonEntity.entityName == "Longboat_(Viking)")
      || (comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
      )) {
    applyRockPaperScissorsModifiers.standardDamage = 50;
  }

  // Galley_(Saracen) line
  if(
	(applyRockPaperScissorsModifiers.entityName == "Galley_(Saracen)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[22] == true) || // Galley
		(comparisonEntity.entityName == "Longboat_(Viking)") ||
		(comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
	)){
    applyRockPaperScissorsModifiers.standardDamage = 35;
  }
  if(
	(applyRockPaperScissorsModifiers.entityName == "War_Galley_(Saracen)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[22] == true) || // Galley
		(comparisonEntity.entityName == "Longboat_(Viking)") ||
		(comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
	)){
    applyRockPaperScissorsModifiers.standardDamage = 50;
  }
  if(
	(applyRockPaperScissorsModifiers.entityName == "Galleon_(Saracen)") && (
		(comparisonEntity.armorClass[1] == true) || // Building
		(comparisonEntity.armorClass[22] == true) || // Galley
		(comparisonEntity.entityName == "Longboat_(Viking)") ||
		(comparisonEntity.armorClass[11] != true) // Non-Ship (Land unit)
	)){
    applyRockPaperScissorsModifiers.standardDamage = 60;
  }

  // Mangonel line
  if (
    (applyRockPaperScissorsModifiers.armorClass[37] == true) && // Mangonel
    (comparisonEntity.armorClass[1] == true)                    // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 80;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[38] == true) && // Onager
    (comparisonEntity.armorClass[1] == true)                    // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 120;
  }
  if (
    (applyRockPaperScissorsModifiers.armorClass[39] == true) && // Siege Onager
    (comparisonEntity.armorClass[1] == true)                    // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 180;
  }

  // Mangonel_(Celt) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Mangonel_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 90;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Onager_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 130;
  }
  if (
    (applyRockPaperScissorsModifiers.entityName == "Siege_Onager_(Celt)")
    && (comparisonEntity.armorClass[1] == true) // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 190;
  }

  // Spearman line
  if (
    (applyRockPaperScissorsModifiers.armorClass[13] == true) && // Spearman
    (comparisonEntity.armorClass[4] == true)                    // Cavalry
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 25;
  }

  // Pikeman line
  if (
    (applyRockPaperScissorsModifiers.armorClass[41] == true) && // Pikeman
    (comparisonEntity.armorClass[4] == true)                    // Cavalry
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 40;
  }

  // Spearman_(Goth) line
  if(
	(applyRockPaperScissorsModifiers.entityName == "Spearman_(Goth)") && (
		(comparisonEntity.armorClass[4] == true) || // Cavalry
		(comparisonEntity.armorClass[1] == true) // Building
	)){
    if (comparisonEntity.armorClass[1] == true) {
      applyRockPaperScissorsModifiers.standardDamage += 2;
    }
  }
  if(
	(applyRockPaperScissorsModifiers.entityName == "Pikeman_(Goth)") && (
		(comparisonEntity.armorClass[4] == true) || // Cavalry
		(comparisonEntity.armorClass[1] == true) // Building
	)){
    if (comparisonEntity.armorClass[1] == true) {
      applyRockPaperScissorsModifiers.standardDamage += 2;
    }
  }

  // Spearman_(Japanese) line
  if (
    (applyRockPaperScissorsModifiers.entityName == "Spearman_(Japanese")
    && (comparisonEntity.armorClass[4] == true) // Cavalry
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 26;
  }

  // Trebuchet line
  if (
    (applyRockPaperScissorsModifiers.armorClass[42] == true) && // Trebuchet
    (comparisonEntity.armorClass[1] == true)                    // Building
  ) {
    applyRockPaperScissorsModifiers.standardDamage = 450;
  }

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

  if (playerTechnologies[0] == 1) {
    // Blast_Furnace - +6 standardDamage to all Cavalry and Infantry
    if (
      (applyTechnologyModifiers.armorClass[4] == true) || // Cavalry
      (applyTechnologyModifiers.armorClass[8] == true)) { // Infantry
      applyTechnologyModifiers.standardDamage += 6;
    }
  }
  if (playerTechnologies[1] == 1) {
    // Bodkin_Arrow - +4 standardDamage and +2 rangedDamage to:
    // Archer, Cavalry Archer, Tower, Castle, Town Center, Galley, Longboat
    // I have yet to include civilization specific versions of the string
    // searched for entities
    if (
      (applyTechnologyModifiers.armorClass[0] == true) ||  // Archer
      (applyTechnologyModifiers.armorClass[5] == true) ||  // Cavalry Archer
      (applyTechnologyModifiers.armorClass[24] == true) || // Watch Tower
      (applyTechnologyModifiers.armorClass[3] == true) ||  // Castle
      (applyTechnologyModifiers.armorClass[25] == true) || // Town Center
      (applyTechnologyModifiers.armorClass[22] == true) || // Galley
      (applyTechnologyModifiers.entityName
       == "Longboat_(Viking)") // Longboat example
    ) {
      applyTechnologyModifiers.standardDamage += 4;
      applyTechnologyModifiers.rangedDamage += 2;
    }
  }
  if (playerTechnologies[2] == 1) {
    // Bracer - +6 standardDamage and +3 rangedDamage to:
    // Archer, Cavalry Archer, Tower, Castle, Town Center, Galley, Longboat
    // I have yet to include civilization specific versions of the string
    // searched for entities
    if (
      (applyTechnologyModifiers.armorClass[0] == true) ||  // Archer
      (applyTechnologyModifiers.armorClass[5] == true) ||  // Cavalry Archer
      (applyTechnologyModifiers.armorClass[24] == true) || // Watch Tower
      (applyTechnologyModifiers.armorClass[3] == true) ||  // Castle
      (applyTechnologyModifiers.armorClass[25] == true) || // Town Center
      (applyTechnologyModifiers.armorClass[22] == true) || // Galley
      (applyTechnologyModifiers.entityName
       == "Longboat_(Viking)") // Longboat example
    ) {
      applyTechnologyModifiers.standardDamage += 6;
      applyTechnologyModifiers.rangedDamage += 3;
    }
  }
  if (playerTechnologies[3] == 1) {
    // Chain_Barding_Armor - +4 HP to all Cavalry
    if (applyTechnologyModifiers.armorClass[4] == true) { // Cavalry
      applyTechnologyModifiers.entityHealth += 4;
    }
  }
  if (playerTechnologies[4] == 1) {
    // Chain_Mail_Armor- +4 HP to all Infantry
    if (applyTechnologyModifiers.armorClass[8] == true) { // Infantry
      applyTechnologyModifiers.entityHealth += 4;
    }
  }
  if (playerTechnologies[5] == 1) {
    // Fletching - +2 standardDamage and +1 rangedDamage to:
    // Archer, Cavalry Archer, Tower, Castle, Town Center, Galley, Longboat
    // I have yet to include civilization specific versions of the string
    // searched for entities
    if (
      (applyTechnologyModifiers.armorClass[0] == true) ||  // Archer
      (applyTechnologyModifiers.armorClass[5] == true) ||  // Cavalry Archer
      (applyTechnologyModifiers.armorClass[24] == true) || // Watch Tower
      (applyTechnologyModifiers.armorClass[3] == true) ||  // Castle
      (applyTechnologyModifiers.armorClass[25] == true) || // Town Center
      (applyTechnologyModifiers.armorClass[22] == true) || // Galley
      (applyTechnologyModifiers.entityName
       == "Longboat_(Viking)") // Longboat example
    ) {
      applyTechnologyModifiers.standardDamage += 2;
      applyTechnologyModifiers.rangedDamage += 1;
    }
  }
  if (playerTechnologies[6] == 1) {
    // Forging - +2 standardDamage to all Cavalry and Infantry
    if (
      (applyTechnologyModifiers.armorClass[4] == true) || // Cavalry
      (applyTechnologyModifiers.armorClass[8] == true)) { // Infantry
      applyTechnologyModifiers.standardDamage += 2;
    }
  }
  if (playerTechnologies[7] == 1) {
    // Hoardings - Increase the HP of Castes by +80
    if (applyTechnologyModifiers.armorClass[3] == true) { // Castle
      applyTechnologyModifiers.entityHealth += 80;
    }
  }
  if (playerTechnologies[8] == 1) {
    // Iron_Casting - +4 standardDamage to all Cavalry and Infantry
    if (
      (applyTechnologyModifiers.armorClass[4] == true) || // Cavalry
      (applyTechnologyModifiers.armorClass[8] == true)) { // Infantry
      applyTechnologyModifiers.standardDamage += 4;
    }
  }
  if (playerTechnologies[9] == 1) {
    // Leather_Archer_Armor - Increase the HP of Archers, Cavalry Archers by 4
    if (
      (applyTechnologyModifiers.armorClass[0] == true) || // Archer
      (applyTechnologyModifiers.armorClass[5] == true)    // Cavalry Archer
    ) {
      applyTechnologyModifiers.entityHealth += 4;
    }
  }
  if (playerTechnologies[10] == 1) {
    // Loom - +2 HP for Villagers
    if (applyTechnologyModifiers.armorClass[26] == true) { // Villager
      applyTechnologyModifiers.entityHealth += 2;
    }
  }
  if (playerTechnologies[11] == 1) {
    // Padded_Archer_Armor - +2 HP to all Archers, Cavalry Archers
    if (
      (applyTechnologyModifiers.armorClass[0] == true) || // Archer
      (applyTechnologyModifiers.armorClass[5] == true)    // Cavalry Archer
    ) {
      applyTechnologyModifiers.entityHealth += 2;
    }
  }
  if (playerTechnologies[12] == 1) {
    // Plate_Barding_Armor - +6 HP to all Cavalry
    if (applyTechnologyModifiers.armorClass[4] == true) { // Cavalry
      applyTechnologyModifiers.entityHealth += 6;
    }
  }
  if (playerTechnologies[13] == 1) {
    // Plate_Mail_Armor - +6 HP to all Infantry
    if (applyTechnologyModifiers.armorClass[8] == true) { // Infantry
      applyTechnologyModifiers.entityHealth += 6;
    }
  }
  if (playerTechnologies[14] == 1) {
    // Ring_Archer_Armor - +6 HP to all Archers, Cavalry Archers
    if (
      (applyTechnologyModifiers.armorClass[0] == true) || // Archer
      (applyTechnologyModifiers.armorClass[5] == true)    // Cavalry Archer
    ) {
      applyTechnologyModifiers.entityHealth += 6;
    }
  }
  if (playerTechnologies[15] == 1) {
    // Scale_Barding_Armor - +2 HP to all Cavalry
    if (applyTechnologyModifiers.armorClass[4] == true) { // Cavalry
      applyTechnologyModifiers.entityHealth += 2;
    }
  }
  if (playerTechnologies[16] == 1) {
    // Scale_Mail_Armor - +2 HP to all Infantry
    if (applyTechnologyModifiers.armorClass[8] == true) { // Infantry
      applyTechnologyModifiers.entityHealth += 2;
    }
  }
  if (playerTechnologies[17] == 1) {
    // Sanctity_Test - +1 HP to all Monks
    if (applyTechnologyModifiers.armorClass[9] == true) { // Monk
      applyTechnologyModifiers.entityHealth += 1;
    }
  }

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
  // [1] Back From A Foreign Land: Byzantine civ bonus: +2 healing rate modifier (done elsewhere)
  if (playerEvents[2] == 1) {
    // [2] Barrel of Grog - If target unit is Celt, they get +2 HP (ought to
    // calculate now)
    std::cout << "Is " << applyEventCardModifiers.entityName << " Celt?"
              << "\n";
    std::cin >> isThisRelevant;

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
                << "\n";
      std::cin >> isThisRelevant;

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
                << "\n";
      std::cin >> isThisRelevant;

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
                << "\n";
      std::cin >> isThisRelevant;

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
                << "\n";
      std::cin >> isThisRelevant;

      if (isThisRelevant == "1") {
        applyEventCardModifiers.entityHealth += 2;
      }
    }
  }
  // [18] It's A Miracle (done elsewhere)
  // [19] Listen To A Story (done elsewhere)
  // [20] Muddy Battlefield (done elsewhere)
  // [21] Non-Compos Mentis (done elsewhere)
  // [22] Back From A Foreign Land (Byzantine civ bonus: All buildings get a HP bonus)
  if (playerEvents[22] == 1) {
    if (applyEventCardModifiers.armorClass[1] == true) { // Building
      if(playerAge == 1){
        applyEventCardModifiers.entityHealth += 10;
      }
      else if(playerAge == 2){
        applyEventCardModifiers.entityHealth += 20;
      }
      else if(playerAge == 3){
        applyEventCardModifiers.entityHealth += 30;
      }
      else if(playerAge == 4){
        applyEventCardModifiers.entityHealth += 40;
      }
      else{
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
      << "\n";
    std::cin >> isThisRelevant;

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
                << "\n";
      std::cin >> isThisRelevant;

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
              << "\n";
    std::cin >> isThisRelevant;

    if (isThisRelevant == "1") {
      applyEventCardModifiers.standardDamage += 2;
      applyEventCardModifiers.entityHealth += 3;
    }
  }
  // [37] While They're Sleeping  (done elsewhere)
  // [38] You Will Die! (Done elsewhere)
  // [39] Zealous Monks (Done elsewhere)
  // [40] Back From A Foreign Land (Teuton civ bonus: Conversion rate modifier is -1) (done elsewhere)

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
    modifiedBattleParticipant = p1AssistingMonkParticipant;
  }
  else if ((returnMode == 0) && (playerNumber == 2)) {
    modifiedBattleParticipant = p2BattleParticipant;
  }
  else if ((returnMode == 1) && (playerNumber == 2)) {
    modifiedBattleParticipant = p2AssistingMonkParticipant;
  }

  // Behaviour: Apply the quantity effects
  modifiedBattleParticipant.standardDamage
    *= modifiedBattleParticipant.entityQuantity;
  modifiedBattleParticipant.rangedDamage
    *= modifiedBattleParticipant.entityQuantity;
  modifiedBattleParticipant.pointValue
    *= modifiedBattleParticipant.entityQuantity;

  // Behaviour: Return the modified entity
  return modifiedBattleParticipant;
}

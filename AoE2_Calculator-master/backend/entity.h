#pragma once
/** The libaries **/
#include <iostream> // Using: cin, cout
#include <string>   // Using: string

/** Class: Entity **/
struct Entity {
  // Variables/Array: Store entity information
  std::string entityName;
  int         entityAge;
  int         entityQuantity;
  int         entityHealth;
  int         standardDamage;
  int         rangedDamage;
  int         garrisonValue;
  int         pointValue;
  static int constexpr numberOfArmorClasses = 28;
  bool armorClass[numberOfArmorClasses];
  /*
  armorClass[0] -> archer
  armorClass[1] -> building
  armorClass[2] -> camel
  armorClass[3] -> castle
  armorClass[4] -> cavalry
  armorClass[5] -> cavalry archer
  armorClass[6] -> eagle_warrior
  armorClass[7] -> gunpowder_unit
  armorClass[8] -> infantry
  armorClass[9] -> monk
  armorClass[10] -> ram
  armorClass[11] -> ship
  armorClass[12] -> siege_weapon
  armorClass[13] -> spearman
  armorClass[14] XXXXXXXXXXXXXXXXXXXX cut
  armorClass[15] -> stone_defence
  armorClass[16] -> turtle_ship
  armorClass[17] -> unique_unit
  armorClass[18] -> wall_&_gate
  armorClass[19] -> elephant
  armorClass[20] -> hero
  armorClass[21] -> demolition_ship
  armorClass[22] -> galley
  armorClass[23] -> fire_ship
  armorClass[24] -> watch_tower
  armorClass[25] -> town_center
  armorClass[26] -> land_unit
  armorClass[27] -> longboat
  */
  bool onlyAttacksInTheSecondRoundOfStandardCombat;
  bool onlyAttacksOnce;
  bool dealsAreaEffectDamage;
  bool isKamikaze;
  int  entitiesArmorClasses;

  // Functions: The constructor and deconstructor
  Entity();
  ~Entity();

  // Function: Return the number of armor classes unique to an entity
  int returnNumberOfArmorClasses();

  // Function: Make text colorful
  void displayColorfulText(
    std::string inputFormatAttribute,
    std::string inputForegroundColor,
    std::string inputBackgroundColor,
    std::string inputText,
    bool        isLineBreak);

  // Function: Return entity information
  void outputEntity(std::string playerName);
};

/** Inherited class from Entity: Wonder **/
struct Wonder : Entity {
  // Variable: Store the number of turns in play
  int turnsInPlay;

  // Functions: The constructor and deconstructor
  Wonder();
  ~Wonder();

  // Function: Update the HP of the Wonder based on the number of turns in play
  void updateWonderHP();
};

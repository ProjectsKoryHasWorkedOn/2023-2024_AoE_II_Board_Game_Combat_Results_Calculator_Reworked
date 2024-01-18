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
  armorClass[14] -> stone_defence
  armorClass[15] -> turtle_ship
  armorClass[16] -> unique_unit
  armorClass[17] -> wall_&_gate
  armorClass[18] -> elephant
  armorClass[19] -> hero
  armorClass[20] -> demolition_ship
  armorClass[21] -> galley
  armorClass[22] -> fire_ship
  armorClass[23] -> watch_tower
  armorClass[24] -> town_center
  armorClass[25] -> land_unit
  armorClass[26] -> longboat
  armorClass[27] -> villager
  */
  bool onlyAttacksInTheSecondRoundOfStandardCombat;
  bool onlyAttacksOnce;
  bool dealsAreaEffectDamage;
  bool isKamikaze;
  int  entitiesArmorClasses;

  int healsAvailable;

  int initialEntityQuantity;

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

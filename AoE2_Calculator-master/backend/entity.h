#pragma once
/** The libaries **/
#include <iostream> // Using: cin, cout
#include <string>   // Using: string

/** Class: Entity **/
#ifndef ENTITY_H
#define ENTITY_H
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
  bool        armorClass[43];
  bool        onlyAttacksInTheSecondRoundOfStandardCombat;
  bool        onlyAttacksOnce;
  bool        dealsAreaEffectDamage;
  bool        isKamikaze;
  int         entitiesArmorClasses;

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
#endif // ENTITY_H

////////////////////////////////// Reference: I could have just used functions
/// or tried to import this information but inheritance works well enough
/////////////////////////////////////
/**
        void Militia(std:: string civilizationDesignator){
                // Default Militia values for entity

                if(civilizationDesignator == "Goth"){
                        entityName += "_(Goth)";
                }
        }
**/

////////////////////////////////// Inherited from Entity Classes
/////////////////////////////////////
/** Inherited class from Entity: Barracks **/
#ifndef BARRACKS_H
#define BARRACKS_H
struct Barracks : Entity {
  // Functions: The constructor and deconstructor
  Barracks();
  ~Barracks();
};
#endif // BARRACKS_H

/** Inherited class from Entity: Hero **/
#ifndef HERO_H
#define HERO_H
struct Hero : Entity {
  // Functions: The constructor and deconstructor
  Hero();
  ~Hero();
};
#endif // HERO_H

/** Inherited class from Entity: Dock **/
#ifndef DOCK_H
#define DOCK_H
struct Dock : Entity {
  // Functions: The constructor and deconstructor
  Dock();
  ~Dock();
};
#endif // DOCK_H

/** Inherited class from Entity: Stone_Wall **/
#ifndef STONE_WALL_H
#define STONE_WALL_H
struct Stone_Wall : Entity {
  // Functions: The constructor and deconstructor
  Stone_Wall();
  ~Stone_Wall();
};
#endif // STONE_WALL_H

/** Inherited class from Entity: Farm **/
#ifndef FARM_H
#define FARM_H
struct Farm : Entity {
  // Functions: The constructor and deconstructor
  Farm();
  ~Farm();
};
#endif // FARM_H

/** Inherited class from Entity: Fishing_Ship **/
#ifndef FISHING_SHIP
#define FISHING_SHIP
struct Fishing_Ship : Entity {
  // Functions: The constructor and deconstructor
  Fishing_Ship();
  ~Fishing_Ship();
};
#endif // FISHING_SHIP

/** Inherited class from Entity: Gold_Mine **/
#ifndef GOLD_MINE_H
#define GOLD_MINE_H
struct Gold_Mine : Entity {
  // Functions: The constructor and deconstructor
  Gold_Mine();
  ~Gold_Mine();
};
#endif // GOLD_MINE_H

/** Inherited class from Entity: House **/
#ifndef HOUSE_H
#define HOUSE_H
struct House : Entity {
  // Functions: The constructor and deconstructor
  House();
  ~House();
};
#endif // HOUSE_H

/** Inherited class from Entity: Lumber_Camp **/
#ifndef LUMBER_CAMP_H
#define LUMBER_CAMP_H
struct Lumber_Camp : Entity {
  // Functions: The constructor and deconstructor
  Lumber_Camp();
  ~Lumber_Camp();
};
#endif // LUMBER_CAMP_H

/** Inherited class from Entity: Militia **/
#ifndef MILITIA_H
#define MILITIA_H
struct Militia : Entity {
  // Functions: The constructor and deconstructor
  Militia();
  ~Militia();
};
#endif // MILITIA_H

/** Inherited class from Entity: Palisade_Wall **/
#ifndef PALISADE_WALL_H
#define PALISADE_WALL_H
struct Palisade_Wall : Entity {
  // Functions: The constructor and deconstructor
  Palisade_Wall();
  ~Palisade_Wall();
};
#endif // PALISADE_WALL_H

/** Inherited class from Entity: Stone_Mine **/
#ifndef STONE_MINE_H
#define STONE_MINE_H
struct Stone_Mine : Entity {
  // Functions: The constructor and deconstructor
  Stone_Mine();
  ~Stone_Mine();
};
#endif // STONE_MINE_H

/** Inherited class from Entity: Villager **/
#ifndef VILLAGER_H
#define VILLAGER_H
struct Villager : Entity {
  // Functions: The constructor and deconstructor
  Villager();
  ~Villager();
};
#endif // VILLAGER_H

/** Inherited class from Entity: Archer **/
#ifndef ARCHER_H
#define ARCHER_H
struct Archer : Entity {
  // Functions: The constructor and deconstructor
  Archer();
  ~Archer();
};
#endif // ARCHER_H

/** Inherited class from Entity: Archery_Range **/
#ifndef ARCHERY_RANGE_H
#define ARCHERY_RANGE_H
struct Archery_Range : Entity {
  // Functions: The constructor and deconstructor
  Archery_Range();
  ~Archery_Range();
};
#endif // ARCHERY_RANGE_H

/** Inherited class from Entity: Blacksmith **/
#ifndef BLACKSMITH_H
#define BLACKSMITH_H
struct Blacksmith : Entity {
  // Functions: The constructor and deconstructor
  Blacksmith();
  ~Blacksmith();
};
#endif // BLACKSMITH_H

/** Inherited class from Entity: Galley **/
#ifndef GALLEY_H
#define GALLEY_H
struct Galley : Entity {
  // Functions: The constructor and deconstructor
  Galley();
  ~Galley();
};
#endif // GALLEY_H

/** Inherited class from Entity: Mill **/
#ifndef MILL_H
#define MILL_H
struct Mill : Entity {
  // Functions: The constructor and deconstructor
  Mill();
  ~Mill();
};
#endif // MILL_H

/** Inherited class from Entity: Market **/
#ifndef MARKET_H
#define MARKET_H
struct Market : Entity {
  // Functions: The constructor and deconstructor
  Market();
  ~Market();
};
#endif // MARKET_H

/** Inherited class from Entity: Monk **/
#ifndef MONK_H
#define MONK_H
struct Monk : Entity {
  // Functions: The constructor and deconstructor
  Monk();
  ~Monk();
};
#endif // MONK_H

/** Inherited class from Entity: Outpost **/
#ifndef OUTPOST_H
#define OUTPOST_H
struct Outpost : Entity {
  // Functions: The constructor and deconstructor
  Outpost();
  ~Outpost();
};
#endif // OUTPOST_H

/** Inherited class from Entity: Scout_Cavalry **/
#ifndef SCOUT_CAVALRY_H
#define SCOUT_CAVALRY_H
struct Scout_Cavalry : Entity {
  // Functions: The constructor and deconstructor
  Scout_Cavalry();
  ~Scout_Cavalry();
};
#endif // SCOUT_CAVALRY_H

/** Inherited class from Entity: Skirmisher **/
#ifndef SKIRMISHER_H
#define SKIRMISHER_H
struct Skirmisher : Entity {
  // Functions: The constructor and deconstructor
  Skirmisher();
  ~Skirmisher();
};
#endif // SKIRMISHER_H

/** Inherited class from Entity: Spearman **/
#ifndef SPEARMAN_H
#define SPEARMAN_H
struct Spearman : Entity {
  // Functions: The constructor and deconstructor
  Spearman();
  ~Spearman();
};
#endif // SPEARMAN_H

/** Inherited class from Entity: Stable **/
#ifndef STABLE_H
#define STABLE_H
struct Stable : Entity {
  // Functions: The constructor and deconstructor
  Stable();
  ~Stable();
};
#endif // STABLE_H

/** Inherited class from Entity: Trade_Cog **/
#ifndef TRADE_COG_H
#define TRADE_COG_H
struct Trade_Cog : Entity {
  // Functions: The constructor and deconstructor
  Trade_Cog();
  ~Trade_Cog();
};
#endif // TRADE_COG_H

/** Inherited class from Entity: Watch_Tower **/
#ifndef WATCH_TOWER_H
#define WATCH_TOWER_H
struct Watch_Tower : Entity {
  // Functions: The constructor and deconstructor
  Watch_Tower();
  ~Watch_Tower();
};
#endif // WATCH_TOWER_H

/** Inherited class from Entity: Battering Ram **/
#ifndef BATTERING_RAM_H
#define BATTERING_RAM_H
struct Battering_Ram : Entity {
  // Functions: The constructor and deconstructor
  Battering_Ram();
  ~Battering_Ram();
};
#endif // BATTERING_RAM_H

/** Inherited class from Entity: Camel **/
#ifndef CAMEL_H
#define CAMEL_H
struct Camel : Entity {
  // Functions: The constructor and deconstructor
  Camel();
  ~Camel();
};
#endif // CAMEL_H

/** Inherited class from Entity: Castle **/
#ifndef CASTLE_H
#define CASTLE_H
struct Castle : Entity {
  // Functions: The constructor and deconstructor
  Castle();
  ~Castle();
};
#endif // CASTLE_H

/** Inherited class from Entity: Fire_Ship **/
#ifndef FIRE_SHIP_H
#define FIRE_SHIP_H
struct Fire_Ship : Entity {
  // Functions: The constructor and deconstructor
  Fire_Ship();
  ~Fire_Ship();
};
#endif // FIRE_SHIP_H

/** Inherited class from Entity: Huskarl_(Goth) **/
#ifndef HUSKARL_GOTH_H
#define HUSKARL_GOTH_H
struct Huskarl_Goth : Entity {
  // Functions: The constructor and deconstructor
  Huskarl_Goth();
  ~Huskarl_Goth();
};
#endif // HUSKARL_GOTH_H

/** Inherited class from Entity: Knight **/
#ifndef KNIGHT_H
#define KNIGHT_H
struct Knight : Entity {
  // Functions: The constructor and deconstructor
  Knight();
  ~Knight();
};
#endif // KNIGHT_H

/** Inherited class from Entity: Longboat_(Viking) **/
#ifndef LONGBOAT_VIKING_H
#define LONGBOAT_VIKING_H
struct Longboat_Viking : Entity {
  // Functions: The constructor and deconstructor
  Longboat_Viking();
  ~Longboat_Viking();
};
#endif // LONGBOAT_VIKING_H

/** Inherited class from Entity: Longbowman_(Briton) **/
#ifndef LONGBOWMAN_BRITON_H
#define LONGBOWMAN_BRITON_H
struct Longbowman_Briton : Entity {
  // Functions: The constructor and deconstructor
  Longbowman_Briton();
  ~Longbowman_Briton();
};
#endif // LONGBOWMAN_BRITON_H

/** Inherited class from Entity: Mameluke_(Saracen) **/
#ifndef MAMELUKE_SARACEN_H
#define MAMELUKE_SARACEN_H
struct Mameluke_Saracen : Entity {
  // Functions: The constructor and deconstructor
  Mameluke_Saracen();
  ~Mameluke_Saracen();
};
#endif // MAMELUKE_SARACEN_H

/** Inherited class from Entity: Mangonel **/
#ifndef MANGONEL_H
#define MANGONEL_H
struct Mangonel : Entity {
  // Functions: The constructor and deconstructor
  Mangonel();
  ~Mangonel();
};
#endif // MANGONEL_H

/** Inherited class from Entity: Wonder **/
#ifndef WONDER_H
#define WONDER_H
struct Wonder : Entity {
  // Variable: Store the number of turns in play
  int turnsInPlay;

  // Functions: The constructor and deconstructor
  Wonder();
  ~Wonder();

  // Function: Update the HP of the Wonder based on the number of turns in play
  void updateWonderHP();
};
#endif // WONDER_H

/** Inherited class from Entity: Trebuchet **/
#ifndef TREBUCHET_H
#define TREBUCHET_H
struct Trebuchet : Entity {
  // Variable: Store whether or not the Trebuchet is packed or unpacked
  int packed;

  // Functions: The constructor and deconstructor
  Trebuchet();
  ~Trebuchet();

  // Function: Update the armor class of the Trebuchet based on if the Trebuchet
  // is packed/unpacked
  void updateTrebuchetArmorClass();
};
#endif // TREBUCHET_H

/** Inherited class from Entity: Samurai_(Japanese) **/
#ifndef SAMURAI_JAPANESE_H
#define SAMURAI_JAPANESE_H
struct Samurai_Japanese : Entity {
  // Functions: The constructor and deconstructor
  Samurai_Japanese();
  ~Samurai_Japanese();
};
#endif // SAMURAI_JAPANESE_H

/** Inherited class from Entity: Woad_Raider_(Celt) **/
#ifndef WOAD_RAIDER_CELT_H
#define WOAD_RAIDER_CELT_H
struct Woad_Raider_Celt : Entity {
  // Functions: The constructor and deconstructor
  Woad_Raider_Celt();
  ~Woad_Raider_Celt();
};
#endif // WOAD_RAIDER_CELT_H

/** Inherited class from Entity: Throwing_Axeman_(Frank) **/
#ifndef THROWING_AXEMAN_FRANK_H
#define THROWING_AXEMAN_FRANK_H
struct Throwing_Axeman_Frank : Entity {
  // Functions: The constructor and deconstructor
  Throwing_Axeman_Frank();
  ~Throwing_Axeman_Frank();
};
#endif // THROWING_AXEMAN_FRANK_H

/** Inherited class from Entity: War_Elephant_(Persian) **/
#ifndef WAR_ELEPHANT_PERSIAN_H
#define WAR_ELEPHANT_PERSIAN_H
struct War_Elephant_Persian : Entity {
  // Functions: The constructor and deconstructor
  War_Elephant_Persian();
  ~War_Elephant_Persian();
};
#endif // WAR_ELEPHANT_PERSIAN_H

/** Inherited class from Entity: Siege_Workshop **/
#ifndef SIEGE_WORKSHOP_H
#define SIEGE_WORKSHOP_H
struct Siege_Workshop : Entity {
  // Functions: The constructor and deconstructor
  Siege_Workshop();
  ~Siege_Workshop();
};
#endif // SIEGE_WORKSHOP_H

/** Inherited class from Entity: Town_Center **/
#ifndef TOWN_CENTER_H
#define TOWN_CENTER_H
struct Town_Center : Entity {
  // Functions: The constructor and deconstructor
  Town_Center();
  ~Town_Center();
};
#endif // TOWN_CENTER_H

/** Inherited class from Entity: Scorpion **/
#ifndef SCORPION_H
#define SCORPION_H
struct Scorpion : Entity {
  // Functions: The constructor and deconstructor
  Scorpion();
  ~Scorpion();
};
#endif // SCORPION_H

/** Inherited class from Entity: Mangudai_(Mongol) **/
#ifndef MANGUDAI_MONGOL_H
#define MANGUDAI_MONGOL_H
struct Mangudai_Mongol : Entity {
  // Functions: The constructor and deconstructor
  Mangudai_Mongol();
  ~Mangudai_Mongol();
};
#endif // MANGUDAI_MONGOL_H

/** Inherited class from Entity: Monastery **/
#ifndef MONASTERY_H
#define MONASTERY_H
struct Monastery : Entity {
  // Functions: The constructor and deconstructor
  Monastery();
  ~Monastery();
};
#endif // MONASTERY_H

/** Inherited class from Entity: Demolition_Ship **/
#ifndef DEMOLITION_SHIP_H
#define DEMOLITION_SHIP_H
struct Demolition_Ship : Entity {
  // Functions: The constructor and deconstructor
  Demolition_Ship();
  ~Demolition_Ship();
};
#endif // DEMOLITION_SHIP_H

/** Inherited class from Entity: Cavalry_Archer **/
#ifndef CAVALRY_ARCHER_H
#define CAVALRY_ARCHER_H
struct Cavalry_Archer : Entity {
  // Functions: The constructor and deconstructor
  Cavalry_Archer();
  ~Cavalry_Archer();
};
#endif // CAVALRY_ARCHER_H

////////////////////////////////// Inhertied from Inherited Classes
/////////////////////////////////////
/** Inherited class from Demolition_Ship: Demolition_Ship_(Viking) **/
#ifndef DEMOLITION_SHIP_VIKING_H
#define DEMOLITION_SHIP_VIKING_H
struct Demolition_Ship_Viking : Demolition_Ship {
  // Functions: The constructor and deconstructor
  Demolition_Ship_Viking();
  ~Demolition_Ship_Viking();
};
#endif // DEMOLITION_SHIP_VIKING_H

/** Inherited class from Dock: Dock_(Persian) **/
#ifndef DOCK_PERSIAN_H
#define DOCK_PERSIAN_H
struct Dock_Persian : Dock {
  // Functions: The constructor and deconstructor
  Dock_Persian();
  ~Dock_Persian();
};
#endif // DOCK_PERSIAN_H

/** Inherited class from Dock: Dock_(Viking) **/
#ifndef DOCK_VIKING_H
#define DOCK_VIKING_H
struct Dock_Viking : Dock {
  // Functions: The constructor and deconstructor
  Dock_Viking();
  ~Dock_Viking();
};
#endif // DOCK_VIKING_H

/** Inherited class from Fishing_Ship: Fishing_Ship_(Japanese) **/
#ifndef FISHING_SHIP_JAPANESE_H
#define FISHING_SHIP_JAPANESE_H
struct Fishing_Ship_Japanese : Fishing_Ship {
  // Functions: The constructor and deconstructor
  Fishing_Ship_Japanese();
  ~Fishing_Ship_Japanese();
};
#endif // FISHING_SHIP_JAPANESE_H

/** Inherited class from Fishing_Ship: Fishing_Ship_(Persian) **/
#ifndef FISHING_SHIP_PERSIAN_H
#define FISHING_SHIP_PERSIAN_H
struct Fishing_Ship_Persian : Fishing_Ship {
  // Functions: The constructor and deconstructor
  Fishing_Ship_Persian();
  ~Fishing_Ship_Persian();
};
#endif // FISHING_SHIP_PERSIAN_H

/** Inherited class from Gold_Mine: Gold_Mine_(Japanese) **/
#ifndef GOLD_MINE_JAPANESE_H
#define GOLD_MINE_JAPANESE_H
struct Gold_Mine_Japanese : Gold_Mine {
  // Functions: The constructor and deconstructor
  Gold_Mine_Japanese();
  ~Gold_Mine_Japanese();
};
#endif // GOLD_MINE_JAPANESE_H

/** Inherited class from Lumber_Camp: Lumber_Camp_(Japanese) **/
#ifndef LUMBER_CAMP_JAPANESE_H
#define LUMBER_CAMP_JAPANESE_H
struct Lumber_Camp_Japanese : Lumber_Camp {
  // Functions: The constructor and deconstructor
  Lumber_Camp_Japanese();
  ~Lumber_Camp_Japanese();
};
#endif // LUMBER_CAMP_JAPANESE_H

/** Inherited class from Militia: Militia_(Celt) **/
#ifndef MILITIA_CELT_H
#define MILITIA_CELT_H
struct Militia_Celt : Militia {
  // Functions: The constructor and deconstructor
  Militia_Celt();
  ~Militia_Celt();
};
#endif // MILITIA_CELT_H

/** Inherited class from Militia: Militia_(Goth) **/
#ifndef MILITIA_GOTH_H
#define MILITIA_GOTH_H
struct Militia_Goth : Militia {
  // Functions: The constructor and deconstructor
  Militia_Goth();
  ~Militia_Goth();
};
#endif // MILITIA_GOTH_H

/** Inherited class from Mill: Mill_(Japanese) **/
#ifndef MILL_JAPANESE_H
#define MILL_JAPANESE_H
struct Mill_Japanese : Mill {
  // Functions: The constructor and deconstructor
  Mill_Japanese();
  ~Mill_Japanese();
};
#endif // MILL_JAPANESE_H

/** Inherited class from Stone_Mine: Stone_Mine_(Japanese) **/
#ifndef STONE_MINE_JAPANESE_H
#define STONE_MINE_JAPANESE_H
struct Stone_Mine_Japanese : Stone_Mine {
  // Functions: The constructor and deconstructor
  Stone_Mine_Japanese();
  ~Stone_Mine_Japanese();
};
#endif // STONE_MINE_JAPANESE_H

/** Inherited class from Archer: Archer_(Saracen) **/
#ifndef ARCHER_SARACEN_H
#define ARCHER_SARACEN_H
struct Archer_Saracen : Archer {
  // Functions: The constructor and deconstructor
  Archer_Saracen();
  ~Archer_Saracen();
};
#endif // ARCHER_SARACEN_H

/** Inherited class from Galley: Galley_(Japanese) **/
#ifndef GALLEY_JAPANESE_H
#define GALLEY_JAPANESE_H
struct Galley_Japanese : Galley {
  // Functions: The constructor and deconstructor
  Galley_Japanese();
  ~Galley_Japanese();
};
#endif // GALLEY_JAPANESE_H

/** Inherited class from Galley: Galley_(Saracen) **/
#ifndef GALLEY_SARACEN_H
#define GALLEY_SARACEN_H
struct Galley_Saracen : Galley {
  // Functions: The constructor and deconstructor
  Galley_Saracen();
  ~Galley_Saracen();
};
#endif // GALLEY_SARACEN_H

/** Inherited class from Galley: Galley_(Viking) **/
#ifndef GALLEY_VIKING_H
#define GALLEY_VIKING_H
struct Galley_Viking : Galley {
  // Functions: The constructor and deconstructor
  Galley_Viking();
  ~Galley_Viking();
};
#endif // GALLEY_VIKING_H

/** Inherited class from Stone_Wall: Gate **/
#ifndef STONE_GATE_H
#define STONE_GATE_H
struct Stone_Gate : Stone_Wall {
  // Functions: The constructor and deconstructor
  Stone_Gate();
  ~Stone_Gate();
};
#endif // STONE_GATE_H

/** Inherited class from Militia: Man_At_Arms **/
#ifndef MAN_AT_ARMS_H
#define MAN_AT_ARMS_H
struct Man_At_Arms : Militia {
  // Functions: The constructor and deconstructor
  Man_At_Arms();
  ~Man_At_Arms();
};
#endif // MAN_AT_ARMS_H

/** Inherited class from Man_At_Arms: Man_At_Arms_(Viking) **/
#ifndef MAN_AT_ARMS_VIKING_H
#define MAN_AT_ARMS_VIKING_H
struct Man_At_Arms_Viking : Man_At_Arms {
  // Functions: The constructor and deconstructor
  Man_At_Arms_Viking();
  ~Man_At_Arms_Viking();
};
#endif // MAN_AT_ARMS_VIKING_H

/** Inherited class from Man_At_Arms: Man_At_Arms_(Celt) **/
#ifndef MAN_AT_ARMS_CELT_H
#define MAN_AT_ARMS_CELT_H
struct Man_At_Arms_Celt : Man_At_Arms {
  // Functions: The constructor and deconstructor
  Man_At_Arms_Celt();
  ~Man_At_Arms_Celt();
};
#endif // MAN_AT_ARMS_CELT_H

/** Inherited class from Man_At_Arms: Man_At_Arms_(Goth) **/
#ifndef MAN_AT_ARMS_GOTH_H
#define MAN_AT_ARMS_GOTH_H
struct Man_At_Arms_Goth : Man_At_Arms {
  // Functions: The constructor and deconstructor
  Man_At_Arms_Goth();
  ~Man_At_Arms_Goth();
};
#endif // MAN_AT_ARMS_GOTH_H

/** Inherited class from Man_At_Arms: Man_At_Arms_(Japanese) **/
#ifndef MAN_AT_ARMS_JAPANESE_H
#define MAN_AT_ARMS_JAPANESE_H
struct Man_At_Arms_Japanese : Man_At_Arms {
  // Functions: The constructor and deconstructor
  Man_At_Arms_Japanese();
  ~Man_At_Arms_Japanese();
};
#endif // MAN_AT_ARMS_JAPANESE_H

/** Inherited class from Market: Market_(Saracen) **/
#ifndef MARKET_SARACEN_H
#define MARKET_SARACEN_H
struct Market_Saracen : Market {
  // Functions: The constructor and deconstructor
  Market_Saracen();
  ~Market_Saracen();
};
#endif // MARKET_SARACEN_H

/** Inherited class from Spearman: Spearman_(Japanese) **/
#ifndef SPEARMAN_JAPANESE_H
#define SPEARMAN_JAPANESE_H
struct Spearman_Japanese : Spearman {
  // Functions: The constructor and deconstructor
  Spearman_Japanese();
  ~Spearman_Japanese();
};
#endif // SPEARMAN_JAPANESE_H

/** Inherited class from Spearman: Spearman_(Viking) **/
#ifndef SPEARMAN_VIKING_H
#define SPEARMAN_VIKING_H
struct Spearman_Viking : Spearman {
  // Functions: The constructor and deconstructor
  Spearman_Viking();
  ~Spearman_Viking();
};
#endif // SPEARMAN_VIKING_H

/** Inherited class from Spearman: Spearman_(Celt) **/
#ifndef SPEARMAN_CELT_H
#define SPEARMAN_CELT_H
struct Spearman_Celt : Spearman {
  // Functions: The constructor and deconstructor
  Spearman_Celt();
  ~Spearman_Celt();
};
#endif // SPEARMAN_CELT_H

/** Inherited class from Spearman: Spearman_(Goth) **/
#ifndef SPEARMAN_GOTH_H
#define SPEARMAN_GOTH_H
struct Spearman_Goth : Spearman {
  // Functions: The constructor and deconstructor
  Spearman_Goth();
  ~Spearman_Goth();
};
#endif // SPEARMAN_GOTH_H

/** Inherited class from Battering_Ram: Battering_Ram_(Celt) **/
#ifndef BATTERING_RAM_CELT_H
#define BATTERING_RAM_CELT_H
struct Battering_Ram_Celt : Battering_Ram {
  // Functions: The constructor and deconstructor
  Battering_Ram_Celt();
  ~Battering_Ram_Celt();
};
#endif // BATTERING_RAM_CELT_H

/** Inherited class from Entity: Berserk_(Viking) **/
#ifndef BERSERK_VIKING_H
#define BERSERK_VIKING_H
struct Berserk_Viking : Entity {
  // Functions: The constructor and deconstructor
  Berserk_Viking();
  ~Berserk_Viking();
};
#endif // BERSERK_VIKING_H

/** Inherited class from Castle: Castle_(Frank) **/
#ifndef CASTLE_FRANK_H
#define CASTLE_FRANK_H
struct Castle_Frank : Castle {
  // Functions: The constructor and deconstructor
  Castle_Frank();
  ~Castle_Frank();
};
#endif // CASTLE_FRANK_H

/** Inherited class from Cavalry_Archer: Cavalry_Archer_(Mongol) **/
#ifndef CAVALRY_ARCHER_MONGOL_H
#define CAVALRY_ARCHER_MONGOL_H
struct Cavalry_Archer_Mongol : Cavalry_Archer {
  // Functions: The constructor and deconstructor
  Cavalry_Archer_Mongol();
  ~Cavalry_Archer_Mongol();
};
#endif // CAVALRY_ARCHER_MONGOL_H

/** Inherited class from Cavalry_Archer: Cavalry_Archer_(Saracen) **/
#ifndef CAVALRY_ARCHER_SARACEN_H
#define CAVALRY_ARCHER_SARACEN_H
struct Cavalry_Archer_Saracen : Cavalry_Archer {
  // Functions: The constructor and deconstructor
  Cavalry_Archer_Saracen();
  ~Cavalry_Archer_Saracen();
};
#endif // CAVALRY_ARCHER_SARACEN_H

/** Inherited class from Archer: Crossbowman **/
#ifndef CROSSBOWMAN_H
#define CROSSBOWMAN_H
struct Crossbowman : Archer {
  // Functions: The constructor and deconstructor
  Crossbowman();
  ~Crossbowman();
};
#endif // CROSSBOWMAN_H

/** Inherited class from Crossbowman: Crossbowman_(Saracen) **/
#ifndef CROSSBOWMAN_SARACEN_H
#define CROSSBOWMAN_SARACEN_H
struct Crossbowman_Saracen : Crossbowman {
  // Functions: The constructor and deconstructor
  Crossbowman_Saracen();
  ~Crossbowman_Saracen();
};
#endif // CROSSBOWMAN_SARACEN_H

/** Inherited class from Stone_Wall: Fortified_Wall **/
#ifndef FORTIFIED_WALL_H
#define FORTIFIED_WALL_H
struct Fortified_Wall : Stone_Wall {
  // Functions: The constructor and deconstructor
  Fortified_Wall();
  ~Fortified_Wall();
};
#endif // FORTIFIED_WALL_H

/** Inherited class from Knight: Knight_(Frank) **/
#ifndef KNIGHT_FRANK_H
#define KNIGHT_FRANK_H
struct Knight_Frank : Knight {
  // Functions: The constructor and deconstructor
  Knight_Frank();
  ~Knight_Frank();
};
#endif // KNIGHT_FRANK_H

/** Inherited class from Knight: Knight_(Persian) **/
#ifndef KNIGHT_PERSIAN_H
#define KNIGHT_PERSIAN_H
struct Knight_Persian : Knight {
  // Functions: The constructor and deconstructor
  Knight_Persian();
  ~Knight_Persian();
};
#endif // KNIGHT_PERSIAN_H

/** Inherited class from Scout_Cavalry: Light_Cavalry **/
#ifndef LIGHT_CAVALRY_H
#define LIGHT_CAVALRY_H
struct Light_Cavalry : Scout_Cavalry {
  // Functions: The constructor and deconstructor
  Light_Cavalry();
  ~Light_Cavalry();
};
#endif // LIGHT_CAVALRY_H

/** Inherited class from Light_Cavalry: Light_Cavalry_(Mongol) **/
#ifndef LIGHT_CAVALRY_MONGOL_H
#define LIGHT_CAVALRY_MONGOL_H
struct Light_Cavalry_Mongol : Light_Cavalry {
  // Functions: The constructor and deconstructor
  Light_Cavalry_Mongol();
  ~Light_Cavalry_Mongol();
};
#endif // LIGHT_CAVALRY_MONGOL_H

/** Inherited class from Man_At_Arms: Long_Swordsman **/
#ifndef LONG_SWORDSMAN_H
#define LONG_SWORDSMAN_H
struct Long_Swordsman : Man_At_Arms {
  // Functions: The constructor and deconstructor
  Long_Swordsman();
  ~Long_Swordsman();
};
#endif // LONG_SWORDSMAN_H

/** Inherited class from Long_Swordsman: Long_Swordsman_(Celt) **/
#ifndef LONG_SWORDSMAN_CELT_H
#define LONG_SWORDSMAN_CELT_H
struct Long_Swordsman_Celt : Long_Swordsman {
  // Functions: The constructor and deconstructor
  Long_Swordsman_Celt();
  ~Long_Swordsman_Celt();
};
#endif // LONG_SWORDSMAN_CELT_H

/** Inherited class from Long_Swordsman: Long_Swordsman_(Goth) **/
#ifndef LONG_SWORDSMAN_GOTH_H
#define LONG_SWORDSMAN_GOTH_H
struct Long_Swordsman_Goth : Long_Swordsman {
  // Functions: The constructor and deconstructor
  Long_Swordsman_Goth();
  ~Long_Swordsman_Goth();
};
#endif // LONG_SWORDSMAN_GOTH_H

/** Inherited class from Long_Swordsman: Longswordsman_(Japanese) **/
#ifndef LONGSWORDSMAN_JAPANESE_H
#define LONGSWORDSMAN_JAPANESE_H
struct Long_Swordsman_Japanese : Long_Swordsman {
  // Functions: The constructor and deconstructor
  Long_Swordsman_Japanese();
  ~Long_Swordsman_Japanese();
};
#endif // LONGSWORDSMAN_JAPANESE_H

/** Inherited class from Entity: Longswordsman_(Viking) **/
#ifndef LONGSWORDSMAN_VIKING_H
#define LONGSWORDSMAN_VIKING_H
struct Long_Swordsman_Viking : Long_Swordsman {
  // Functions: The constructor and deconstructor
  Long_Swordsman_Viking();
  ~Long_Swordsman_Viking();
};
#endif // LONGSWORDSMAN_VIKING_H

/** Inherited class from Mangonel: Mangonel_(Celt) **/
#ifndef MANGONEL_CELT_H
#define MANGONEL_CELT_H
struct Mangonel_Celt : Mangonel {
  // Functions: The constructor and deconstructor
  Mangonel_Celt();
  ~Mangonel_Celt();
};
#endif // MANGONEL_CELT_H

/** Inherited class from Spearman: Pikeman **/
#ifndef PIKEMAN_H
#define PIKEMAN_H
struct Pikeman : Spearman {
  // Functions: The constructor and deconstructor
  Pikeman();
  ~Pikeman();
};
#endif // PIKEMAN_H

/** Inherited class from Pikeman: Pikeman_(Celt) **/
#ifndef PIKEMAN_CELT_H
#define PIKEMAN_CELT_H
struct Pikeman_Celt : Pikeman {
  // Functions: The constructor and deconstructor
  Pikeman_Celt();
  ~Pikeman_Celt();
};
#endif // PIKEMAN_CELT_H

/** Inherited class from Pikeman: Pikeman_(Goth) **/
#ifndef PIKEMAN_GOTH_H
#define PIKEMAN_GOTH_H
struct Pikeman_Goth : Pikeman {
  // Functions: The constructor and deconstructor
  Pikeman_Goth();
  ~Pikeman_Goth();
};
#endif // PIKEMAN_GOTH_H

/** Inherited class from Pikeman: Pikeman_(Japanese) **/
#ifndef PIKEMAN_JAPANESE_H
#define PIKEMAN_JAPANESE_H
struct Pikeman_Japanese : Pikeman {
  // Functions: The constructor and deconstructor
  Pikeman_Japanese();
  ~Pikeman_Japanese();
};
#endif // PIKEMAN_JAPANESE_H

/** Inherited class from Pikeman: Pikeman_(Viking) **/
#ifndef PIKEMAN_VIKING_H
#define PIKEMAN_VIKING_H
struct Pikeman_Viking : Pikeman {
  // Functions: The constructor and deconstructor
  Pikeman_Viking();
  ~Pikeman_Viking();
};
#endif // PIKEMAN_VIKING_H

/** Inherited class from Scorpion: Scorpion_(Celt) **/
#ifndef SCORPION_CELT_H
#define SCORPION_CELT_H
struct Scorpion_Celt : Scorpion {
  // Functions: The constructor and deconstructor
  Scorpion_Celt();
  ~Scorpion_Celt();
};
#endif // SCORPION_CELT_H

/** Inherited class from Town_Center: Town_Center_(Briton) **/
#ifndef TOWN_CENTER_BRITON_H
#define TOWN_CENTER_BRITON_H
struct Town_Center_Briton : Town_Center {
  // Functions: The constructor and deconstructor
  Town_Center_Briton();
  ~Town_Center_Briton();
};
#endif // TOWN_CENTER_BRITON_H

/** Inherited class from Town_Center: Town_Center_(Persian) **/
#ifndef TOWN_CENTER_PERSIAN_H
#define TOWN_CENTER_PERSIAN_H
struct Town_Center_Persian : Town_Center {
  // Functions: The constructor and deconstructor
  Town_Center_Persian();
  ~Town_Center_Persian();
};
#endif // TOWN_CENTER_PERSIAN_H

/** Inherited class from Galley: War_Galley **/
#ifndef WAR_GALLEY_H
#define WAR_GALLEY_H
struct War_Galley : Galley {
  // Functions: The constructor and deconstructor
  War_Galley();
  ~War_Galley();
};
#endif // WAR_GALLEY_H

/** Inherited class from War_Galley: War_Galley_(Japanese) **/
#ifndef WAR_GALLEY_JAPANESE_H
#define WAR_GALLEY_JAPANESE_H
struct War_Galley_Japanese : War_Galley {
  // Functions: The constructor and deconstructor
  War_Galley_Japanese();
  ~War_Galley_Japanese();
};
#endif // WAR_GALLEY_JAPANESE_H

/** Inherited class from War_Galley: War_Galley_(Saracen) **/
#ifndef WAR_GALLEY_SARACEN_H
#define WAR_GALLEY_SARACEN_H
struct War_Galley_Saracen : War_Galley {
  // Functions: The constructor and deconstructor
  War_Galley_Saracen();
  ~War_Galley_Saracen();
};
#endif // WAR_GALLEY_SARACEN_H

/** Inherited class from War_Galley: War_Galley_(Viking) **/
#ifndef WAR_GALLEY_VIKING_H
#define WAR_GALLEY_VIKING_H
struct War_Galley_Viking : War_Galley {
  // Functions: The constructor and deconstructor
  War_Galley_Viking();
  ~War_Galley_Viking();
};
#endif // WAR_GALLEY_VIKING_H

/** Inherited class from Crossbowman: Arbalest **/
#ifndef ARBALEST_H
#define ARBALEST_H
struct Arbalest : Crossbowman {
  // Functions: The constructor and deconstructor
  Arbalest();
  ~Arbalest();
};
#endif // ARBALEST_H

/** Inherited class from Arbalest: Arbalest_(Briton) **/
#ifndef ARBALEST_BRITON_H
#define ARBALEST_BRITON_H
struct Arbalest_Briton : Arbalest {
  // Functions: The constructor and deconstructor
  Arbalest_Briton();
  ~Arbalest_Briton();
};
#endif // ARBALEST_BRITON_H

/** Inherited class from Arbalest: Arbalest_(Saracen) **/
#ifndef ARBALEST_SARACEN_H
#define ARBALEST_SARACEN_H
struct Arbalest_Saracen : Arbalest {
  // Functions: The constructor and deconstructor
  Arbalest_Saracen();
  ~Arbalest_Saracen();
};
#endif // ARBALEST_SARACEN_H

/** Inherited class from Battering_Ram: Capped_Ram **/
#ifndef CAPPED_RAM_H
#define CAPPED_RAM_H
struct Capped_Ram : Battering_Ram {
  // Functions: The constructor and deconstructor
  Capped_Ram();
  ~Capped_Ram();
};
#endif // CAPPED_RAM_H

/** Inherited class from Capped_Ram: Capped_Ram_(Celt) **/
#ifndef CAPPED_RAM_CELT_H
#define CAPPED_RAM_CELT_H
struct Capped_Ram_Celt : Capped_Ram {
  // Functions: The constructor and deconstructor
  Capped_Ram_Celt();
  ~Capped_Ram_Celt();
};
#endif // CAPPED_RAM_CELT_H

/** Inherited class from Knight: Cavalier **/
#ifndef CAVALIER_H
#define CAVALIER_H
struct Cavalier : Knight {
  // Functions: The constructor and deconstructor
  Cavalier();
  ~Cavalier();
};
#endif // CAVALIER_H

/** Inherited class from Cavalier: Cavalier_(Frank) **/
#ifndef CAVALIER_FRANK_H
#define CAVALIER_FRANK_H
struct Cavalier_Frank : Cavalier {
  // Functions: The constructor and deconstructor
  Cavalier_Frank();
  ~Cavalier_Frank();
};
#endif // CAVALIER_FRANK_H

/** Inherited class from Cavalier: Cavalier_(Persian) **/
#ifndef CAVALIER_PERSIAN_H
#define CAVALIER_PERSIAN_H
struct Cavalier_Persian : Cavalier {
  // Functions: The constructor and deconstructor
  Cavalier_Persian();
  ~Cavalier_Persian();
};
#endif // CAVALIER_PERSIAN_H

/** Inherited class from Long_Swordsman: Two_Handed_Swordsman **/
#ifndef TWO_HANDED_SWORDSMAN_H
#define TWO_HANDED_SWORDSMAN_H
struct Two_Handed_Swordsman : Long_Swordsman {
  // Functions: The constructor and deconstructor
  Two_Handed_Swordsman();
  ~Two_Handed_Swordsman();
};
#endif // TWO_HANDED_SWORDSMAN_H

/** Inherited class from Two_Handed_Swordsman: Two_Handed_Swordsman_(Celt) **/
#ifndef TWO_HANDED_SWORDSMAN_CELT_H
#define TWO_HANDED_SWORDSMAN_CELT_H
struct Two_Handed_Swordsman_Celt : Two_Handed_Swordsman {
  // Functions: The constructor and deconstructor
  Two_Handed_Swordsman_Celt();
  ~Two_Handed_Swordsman_Celt();
};
#endif // TWO_HANDED_SWORDSMAN_CELT_H

/** Inherited class from Two_Handed_Swordsman: Two_Handed_Swordsman_(Goth) **/
#ifndef TWO_HANDED_SWORDSMAN_GOTH_H
#define TWO_HANDED_SWORDSMAN_GOTH_H
struct Two_Handed_Swordsman_Goth : Two_Handed_Swordsman {
  // Functions: The constructor and deconstructor
  Two_Handed_Swordsman_Goth();
  ~Two_Handed_Swordsman_Goth();
};
#endif // TWO_HANDED_SWORDSMAN_GOTH_H

/** Inherited class from Two_Handed_Swordsman: Two_Handed_Swordsman_(Japanese)
 * **/
#ifndef TWO_HANDED_SWORDSMAN_JAPANESE_H
#define TWO_HANDED_SWORDSMAN_JAPANESE_H
struct Two_Handed_Swordsman_Japanese : Two_Handed_Swordsman {
  // Functions: The constructor and deconstructor
  Two_Handed_Swordsman_Japanese();
  ~Two_Handed_Swordsman_Japanese();
};
#endif // TWO_HANDED_SWORDSMAN_JAPANESE_H

/** Inherited class from Two_Handed_Swordsman: Two_Handed_Swordsman_(Viking) **/
#ifndef TWO_HANDED_SWORDSMAN_VIKING_H
#define TWO_HANDED_SWORDSMAN_VIKING_H
struct Two_Handed_Swordsman_Viking : Two_Handed_Swordsman {
  // Functions: The constructor and deconstructor
  Two_Handed_Swordsman_Viking();
  ~Two_Handed_Swordsman_Viking();
};
#endif // TWO_HANDED_SWORDSMAN_VIKING_H

/** Inherited class from Two_Handed_Swordsman: Champion **/
#ifndef CHAMPION_H
#define CHAMPION_H
struct Champion : Two_Handed_Swordsman {
  // Functions: The constructor and deconstructor
  Champion();
  ~Champion();
};
#endif // CHAMPION_H

/** Inherited class from Champion: Champion_(Celt) **/
#ifndef CHAMPION_CELT_H
#define CHAMPION_CELT_H
struct Champion_Celt : Champion {
  // Functions: The constructor and deconstructor
  Champion_Celt();
  ~Champion_Celt();
};
#endif // CHAMPION_CELT_H

/** Inherited class from Champion: Champion_(Goth) **/
#ifndef CHAMPION_GOTH_H
#define CHAMPION_GOTH_H
struct Champion_Goth : Champion {
  // Functions: The constructor and deconstructor
  Champion_Goth();
  ~Champion_Goth();
};
#endif // CHAMPION_GOTH_H

/** Inherited class from Champion: Champion_(Japanese) **/
#ifndef CHAMPION_JAPANESE_H
#define CHAMPION_JAPANESE_H
struct Champion_Japanese : Champion {
  // Functions: The constructor and deconstructor
  Champion_Japanese();
  ~Champion_Japanese();
};
#endif // CHAMPION_JAPANESE_H

/** Inherited class from Champion: Champion_(Viking) **/
#ifndef CHAMPION_VIKING_H
#define CHAMPION_VIKING_H
struct Champion_Viking : Champion {
  // Functions: The constructor and deconstructor
  Champion_Viking();
  ~Champion_Viking();
};
#endif // CHAMPION_VIKING_H

/** Inherited class from Berserk_(Viking): Elite_Berserk_(Viking) **/
#ifndef ELITE_BERSERK_VIKING_H
#define ELITE_BERSERK_VIKING_H
struct Elite_Berserk_Viking : Huskarl_Goth {
  // Functions: The constructor and deconstructor
  Elite_Berserk_Viking();
  ~Elite_Berserk_Viking();
};
#endif // ELITE_BERSERK_VIKING_H

/** Inherited class from Huskarl_(Goth): Elite_Huskarl_(Goth) **/
#ifndef ELITE_HUSKARL_GOTH_H
#define ELITE_HUSKARL_GOTH_H
struct Elite_Huskarl_Goth : Huskarl_Goth {
  // Functions: The constructor and deconstructor
  Elite_Huskarl_Goth();
  ~Elite_Huskarl_Goth();
};
#endif // ELITE_HUSKARL_GOTH_H

/** Inherited class from Longboat_(Viking): Elite_Longboat_(Viking) **/
#ifndef ELITE_LONGBOAT_VIKING_H
#define ELITE_LONGBOAT_VIKING_H
struct Elite_Longboat_Viking : Longboat_Viking {
  // Functions: The constructor and deconstructor
  Elite_Longboat_Viking();
  ~Elite_Longboat_Viking();
};
#endif // ELITE_LONGBOAT_VIKING_H

/** Inherited class from Longbowman_(Briton): Elite_Longbowman_(Briton) **/
#ifndef ELITE_LONGBOWMAN_BRITON_H
#define ELITE_LONGBOWMAN_BRITON_H
struct Elite_Longbowman_Briton : Longbowman_Briton {
  // Functions: The constructor and deconstructor
  Elite_Longbowman_Briton();
  ~Elite_Longbowman_Briton();
};
#endif // ELITE_LONGBOWMAN_BRITON_H

/** Inherited class from Skirmisher: Elite_Skirmisher **/
#ifndef ELITE_SKIRMISHER_H
#define ELITE_SKIRMISHER_H
struct Elite_Skirmisher : Skirmisher {
  // Functions: The constructor and deconstructor
  Elite_Skirmisher();
  ~Elite_Skirmisher();
};
#endif // ELITE_SKIRMISHER_H

/** Inherited class from Mangudai: Elite_Mangudai_(Mongol) **/
#ifndef ELITE_MANGUDAI_MONGOL_H
#define ELITE_MANGUDAI_MONGOL_H
struct Elite_Mangudai_Mongol : Mangudai_Mongol {
  // Functions: The constructor and deconstructor
  Elite_Mangudai_Mongol();
  ~Elite_Mangudai_Mongol();
};
#endif // ELITE_MANGUDAI_MONGOL_H

/** Inherited class from Mameluke: Elite_Mameluke_(Saracen) **/
#ifndef ELITE_MAMELUKE_SARACEN_H
#define ELITE_MAMELUKE_SARACEN_H
struct Elite_Mameluke_Saracen : Mameluke_Saracen {
  // Functions: The constructor and deconstructor
  Elite_Mameluke_Saracen();
  ~Elite_Mameluke_Saracen();
};
#endif // ELITE_MAMELUKE_SARACEN_H

/** Inherited class from Throwing_Axeman_(Frank): Elite_Throwing_Axeman_(Frank)
 * **/
#ifndef ELITE_THROWING_AXEMAN_FRANK_H
#define ELITE_THROWING_AXEMAN_FRANK_H
struct Elite_Throwing_Axeman_Frank : Throwing_Axeman_Frank {
  // Functions: The constructor and deconstructor
  Elite_Throwing_Axeman_Frank();
  ~Elite_Throwing_Axeman_Frank();
};
#endif // ELITE_THROWING_AXEMAN_FRANK_H

/** Inherited class from Samurai_(Japanese): Elite_Samurai_(Japanese) **/
#ifndef ELITE_SAMURAI_JAPANESE_H
#define ELITE_SAMURAI_JAPANESE_H
struct Elite_Samurai_Japanese : Samurai_Japanese {
  // Functions: The constructor and deconstructor
  Elite_Samurai_Japanese();
  ~Elite_Samurai_Japanese();
};
#endif // ELITE_SAMURAI_JAPANESE_H

/** Inherited class from War_Elephant_(Persian): Elite_War_Elephant_(Persian)
 * **/
#ifndef ELITE_WAR_ELEPHANT_PERSIAN_H
#define ELITE_WAR_ELEPHANT_PERSIAN_H
struct Elite_War_Elephant_Persian : War_Elephant_Persian {
  // Functions: The constructor and deconstructor
  Elite_War_Elephant_Persian();
  ~Elite_War_Elephant_Persian();
};
#endif // ELITE_WAR_ELEPHANT_PERSIAN_H

/** Inherited class from Woad_Raider_(Celt): Elite_Woad_Raider_(Celt) **/
#ifndef ELITE_WOAD_RAIDER_CELT_H
#define ELITE_WOAD_RAIDER_CELT_H
struct Elite_Woad_Raider_Celt : Woad_Raider_Celt {
  // Functions: The constructor and deconstructor
  Elite_Woad_Raider_Celt();
  ~Elite_Woad_Raider_Celt();
};
#endif // ELITE_WOAD_RAIDER_CELT_H

/** Inherited class from Fire_Ship: Fast_Fire_Ship **/
#ifndef FAST_FIRE_SHIP_H
#define FAST_FIRE_SHIP_H
struct Fast_Fire_Ship : Fire_Ship {
  // Functions: The constructor and deconstructor
  Fast_Fire_Ship();
  ~Fast_Fire_Ship();
};
#endif // FAST_FIRE_SHIP_H

/** Inherited class from War_Galley: Galleon **/
#ifndef GALLEON_H
#define GALLEON_H
struct Galleon : War_Galley {
  // Functions: The constructor and deconstructor
  Galleon();
  ~Galleon();
};
#endif // GALLEON_H

/** Inherited class from Galleon: Galleon_(Saracen) **/
#ifndef GALLEON_SARACEN_H
#define GALLEON_SARACEN_H
struct Galleon_Saracen : Galleon {
  // Functions: The constructor and deconstructor
  Galleon_Saracen();
  ~Galleon_Saracen();
};
#endif // GALLEON_SARACEN_H

/** Inherited class from Galleon: Galleon_(Viking) **/
#ifndef GALLEON_VIKING_H
#define GALLEON_VIKING_H
struct Galleon_Viking : Galleon {
  // Functions: The constructor and deconstructor
  Galleon_Viking();
  ~Galleon_Viking();
};
#endif // GALLEON_VIKING_H

/** Inherited class from Galleon: Galleon_(Japanese) **/
#ifndef GALLEON_JAPANESE_H
#define GALLEON_JAPANESE_H
struct Galleon_Japanese : Galleon {
  // Functions: The constructor and deconstructor
  Galleon_Japanese();
  ~Galleon_Japanese();
};
#endif // GALLEON_JAPANESE_H

/** Inherited class from Camel: Heavy_Camel **/
#ifndef HEAVY_CAMEL_H
#define HEAVY_CAMEL_H
struct Heavy_Camel : Camel {
  // Functions: The constructor and deconstructor
  Heavy_Camel();
  ~Heavy_Camel();
};
#endif // HEAVY_CAMEL_H

/** Inherited class from Cavalry_Archer: Heavy_Cavalry_Archer **/
#ifndef HEAVY_CAVALRY_ARCHER_H
#define HEAVY_CAVALRY_ARCHER_H
struct Heavy_Cavalry_Archer : Cavalry_Archer {
  // Functions: The constructor and deconstructor
  Heavy_Cavalry_Archer();
  ~Heavy_Cavalry_Archer();
};
#endif // HEAVY_CAVALRY_ARCHER_H

/** Inherited class from Heavy_Cavalry_Archer: Heavy_Cavalry_Archer_(Mongol) **/
#ifndef HEAVY_CAVALRY_ARCHER_MONGOL_H
#define HEAVY_CAVALRY_ARCHER_MONGOL_H
struct Heavy_Cavalry_Archer_Mongol : Heavy_Cavalry_Archer {
  // Functions: The constructor and deconstructor
  Heavy_Cavalry_Archer_Mongol();
  ~Heavy_Cavalry_Archer_Mongol();
};
#endif // HEAVY_CAVALRY_ARCHER_MONGOL_H

/** Inherited class from Heavy_Cavalry_Archer: Heavy_Cavalry_Archer_(Saracen)
 * **/
#ifndef HEAVY_CAVALRY_ARCHER_SARACEN_H
#define HEAVY_CAVALRY_ARCHER_SARACEN_H
struct Heavy_Cavalry_Archer_Saracen : Heavy_Cavalry_Archer {
  // Functions: The constructor and deconstructor
  Heavy_Cavalry_Archer_Saracen();
  ~Heavy_Cavalry_Archer_Saracen();
};
#endif // HEAVY_CAVALRY_ARCHER_SARACEN_H

/** Inherited class from Demolition_Ship: Heavy_Demolition_Ship **/
#ifndef HEAVY_DEMOLITION_SHIP_H
#define HEAVY_DEMOLITION_SHIP_H
struct Heavy_Demolition_Ship : Demolition_Ship {
  // Functions: The constructor and deconstructor
  Heavy_Demolition_Ship();
  ~Heavy_Demolition_Ship();
};
#endif // HEAVY_DEMOLITION_SHIP_H

/** Inherited class from Heavy_Demolition_Ship: Heavy_Demolition_Ship_(Viking)
 * **/
#ifndef HEAVY_DEMOLITION_SHIP_VIKING_H
#define HEAVY_DEMOLITION_SHIP_VIKING_H
struct Heavy_Demolition_Ship_Viking : Heavy_Demolition_Ship {
  // Functions: The constructor and deconstructor
  Heavy_Demolition_Ship_Viking();
  ~Heavy_Demolition_Ship_Viking();
};
#endif // HEAVY_DEMOLITION_SHIP_VIKING_H

/** Inherited class from Scorpion: Heavy_Scorpion **/
#ifndef HEAVY_SCORPION_H
#define HEAVY_SCORPION_H
struct Heavy_Scorpion : Scorpion {
  // Functions: The constructor and deconstructor
  Heavy_Scorpion();
  ~Heavy_Scorpion();
};
#endif // HEAVY_SCORPION_H

/** Inherited class from Heavy_Scorpion: Heavy_Scorpion_(Celt) **/
#ifndef HEAVY_SCORPION_CELT_H
#define HEAVY_SCORPION_CELT_H
struct Heavy_Scorpion_Celt : Heavy_Scorpion {
  // Functions: The constructor and deconstructor
  Heavy_Scorpion_Celt();
  ~Heavy_Scorpion_Celt();
};
#endif // HEAVY_SCORPION_CELT_H

/** Inherited class from Mangonel: Onager **/
#ifndef ONAGER_H
#define ONAGER_H
struct Onager : Mangonel {
  // Functions: The constructor and deconstructor
  Onager();
  ~Onager();
};
#endif // ONAGER_H

/** Inherited class from Onager: Onager_(Celt) **/
#ifndef ONAGER_CELT_H
#define ONAGER_CELT_H
struct Onager_Celt : Onager {
  // Functions: The constructor and deconstructor
  Onager_Celt();
  ~Onager_Celt();
};
#endif // ONAGER_CELT_H

/** Inherited class from Cavalier: Paladin **/
#ifndef PALADIN_H
#define PALADIN_H
struct Paladin : Cavalier {
  // Functions: The constructor and deconstructor
  Paladin();
  ~Paladin();
};
#endif // PALADIN_H

/** Inherited class from Paladin: Paladin_(Persian) **/
#ifndef PALADIN_PERSIAN_H
#define PALADIN_PERSIAN_H
struct Paladin_Persian : Paladin {
  // Functions: The constructor and deconstructor
  Paladin_Persian();
  ~Paladin_Persian();
};
#endif // PALADIN_PERSIAN_H

/** Inherited class from Paladin: Paladin_(Frank) **/
#ifndef PALADIN_FRANK_H
#define PALADIN_FRANK_H
struct Paladin_Frank : Paladin {
  // Functions: The constructor and deconstructor
  Paladin_Frank();
  ~Paladin_Frank();
};
#endif // PALADIN_FRANK_H

/** Inherited class from Onager: Siege_Onager **/
#ifndef SIEGE_ONAGER_H
#define SIEGE_ONAGER_H
struct Siege_Onager : Onager {
  // Functions: The constructor and deconstructor
  Siege_Onager();
  ~Siege_Onager();
};
#endif // SIEGE_ONAGER_H

/** Inherited class from Siege_Onager: Siege_Onager_(Celt) **/
#ifndef SIEGE_ONAGER_CELT_H
#define SIEGE_ONAGER_CELT_H
struct Siege_Onager_Celt : Siege_Onager {
  // Functions: The constructor and deconstructor
  Siege_Onager_Celt();
  ~Siege_Onager_Celt();
};
#endif // SIEGE_ONAGER_CELT_H

/** Inherited class from Capped_Ram: Siege_Ram **/
#ifndef SIEGE_RAM_H
#define SIEGE_RAM_H
struct Siege_Ram : Capped_Ram {
  // Functions: The constructor and deconstructor
  Siege_Ram();
  ~Siege_Ram();
};
#endif // SIEGE_RAM_H

/** Inherited class from Siege_Ram: Siege_Ram_(Celt) **/
#ifndef SIEGE_RAM_CELT_H
#define SIEGE_RAM_CELT_H
struct Siege_Ram_Celt : Siege_Ram {
  // Functions: The constructor and deconstructor
  Siege_Ram_Celt();
  ~Siege_Ram_Celt();
};
#endif // SIEGE_RAM_CELT_H

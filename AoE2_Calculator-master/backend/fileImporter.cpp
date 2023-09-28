/** The libaries **/
#include "fileImporter.h" // Using: entitiesFile functions
#include "entity.h"       // Using: entity class
#include <algorithm>      // Using: transform, toupper, .begin(), .end()
#include <cstdlib>        // Using: exit(EXIT_FAILURE)
#include <fstream>        // Using: ifstream, is_open(), good, close()
#include <iostream>       // Using: cin and cout
#include <sstream>        // Using: stringstream
#include <stdlib.h>       // Using: atoi
#include <string>         // Using: string

// Functions: The constructor and deconstructor
fileImporter::fileImporter()
{
}
fileImporter::~fileImporter()
{
}

// Function: Get the rest of the information about an entity
Entity fileImporter::conductASearch(
  std::string inputEntityName,
  int         inputEntityQuantity)
{
  // Struct: Declare a blank entity to store the return information
  Entity returnEntity;

  // Behaviour: Search for the input entity name and return the associated
  // entity
  if (inputEntityName == "ARCHER") {
    Archer returnArcher;
    returnEntity = returnArcher;
  }
  else if (inputEntityName == "ARCHER_(SARACEN)") {
    Archer_Saracen returnArcherSaracen;
    returnEntity = returnArcherSaracen;
  }
  else if (inputEntityName == "ARBALEST") {
    Arbalest returnArbalest;
    returnEntity = returnArbalest;
  }
  else if (inputEntityName == "ARBALEST_(BRITON)") {
    Arbalest_Briton returnArbalestBriton;
    returnEntity = returnArbalestBriton;
  }
  else if (inputEntityName == "ARBALEST_(SARACEN)") {
    Arbalest_Saracen returnArbalestSaracen;
    returnEntity = returnArbalestSaracen;
  }
  else if (inputEntityName == "ARCHERY_RANGE") {
    Archery_Range returnArcheryRange;
    returnEntity = returnArcheryRange;
  }
  else if (inputEntityName == "BARRACKS") {
    Barracks returnBarracks;
    returnEntity = returnBarracks;
  }
  else if (inputEntityName == "BATTERING_RAM") {
    Battering_Ram returnBatteringRam;
    returnEntity = returnBatteringRam;
  }
  else if (inputEntityName == "BATTERING_RAM_(CELT)") {
    Battering_Ram_Celt returnBatteringRamCelt;
    returnEntity = returnBatteringRamCelt;
  }
  else if (inputEntityName == "BERSERK_(VIKING)") {
    Berserk_Viking returnBerserkViking;
    returnEntity = returnBerserkViking;
  }
  else if (inputEntityName == "BLACKSMITH") {
    Blacksmith returnBlacksmith;
    returnEntity = returnBlacksmith;
  }
  else if (inputEntityName == "CAPPED_RAM") {
    Capped_Ram returnCappedRam;
    returnEntity = returnCappedRam;
  }
  else if (inputEntityName == "CAPPED_RAM_(CELT)") {
    Capped_Ram_Celt returnCappedRamCelt;
    returnEntity = returnCappedRamCelt;
  }
  else if (inputEntityName == "CASTLE") {
    Castle returnCastle;
    returnEntity = returnCastle;
  }
  else if (inputEntityName == "CASTLE_(FRANK)") {
    Castle_Frank returnCastleFrank;
    returnEntity = returnCastleFrank;
  }
  else if (inputEntityName == "CAVALIER") {
    Cavalier returnCavalier;
    returnEntity = returnCavalier;
  }
  else if (inputEntityName == "CAVALIER_(FRANK)") {
    Cavalier_Frank returnCavalierFrank;
    returnEntity = returnCavalierFrank;
  }
  else if (inputEntityName == "CAVALIER_(PERSIAN)") {
    Cavalier_Persian returnCavalierPersian;
    returnEntity = returnCavalierPersian;
  }
  else if (inputEntityName == "CAVALRY_ARCHER") {
    Cavalry_Archer returnCavalryArcher;
    returnEntity = returnCavalryArcher;
  }
  else if (inputEntityName == "CAVALRY_ARCHER_(MONGOL)") {
    Cavalry_Archer_Mongol returnCavalryArcherMongol;
    returnEntity = returnCavalryArcherMongol;
  }
  else if (inputEntityName == "CHAMPION") {
    Champion returnChampion;
    returnEntity = returnChampion;
  }
  else if (inputEntityName == "CHAMPION_(CELT)") {
    Champion_Celt returnChampionCelt;
    returnEntity = returnChampionCelt;
  }
  else if (inputEntityName == "CHAMPION_(GOTH)") {
    Champion_Goth returnChampionGoth;
    returnEntity = returnChampionGoth;
  }
  else if (inputEntityName == "CHAMPION_(JAPANESE)") {
    Champion_Japanese returnChampionJapanese;
    returnEntity = returnChampionJapanese;
  }
  else if (inputEntityName == "CHAMPION_(VIKING)") {
    Champion_Viking returnChampionViking;
    returnEntity = returnChampionViking;
  }
  else if (
    (inputEntityName == "CHARLAMAGNE'S_PALACE_AT_AIX_LA'CHAPELLE_(BRITON)")
    || (inputEntityName == "WONDER_(BRITON)")
    || (inputEntityName == "ROCK_OF_CASHEL_(CELT)")
    || (inputEntityName == "WONDER_(CELT)")
    || (inputEntityName == "THE_GOLDEN_TENT_OF_THE_GREAT_KHAN_(MONGOL)")
    || (inputEntityName == "WONDER_(MONGOL)")
    || (inputEntityName == "THE_PALACE_OF_CTESIPHON_ON_THE_TIGRIS_(PERSIAN)")
    || (inputEntityName == "WONDER_(PERSIAN)")
    || (inputEntityName == "TOMB_OF_THEODORIC_(GOTH)")
    || (inputEntityName == "WONDER_(GOTH)")
    || (inputEntityName == "NOTRE-DAME_CATHEDRAL_(FRANK)")
    || (inputEntityName == "NOTRE_DAME_CATHEDRAL_(FRANK)")
    || (inputEntityName == "WONDER_(FRANK)")
    || (inputEntityName == "STAVE_CHURCH_AT_URNES_(VIKING)")
    || (inputEntityName == "WONDER_(VIKING)")
    || (inputEntityName == "THE_GREAT_TEMPLE_AT_NARA_(JAPANESE)")
    || (inputEntityName == "WONDER_(JAPANESE)")
    || (inputEntityName == "WONDER")) {
    Wonder returnWonder;

    // Behaviour: Get further information about the Wonder
    returnWonder.updateWonderHP();

    returnEntity = returnWonder;
  }
  else if (inputEntityName == "CROSSBOWMAN") {
    Crossbowman returnCrossbowman;
    returnEntity = returnCrossbowman;
  }
  else if (inputEntityName == "CROSSBOWMAN_(SARACEN)") {
    Crossbowman_Saracen returnCrossbowmanSaracen;
    returnEntity = returnCrossbowmanSaracen;
  }
  else if (inputEntityName == "DEMOLITION_SHIP") {
    Demolition_Ship returnDemolitionShip;
    returnEntity = returnDemolitionShip;
  }
  else if (inputEntityName == "DEMOLITION_SHIP_(VIKING)") {
    Demolition_Ship_Viking returnDemolitionShipViking;
    returnEntity = returnDemolitionShipViking;
  }
  else if (inputEntityName == "DOCK") {
    Dock returnDock;
    returnEntity = returnDock;
  }
  else if (inputEntityName == "DOCK_(PERSIAN)") {
    Dock_Persian returnDockPersian;
    returnEntity = returnDockPersian;
  }
  else if (inputEntityName == "DOCK_(VIKING)") {
    Dock_Viking returnDockViking;
    returnEntity = returnDockViking;
  }
  else if (inputEntityName == "ELITE_BERSERK_(VIKING)") {
    Elite_Berserk_Viking returnEliteBerserkViking;
    returnEntity = returnEliteBerserkViking;
  }
  else if (inputEntityName == "ELITE_HUSKARL_(GOTH)") {
    Elite_Huskarl_Goth returnEliteHuskarlGoth;
    returnEntity = returnEliteHuskarlGoth;
  }
  else if (inputEntityName == "ELITE_LONGBOAT_(VIKING)") {
    Elite_Longboat_Viking returnEliteLongboatViking;
    returnEntity = returnEliteLongboatViking;
  }
  else if (inputEntityName == "ELITE_LONGBOWMAN_(Briton)") {
    Elite_Longbowman_Briton returnEliteLongbowmanBriton;
    returnEntity = returnEliteLongbowmanBriton;
  }
  else if (inputEntityName == "ELITE_MAMELUKE_(SARACEN)") {
    Elite_Mameluke_Saracen returnEliteMamelukeSaracen;
    returnEntity = returnEliteMamelukeSaracen;
  }
  else if (inputEntityName == "ELITE_MANGUDAI_(MONGOL)") {
    Elite_Mangudai_Mongol returnEliteMangudaiMongol;
    returnEntity = returnEliteMangudaiMongol;
  }
  else if (inputEntityName == "ELITE_SAMURAI_(JAPANESE)") {
    Elite_Samurai_Japanese returnEliteSamurai;
    returnEntity = returnEliteSamurai;
  }
  else if (inputEntityName == "ELITE_SKIRMISHER") {
    Elite_Skirmisher returnEliteSkirmisher;
    returnEntity = returnEliteSkirmisher;
  }
  else if (inputEntityName == "ELITE_THROWING_AXEMAN_(FRANK)") {
    Elite_Throwing_Axeman_Frank returnEliteThrowingAxemanFrank;
    returnEntity = returnEliteThrowingAxemanFrank;
  }
  else if (inputEntityName == "ELITE_WAR_ELEPHANT_(PERSIAN)") {
    Elite_War_Elephant_Persian returnEliteWarElephantPersian;
    returnEntity = returnEliteWarElephantPersian;

    // Behaviour: The current entity is limited to 1 token per card
    if (inputEntityQuantity != 1) {
      std::cout << "Error: " << returnEntity.entityName
                << " can only be of 1 quantity"
                << "\n";
      exit(EXIT_FAILURE);
    }
  }
  else if (inputEntityName == "ELITE_WOAD_RAIDER_(CELT)") {
    Elite_Woad_Raider_Celt returnEliteWoadRaiderCelt;
    returnEntity = returnEliteWoadRaiderCelt;
  }
  else if (inputEntityName == "FARM") {
    Farm returnFarm;
    returnEntity = returnFarm;
  }
  else if (inputEntityName == "FAST_FIRE_SHIP") {
    Fast_Fire_Ship returnFastFireShip;
    returnEntity = returnFastFireShip;
  }
  else if (inputEntityName == "FIRE_SHIP") {
    Fire_Ship returnFireShip;
    returnEntity = returnFireShip;
  }
  else if (inputEntityName == "FISHING_SHIP_(JAPANESE)") {
    Fishing_Ship_Japanese returnFishingShipJapanese;
    returnEntity = returnFishingShipJapanese;
  }
  else if (inputEntityName == "FISHING_SHIP_(PERSIAN)") {
    Fishing_Ship_Persian returnFishingShipPersian;
    returnEntity = returnFishingShipPersian;
  }
  else if (inputEntityName == "FORTIFIED_WALL") {
    Fortified_Wall returnFortifiedWall;
    returnEntity = returnFortifiedWall;
  }
  else if (inputEntityName == "GALLEY") {
    Galley returnGalley;
    returnEntity = returnGalley;
  }
  else if (inputEntityName == "GALLEY_(JAPANESE)") {
    Galley_Japanese returnGalleyJapanese;
    returnEntity = returnGalleyJapanese;
  }
  else if (inputEntityName == "GALLEY_(SARACEN)") {
    Galley_Saracen returnGalleySaracen;
    returnEntity = returnGalleySaracen;
  }
  else if (inputEntityName == "GALLEY_(VIKING)") {
    Galley_Viking returnGalleyViking;
    returnEntity = returnGalleyViking;
  }
  else if (inputEntityName == "HERO") {
    Hero returnHero;
    returnEntity = returnHero;

    // Behaviour: The current entity is limited to 1 token per card
    if (inputEntityQuantity != 1) {
      std::cout << "Error: " << returnEntity.entityName
                << " can only be of 1 quantity"
                << "\n";
      exit(EXIT_FAILURE);
    }
  }
  else if (inputEntityName == "STONE_GATE") {
    Stone_Gate returnStoneGate;
    returnEntity = returnStoneGate;
  }
  else if (inputEntityName == "GALLEON") {
    Galleon returnGalleon;
    returnEntity = returnGalleon;
  }
  else if (inputEntityName == "GALLEON_(SARACEN)") {
    Galleon_Saracen returnGalleonSaracen;
    returnEntity = returnGalleonSaracen;
  }
  else if (inputEntityName == "GALLEON_(VIKING)") {
    Galleon_Viking returnGalleonViking;
    returnEntity = returnGalleonViking;
  }
  else if (inputEntityName == "GALLEON_(JAPANESE)") {
    Galleon_Japanese returnGalleonJapanese;
    returnEntity = returnGalleonJapanese;
  }
  else if (inputEntityName == "GOLD_MINE") {
    Gold_Mine returnGoldMine;
    returnEntity = returnGoldMine;
  }
  else if (inputEntityName == "GOLD_MINE_(JAPANESE)") {
    Gold_Mine_Japanese returnGoldMineJapanese;
    returnEntity = returnGoldMineJapanese;
  }
  else if (inputEntityName == "CAMEL") {
    Camel returnCamel;
    returnEntity = returnCamel;
  }
  else if (inputEntityName == "HEAVY_CAMEL") {
    Heavy_Camel returnHeavyCamel;
    returnEntity = returnHeavyCamel;
  }
  else if (inputEntityName == "HEAVY_CAVALRY_ARCHER") {
    Heavy_Cavalry_Archer returnHeavyCavalryArhcer;
    returnEntity = returnHeavyCavalryArhcer;
  }
  else if (inputEntityName == "HEAVY_CAVALRY_ARCHER_(MONGOL)") {
    Heavy_Cavalry_Archer_Mongol returnHeavyCavalryArcherMongol;
    returnEntity = returnHeavyCavalryArcherMongol;
  }
  else if (inputEntityName == "HEAVY_CAVALRY_ARCHER_(SARACEN)") {
    Heavy_Cavalry_Archer_Saracen returnHeavyCavalryArcherSaracen;
    returnEntity = returnHeavyCavalryArcherSaracen;
  }
  else if (inputEntityName == "HEAVY_DEMOLITION_SHIP") {
    Heavy_Demolition_Ship returnHeavyDemolitionShip;
    returnEntity = returnHeavyDemolitionShip;
  }
  else if (inputEntityName == "HEAVY_DEMOLITION_SHIP_(VIKING)") {
    Heavy_Demolition_Ship_Viking returnHeavyDemolitionShipViking;
    returnEntity = returnHeavyDemolitionShipViking;
  }
  else if (inputEntityName == "HEAVY_SCORPION") {
    Heavy_Scorpion returnHeavyScorpion;
    returnEntity = returnHeavyScorpion;
  }
  else if (inputEntityName == "HEAVY_SCORPION_(CELT)") {
    Heavy_Scorpion_Celt returnHeavyScorpionCelt;
    returnEntity = returnHeavyScorpionCelt;
  }
  else if (inputEntityName == "HOUSE") {
    House returnHouse;
    returnEntity = returnHouse;
  }
  else if (inputEntityName == "HUSKARL_(GOTH)") {
    Huskarl_Goth returnHuskarlGoth;
    returnEntity = returnHuskarlGoth;
  }
  else if (inputEntityName == "KNIGHT") {
    Knight returnKnight;
    returnEntity = returnKnight;
  }
  else if (inputEntityName == "KNIGHT_(FRANK)") {
    Knight_Frank returnKnightFrank;
    returnEntity = returnKnightFrank;
  }
  else if (inputEntityName == "KNIGHT_(PERSIAN)") {
    Knight_Persian returnKnightPersian;
    returnEntity = returnKnightPersian;
  }
  else if (inputEntityName == "LIGHT_CAVALRY") {
    Light_Cavalry returnLightCavalry;
    returnEntity = returnLightCavalry;
  }
  else if (inputEntityName == "LIGHT_CAVALRY_(MONGOL)") {
    Light_Cavalry_Mongol returnLightCavalryMongol;
    returnEntity = returnLightCavalryMongol;
  }
  else if (inputEntityName == "LONG_SWORDSMAN") {
    Long_Swordsman returnLongSwordsman;
    returnEntity = returnLongSwordsman;
  }
  else if (inputEntityName == "LONG_SWORDSMAN_(CELT)") {
    Long_Swordsman_Celt returnLongSwordsmanCelt;
    returnEntity = returnLongSwordsmanCelt;
  }
  else if (inputEntityName == "LONG_SWORDSMAN_(GOTH)") {
    Long_Swordsman_Goth returnLongSwordsmanGoth;
    returnEntity = returnLongSwordsmanGoth;
  }
  else if (inputEntityName == "LONG_SWORDSMAN_(JAPANESE)") {
    Long_Swordsman_Japanese returnLongSwordsmanJapanese;
    returnEntity = returnLongSwordsmanJapanese;
  }
  else if (inputEntityName == "LONG_SWORDSMAN_(VIKING)") {
    Long_Swordsman_Viking returnLongswordsmanViking;
    returnEntity = returnLongswordsmanViking;
  }
  else if (inputEntityName == "LONGBOAT_(VIKING)") {
    Longboat_Viking returnLongboatViking;
    returnEntity = returnLongboatViking;
  }
  else if (inputEntityName == "LONGBOWMAN_(BRITON)") {
    Longbowman_Briton returnLongbowmanBriton;
    returnEntity = returnLongbowmanBriton;
  }
  else if (inputEntityName == "LUMBER_CAMP") {
    Lumber_Camp returnLumberCamp;
    returnEntity = returnLumberCamp;
  }
  else if (inputEntityName == "LUMBER_CAMP_(JAPANESE)") {
    Lumber_Camp_Japanese returnLumberCampJapanese;
    returnEntity = returnLumberCampJapanese;
  }
  else if (inputEntityName == "MAMELUKE_(SARACEN)") {
    Mameluke_Saracen returnMamelukeSaracen;
    returnEntity = returnMamelukeSaracen;
  }
  else if (
    (inputEntityName == "MAN_AT_ARMS") || (inputEntityName == "MAN-AT-ARMS")) {
    Man_At_Arms returnManAtArms;
    returnEntity = returnManAtArms;
  }
  else if (
    (inputEntityName == "MAN_AT_ARMS_(VIKING)")
    || (inputEntityName == "MAN-AT-ARMS_(VIKING)")) {
    Man_At_Arms_Viking returnManAtArmsViking;
    returnEntity = returnManAtArmsViking;
  }
  else if (
    (inputEntityName == "MAN_AT_ARMS_(CELT)")
    || (inputEntityName == "MAN-AT-ARMS_(CELT)")) {
    Man_At_Arms_Celt returnManAtArmsCelt;
    returnEntity = returnManAtArmsCelt;
  }
  else if (
    (inputEntityName == "MAN_AT_ARMS_(GOTH)")
    || (inputEntityName == "MAN-AT-ARMS_(GOTH)")) {
    Man_At_Arms_Goth returnManAtArmsGoth;
    returnEntity = returnManAtArmsGoth;
  }
  else if (
    (inputEntityName == "MAN_AT_ARMS_(JAPANESE)")
    || (inputEntityName == "MAN-AT-ARMS_(JAPANESE)")) {
    Man_At_Arms_Japanese returnManAtArmsJapanese;
    returnEntity = returnManAtArmsJapanese;
  }
  else if (inputEntityName == "MANGONEL") {
    Mangonel returnMangonel;
    returnEntity = returnMangonel;
  }
  else if (inputEntityName == "MANGONEL_(CELT)") {
    Mangonel_Celt returnMangonelCelt;
    returnEntity = returnMangonelCelt;
  }
  else if (inputEntityName == "MANGUDAI_(MONGOL)") {
    Mangudai_Mongol returnMangudaiMongol;
    returnEntity = returnMangudaiMongol;
  }
  else if (inputEntityName == "MARKET") {
    Market returnMarket;
    returnEntity = returnMarket;
  }
  else if (inputEntityName == "MARKET_(SARACEN)") {
    Market_Saracen returnMarketSaracen;
    returnEntity = returnMarketSaracen;
  }
  else if (inputEntityName == "MILITIA") {
    Militia returnMilitia;
    returnEntity = returnMilitia;
  }
  else if (inputEntityName == "MILITIA_(CELT)") {
    Militia_Celt returnMilitiaCelt;
    returnEntity = returnMilitiaCelt;
  }
  else if (inputEntityName == "MILITIA_(GOTH)") {
    Militia_Goth returnMilitiaGoth;
    returnEntity = returnMilitiaGoth;
  }
  else if (inputEntityName == "MILL") {
    Mill returnMill;
    returnEntity = returnMill;
  }
  else if (inputEntityName == "MILL_(JAPANESE)") {
    Mill_Japanese returnMillJapanese;
    returnEntity = returnMillJapanese;
  }
  else if (inputEntityName == "MONASTERY") {
    Monastery returnMonastery;
    returnEntity = returnMonastery;
  }
  else if (inputEntityName == "MONK") {
    Monk returnMonk;
    returnEntity = returnMonk;

    // Behaviour: The current entity is limited to < 5 tokens
    if (inputEntityQuantity > 5) {
      std::cout << "Error: " << returnEntity.entityName
                << " cannot be > 5 quantity"
                << "\n";
      std::cout
        << "Checks for a non-0 quantity of the primary monk already occured"
        << "\n";
      exit(EXIT_FAILURE);
    }
  }
  else if (inputEntityName == "ONAGER") {
    Onager returnOnager;
    returnEntity = returnOnager;
  }
  else if (inputEntityName == "ONAGER_(CELT)") {
    Onager_Celt returnOnagerCelt;
    returnEntity = returnOnagerCelt;
  }
  else if (inputEntityName == "OUTPOST") {
    Outpost returnOutpost;
    returnEntity = returnOutpost;
  }
  else if (inputEntityName == "PALADIN") {
    Paladin returnPaladin;
    returnEntity = returnPaladin;
  }
  else if (inputEntityName == "PALADIN_(PERSIAN)") {
    Paladin_Persian returnPaladinPersian;
    returnEntity = returnPaladinPersian;
  }
  else if (inputEntityName == "PALADIN_(FRANK)") {
    Paladin_Frank returnPaladinFrank;
    returnEntity = returnPaladinFrank;
  }
  else if (inputEntityName == "PALISADE_WALL") {
    Palisade_Wall returnPalisadeWall;
    returnEntity = returnPalisadeWall;
  }
  else if (inputEntityName == "PIKEMAN") {
    Pikeman returnPikeman;
    returnEntity = returnPikeman;
  }
  else if (inputEntityName == "PIKEMAN_(CELT)") {
    Pikeman_Celt returnPikemanCelt;
    returnEntity = returnPikemanCelt;
  }
  else if (inputEntityName == "PIKEMAN_(GOTH)") {
    Pikeman_Goth returnPikemanGoth;
    returnEntity = returnPikemanGoth;
  }
  else if (inputEntityName == "PIKEMAN_(JAPANESE)") {
    Pikeman_Japanese returnPikemanJapanese;
    returnEntity = returnPikemanJapanese;
  }
  else if (inputEntityName == "PIKEMAN_(VIKING)") {
    Pikeman_Viking returnPikemanViking;
    returnEntity = returnPikemanViking;
  }
  else if (inputEntityName == "SAMURAI_(JAPANESE)") {
    Samurai_Japanese returnSamurai;
    returnEntity = returnSamurai;
  }
  else if (inputEntityName == "SCORPION") {
    Scorpion returnScorpion;
    returnEntity = returnScorpion;
  }
  else if (inputEntityName == "SCORPION_(CELT)") {
    Scorpion_Celt returnScorpionCelt;
    returnEntity = returnScorpionCelt;
  }
  else if (inputEntityName == "SCOUT_CAVALRY") {
    Scout_Cavalry returnScoutCavalry;
    returnEntity = returnScoutCavalry;
  }
  else if (inputEntityName == "SIEGE_ONAGER") {
    Siege_Onager returnSiegeOnager;
    returnEntity = returnSiegeOnager;
  }
  else if (inputEntityName == "SIEGE_ONAGER_(CELT)") {
    Siege_Onager_Celt returnSiegeOnagerCelt;
    returnEntity = returnSiegeOnagerCelt;
  }
  else if (inputEntityName == "SIEGE_RAM") {
    Siege_Ram returnSiegeRam;
    returnEntity = returnSiegeRam;
  }
  else if (inputEntityName == "SIEGE_RAM_(CELT)") {
    Siege_Ram_Celt returnSiegeRamCelt;
    returnEntity = returnSiegeRamCelt;
  }
  else if (inputEntityName == "SIEGE_WORKSHOP") {
    Siege_Workshop returnSiegeWorkshop;
    returnEntity = returnSiegeWorkshop;
  }
  else if (inputEntityName == "SKIRMISHER") {
    Skirmisher returnSkirmisher;
    returnEntity = returnSkirmisher;
  }
  else if (inputEntityName == "SPEARMAN") {
    Spearman returnSpearman;
    returnEntity = returnSpearman;
  }
  else if (inputEntityName == "SPEARMAN_(JAPANESE)") {
    Spearman_Japanese returnSpearmanJapanese;
    returnEntity = returnSpearmanJapanese;
  }
  else if (inputEntityName == "SPEARMAN_(VIKING)") {
    Spearman_Viking returnSpearmanViking;
    returnEntity = returnSpearmanViking;
  }
  else if (inputEntityName == "SPEARMAN_(CELT)") {
    Spearman_Celt returnSpearmanCelt;
    returnEntity = returnSpearmanCelt;
  }
  else if (inputEntityName == "SPEARMAN_(GOTH)") {
    Spearman_Goth returnSpearmanGoth;
    returnEntity = returnSpearmanGoth;
  }
  else if (inputEntityName == "STABLE") {
    Stable returnStable;
    returnEntity = returnStable;
  }
  else if (inputEntityName == "STONE_MINE") {
    Stone_Mine returnStoneMine;
    returnEntity = returnStoneMine;
  }
  else if (inputEntityName == "STONE_MINE_(JAPANESE)") {
    Stone_Mine_Japanese returnStoneMineJapanese;
    returnEntity = returnStoneMineJapanese;
  }
  else if (inputEntityName == "STONE_WALL") {
    Stone_Wall returnStoneWall;
    returnEntity = returnStoneWall;
  }
  else if (inputEntityName == "THROWING_AXEMAN_(FRANK)") {
    Throwing_Axeman_Frank returnThrowingAxemanFrank;
    returnEntity = returnThrowingAxemanFrank;
  }
  else if (inputEntityName == "TOWN_CENTER") {
    Town_Center returnTownCenter;
    returnEntity = returnTownCenter;
  }
  else if (inputEntityName == "TOWN_CENTER_(BRITON)") {
    Town_Center_Briton returnTownCenterBriton;
    returnEntity = returnTownCenterBriton;
  }
  else if (inputEntityName == "TOWN_CENTER_(PERSIAN)") {
    Town_Center_Persian returnTownCenterPersian;
    returnEntity = returnTownCenterPersian;
  }
  else if (inputEntityName == "TRADE_COG") {
    Trade_Cog returnTradeCog;
    returnEntity = returnTradeCog;
  }
  else if (inputEntityName == "TREBUCHET") {
    Trebuchet returnTrebuchet;
    // Behaviour: Get further information about the Trebuchet
    returnTrebuchet.updateTrebuchetArmorClass();
    returnEntity = returnTrebuchet;
  }
  else if (
    (inputEntityName == "TWO-HANDED_SWORDSMAN")
    || (inputEntityName == "TWO_HANDED_SWORDSMAN")) {
    Two_Handed_Swordsman returnTwoHandedSwordsman;
    returnEntity = returnTwoHandedSwordsman;
  }
  else if (
    (inputEntityName == "TWO-HANDED_SWORDSMAN_(CELT)")
    || (inputEntityName == "TWO_HANDED_SWORDSMAN_(CELT)")) {
    Two_Handed_Swordsman_Celt returnTwoHandedSwordsmanCelt;
    returnEntity = returnTwoHandedSwordsmanCelt;
  }
  else if (
    (inputEntityName == "TWO-HANDED_SWORDSMAN_(GOTH)")
    || (inputEntityName == "TWO_HANDED_SWORDSMAN_(GOTH)")) {
    Two_Handed_Swordsman_Goth returnTwoHandedSwordsmanGoth;
    returnEntity = returnTwoHandedSwordsmanGoth;
  }
  else if (
    (inputEntityName == "TWO-HANDED_SWORDSMAN_(JAPANESE)")
    || (inputEntityName == "TWO_HANDED_SWORDSMAN_(JAPANESE)")) {
    Two_Handed_Swordsman_Japanese returnTwoHandedSwordsmanJapanese;
    returnEntity = returnTwoHandedSwordsmanJapanese;
  }
  else if (
    (inputEntityName == "TWO-HANDED_SWORDSMAN_(VIKING)")
    || (inputEntityName == "TWO_HANDED_SWORDSMAN_(VIKING)")) {
    Two_Handed_Swordsman_Viking returnTwoHandedSwordsmanViking;
    returnEntity = returnTwoHandedSwordsmanViking;
  }
  else if (inputEntityName == "VILLAGER") {
    Villager returnVillager;
    returnEntity = returnVillager;
  }
  else if (inputEntityName == "WAR_ELEPHANT_(PERSIAN)") {
    War_Elephant_Persian returnWarElephantPersian;
    returnEntity = returnWarElephantPersian;

    // The current entity is limited to 1 token per card
    if (inputEntityQuantity != 1) {
      std::cout << "Error: " << returnEntity.entityName
                << " can only be of 1 quantity"
                << "\n";
      exit(EXIT_FAILURE);
    }
  }
  else if (inputEntityName == "WAR_GALLEY") {
    War_Galley returnWarGalley;
    returnEntity = returnWarGalley;
  }
  else if (inputEntityName == "WAR_GALLEY_(JAPANESE)") {
    War_Galley_Japanese returnWarGalleyJapanese;
    returnEntity = returnWarGalleyJapanese;
  }
  else if (inputEntityName == "WAR_GALLEY_(SARACEN)") {
    War_Galley_Saracen returnWarGalleySaracen;
    returnEntity = returnWarGalleySaracen;
  }
  else if (inputEntityName == "WAR_GALLEY_(VIKING)") {
    War_Galley_Viking returnWarGalleyViking;
    returnEntity = returnWarGalleyViking;
  }
  else if (inputEntityName == "WATCH_TOWER") {
    Watch_Tower returnWatchTower;
    returnEntity = returnWatchTower;
  }
  else if (inputEntityName == "WOAD_RAIDER_(CELT)") {
    Woad_Raider_Celt returnWoadRaiderCelt;
    returnEntity = returnWoadRaiderCelt;
  }
  else {
    std::cout << "Error: " << inputEntityName
              << " inside of entities.csv is an unrecognized name of an entity"
              << "\n";
    std::cout << "Either I have not implemented the entity or you mistyped the "
                 "name of the entity"
              << "\n";
    exit(EXIT_FAILURE);
  }

  // Behaviour: Make sure that the quantity is > 0 for non-Monks
  if ((inputEntityName != "MONK") && (inputEntityQuantity <= 0)) {
    std::cout << "Error: All entities (except assisting monks) must have a "
                 "quantity of at least 1"
              << "\n";
    exit(EXIT_FAILURE);
  }

  // Behaviour: Make sure that an Age within the range of 1-4 was entered
  if (returnEntity.entityAge < 1 || returnEntity.entityAge > 5) {
    // Error: Age not recognized
    std::cout << "Error: " << returnEntity.entityName
              << " contains an Age other than 1-4"
              << "\n";
    exit(EXIT_FAILURE);
  }

  // Behaviour: Make sure that each entity (except for Villagers) has at least 1
  // armor class
  bool hasArmorClass = false;
  for (int i = 0; i < 20; i++) {
    if (returnEntity.armorClass[i] == true) {
      hasArmorClass = true;
    }
  }
  if (hasArmorClass == false) {
    // Error: Each entity should have at least 1 armor class unless a villager
    if (returnEntity.entityName != "Villager") {
      std::cout << "Error: " << returnEntity.entityName
                << " is missing at least 1 armor class"
                << "\n";
      exit(EXIT_FAILURE);
    }
  }

  // Behaviour: Calculate the number of armor classes the entity has now
  returnEntity.entitiesArmorClasses = returnEntity.returnNumberOfArmorClasses();

  // Behaviour: Store the quantity of the entity inside of the entity now
  returnEntity.entityQuantity = inputEntityQuantity;

  // Behaviour: Make sure that entities with the building armor class only have
  // a quantity of 1
  if (returnEntity.armorClass[1] == true && inputEntityQuantity != 1) {
    std::cout << "Error: " << returnEntity.entityName
              << " is a building and buildings can only be of 1 quantity"
              << "\n";
    exit(EXIT_FAILURE);
  }
  // Behaviour: Make sure that entities with the siege weapon armor class only
  // have a quantity of 1
  if (returnEntity.armorClass[12] == true && inputEntityQuantity != 1) {
    std::cout
      << "Error: " << returnEntity.entityName
      << " is a siege weapon and siege weapons can only be of 1 quantity"
      << "\n";
    exit(EXIT_FAILURE);
  }
  // Behavuour: Make sure that entities with the ship armor class only have a
  // quantity of 1
  if (returnEntity.armorClass[11] == true && inputEntityQuantity != 1) {
    std::cout << "Error: " << returnEntity.entityName
              << " is a ship and ships can only be of 1 quantity"
              << "\n";
    exit(EXIT_FAILURE);
  }

  return returnEntity;
}
// BANANA

// Function: Check if a string is an integer
int fileImporter::checkIsInteger(std::string inputWord)
{
  // Variable: Declare an integer data type
  int anInteger = 0;

  // Object: Declare the object that shall store the word and try to put the
  // word into the integer
  std::stringstream wordConversion;
  wordConversion << inputWord;
  wordConversion >> anInteger;

  // Behaviour: See if the string to integer conversion was successful
  if (wordConversion.good()) {
    std::cout << "Error: No valid first quantity"
              << "\n";
    exit(EXIT_FAILURE);
  }
  else if (anInteger == 0) {
    if (inputWord != "0") {
      std::cout << "Error: The output was 0 but the input was not 0 for the "
                   "first quantity"
                << "\n";
      exit(EXIT_FAILURE);
    }
    else {
      // A quantity of < 1 is acceptable for the non-monk values
      // I'll check for the appropriate quantity based on entity types later
    }
  }
  else if (anInteger > 99) {
    std::cout << "Error: A limit of 100 units is more than reasonable"
              << "\n";
    exit(EXIT_FAILURE);
  }

  return anInteger;
}

// Function: Get the entity name and quantity
Entity fileImporter::entitiesFile(
  std::string inputEntityFilename,
  int         inputNumberOfWords,
  int         inputPlayerNumber,
  int         inputMonkReturnSetting)
{
  // Struct: Declare an entity to store the return values
  Entity returnEntityCSV;

  // Behaviour: Check that the filename is correct
  if (inputEntityFilename != "import/entities.csv") {
    std::cout
      << "Error: Input filename supposed to be called 'import/entities.csv'"
      << "\n";
    exit(EXIT_FAILURE);
  }

  // Behaviour: Open "entities.csv"
  inputFile.open(inputEntityFilename);

  // Behaviour: Check if "entities.csv" did not open successfully
  if (!inputFile.is_open()) {
    std::cout << "Error: Failed to open the file called" << inputEntityFilename
              << "\n";
    exit(EXIT_FAILURE);
  }
  else {
    // Array: Hold up to four words to represent the four words inside of
    // 'entites.csv'
    std::string words[inputNumberOfWords];
    for (int i = 0; i < inputNumberOfWords; i++) {
      words[i] = "";
    }

    // Variable: A counter used to increment the words array
    int count = 0;

    // Variable: A string that can hold one input word at a time
    std::string word;

    // Behaviour: While we aren't at the end of the file keep running. Similar
    // to .good() and .eof()
    while (inputFile >> word) {
      // Behaviour: Ensure that the user does not exceed the array bounds of
      // words[4]
      if (count == inputNumberOfWords) {
        std::cout << "Error: Too much input. Only enter the values for up to "
                     "two players"
                  << "\n";
        std::cout << "The input is supposed to be:"
                  << "\n";
        std::cout << "[P1_Non-Monk_Entity_Name] [P1_Entity_Quantity] Monk "
                     "[P1_Monk_Quantity]"
                  << "\n";
        std::cout << "[P2_Non-Monk_Entity_Name] [P2_Entity_Quantity] Monk "
                     "[P2_Monk_Quantity]"
                  << "\n";
        exit(EXIT_FAILURE);
      }
      else {
        // Behaviour: Pass the word into the words array and increment the
        // counter
        words[count] = word;
        count++;
      }
    }

    // Behaviour: Ensure that there are at least four words inside of the words
    // array
    if (count < (inputNumberOfWords - 1)) {
      std::cout << "Error: Too little input. You must enter the values for up "
                   "to two players"
                << "\n";
      std::cout << "The input is supposed to be:"
                << "\n";
      std::cout << "[P1_Non-Monk_Entity_Name] [P1_Entity_Quantity] Monk "
                   "[P1_Monk_Quantity]"
                << "\n";
      std::cout << "[P2_Non-Monk_Entity_Name] [P2_Entity_Quantity] Monk "
                   "[P2_Monk_Quantity]"
                << "\n";
      exit(EXIT_FAILURE);
    }

    // Behaviour: Make sure that the second words are both monks
    if (words[2] != "Monk") {
      std::cout << "Error: Non-Monk entity name for player 1's 'assisting "
                   "monks' data field"
                << "\n";
      exit(EXIT_FAILURE);
    }
    else if (words[6] != "Monk") {
      std::cout << "Error: Non-Monk entity name for player 2's 'assisting "
                   "monks' data field"
                << "\n";
      exit(EXIT_FAILURE);
    }

    // Behaviour: Check what player's values ought to be returned and if the
    // monk or non-monk values ought to be returned
    if (inputPlayerNumber == 1) {
      int p1Quantity = 0;

      // Behaviour: Return the non-monk entity
      if (inputMonkReturnSetting == 0) {
        // Behaviour: Ensure that the integer elements are of the correct data
        // type (i.e. they are integers)
        p1Quantity = checkIsInteger(words[1]);

        // Behaviour: Convert words[0] into all caps
        std::transform(
          words[0].begin(), words[0].end(), words[0].begin(), ::toupper);

        // Behaviour: Put the input entity name and quantity for player 1 inside
        // of the returnEntityCSV object.
        returnEntityCSV.entityQuantity = p1Quantity;
        returnEntityCSV.entityName     = words[0];
      }
      // Behaviour: Return the monk entity
      else if (inputMonkReturnSetting == 1) {
        // Behaviour: Ensure that the integer elements are of the correct data
        // type (i.e. they are integers)
        p1Quantity = checkIsInteger(words[3]);

        // Behaviour: Convert words[2] into all caps
        std::transform(
          words[2].begin(), words[2].end(), words[2].begin(), ::toupper);

        // Behaviour: Put the input entity name and quantity for player 1 inside
        // of the returnEntityCSV object.
        returnEntityCSV.entityQuantity = p1Quantity;
        returnEntityCSV.entityName     = words[2];
      }
    }
    else if (inputPlayerNumber == 2) {
      int p2Quantity = 0;

      // Behaviour: Return the non-monk entity
      if (inputMonkReturnSetting == 0) {
        // Behaviour: Ensure that the integer elements are of the correct data
        // type (i.e. they are integers)
        p2Quantity = checkIsInteger(words[5]);

        // Behaviour: Convert words[0] into all caps
        std::transform(
          words[4].begin(), words[4].end(), words[4].begin(), ::toupper);

        // Behaviour: Put the input entity name and quantity for player 1 inside
        // of the returnEntityCSV object.
        returnEntityCSV.entityQuantity = p2Quantity;
        returnEntityCSV.entityName     = words[4];
      }
      // Behaviour: Return the monk entity
      else if (inputMonkReturnSetting == 1) {
        // Behaviour: Ensure that the integer elements are of the correct data
        // type (i.e. they are integers)
        p2Quantity = checkIsInteger(words[7]);

        // Behaviour: Convert words[2] into all caps
        std::transform(
          words[6].begin(), words[6].end(), words[6].begin(), ::toupper);

        // Behaviour: Put the input entity name and quantity for player 1 inside
        // of the returnEntityCSV object.
        returnEntityCSV.entityQuantity = p2Quantity;
        returnEntityCSV.entityName     = words[6];
      }
    }

    // Behaviour: Resolve conflicts for double monk quantities
    if ((words[0] == "Monk") && (words[2] == "Monk")) {
      int p1Quantity1 = checkIsInteger(words[1]);
      int p1Quantity2 = checkIsInteger(words[3]);

      if (p1Quantity1 <= 0) {
        std::cout << "Error: Player 1's first monk must be of > 1 quantity"
                  << "\n";
        exit(EXIT_FAILURE);
      }
      else if (p1Quantity2 >= 1) {
        std::cout << "Error: Player 1 cannot have the second monk 'assisting' "
                     "the first monk in combat"
                  << "\n";
        std::cout << "The second monk must have a quantity of 0"
                  << "\n";
        exit(EXIT_FAILURE);
      }
    }

    if ((words[4] == "Monk") && (words[6] == "Monk")) {
      int p2Quantity1 = checkIsInteger(words[5]);
      int p2Quantity2 = checkIsInteger(words[7]);

      if (p2Quantity1 <= 0) {
        std::cout << "Error: Player 2's first monk must be of > 1 quantity"
                  << "\n";
        exit(EXIT_FAILURE);
      }
      else if (p2Quantity2 >= 1) {
        std::cout << "Error: Player 2 cannot have the second monk 'assisting' "
                     "the first monk in combat"
                  << "\n";
        std::cout << "The second monk must have a quantity of 0"
                  << "\n";
        exit(EXIT_FAILURE);
      }
    }

    // Behaviour: Get the rest of the information about the input entity
    // (without user input)
    returnEntityCSV = conductASearch(
      returnEntityCSV.entityName, returnEntityCSV.entityQuantity);

    // Behaviour: Close the file
    inputFile.close();

    // Behaviour: Return the information about a single entity
    return returnEntityCSV;
  }
}

// Function: Get the active technologies from '[p1/p2]_technologies.csv', the
// active events from '[p1/p2]_events.csv', or player details from 'players.csv'
int* fileImporter::aSplitColumnFile(
  std::string inputTechnologyOrEventsOrPlayerAgeFilename,
  int         inputNumberOfRows)
{
  // Behaviour: Open "[p1/p2]_technologies.csv" or "[p1/p2]_events.csv" for
  // player 1 or 2 (not both)
  inputFile.open(inputTechnologyOrEventsOrPlayerAgeFilename);

  // Behaviour: Check that the filename is correct
  if (
    (inputTechnologyOrEventsOrPlayerAgeFilename != "import/technologies_p1.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/technologies_p2.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/events_p1.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/events_p2.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/playerAge.csv")) {
    std::cout << "Error: Input filename supposed to be called "
                 "'import/[p1/p2]_technologies.csv', "
                 "'import/[p1/p2]_events.csv', or 'import/playerDetails.csv'"
              << "\n";
    exit(EXIT_FAILURE);
  }

  // Behaviour: Check if "technologies_[p1/p2].csv" or "[p1/p2]_events.csv"
  // successfully opened
  if (!inputFile.is_open()) {
    std::cout << "Error: Failed to open the file called "
              << inputTechnologyOrEventsOrPlayerAgeFilename << "\n";
    exit(EXIT_FAILURE);
  }
  else {
    // Variable: Declare the number of rows in the .csv files for events, technologies, ...
    int rows = inputNumberOfRows;

    // Variable: Declare the layout (an active column and the name column)
    int columns = 2;

    // Variable: Declare an array to store the number of words in the file
    int arrayWords = columns * rows;

    // Array: Declare and initialize the words array
    std::string words[arrayWords];
    for (int i = 0; i < arrayWords; i++) {
      words[i] = "";
    }

    // Variable: Declare the words array incrementer
    int count = 0;

    // Variable: Declare a string to store current word in the file
    std::string word = "";

    // Behaviour: While we are not at the end of the file keep running. Similar
    // to .good() and .eof()
    while (inputFile >> word) {
      // Behaviour: Ensure that the user does not exceed the array bounds
      if (count >= arrayWords + 1) {
        std::cout << "Error: You have entered too much input into "
                  << inputTechnologyOrEventsOrPlayerAgeFilename << "\n";
        std::cout << "You will have to replace the existing "
                  << inputTechnologyOrEventsOrPlayerAgeFilename
                  << " file with the default one"
                  << "\n";
        exit(EXIT_FAILURE);
      }
      else {
        // Behaviour: Pass the word into the words array and increment the
        // counter
        words[count] = word;
        count++;
      }
    }

    // Behaviour: Ensure that the words array contains all of the words
    if (count <= arrayWords - 1) {
      std::cout << "Error: You have entered too little input into "
                << inputTechnologyOrEventsOrPlayerAgeFilename << "\n";
      std::cout << "You will have to replace the existing "
                << inputTechnologyOrEventsOrPlayerAgeFilename
                << " file with the default one"
                << "\n";
      exit(EXIT_FAILURE);
    }

    // Pointer: Declare an array to store the even words
    // Fun fact:
    // - The new keyword is required to store the pointer in the heap and not
    // the stack
    // - Storing the pointer in the heap will prevent the pointer from being
    // deleted straight away

    int* everySecondElement = new int[arrayWords / 2];

    // Integer: Declare an integer to increment the everySecondElement array
    int incrementInnerArray = 0;

    // Integer: Declare an integer to increment the words array (checking if the
    // words are even)
    int incrementOutsideArray = 0;

    // Behaviour: Run a loop to find the even numbers of the array
    for (int i = 0; i < arrayWords; i++) {
      // Behaviour: Check if the words array counter is an even number
      if (incrementOutsideArray % 2 == 0) {
        // Behaviour: Make sure that the words input is either 0 or 1 before
        // trying to convert the string into an integer
        if (
          (words[i] != "0") && (words[i] != "1") && (words[i] != "2")
          && (words[i] != "3") && (words[i] != "4")) {
          std::cout << "The technologies/events file..."
                    << "\n";
          std::cout
            << "Error: For each row, enter 0 (inactive) or 1 (active) only"
            << "\n";

          std::cout << "The player details file..."
                    << inputTechnologyOrEventsOrPlayerAgeFilename
                    << " file with the default one"
                    << "\n";
          std::cout << "Error: For each row, enter a Medieval Age (1-4)"
                    << "\n";

          std::cout << "Generally.."
                    << "\n";
          std::cout << "Note that you must leave the names next to the numbers"
                    << "\n";

          exit(EXIT_FAILURE);
        }
        // Behaviour: Get the even element of the words array and pass this into
        // the every second element array
        everySecondElement[incrementInnerArray] = atoi(words[i].c_str());

        // Behaviour: Increment the every second element array
        incrementInnerArray++;
      }
      // Behaviour: Increment the words array counter
      incrementOutsideArray++;
    }

    // Behaviour: Close the file
    inputFile.close();

    // Behaviour: Return a string of technologies
      return everySecondElement;


  }
}

// Read from the player names file
std::string* fileImporter::playerNames(
    std::string inputPlayerNamesFilename,
    int         inputNumberOfRows)
{

  inputFile.open(inputPlayerNamesFilename);

  // Behaviour: Check that the filename is correct
  if (inputPlayerNamesFilename != "import/playerNames.csv") {
      std::cout << "Error: Input filename supposed to be called "
                   "'import/playerNames.csv'"
                << "\n";
      exit(EXIT_FAILURE);
  }

  std::string *lineString  = new std::string[inputNumberOfRows];

  std::string myline;

  if (inputFile.is_open()) {
      for (int i = 0; i < inputNumberOfRows; i ++){
        inputFile >> myline;
      std::replace(myline.begin(), myline.end(), '_', ' ');
        lineString[i] = myline;
      }

  }
  else{
      std::cout << "Error: couldn't open the file";
      exit(EXIT_FAILURE);
  }

  return lineString;
}

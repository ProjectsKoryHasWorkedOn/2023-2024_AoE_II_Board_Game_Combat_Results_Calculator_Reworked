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
#include <unordered_map>  // Using: std::unordered_map

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

  std::unordered_map<std::string, Entity> map = {
    {"ARCHER", Archer{}},
    {"ARCHER_(SARACEN)", Archer_Saracen{}},
    {"ARBALEST", Arbalest{}},
    {"ARBALEST_(BRITON)", Arbalest_Briton{}},
    {"ARBALEST_(SARACEN)", Arbalest_Saracen{}},
    {"ARCHERY_RANGE", Archery_Range{}},
    {"BARRACKS", Barracks{}},
    {"BATTERING_RAM", Battering_Ram{}},
    {"BATTERING_RAM", Battering_Ram_Celt{}},
    {"BERSERK_(VIKING)", Berserk_Viking{}},
    {"BLACKSMITH", Blacksmith{}},
    {"CAPPED_RAM", Capped_Ram{}},
    {"CAPPED_RAM_(CELT)", Capped_Ram_Celt{}},
    {"CASTLE", Castle{}},
    {"CASTLE_(FRANK)", Castle_Frank{}},
    {"CAVALIER", Cavalier{}},
    {"CAVALIER_(FRANK)", Cavalier_Frank{}},
    {"CAVALIER_(PERSIAN)", Cavalier_Persian{}},
    {"CAVALRY_ARCHER", Cavalry_Archer{}},
    {"CAVALRY_ARCHER_(MONGOL)", Cavalry_Archer_Mongol{}},
    {"CHAMPION", Champion{}},
    {"CHAMPION_(CELT)", Champion_Celt{}},
    {"CHAMPION_(GOTH)", Champion_Goth{}},
    {"CHAMPION_(JAPANESE)", Champion_Japanese{}},
    {"CHAMPION_(VIKING)", Champion_Viking{}},
    {"CHARLAMAGNE'S_PALACE_AT_AIX_LA'CHAPELLE_(BRITON)", Wonder{}},
    {"ROCK_OF_CASHEL_(CELT)", Wonder{}},
    {"THE_GOLDEN_TENT_OF_THE_GREAT_KHAN_(MONGOL)", Wonder{}},
    {"THE_PALACE_OF_CTESIPHON_ON_THE_TIGRIS_(PERSIAN)", Wonder{}},
    {"TOMB_OF_THEODORIC_(GOTH)", Wonder{}},
    {"NOTRE-DAME_CATHEDRAL_(FRANK)", Wonder{}},
    {"STAVE_CHURCH_AT_URNES_(VIKING)", Wonder{}},
    {"THE_GREAT_TEMPLE_AT_NARA_(JAPANESE)", Wonder{}},
    {"CROSSBOWMAN", Crossbowman{}},
    {"CROSSBOWMAN_(SARACEN)", Crossbowman_Saracen{}},
    {"DEMOLITION_SHIP", Demolition_Ship{}},
    {"DEMOLITION_SHIP_(VIKING)", Demolition_Ship_Viking{}},
    {"DOCK", Dock{}},
    {"DOCK_(PERSIAN)", Dock_Persian{}},
    {"DOCK_(VIKING)", Dock_Viking{}},
    {"ELITE_BERSERK_(VIKING)", Elite_Berserk_Viking{}},
    {"ELITE_HUSKARL_(GOTH)", Elite_Huskarl_Goth{}},
    {"ELITE_LONGBOAT_(VIKING)", Elite_Longboat_Viking{}},
    {"ELITE_LONGBOWMAN_(Briton)", Elite_Longbowman_Briton{}},
    {"ELITE_MAMELUKE_(SARACEN)", Elite_Mameluke_Saracen{}},
    {"ELITE_MANGUDAI_(MONGOL)", Elite_Mangudai_Mongol{}},
    {"ELITE_SAMURAI_(JAPANESE)", Elite_Samurai_Japanese{}},
    {"ELITE_SKIRMISHER", Elite_Skirmisher{}},
    {"ELITE_THROWING_AXEMAN_(FRANK)", Elite_Throwing_Axeman_Frank{}},
    {"ELITE_WAR_ELEPHANT_(PERSIAN)", Elite_War_Elephant_Persian{}},
    {"ELITE_WOAD_RAIDER_(CELT)", Elite_Woad_Raider_Celt{}},
    {"FARM", Farm{}},
    {"FAST_FIRE_SHIP", Fast_Fire_Ship{}},
    {"FIRE_SHIP", Fire_Ship{}},
    {"FISHING_SHIP_(JAPANESE)", Fishing_Ship_Japanese{}},
    {"FISHING_SHIP_(PERSIAN)", Fishing_Ship_Persian{}},
    {"FORTIFIED_WALL", Fortified_Wall{}},
    {"GALLEY", Galley{}},
    {"GALLEY_(JAPANESE)", Galley_Japanese{}},
    {"GALLEY_(SARACEN)", Galley_Saracen{}},
    {"GALLEY_(VIKING)", Galley_Viking{}},
    {"HERO", Hero{}},
    {"STONE_GATE", Stone_Gate{}},
    {"GALLEON", Galleon{}},
    {"GALLEON_(SARACEN)", Galleon_Saracen{}},
    {"GALLEON_(VIKING)", Galleon_Viking{}},
    {"GALLEON_(JAPANESE)", Galleon_Japanese{}},
    {"GOLD_MINE", Gold_Mine{}},
    {"GOLD_MINE_(JAPANESE)", Gold_Mine_Japanese{}},
    {"CAMEL", Camel{}},
    {"HEAVY_CAMEL", Heavy_Camel{}},
    {"HEAVY_CAVALRY_ARCHER", Heavy_Cavalry_Archer{}},
    {"HEAVY_CAVALRY_ARCHER_(MONGOL)", Heavy_Cavalry_Archer_Mongol{}},
    {"HEAVY_CAVALRY_ARCHER_(SARACEN)", Heavy_Cavalry_Archer_Saracen{}},
    {"HEAVY_DEMOLITION_SHIP", Heavy_Demolition_Ship{}},
    {"HEAVY_DEMOLITION_SHIP_(VIKING)", Heavy_Demolition_Ship_Viking{}},
    {"HEAVY_SCORPION", Heavy_Scorpion{}},
    {"HEAVY_SCORPION_(CELT)", Heavy_Scorpion_Celt{}},
    {"HOUSE", House{}},
    {"HUSKARL_(GOTH)", Huskarl_Goth{}},
    {"KNIGHT", Knight{}},
    {"KNIGHT_(FRANK)", Knight_Frank{}},
    {"KNIGHT_(PERSIAN)", Knight_Persian{}},
    {"LIGHT_CAVALRY", Light_Cavalry{}},
    {"LIGHT_CAVALRY_(MONGOL)", Light_Cavalry_Mongol{}},
    {"LONG_SWORDSMAN", Long_Swordsman{}},
    {"LONG_SWORDSMAN_(CELT)", Long_Swordsman_Celt{}},
    {"LONG_SWORDSMAN_(GOTH)", Long_Swordsman_Goth{}},
    {"LONG_SWORDSMAN_(JAPANESE)", Long_Swordsman_Japanese{}},
    {"LONG_SWORDSMAN_(VIKING)", Long_Swordsman_Viking{}},
    {"LONGBOAT_(VIKING)", Longboat_Viking{}},
    {"LONGBOWMAN_(BRITON)", Longbowman_Briton{}},
    {"LUMBER_CAMP", Lumber_Camp{}},
    {"LUMBER_CAMP_(JAPANESE)", Lumber_Camp_Japanese{}},
    {"MAMELUKE_(SARACEN)", Mameluke_Saracen{}},
    {"MAN-AT-ARMS", Man_At_Arms{}},
    {"MAN_AT_ARMS_(VIKING)", Man_At_Arms_Viking{}},
    {"MAN_AT_ARMS_(CELT)", Man_At_Arms_Celt{}},
    {"MAN_AT_ARMS_(GOTH)", Man_At_Arms_Goth{}},
    {"MAN_AT_ARMS_(JAPANESE)", Man_At_Arms_Japanese{}},
    {"MANGONEL", Mangonel{}},
    {"MANGONEL_(CELT)", Mangonel_Celt{}},
    {"MANGUDAI_(MONGOL)", Mangudai_Mongol{}},
    {"MARKET", Market{}},
    {"MARKET_(SARACEN)", Market_Saracen{}},
    {"MILITIA", Militia{}},
    {"MILITIA_(CELT)", Militia_Celt{}},
    {"MILITIA_(GOTH)", Militia_Goth{}},
    {"MILL", Mill{}},
    {"MILL_(JAPANESE)", Mill_Japanese{}},
    {"MONASTERY", Monastery{}},
    {"MONK", Monk{}},
    {"ONAGER", Onager{}},
    {"ONAGER_(CELT)", Onager_Celt{}},
    {"OUTPOST", Outpost{}},
    {"PALADIN", Paladin{}},
    {"PALADIN_(PERSIAN)", Paladin_Persian{}},
    {"PALADIN_(FRANK)", Paladin_Frank{}},
    {"PALISADE_WALL", Palisade_Wall{}},
    {"PIKEMAN", Pikeman{}},
    {"PIKEMAN_(CELT)", Pikeman_Celt{}},
    {"PIKEMAN_(GOTH)", Pikeman_Goth{}},
    {"PIKEMAN_(JAPANESE)", Pikeman_Japanese{}},
    {"PIKEMAN_(VIKING)", Pikeman_Viking{}},
    {"SAMURAI_(JAPANESE)", Samurai_Japanese{}},
    {"SCORPION", Scorpion{}},
    {"SCORPION_(CELT)", Scorpion_Celt{}},
    {"SCOUT_CAVALRY", Scout_Cavalry{}},
    {"SIEGE_ONAGER", Siege_Onager{}},
    {"SIEGE_ONAGER_(CELT)", Siege_Onager_Celt{}},
    {"SIEGE_RAM", Siege_Ram{}},
    {"SIEGE_RAM_(CELT)", Long_Swordsman{}},
    {"SIEGE_WORKSHOP", Siege_Workshop{}},
    {"SKIRMISHER", Skirmisher{}},
    {"SPEARMAN", Spearman{}},
    {"SPEARMAN_(JAPANESE)", Spearman_Japanese{}},
    {"SPEARMAN_(VIKING)", Spearman_Viking{}},
    {"SPEARMAN_(CELT)", Spearman_Celt{}},
    {"SPEARMAN_(GOTH)", Spearman_Goth{}},
    {"STABLE", Stable{}},
    {"STONE_MINE", Stone_Mine{}},
    {"STONE_MINE_(JAPANESE)", Stone_Mine_Japanese{}},
    {"STONE_WALL", Stone_Wall{}},
    {"THROWING_AXEMAN_(FRANK)", Throwing_Axeman_Frank{}},
    {"TOWN_CENTER", Town_Center{}},
    {"TOWN_CENTER_(BRITON)", Town_Center_Briton{}},
    {"TOWN_CENTER_(PERSIAN)", Town_Center_Persian{}},
    {"TRADE_COG", Trade_Cog{}},
    {"TREBUCHET", Trebuchet{}},
    {"TWO-HANDED_SWORDSMAN", Two_Handed_Swordsman{}},
    {"TWO-HANDED_SWORDSMAN_(CELT)", Two_Handed_Swordsman_Celt{}},
    {"TWO-HANDED_SWORDSMAN_(GOTH)", Two_Handed_Swordsman_Goth{}},
    {"TWO-HANDED_SWORDSMAN_(JAPANESE)", Two_Handed_Swordsman_Japanese{}},
    {"TWO-HANDED_SWORDSMAN_(VIKING)", Two_Handed_Swordsman_Viking{}},
    {"VILLAGER", Villager{}},
    {"WAR_ELEPHANT_(PERSIAN)", War_Elephant_Persian{}},
    {"WAR_GALLEY", War_Galley{}},
    {"WAR_GALLEY_(JAPANESE)", War_Galley_Japanese{}},
    {"WAR_GALLEY_(SARACEN)", War_Galley_Saracen{}},
    {"WAR_GALLEY_(VIKING)", War_Galley_Viking{}},
    {"WATCH_TOWER", Watch_Tower{}},
    {"WOAD_RAIDER_(CELT)", Woad_Raider_Celt{}}};

  returnEntity = map[inputEntityName];

  // @ PHillip could perhaps do this more eleganty with map function?
  // Behaviour: if it's a trebuchet, get further information about the Trebuchet
  if (inputEntityName == "Trebuchet") {
    Trebuchet returnTrebuchet;
    returnTrebuchet.updateTrebuchetArmorClass();
    returnEntity = returnTrebuchet;
  }

  // Behaviour: if it's a wonder, get further information about the Wonder
  if (returnEntity.entityName == "Wonder") {
    Wonder returnWonder;
    returnWonder.updateWonderHP();
    returnEntity = returnWonder;
  }

  // Behaviour: Search for the input entity name and return the associated
  // entity

  // Behaviour: Make sure that the quantity is > 0 for non-Monks
  if ((inputEntityName != "MONK") && (inputEntityQuantity <= 0)) {
    std::cout << "Error: All entities (except assisting monks) must have a "
                 "quantity of at least 1"
              << "\n";
    std::terminate();
  }

  // Behaviour: Make sure that an Age within the range of 1-4 was entered
  if( (returnEntity.entityAge < 1) || (returnEntity.entityAge > 5) ) {
    // Error: Age not recognized
    std::cout << "Error: " << returnEntity.entityName
              << " contains an Age other than 1-4"
              << "\n";
    std::terminate();
  }

  // Behaviour: Make sure that each entity (except for Villagers) has at least 1
  // armor class
  bool hasArmorClass = false;
  for (int i = 0; i < returnEntity.numberOfArmorClasses; i++) {
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
      std::terminate();
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
    std::terminate();
  }
  // Behaviour: Make sure that entities with the siege weapon armor class only
  // have a quantity of 1
  if (returnEntity.armorClass[12] == true && inputEntityQuantity != 1) {
    std::cout
      << "Error: " << returnEntity.entityName
      << " is a siege weapon and siege weapons can only be of 1 quantity"
      << "\n";
    std::terminate();
  }
  // Behavuour: Make sure that entities with the ship armor class only have a
  // quantity of 1
  if (returnEntity.armorClass[11] == true && inputEntityQuantity != 1) {
    std::cout << "Error: " << returnEntity.entityName
              << " is a ship and ships can only be of 1 quantity"
              << "\n";
    std::terminate();
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
    std::terminate();
  }
  else if (anInteger == 0) {
    if (inputWord != "0") {
      std::cout << "Error: The output was 0 but the input was not 0 for the "
                   "first quantity"
                << "\n";
      std::terminate();
    }
    else {
      // A quantity of < 1 is acceptable for the non-monk values
      // I'll check for the appropriate quantity based on entity types later
    }
  }
  else if (anInteger > 99) {
    std::cout << "Error: A limit of 100 units is more than reasonable"
              << "\n";
    std::terminate();
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
    std::terminate();
  }

  // Behaviour: Open "entities.csv"
  inputFile.open(inputEntityFilename);

  // Behaviour: Check if "entities.csv" did not open successfully
  if (!inputFile.is_open()) {
    std::cout << "Error: Failed to open the file called" << inputEntityFilename
              << "\n";
    std::terminate();
  }
  else {
    // Vector: Hold up to four words to represent the four words inside of
    // 'entites.csv'. ISO C++ does not support variable length arrays (VLA).
    std::vector<std::string> words(inputNumberOfWords);
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
        std::terminate();
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
      std::terminate();
    }

    // Behaviour: Make sure that the second words are both monks
    if (words[2] != "Monk") {
      std::cout << "Error: Non-Monk entity name for player 1's 'assisting "
                   "monks' data field"
                << "\n";
      std::terminate();
    }
    else if (words[6] != "Monk") {
      std::cout << "Error: Non-Monk entity name for player 2's 'assisting "
                   "monks' data field"
                << "\n";
      std::terminate();
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
        std::terminate();
      }
      else if (p1Quantity2 >= 1) {
        std::cout << "Error: Player 1 cannot have the second monk 'assisting' "
                     "the first monk in combat"
                  << "\n";
        std::cout << "The second monk must have a quantity of 0"
                  << "\n";
        std::terminate();
      }
    }

    if ((words[4] == "Monk") && (words[6] == "Monk")) {
      int p2Quantity1 = checkIsInteger(words[5]);
      int p2Quantity2 = checkIsInteger(words[7]);

      if (p2Quantity1 <= 0) {
        std::cout << "Error: Player 2's first monk must be of > 1 quantity"
                  << "\n";
        std::terminate();
      }
      else if (p2Quantity2 >= 1) {
        std::cout << "Error: Player 2 cannot have the second monk 'assisting' "
                     "the first monk in combat"
                  << "\n";
        std::cout << "The second monk must have a quantity of 0"
                  << "\n";
        std::terminate();
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
    std::terminate();
  }

  // Behaviour: Check if "technologies_[p1/p2].csv" or "[p1/p2]_events.csv"
  // successfully opened
  if (!inputFile.is_open()) {
    std::cout << "Error: Failed to open the file called "
              << inputTechnologyOrEventsOrPlayerAgeFilename << "\n";
    std::terminate();
  }
  else {
    // Variable: Declare the number of rows in the .csv files for events,
    // technologies, ...
    int rows = inputNumberOfRows;

    // Variable: Declare the layout (an active column and the name column)
    int columns = 2;

    // Variable: Declare an array to store the number of words in the file
    int arrayWords = columns * rows;

    // Vector: Declare and initialize the words array
    std::vector<std::string> words(arrayWords);
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
        std::terminate();
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
      std::terminate();
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

          std::terminate();
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
    std::terminate();
  }

  std::string* lineString = new std::string[inputNumberOfRows];

  std::string myline;

  if (inputFile.is_open()) {
    for (int i = 0; i < inputNumberOfRows; i++) {
      inputFile >> myline;
      std::replace(myline.begin(), myline.end(), '_', ' ');
      lineString[i] = myline;
    }
  }
  else {
    std::cout << "Error: couldn't open the file";
    std::terminate();
  }

  return lineString;
}

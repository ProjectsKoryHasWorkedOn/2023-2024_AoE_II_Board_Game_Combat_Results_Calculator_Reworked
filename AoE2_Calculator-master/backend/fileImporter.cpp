/** The headers **/
#include "fileImporter.h" // Using: entitiesFile functions
#include "database.hpp"
#include "entity.h"      // Using: entity class
#include <algorithm>     // Using: transform, toupper, .begin(), .end()
#include <cstdlib>       // Using: exit(EXIT_FAILURE)
#include <fstream>       // Using: ifstream, is_open(), good, close()
#include <iostream>      // Using: cin and cout
#include <sstream>       // Using: stringstream
#include <stdlib.h>      // Using: atoi
#include <string>        // Using: string
#include <unordered_map> // Using: std::unordered_map

// Functions: The constructor and deconstructor
fileImporter::fileImporter(Database* database) : m_database{database}
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

  std::unordered_map<std::string, Entity> map = m_database->getUnitEntities();
  std::unordered_map<std::string, Entity> buildings
    = m_database->getBuildingEntities();
  std::unordered_map<std::string, Entity> wonders = {
    {"CHARLAMAGNE'S_PALACE_AT_AIX_LA'CHAPELLE_(BRITON)", Wonder{}},
    {"ROCK_OF_CASHEL_(CELT)", Wonder{}},
    {"THE_GOLDEN_TENT_OF_THE_GREAT_KHAN_(MONGOL)", Wonder{}},
    {"THE_PALACE_OF_CTESIPHON_ON_THE_TIGRIS_(PERSIAN)", Wonder{}},
    {"TOMB_OF_THEODORIC_(GOTH)", Wonder{}},
    {"NOTRE-DAME_CATHEDRAL_(FRANK)", Wonder{}},
    {"STAVE_CHURCH_AT_URNES_(VIKING)", Wonder{}},
    {"THE_GREAT_TEMPLE_AT_NARA_(JAPANESE)", Wonder{}}};
  map.insert(
    std::make_move_iterator(buildings.begin()),
    std::make_move_iterator(buildings.end()));
  map.insert(
    std::make_move_iterator(wonders.begin()),
    std::make_move_iterator(wonders.end()));
  returnEntity = map[inputEntityName];

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
  if ((returnEntity.entityAge < 1) || (returnEntity.entityAge > 5)) {
    // Error: Age not recognized
    std::cout << "Error: " << returnEntity.entityName
              << " contains an Age other than 1-4"
              << "\n";
    std::terminate();
  }

  // Behaviour: Make sure that each entity has at least 1 armor class
  bool hasArmorClass = false;

  for (int i = 0; i < returnEntity.numberOfArmorClasses; i++) {
    if (returnEntity.armorClass[i] == true) {
      hasArmorClass = true;
    }
  }
  if (hasArmorClass == false) {
    std::cout << "Error: " << returnEntity.entityName
              << " is missing at least 1 armor class"
              << "\n";
    std::terminate();
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
  else if (anInteger > 500) {
    std::cout << "Error: A limit of 500 units is more than reasonable"
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
  if (inputEntityFilename != "import/csv/entities.csv") {
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
    (inputTechnologyOrEventsOrPlayerAgeFilename != "import/csv/technologies_p1.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/csv/technologies_p2.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/csv/events_p1.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/csv/events_p2.csv")
    && (inputTechnologyOrEventsOrPlayerAgeFilename != "import/csv/playerAge.csv")) {
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
  if (inputPlayerNamesFilename != "import/csv/playerNames.csv") {
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

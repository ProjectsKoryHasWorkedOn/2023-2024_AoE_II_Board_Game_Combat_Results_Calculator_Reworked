#pragma once
/** The libaries **/
#include "entity.h" // Using: entity class
#include <fstream>  // Using: ifstream, is_open(), good, close()
#include <string>   // Using: string

/** The file importer class **/
#ifndef FILE_IMPORTER_H
#define FILE_IMPORTER_H

class Database;

class fileImporter {
public:
  // Object: A single common file object to store the input file
  std::ifstream inputFile;

  std::string directoryToWorkOffOf;

  // Functions: The constructor and deconstructor
  explicit fileImporter(Database* database, std::string directoryWeAreWorkingIn);
  ~fileImporter();

  // Function: Check if a string is an integer
  int checkIsInteger(std::string inputWord);

  // Function: Get the entity name and quantity from 'entities.csv'
  Entity entitiesFile(
    std::string inputEntityFilename,
    int         inputNumberOfWords,
    int         inputPlayerNumber,
    int         inputMonkReturnSetting);

  // Function: Get the rest of the information about an entity
  Entity conductASearch(std::string inputEntityName, int inputEntityQuantity);

  // Function: Get the active technologies from '[p1/p2]_technologies.csv', the
  // active events from '[p1/p2]_events.csv', or player details from
  // 'players.csv'
  int* aSplitColumnFile(
    std::string inputTechnologyOrEventsOrPlayerAgeFilename,
    int         inputNumberOfRows);

  std::string* playerNames(
    std::string inputPlayerNamesFilename,
    int         inputNumberOfRows);

private:
  Database* m_database;
};

#endif // FILE_IMPORTER_H

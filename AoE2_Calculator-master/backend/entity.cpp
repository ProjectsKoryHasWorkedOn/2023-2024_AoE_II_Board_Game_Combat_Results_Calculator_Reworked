/** The headers **/
#include "entity.h" // Using: entity class
#include "dialog_input.h"
#include <cstdlib>  // Using: exit(EXIT_FAILURE)
#include <iostream> // Using: cin and cout
#include <string>   // Using: string, to_string

// Entity functions: The constructor and deconstructor
Entity::Entity()
{
  // Behaviour: Set the initial and often derived values
  entityName     = "";
  entityAge      = 0;
  entityQuantity = 0;
  entityHealth   = 0;
  standardDamage = 0;
  rangedDamage   = 0;
  garrisonValue  = 0;
  pointValue     = 0;
  for (int i = 0; i < numberOfArmorClasses; i++) {
    armorClass[i] = false;
  }
  entitiesArmorClasses                    = 0;
  onlyAttacksOnceInTheFirstRoundOfCombat  = false;
  dealsAreaEffectDamage                   = false;
  onlyAttacksOnceInTheSecondRoundOfCombat = false;
  isKamikaze                              = false;
  initialEntityQuantity                   = 0;
  healsAvailable                          = 0;
}

Entity::~Entity()
{
}

Wonder::Wonder()
{
  turnsInPlay   = 0;
  entityName    = "Wonder";
  entityAge     = 4;
  pointValue    = 90;   // 20 wood, 20 stone, 20 gold, 30 bodies.
  armorClass[1] = true; // Building armor class
}

Wonder::~Wonder()
{
}

// Function: Update Wonder HP
void Wonder::updateWonderHP()
{
  // Behaviour: The rules state that you win if the wonder is in play for 6
  // turns
  turnsInPlay = 0;
  std::cout << "How many turns have the Wonder been in play for?"
            << "<br>";
  DIN >> turnsInPlay;

  if ((turnsInPlay < 1) || (turnsInPlay > 6)) {
    std::cout << "Error: Wonder ought to be in play for 1-6 turns only"
              << "<br>";
    std::terminate();
  }
  // Behaviour: Update wonder HP based on the number of turns in play
  else if (turnsInPlay == 1) {
    entityHealth = 110;
  }
  else if (turnsInPlay == 2) {
    entityHealth = 220;
  }
  else if (turnsInPlay == 3) {
    entityHealth = 330;
  }
  else {
    entityHealth = 440;
  }
}

// Function: Calculate the number of armor classes unique to an entity
int Entity::returnNumberOfArmorClasses()
{
  entitiesArmorClasses = 0;

  // Behaviour: Increment entitiesArmorClasses for each present armorClass
  for (int i = 0; i < 44; i++) {
    if (armorClass[i] == true) {
      entitiesArmorClasses++;
    }
  }

  // Behaviour: Return the number of armor classes
  return entitiesArmorClasses;
}

// Function: Make text colorful
void Entity::displayColorfulText(
  std::string inputFormatAttribute,
  std::string inputForegroundColor,
  std::string inputBackgroundColor,
  std::string inputText,
  bool        isLineBreak)
{
  // Variables: Store the color codes
  std::string textDecorationValue  = "none";
  std::string fontWeightValue      = "normal";
  std::string textColorValue       = "default";
  std::string backgroundColorValue = "default";

  // There are 19 predefined QColor objects: white, black, red, darkRed, green,
  // darkGreen, blue, darkBlue, cyan, darkCyan, magenta, darkMagenta, yellow,
  // darkYellow, gray, darkGray, lightGray Get error if hex code is used
  /*
   *
QCssParser::parseHexColor: Unknown color name '#F'
QCssParser::parseHexColor: Unknown color name '#FF'
QCssParser::parseHexColor: Unknown color name '#FFFF'
QCssParser::parseHexColor: Unknown color name '#FFFFF'
   */

  // Behaviour: Convert the input text into a color code for the attribute
  if (inputFormatAttribute == "default" || inputFormatAttribute == "") {
    fontWeightValue     = "normal";
    textDecorationValue = "none";
  }
  else if (inputFormatAttribute == "bold") {
    fontWeightValue = "bold";
  }
  else if (inputFormatAttribute == "underlined") {
    textDecorationValue = "underline";
  }
  else if (inputFormatAttribute == "bold_and_underlined") {
    fontWeightValue     = "bold";
    textDecorationValue = "underline";
  }
  else {
    std::cout << "Error: formatAttributeCode not found"
              << "<br>";
    std::terminate();
  }

  // Behaviour: Convert the input text into a color code for the foreground
  // color (text color)
  if (inputForegroundColor == "black") {
    textColorValue = "black";
  }
  else if (inputForegroundColor == "red") {
    textColorValue = "red";
  }
  else if (inputForegroundColor == "green") {
    textColorValue = "green";
  }
  else if (inputForegroundColor == "yellow") {
    textColorValue = "yellow";
  }
  else if (inputForegroundColor == "blue") {
    textColorValue = "blue";
  }
  else if (inputForegroundColor == "magenta") {
    textColorValue = "magenta";
  }
  else if (inputForegroundColor == "cyan") {
    textColorValue = "cyan";
  }
  else if (inputForegroundColor == "light_gray") {
    textColorValue = "lightgray";
  }
  else if (inputForegroundColor == "default" || inputForegroundColor == "") {
    textColorValue = "default";
  }
  else if (inputForegroundColor == "dark_gray") {
    textColorValue = "darkgray";
  }
  else if (inputForegroundColor == "light_red") {
    textColorValue = "rgb(255,204,203)";
  }
  else if (inputForegroundColor == "light_green") {
    textColorValue = "rgb(144,238,144)";
  }
  else if (inputForegroundColor == "light_yellow") {
    textColorValue = "rgb(255,255,224)";
  }
  else if(inputForegroundColor == "orange"){
    textColorValue = "rgb(255,140,0)";
  }
  else if (inputForegroundColor == "light_blue") {
    textColorValue = "rgb(173,216,230)";
  }
  else if (inputForegroundColor == "light_magenta") {
    textColorValue = "rgb(255,128,255)";
  }
  else if (inputForegroundColor == "light_cyan") {
    textColorValue = "rgb(224,255,255)";
  }
  else if (inputForegroundColor == "dark_cyan") {
    textColorValue = "darkcyan";
  }
  else if (inputForegroundColor == "white") {
    textColorValue = "rgb(255,255,255)";
  }
  else {
    std::cout << "Error: foregroundColorCode not found"
              << "<br>";
    std::terminate();
  }

  // Behaviour: Convert the input text into a color code for the background
  // color
  if (inputBackgroundColor == "black") {
    backgroundColorValue = "black";
  }
  else if (inputBackgroundColor == "red") {
    backgroundColorValue = "red";
  }
  else if (inputBackgroundColor == "green") {
    backgroundColorValue = "green";
  }
  else if (inputBackgroundColor == "yellow") {
    backgroundColorValue = "yellow";
  }
  else if (inputBackgroundColor == "blue") {
    backgroundColorValue = "blue";
  }
  else if (inputBackgroundColor == "magenta") {
    backgroundColorValue = "magenta";
  }
  else if (inputBackgroundColor == "cyan") {
    backgroundColorValue = "cyan";
  }
  else if(inputBackgroundColor == "orange"){
    backgroundColorValue = "rgb(255,140,0)";
  }
  else if (inputBackgroundColor == "light_gray") {
    backgroundColorValue = "LightGray";
  }
  else if (inputBackgroundColor == "default" || inputBackgroundColor == "") {
    backgroundColorValue = "default";
  }
  else if (inputBackgroundColor == "dark_gray") {
    backgroundColorValue = "DarkGray";
  }
  else if (inputBackgroundColor == "light_red") {
    backgroundColorValue = "rgb(255,204,203)";
  }
  else if (inputBackgroundColor == "light_green") {
    backgroundColorValue = "rgb(144,238,144)";
  }
  else if (inputBackgroundColor == "light_yellow") {
    backgroundColorValue = "rgb(255,255,224);";
  }
  else if (inputBackgroundColor == "light_blue") {
    backgroundColorValue = "rgb(173,216,230)";
  }
  else if (inputBackgroundColor == "light_magenta") {
    backgroundColorValue = "rgb(255,128,255)";
  }
  else if (inputBackgroundColor == "light_cyan") {
    backgroundColorValue = "rgb(224,255,255)";
  }
  else if (inputBackgroundColor == "dark_cyan") {
    backgroundColorValue = "DarkCyan";
  }
  else if (inputBackgroundColor == "white") {
    backgroundColorValue = "white";
  }
  else {
    std::cout << "Error: backgroundColorCode not found"
              << "<br>";
    std::terminate();
  }

  // Behaviour: Display the text to the terminal
  if (isLineBreak == false) {
    // Behaviour: Do not add a line break

    std::cout << "<span style=\"text-decoration: " << textDecorationValue
              << ";font-weight: " << fontWeightValue
              << ";color: " << textColorValue
              << ";background-color: " << backgroundColorValue << "\">"
              << inputText << "</span>";
  }
  else {
    // Behaviour: Add a line break
    std::cout << "<span style=\"text-decoration: " << textDecorationValue
              << ";font-weight: " << fontWeightValue
              << ";color: " << textColorValue
              << ";background-color: " << backgroundColorValue << "\">"
              << inputText << "</span>"
              << "<br>";
  }
}

// Function: Return entity information
void Entity::outputEntity(
  std::string        playerName,
  EntityOutputConfig entityOutputConfig)
{
  // TODO: HERE
  const bool
    showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput{
      entityOutputConfig
        .showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput};
  const bool showFurtherInformationAboutTheBattleParticipantsInTheOutput{
    entityOutputConfig
      .showFurtherInformationAboutTheBattleParticipantsInTheOutput};








  // Behaviour: Display the player's name first (no matter what)
  displayColorfulText("bold", "white", "default", playerName + "'s", false);
  std::cout << " ";

  // Behaviour: Check that the entity is not dead before proceeding
  if (entityQuantity >= 1) {
    // Variable: Store the pre-requisite Age in text format
    std::string medievalAge = "";

    // Behaviour: Convert the integer entityAge to text format
    switch (entityAge) {
    case 1:
      medievalAge = "Dark_Age";
      break;
    case 2:
      medievalAge = "Feudal_Age";
      break;
    case 3:
      medievalAge = "Castle_Age";
      break;
    case 4:
      medievalAge = "Imperial_Age";
      break;
    default:
      std::cout << "Error: Age not recognized for " << entityName << "<br>";
      std::terminate();
    }

    // Behaviour: Return standard information about the entity to the console
    displayColorfulText(
      "bold", "white", "magenta", std::to_string(entityQuantity), false);

    std::cout << " ";

    displayColorfulText("bold", "white", "dark_gray", medievalAge, false);

    std::cout << " ";



      if (entityQuantity > 1) {
        displayColorfulText("bold", "white", "dark_cyan", getPluralName(), false);

        if(showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput == true){
          std::cout << " have a total of ";
        }
        else{
          std::cout << " each have ";
        }

      }
      else {
        displayColorfulText("bold", "white", "dark_cyan", entityName, false);

        std::cout << " has ";
      }

    int healthValueToSendToOutput = entityHealth;
    int standardDamageValueToSendToOutput = standardDamage;
    int rangedDamageValueToSendToOutput = rangedDamage;
    int pointValueToSendToOutput = pointValue;


    if(showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput == true){
      healthValueToSendToOutput = entityHealth * entityQuantity;
      standardDamageValueToSendToOutput = standardDamage * entityQuantity;
      rangedDamageValueToSendToOutput = rangedDamage * entityQuantity;
      pointValueToSendToOutput = pointValue * entityQuantity;

    }

    displayColorfulText(
      "bold", "red", "default", std::to_string(healthValueToSendToOutput), false);
    std::cout << " "
              << "HP";

    // Behaviour: Return further information about the entities values if the
    // value is present
    if (standardDamage != 0) {
      std::cout << ", ";
      displayColorfulText(
        "bold", "yellow", "default", std::to_string(standardDamageValueToSendToOutput), false);
      std::cout << " "
                << "melee damage";
    }
    if (rangedDamage != 0) {
      std::cout << ", ";
      displayColorfulText(
        "bold", "magenta", "default", std::to_string(rangedDamageValueToSendToOutput), false);
      std::cout << " "
                << "ranged damage";
    }

    if(showFurtherInformationAboutTheBattleParticipantsInTheOutput == true){
      if (garrisonValue != 0) {
        std::cout << ", ";
        displayColorfulText(
          "bold", "white", "brown", std::to_string(garrisonValue), false);
        std::cout << "garrison value";
      }
    }

    //  Show maximum range
    if (maximumRange != 0) {
      std::cout << ", ";
      displayColorfulText(
        "bold", "green", "default", std::to_string(maximumRange), false);
      std::cout << " ";
      std::cout << "max. range";
    }

    // Show minimum range value
    if (minimumRange != 0) {
      std::cout << ", ";
      displayColorfulText(
        "bold", "green", "default", std::to_string(minimumRange), false);
      std::cout << " ";
      std::cout << "min. range";
    }

    if (pointValue != 0) {
      if(showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput == true){
      std::cout << ", and are worth a total of ";

      }
      else{
      std::cout << ", and are each worth ";
      }




      displayColorfulText(
        "bold", "cyan", "default", std::to_string(pointValueToSendToOutput), false);
      std::cout << " ";
      if (pointValue > 1 || showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput == true) {
        std::cout << "points";
      }
      else {
        std::cout << "point";
      }
    }

    // Behaviour: Return further information about the entities armor classes if
    // the armor class is present and if the setting to show this has been
    // toggled

    // @todo Kory add toggle for this in mainwindow.ui and carry that over to
    // here


    if (showFurtherInformationAboutTheBattleParticipantsInTheOutput == true) {
      std::cout << ". It's of type: ";

      // Array: Store whether or not an armor class has been displayed
      bool displayedArmorClass[numberOfArmorClasses] = {false};

      // Behaviour: Only display each armor class once and add a space after
      // each armor class except for the last class

      // Only shows armor classes from the board game, not video game

      // Could have fewer armor classes if I referred to entityName (e.g. for
      // capped ram) instead of having an armor class for it in
      // modifiersCalculator.cpp


      for (int i = 0; i != entitiesArmorClasses; i++) {
        if ((armorClass[0] == true) && (displayedArmorClass[0] == false)) {
          displayColorfulText("bold", "white", "blue", "Archer", false);
          displayedArmorClass[0] = true;
        }
        else if ((armorClass[1] == true) && (displayedArmorClass[1] == false)) {
          displayColorfulText("bold", "white", "blue", "Building", false);
          displayedArmorClass[1] = true;
        }
        else if ((armorClass[2] == true) && (displayedArmorClass[2] == false)) {
          displayColorfulText("bold", "white", "blue", "Camel", false);
          displayedArmorClass[2] = true;
        }
        else if ((armorClass[3] == true) && (displayedArmorClass[3] == false)) {
          displayColorfulText("bold", "white", "blue", "Castle", false);
          displayedArmorClass[3] = true;
        }
        else if ((armorClass[4] == true) && (displayedArmorClass[4] == false)) {
          displayColorfulText("bold", "white", "blue", "Cavalry", false);
          displayedArmorClass[4] = true;
        }
        else if ((armorClass[5] == true) && (displayedArmorClass[5] == false)) {
          displayColorfulText("bold", "white", "blue", "Cavalry_Archer", false);
          displayedArmorClass[5] = true;
        }
        else if ((armorClass[6] == true) && (displayedArmorClass[6] == false)) {
          displayColorfulText("bold", "white", "blue", "Eagle_Warrior", false);
          displayedArmorClass[6] = true;
        }
        else if ((armorClass[7] == true) && (displayedArmorClass[7] == false)) {
          displayColorfulText("bold", "white", "blue", "Gunpowder_Unit", false);
          displayedArmorClass[7] = true;
        }
        else if ((armorClass[8] == true) && (displayedArmorClass[8] == false)) {
          displayColorfulText("bold", "white", "blue", "Infantry", false);
          displayedArmorClass[8] = true;
        }
        else if ((armorClass[9] == true) && (displayedArmorClass[9] == false)) {
          displayColorfulText("bold", "white", "blue", "Monk", false);
          displayedArmorClass[9] = true;
        }
        else if (
          (armorClass[10] == true) && (displayedArmorClass[10] == false)) {
          displayColorfulText("bold", "white", "blue", "Ram", false);
          displayedArmorClass[10] = true;
        }
        else if (
          (armorClass[11] == true) && (displayedArmorClass[11] == false)) {
          displayColorfulText("bold", "white", "blue", "Ship", false);
          displayedArmorClass[11] = true;
        }
        else if (
          (armorClass[12] == true) && (displayedArmorClass[12] == false)) {
          displayColorfulText("bold", "white", "blue", "Siege_Weapon", false);
          displayedArmorClass[12] = true;
        }
        else if (
          (armorClass[13] == true) && (displayedArmorClass[13] == false)) {
          displayColorfulText("bold", "white", "blue", "Spearman", false);
          displayedArmorClass[13] = true;
        }
        else if (
          (armorClass[14] == true) && (displayedArmorClass[14] == false)) {
          displayColorfulText("bold", "white", "blue", "Stone_Defence", false);
          displayedArmorClass[14] = true;
        }
        else if (
          (armorClass[15] == true) && (displayedArmorClass[15] == false)) {
          displayColorfulText("bold", "white", "blue", "Turtle_Ship", false);
          displayedArmorClass[15] = true;
        }
        else if (
          (armorClass[16] == true) && (displayedArmorClass[16] == false)) {
          displayColorfulText("bold", "white", "blue", "Unique_Unit", false);
          displayedArmorClass[16] = true;
        }
        else if (
          (armorClass[17] == true) && (displayedArmorClass[17] == false)) {
          displayColorfulText("bold", "white", "blue", "Wall_&_Gate", false);
          displayedArmorClass[17] = true;
        }
        else if (
          (armorClass[18] == true) && (displayedArmorClass[18] == false)) {
          displayColorfulText("bold", "white", "blue", "Elephant", false);
          displayedArmorClass[18] = true;
        }
        else if (
          (armorClass[19] == true) && (displayedArmorClass[19] == false)) {
          displayColorfulText("bold", "white", "blue", "Hero", false);
          displayedArmorClass[19] = true;
        }
        else if (
          (armorClass[20] == true) && (displayedArmorClass[20] == false)) {
          displayColorfulText(
            "bold", "white", "blue", "Demolition_Ship", false);
          displayedArmorClass[20] = true;
        }
        else if (
          (armorClass[21] == true) && (displayedArmorClass[21] == false)) {
          displayColorfulText("bold", "white", "blue", "Galley", false);
          displayedArmorClass[21] = true;
        }
        else if (
          (armorClass[22] == true) && (displayedArmorClass[22] == false)) {
          displayColorfulText("bold", "white", "blue", "Fire_Ship", false);
          displayedArmorClass[22] = true;
        }
        else if (
          (armorClass[23] == true) && (displayedArmorClass[23] == false)) {
          displayColorfulText("bold", "white", "blue", "Watch_Tower", false);
          displayedArmorClass[23] = true;
        }
        else if (
          (armorClass[24] == true) && (displayedArmorClass[24] == false)) {
          displayColorfulText("bold", "white", "blue", "Town_Center", false);
          displayedArmorClass[24] = true;
        }
        else if (
          (armorClass[25] == true) && (displayedArmorClass[25] == false)) {
          displayColorfulText("bold", "white", "blue", "Land_Unit", false);
          displayedArmorClass[25] = true;
        }
        else if (
          (armorClass[26] == true) && (displayedArmorClass[26] == false)) {
          displayColorfulText("bold", "white", "blue", "Longboat", false);
          displayedArmorClass[26] = true;
        }
        else if (
          (armorClass[27] == true) && (displayedArmorClass[27] == false)) {
          displayColorfulText("bold", "white", "blue", "Villager", false);
          displayedArmorClass[27] = true;
        }

        if (i != entitiesArmorClasses - 1) {
          std::cout << " ";
        }
      }



      if(isKamikaze || dealsAreaEffectDamage){
        std::cout << ". It has these unique properties: ";

        if(isKamikaze == true){
          displayColorfulText("bold", "white", "orange", "Suicide attacks", false);
        }

        if(isKamikaze && dealsAreaEffectDamage){
          std::cout << ", ";
        }

        if(dealsAreaEffectDamage == true){
          displayColorfulText("bold", "white", "orange", "Deals area effect damage", false);
        }

        std::cout << ". ";

      }


      if(onlyAttacksOnceInTheFirstRoundOfCombat == true){
        std::cout << "It only attacks in the <b>first</b> round of combat.";
      }


      if(onlyAttacksOnceInTheSecondRoundOfCombat == true){
        std::cout << "It only attacks in the <b>second</b> round of combat.";
      }





    }



  }
  // Behaviour: Display something different if the entity is dead
  else if (entityQuantity <= 0) {
    displayColorfulText("bold", "white", "dark_cyan", entityName, false);
    std::cout << " is dead";
  }

  std::cout << "<br>";
}

std::string Entity::getPluralName() const
{
  if (entityName == "Man-at-Arms") {
    return "Men-at-Arms";
  }

  return entityName + "s";
}

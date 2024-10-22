// Included files
#include "mainwindow.hpp"     // This window
#include "aboutwindow.h"      // A window this window can open
#include "backend/run_game.h" // Age of Empires combat results calculator v1.2
#include "database.hpp"
#include "developerwindow.hpp"
#include "dialog_input.h"
#include "file_paths.h"
#include "openoutputwindowclass.h"
#include "pdfwindow.hpp"
#include "soundEffects.h" // Sound playing class
#include <QLabel>
#include <QMap>
#include <ranges>

#include "cross-window_palette.h" // Coloring of the UI

// Libraries for gifs
#include <QMovie>

// Libraries used for std::cout
#include <iostream>

// background color
#include <QPalette>

// Libraries used for std::copy_if
#include <algorithm>

// Libraries used for debugging
#include <QDebug>

// Libraries used for accessing file paths
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>

#include <QIcon>

// Libraries used for storing data
#include <QColor> // Prefers RGB input over HEX
#include <QListWidgetItem>
#include <QSet>
#include <QString>
#include <QStringList>
#include <vector>

// Libraries for acquiring user input
#include <QInputDialog>

// Librraries used for hotkeys
#include <QShortcut>

// Libraries used for tooltips
#include <QToolTip>

// Animation variables
QString p1BuildingArchitecturalStyle
  = "_western_european"; // _african, _asian, _central_european, _east_asian,
                         // _eastern_european, _mediterranean,
                         // _middle_eastern,_native_american,_south_asian,_southeast_asian,
                         // _western_european,
QString p1UnitStyle = "_western"; // _western, _mesoamerican, _asian

QString p2BuildingArchitecturalStyle = "_western_european";
QString p2UnitStyle = "_western"; // _western, _mesoamerican, _asian

// Regex expressions
QRegularExpression removeBracketedTextExpression(
  "(\\_\\(.*?\\))"); // Remember to add double backslashes in QT

QRegularExpression removeBracketedTextExpressionNoUnderscore(" \\(.*?\\)");

// Declaring class
SoundPlayer playSound;
bool        soundEffectsEnabled   = true;
bool        hasProgramInitialized = false;

// Getting default selection for the prompts
QString retreatingPromptAnswer        = "Ask each time";
QString monkAttackingPromptAnswer     = "Ask each time";
QString convertingHealingPromptAnswer = "Ask each time";

// More global variables
bool    isP1BackFromAForeignLandEventInPlay;
QString player1BackFromAForeignLandCivilizationBonusSelection;

bool    isP2BackFromAForeignLandEventInPlay;
QString player2BackFromAForeignLandCivilizationBonusSelection;

// Civilization selection
QStringList civilizations = {
  "Britons",
  "Celts",
  "Goths",
  "Mongols",
  "Persians",
  "Franks",
  "Japanese",
  "Saracens",
  "Vikings"};
QString player1Civilization;
QString player2Civilization;

// Declaring the variables, arrays for the UI elements
QStringList entityNames;
QStringList unitNames;

QStringList buildingNames;
QStringList age1UnitNames;
QStringList age2UnitNames;
QStringList age3UnitNames;
QStringList age4UnitNames;
QStringList age1BuildingNames;
QStringList age2BuildingNames;
QStringList age3BuildingNames;
QStringList age4BuildingNames;

int     player1EntityQuantity;
int     player1AssistingEntityQuantity;
int     player2EntityQuantity;
int     player2AssistingEntityQuantity;
QString player1Name;
QString player2Name;
QString player1Color;
QString player2Color;
QString player1BattleAssistantName;
QString player2BattleAssistantName;

int representationOfPlayer1Age;
int representationOfPlayer2Age;

// Remember the last player selection
int p1VillagerMemoryOld;
int p2VillagerMemoryOld;

// Get what age the player is in
QString     player1Age;
QString     player2Age;
QStringList ages;

QStringList backFromAForeignLandCivilizationBonuses;

QString MainWindow::convertSpacesToUnderscores(QString text)
{
  std::replace(text.begin(), text.end(), ' ', '_');
  return text;
}

QString MainWindow::convertUnderscoresToSpaces(QString text)
{
  std::replace(text.begin(), text.end(), '_', ' ');
  return text;
}

static bool isBuilding(const QString& entity)
{
  return (entity.contains("ARCHERY_RANGE")) || (entity.contains("BARRACKS"))
         || (entity.contains("BLACKSMITH")) || (entity.contains("CASTLE"))
         || (entity.contains(
           "CHARLAMAGNE'S_PALACE_AT_AIX_LA'CHAPELLE_(BRITON)"))
         || (entity.contains("ROCK_OF_CASHEL_(CELT)"))
         || (entity.contains("THE_GOLDEN_TENT_OF_THE_GREAT_KHAN_(MONGOL)"))
         || (entity.contains("THE_PALACE_OF_CTESIPHON_ON_THE_TIGRIS_(PERSIAN)"))
         || (entity.contains("THE_GREAT_MOSQUE_AT_SAMARRA_(SARACEN)"))
         || (entity.contains("TOMB_OF_THEODORIC_(GOTH)"))
         || (entity.contains("NOTRE-DAME_CATHEDRAL_(FRANK)"))
         || (entity.contains("STAVE_CHURCH_AT_URNES_(VIKING)"))
         || (entity.contains("THE_GREAT_TEMPLE_AT_NARA_(JAPANESE)"))
         || (entity.contains("DOCK")) || (entity.contains("WALL"))
         || (entity.contains("GATE")) || (entity.contains("GOLD_MINE"))
         || (entity.contains("HOUSE")) || (entity.contains("LUMBER_CAMP"))
         || (entity.contains("MARKET")) || (entity.contains("MILL"))
         || (entity.contains("MONASTERY")) || (entity.contains("OUTPOST"))
         || (entity.contains("SIEGE_WORKSHOP")) || (entity.contains("STABLE"))
         || (entity.contains("STONE_MINE")) || (entity.contains("TOWN_CENTER"))
         || (entity.contains("WATCH_TOWER")) || (entity.contains("FARM"));
}


QString MainWindow::returnTechnologyFilePathThatMatchesTechnologyName(
  QString technologyName)
{
  if (technologyName == "Blast Furnace") {
    return tBlastFurnaceIconFilePath;
  }
  else if (technologyName == "Bloodlines") {
    return tBloodlinesIconFilePath;
  }
  else if (technologyName == "Bodkin Arrow") {
    return tBodkinArrowIconFilePath;
  }
  else if (technologyName == "Chain Barding Armor") {
    return tChainBardingArmorIconFilePath;
  }
  else if (technologyName == "Chain Mail Armor") {
    return tChainMailArmorIconFilePath;
  }
  else if (technologyName == "Fletching") {
    return tFletchingIconFilePath;
  }
  else if (technologyName == "Forging") {
    return tForgingIconFilePath;
  }
  else if (technologyName == "Hoardings") {
    return tHoardingsIconFilePath;
  }
  else if (technologyName == "Iron Casting") {
    return tIronCastingIconFilePath;
  }
  else if (technologyName == "Leather Archer Armor") {
    return tLeatherArcherArmorIconFilePath;
  }
  else if (technologyName == "Loom") {
    return tLoomIconFilePath;
  }
  else if (technologyName == "Padded Archer Armor") {
    return tPaddedArcherArmorIconFilePath;
  }
  else if (technologyName == "Redemption {2E}") {
    return tRedemptionIconFilePath;
  }
  else if (technologyName == "Sanctity {2E}") {
    return tSanctityIconFilePath;
  }
  else if (technologyName == "Scale Barding Armor") {
    return tScaleBardingArmorIconFilePath;
  }
  else if (technologyName == "Scale Mail Armor") {
    return tScaleMailArmorIconFilePath;
  }
  else if (technologyName == "Bracer") {
    return tBracerIconFilePath;
  }
  else if (technologyName == "Plate Barding Armor") {
    return tPlateBardingArmorIconFilePath;
  }
  else if (technologyName == "Plate Mail Armor") {
    return tPlateMailArmorIconFilePath;
  }
  else if (technologyName == "Ring Archer Armor") {
    return tRingArcherArmorIconFilePath;
  }
  else {
    qDebug() << "Could not find: " << technologyName;
    Q_UNREACHABLE();
  }
}

void MainWindow::linkUnitNamesToFilePathsMap()
{
  unitNameToUnitFilePathMap.insert("Arambai", uArambaiIconFilePath);
  unitNameToUnitFilePathMap.insert("Arbalest", uArbalestIconFilePath);
  unitNameToUnitFilePathMap.insert("Archer", uArcherIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Armored Elephant", uArmoredElephantIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Ballista Elephant", uBallistaElephantIconFilePath);
  unitNameToUnitFilePathMap.insert("Battering Ram", uBatteringRamIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Battle Elephant", uBattleElephantIconFilePath);
  unitNameToUnitFilePathMap.insert("Berserk", uBerserkIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Bombard Cannon", uBombardCannonIconFilePath);
  unitNameToUnitFilePathMap.insert("Boyar", uBoyarIconFilePath);
  unitNameToUnitFilePathMap.insert("Camel Archer", uCamelArcherIconFilePath);
  unitNameToUnitFilePathMap.insert("Camel Rider", uCamelRiderIconFilePath);
  unitNameToUnitFilePathMap.insert("Camel Scout", uCamelScoutIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Cannon Galleon", uCannonGalleonIconFilePath);
  unitNameToUnitFilePathMap.insert("Capped Ram", uCappedRamIconFilePath);
  unitNameToUnitFilePathMap.insert("Caravel", uCaravelIconFilePath);
  unitNameToUnitFilePathMap.insert("Cataphract", uCataphractIconFilePath);
  unitNameToUnitFilePathMap.insert("Cavalier", uCavalierIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Cavalry Archer", uCavalryArcherIconFilePath);
  unitNameToUnitFilePathMap.insert("Centurion", uCenturionIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Chakram Thrower", uChakramThrowerIconFilePath);
  unitNameToUnitFilePathMap.insert("Champion", uChampionIconFilePath);
  unitNameToUnitFilePathMap.insert("Chu Ko Nu", uChuKoNuIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Composite Bowman", uCompositeBowmanIconFilePath);
  unitNameToUnitFilePathMap.insert("Condottiero", uCondottieroIconFilePath);
  unitNameToUnitFilePathMap.insert("Conquistador", uConquistadorIconFilePath);
  unitNameToUnitFilePathMap.insert("Coustillier", uCoustillierIconFilePath);
  unitNameToUnitFilePathMap.insert("Crossbowman", uCrossbowmanIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Demolition Raft", uDemolitionRaftIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Demolition Ship", uDemolitionShipIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Dismounted Konnik", uDismountedKonnikIconFilePath);
  unitNameToUnitFilePathMap.insert("Dromon", uDromonIconFilePath);
  unitNameToUnitFilePathMap.insert("Eagle Scout", uEagleScoutIconFilePath);
  unitNameToUnitFilePathMap.insert("Eagle Warrior", uEagleWarriorIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Elephant Archer", uElephantArcherIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Elite Battle Elephant", uEliteBattleElephantIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Elite Cannon Galleon", uEliteCannonGalleonIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Elite Eagle Warrior", uEliteEagleWarriorIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Elite Elephant Archer", uEliteElephantArcherIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Elite Skirmisher", uEliteSkirmisherIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Elite Steppe Lancer", uEliteSteppeLancerIconFilePath);
  unitNameToUnitFilePathMap.insert("Fast Fire Ship", uFastFireShipIconFilePath);
  unitNameToUnitFilePathMap.insert("Fire Galley", uFireGalleyIconFilePath);
  unitNameToUnitFilePathMap.insert("Fire Ship", uFireShipIconFilePath);
  unitNameToUnitFilePathMap.insert("Fishing Ship", uFishingShipIconFilePath);
  unitNameToUnitFilePathMap.insert("Flaming Camel", uFlamingCamelIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Flemish Militia", uFlemishMilitiaIconFilePath);
  unitNameToUnitFilePathMap.insert("Galleon", uGalleonIconFilePath);
  unitNameToUnitFilePathMap.insert("Galley", uGalleyIconFilePath);
  unitNameToUnitFilePathMap.insert("Gbeto", uGbetoIconFilePath);
  unitNameToUnitFilePathMap.insert("Genitour", uGenitourIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Genoese Crossbowman", uGenoeseCrossbowmanIconFilePath);
  unitNameToUnitFilePathMap.insert("Ghulam", uGhulamIconFilePath);
  unitNameToUnitFilePathMap.insert("Halbedier", uHalbedierIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Hand Cannoneer", uHandCannoneerIconFilePath);
  unitNameToUnitFilePathMap.insert("Heavy Camel", uHeavyCamelRiderIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Heavy Cavalry Archer", uHeavyCavalryArcherIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Heavy Demolition Ship", uHeavyDemolitionShipIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Heavy Scorpion", uHeavyScorpionIconFilePath);
  unitNameToUnitFilePathMap.insert("Houfnice", uHoufniceIconFilePath);
  unitNameToUnitFilePathMap.insert("Huskarl", uHuskarlIconFilePath);
  unitNameToUnitFilePathMap.insert("Hussar", uHussarIconFilePath);
  unitNameToUnitFilePathMap.insert("Hussite Wagon", uHussiteWagonIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Imperial Camel Rider", uImperialCamelRiderIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Imperial Skirmisher", uImperialSkirmisherIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Jaguar Warrior", uJaguarWarriorIconFilePath);
  unitNameToUnitFilePathMap.insert("Janissary", uJanissaryIconFilePath);
  unitNameToUnitFilePathMap.insert("Kamayuk", uKamayukIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Karambit Warrior", uKarambitWarriorIconFilePath);
  unitNameToUnitFilePathMap.insert("Keshik", uKeshikIconFilePath);
  unitNameToUnitFilePathMap.insert("Kipchak", uKipchakIconFilePath);
  unitNameToUnitFilePathMap.insert("Knight", uKnightIconFilePath);
  unitNameToUnitFilePathMap.insert("Konnik", uKonnikIconFilePath);
  unitNameToUnitFilePathMap.insert("Legionary", uLegionaryIconFilePath);
  unitNameToUnitFilePathMap.insert("Leitis", uLeitisIconFilePath);
  unitNameToUnitFilePathMap.insert("Light Cavalry", uLightCavalryIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Long Swordsman", uLongSwordsmanIconFilePath);
  unitNameToUnitFilePathMap.insert("Longboat", uLongboatIconFilePath);
  unitNameToUnitFilePathMap.insert("Longbowman", uLongbowmanIconFilePath);
  unitNameToUnitFilePathMap.insert("Magyar Huszar", uMagyarHuszarIconFilePath);
  unitNameToUnitFilePathMap.insert("Mameluke", uMamelukeIconFilePath);
  unitNameToUnitFilePathMap.insert("Man-at-Arms", uManAtArmsIconFilePath);
  unitNameToUnitFilePathMap.insert("Mangonel", uMangonelIconFilePath);
  unitNameToUnitFilePathMap.insert("Mangudai", uMangudaiIconFilePath);
  unitNameToUnitFilePathMap.insert("Militia", uMilitiaIconFilePath);
  unitNameToUnitFilePathMap.insert("Missionary", uMissionaryIconFilePath);
  unitNameToUnitFilePathMap.insert("Monaspa", uMonaspaIconFilePath);
  unitNameToUnitFilePathMap.insert("Monk", uMonkIconFilePath);
  unitNameToUnitFilePathMap.insert("Obuch", uObuchIconFilePath);
  unitNameToUnitFilePathMap.insert("Onager", uOnagerIconFilePath);
  unitNameToUnitFilePathMap.insert("Organ Gun", uOrganGunIconFilePath);
  unitNameToUnitFilePathMap.insert("Paladin", uPaladinIconFilePath);
  unitNameToUnitFilePathMap.insert("Petard", uPetardIconFilePath);
  unitNameToUnitFilePathMap.insert("Pikeman", uPikemanIconFilePath);
  unitNameToUnitFilePathMap.insert("Plumed Archer", uPlumedArcherIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Ratha Melee Mode", uRathaMeleeModeIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Ratha Ranged Mode", uRathaRangedModeIconFilePath);
  unitNameToUnitFilePathMap.insert("Rattan Archer", uRattanArcherIconFilePath);
  unitNameToUnitFilePathMap.insert("Samurai", uSamuraiIconFilePath);
  unitNameToUnitFilePathMap.insert("Savar", uSavarIconFilePath);
  unitNameToUnitFilePathMap.insert("Scorpion", uScorpionIconFilePath);
  unitNameToUnitFilePathMap.insert("Scout Cavalry", uScoutCavalryIconFilePath);
  unitNameToUnitFilePathMap.insert("Serjeant", uSerjeantIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Shotel Warrior", uShotelWarriorIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Shivamsha Rider", uShivamshaRiderIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Siege Elephant", uSiegeElephantIconFilePath);
  unitNameToUnitFilePathMap.insert("Siege Onager", uSiegeOnagerIconFilePath);
  unitNameToUnitFilePathMap.insert("Siege Ram", uSiegeRamIconFilePath);
  unitNameToUnitFilePathMap.insert("Siege Tower", uSiegeTowerIconFilePath);
  unitNameToUnitFilePathMap.insert("Skirmisher", uSkirmisherIconFilePath);
  unitNameToUnitFilePathMap.insert("Slinger", uSlingerIconFilePath);
  unitNameToUnitFilePathMap.insert("Spearman", uSpearmanIconFilePath);
  unitNameToUnitFilePathMap.insert("Steppe Lancer", uSteppeLancerIconFilePath);
  unitNameToUnitFilePathMap.insert("Tarkan", uTarkanIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Teutonic Knight", uTeutonicKnightIconFilePath);
  unitNameToUnitFilePathMap.insert("Thirisadai", uThirisadaiIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Throwing Axeman", uThrowingAxemanIconFilePath);
  unitNameToUnitFilePathMap.insert("Trade Cart", uTradeCartIconFilePath);
  unitNameToUnitFilePathMap.insert("Trade Cog", uTradeCogIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Transport Ship", uTransportShipIconFilePath);
  unitNameToUnitFilePathMap.insert("Trebuchet", uTrebuchetUnpackedIconFilePath);
  // unitNameToUnitFilePathMap.insert("Trebuchet Packed",
  // uTrebuchetPackedIconFilePath);
  unitNameToUnitFilePathMap.insert("Turtle Ship", uTurtleShipIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Two-handed Swordsman", uTwoHandedSwordsmanIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Urumi Swordsman", uUrumiSwordsmanIconFilePath);
  unitNameToUnitFilePathMap.insert("Villager", uFemaleVillagerIconFilePath);
  // unitNameToUnitFilePathMap.insert("Villager Male",
  // uMaleVillagerIconFilePath);
  // // @kory todo make this in sync with anim
  unitNameToUnitFilePathMap.insert("War Elephant", uWarElephantIconFilePath);
  unitNameToUnitFilePathMap.insert("War Galley", uWarGalleyIconFilePath);
  unitNameToUnitFilePathMap.insert("War Wagon", uWarWagonIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Warrior Priest", uWarriorPriestIconFilePath);
  unitNameToUnitFilePathMap.insert("Winged Hussar", uWingedHussarIconFilePath);
  unitNameToUnitFilePathMap.insert("Woad Raider", uWoadRaiderIconFilePath);
  unitNameToUnitFilePathMap.insert(
    "Xolotl Warrior", uXolotlWarriorIconFilePath);
}

QString MainWindow::returnUnitFilePathThatMatchesUnitName(QString unitName)
{
  QString unbracketedUnitName
    = unitName.remove(removeBracketedTextExpressionNoUnderscore);
  QString returnFilePath = "";

  if (unitNameToUnitFilePathMap.contains(unbracketedUnitName)) {
    returnFilePath = unitNameToUnitFilePathMap.value(unbracketedUnitName);
    return returnFilePath;
  }
  else { // Get the non-elite version of the unit if the elite version of it is
         // not found
    if (unbracketedUnitName.startsWith("Elite")) {
      QString unbracketedUnitNameWithEliteRemoved
        = unbracketedUnitName.remove(0, 6);

      if (unitNameToUnitFilePathMap.contains(
            unbracketedUnitNameWithEliteRemoved)) {
        returnFilePath = unitNameToUnitFilePathMap.value(
          unbracketedUnitNameWithEliteRemoved);
        return returnFilePath;
      }
      else {
        qDebug() << "Could not find: " << unitName;
        Q_UNREACHABLE();
      }
    }
    else {
      qDebug() << "Could not find: " << unitName;
      Q_UNREACHABLE();
    }
  }
}

QString MainWindow::returnBuildingFilePathThatMatchesBuildingName(
  QString buildingName)
{
  if (buildingName.startsWith("Archery Range", Qt::CaseSensitive)) {
    return bArcheryRangeIconFilePath;
  }
  else if (buildingName.startsWith("Barracks", Qt::CaseSensitive)) {
    return bBarracksIconFilePath;
  }
  else if (buildingName.startsWith("Blacksmith", Qt::CaseSensitive)) {
    return bBlacksmithIconFilePath;
  }
  else if (buildingName.startsWith("Bombard Tower", Qt::CaseSensitive)) {
    return bBombardTowerIconFilePath;
  }
  else if (buildingName.startsWith("Caravanserai", Qt::CaseSensitive)) {
    return bCaravanseraiIconFilePath;
  }
  else if (buildingName.startsWith("Castle", Qt::CaseSensitive)) {
    return bCastleIconFilePath;
  }
  else if (buildingName.startsWith("Dock", Qt::CaseSensitive)) {
    return bDockIconFilePath;
  }
  else if (buildingName.startsWith("Donjon", Qt::CaseSensitive)) {
    return bDonjonIconFilePath;
  }
  else if (buildingName.startsWith("Farm", Qt::CaseSensitive)) {
    return bFarmIconFilePath;
  }
  else if (buildingName.startsWith("Feitoria", Qt::CaseSensitive)) {
    return bFeitoriaIconFilePath;
  }
  else if (buildingName.startsWith("Fish Trap", Qt::CaseSensitive)) {
    return bFishTrapIconFilePath;
  }
  else if (buildingName.startsWith("Folwark", Qt::CaseSensitive)) {
    return bFolwarkIconFilePath;
  }
  else if (buildingName.startsWith("Fortified Church", Qt::CaseSensitive)) {
    return bFortifiedChurchIconFilePath;
  }
  else if (buildingName.startsWith("Guard Tower", Qt::CaseSensitive)) {
    return bGuardTowerIconFilePath;
  }
  else if (buildingName.startsWith("Harbor", Qt::CaseSensitive)) {
    return bHarborIconFilePath;
  }
  else if (buildingName.startsWith("House", Qt::CaseSensitive)) {
    return bHouseIconFilePath;
  }
  else if (buildingName.startsWith("Keep", Qt::CaseSensitive)) {
    return bKeepIconFilePath;
  }
  else if (buildingName.startsWith("Krepost", Qt::CaseSensitive)) {
    return bKrepostIconFilePath;
  }
  else if (buildingName.startsWith("Lumber Camp", Qt::CaseSensitive)) {
    return bLumberCampIconFilePath;
  }
  else if (buildingName.startsWith("Market", Qt::CaseSensitive)) {
    return bMarketIconFilePath;
  }
  else if (buildingName.startsWith("Mill", Qt::CaseSensitive)) {
    return bMillIconFilePath;
  }
  else if (
    buildingName.startsWith("Gold Mine", Qt::CaseSensitive)
    || buildingName.startsWith("Stone Mine", Qt::CaseSensitive)) {
    return bMiningCampIconFilePath;
  }
  else if (buildingName.startsWith("Monastery", Qt::CaseSensitive)) {
    return bMonasteryIconFilePath;
  }
  else if (buildingName.startsWith("Mule Cart", Qt::CaseSensitive)) {
    return bMuleCartIconFilePath;
  }
  else if (buildingName.startsWith("Outpost", Qt::CaseSensitive)) {
    return bOutpostIconFilePath;
  }
  else if (buildingName.startsWith("Palisade Gate", Qt::CaseSensitive)) {
    return bPalisadeGateIconFilePath;
  }
  else if (buildingName.startsWith("Palisade Wall", Qt::CaseSensitive)) {
    return bPalisadeWallIconFilePath;
  }
  else if (buildingName.startsWith("Siege Workshop", Qt::CaseSensitive)) {
    return bSiegeWorkshopIconFilePath;
  }
  else if (buildingName.startsWith("Stable", Qt::CaseSensitive)) {
    return bStableIconFilePath;
  }
  else if (
    buildingName.startsWith("Stone Gate", Qt::CaseSensitive)
    || buildingName.startsWith("Fortified Gate", Qt::CaseSensitive)) {
    return bStoneGateIconFilePath;
  }
  else if (
    buildingName.startsWith("Stone Wall", Qt::CaseSensitive)
    || buildingName.startsWith("Fortified Wall", Qt::CaseSensitive)) {
    return bStoneWallIconFilePath;
  }
  else if (buildingName.startsWith("Town Center", Qt::CaseSensitive)) {
    return bTownCenterIconFilePath;
  }
  else if (buildingName.startsWith("University", Qt::CaseSensitive)) {
    return bUniversityIconFilePath;
  }
  else if (buildingName.startsWith("Watch Tower", Qt::CaseSensitive)) {
    return bWatchTowerIconFilePath;
  }
  else if (
    buildingName == "Charlamagne's Palace At Aix La'Chapelle (Briton)"
    || buildingName == "Rock Of Cashel (Celt)"
    || buildingName == "The Golden Tent Of The Great Khan (Mongol)"
    || buildingName == "The Palace Of Ctesiphon On The Tigris (Persian)"
    || buildingName == "Tomb Of Theodoric (Goth)"
    || buildingName == "Notre-Dame Cathedral (Frank)"
    || buildingName == "Stave Church At Urnes (Viking)"
    || buildingName == "The Great Temple At Nara (Japanese)"
    || buildingName == "The Great Mosque at Samarra (Saracen)") {
    return bWonderIconFilePath;
  }
  else {
    qDebug() << "Could not find: " << buildingName;
    Q_UNREACHABLE();
  }
}

MainWindow::MainWindow(
  Database*              database,
  openOutputWindowClass* opOutWin,
  QWidget*               parent)
  : QMainWindow{parent}
  , m_database{database}
  , m_myOutputWindowClass{opOutWin}
  , m_lastLine{}
  , m_outputRedirector{std::cout, ui.gameOutputTextEdit, m_lastLine}
  , m_aliases{}
  , m_entities{}
  , m_player_names{}
  , m_player1Events{Player::Player1}
  , m_player2Events{Player::Player2}
  , m_player1Technologies{Player::Player1}
  , m_player2Technologies{Player::Player2}
{
  ui.setupUi(this);
  DialogInput::initialize(
    this, retreatingPromptAnswer, convertingHealingPromptAnswer);

  connect(
    ui.actionShow_database_window,
    &QAction::triggered,
    this,
    &MainWindow::onShowDatabaseWindowTriggered);
  connect(
    ui.actionSet_default_answer_to_attack_monks_prompt,
    &QAction::triggered,
    this,
    &MainWindow::onActionSetDefaultAnswerToAttackMonksPromptTriggered);

  QIntValidator myName;
  myName.setRange(100, 999);

  // create shortcut
  QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_R), this);

  // connect its 'activated' signal to the 'on_calculateResultsButton_clicked'
  // function
  QObject::connect(
    shortcut,
    &QShortcut::activated,
    this,
    &MainWindow::on_calculateResultsButton_clicked);

  // Populate this
  linkUnitNamesToFilePathsMap();

  // Indicate that there's a hotkey for this in the tooltip
  ui.calculateResultsButton->setToolTip("<b>Hotkey:</b> R");

  // What the civ bonuses are
  backFromAForeignLandCivilizationBonuses
    << tr("Byzantine bonus: Monk healing rate has a +2 modifier")
    << tr("Byzantine bonus: All buildings get a HP bonus of + 10 HP per Age")
    << tr("Teuton bonus: Conversion rate modifier is -1");

  // What the ages are
  ages << tr("Dark Age") << tr("Feudal Age") << tr("Castle Age")
       << tr("Imperial Age");

  // What the initial name of the players are
  player1Name = "Player 1";
  player2Name = "Player 2";

  // What the initial player color of the players are
  player1Color = "black";
  player2Color = "black";

  age1UnitNames = m_database->getUnitNames(1);
  age2UnitNames = m_database->getUnitNames(2);
  age3UnitNames = m_database->getUnitNames(3);
  age4UnitNames = m_database->getUnitNames(4);
  unitNames     = age1UnitNames + age2UnitNames + age3UnitNames + age4UnitNames;

  age1BuildingNames = m_database->getBuildingNames(1);
  age2BuildingNames = m_database->getBuildingNames(2);
  age3BuildingNames = m_database->getBuildingNames(3);
  age4BuildingNames = m_database->getBuildingNames(4);
  buildingNames     = age1BuildingNames + age2BuildingNames + age3BuildingNames
                  + age4BuildingNames;

  initializeEntityAliases();
  entityNames = unitNames + buildingNames;

  // Sort the list in alphabetical order
  buildingNames.sort();
  unitNames.sort();
  entityNames.sort();

  // What the possible names of technologies are
  // @Reference: What row in the .csv file it goes to
  QStringList technologies = m_database->getTechnologyNames();

  // Sort the list in alphabetical order
  technologies.sort();

  // What the possible names of event cards are
  // @Reference: What row in the .csv file it goes to
  QStringList events = m_database->getEventNames();

  // Sort the list in alphabetical order
  events.sort();

  // Populate the UI elements with elements
  // Both player 1 & 2 UI elements
  for (int b = 0; b < buildingNames.size(); b++) {
    QListWidgetItem* buildingPlayer1 = new QListWidgetItem(buildingNames[b]);
    QListWidgetItem* buildingPlayer2 = new QListWidgetItem(buildingNames[b]);

    QString buildingIconFileNamePlayer1
      = returnBuildingFilePathThatMatchesBuildingName(buildingPlayer1->text());

    QIcon buildingIconPlayer1(
      (workingDirectory.absolutePath() + buildingIconFileNamePlayer1));

    buildingPlayer1->setIcon(buildingIconPlayer1);

    QString buildingIconFileNamePlayer2
      = returnBuildingFilePathThatMatchesBuildingName(buildingPlayer2->text());

    QIcon buildingIconPlayer2(
      (workingDirectory.absolutePath() + buildingIconFileNamePlayer2));

    buildingPlayer2->setIcon(buildingIconPlayer2);

    ui.player1EntityNames->addItem(buildingPlayer1);
    ui.player2EntityNames->addItem(buildingPlayer2);
  }

  for (int u = 0; u < unitNames.size(); u++) {
    QListWidgetItem* unitPlayer1 = new QListWidgetItem(unitNames[u]);
    QListWidgetItem* unitPlayer2 = new QListWidgetItem(unitNames[u]);

    QString unitIconFileNamePlayer1
      = returnUnitFilePathThatMatchesUnitName(unitPlayer1->text());

    QIcon unitIconPlayer1(
      (workingDirectory.absolutePath() + unitIconFileNamePlayer1));

    unitPlayer1->setIcon(unitIconPlayer1);

    QString unitIconFileNamePlayer2
      = returnUnitFilePathThatMatchesUnitName(unitPlayer2->text());

    QIcon unitIconPlayer2(
      (workingDirectory.absolutePath() + unitIconFileNamePlayer2));

    unitPlayer2->setIcon(unitIconPlayer2);

    ui.player1EntityNames->addItem(unitPlayer1);
    ui.player2EntityNames->addItem(unitPlayer2);
  }

  // Sort the list in alphabetical order
  ui.player1EntityNames->sortItems(Qt::AscendingOrder);
  ui.player2EntityNames->sortItems(Qt::AscendingOrder);

  // Can only have one list widget item per list
  // C++11 range based for loop
  for (const QString& technology : technologies) {
    QListWidgetItem* technologyPlayer1 = new QListWidgetItem(technology);
    QListWidgetItem* technologyPlayer2 = new QListWidgetItem(technology);

    const QString technologyWithUnderscores{
      convertSpacesToUnderscores(technology)};

    technologyPlayer1->setData(
      Qt::CheckStateRole,
      m_player1Technologies.isActive(technologyWithUnderscores)
        ? Qt::Checked
        : Qt::Unchecked);

    technologyPlayer2->setData(
      Qt::CheckStateRole,
      m_player2Technologies.isActive(technologyWithUnderscores)
        ? Qt::Checked
        : Qt::Unchecked);

    QString technologyIconFileNamePlayer1
      = returnTechnologyFilePathThatMatchesTechnologyName(
        technologyPlayer1->text());

    QIcon technologyIconPlayer1(
      workingDirectory.absolutePath() + technologyIconFileNamePlayer1);

    technologyPlayer1->setIcon(technologyIconPlayer1);

    QString technologyIconFileNamePlayer2
      = returnTechnologyFilePathThatMatchesTechnologyName(
        technologyPlayer2->text());

    QIcon technologyIconPlayer2(
      (workingDirectory.absolutePath() + technologyIconFileNamePlayer2));

    technologyPlayer2->setIcon(technologyIconPlayer2);

    // Mark which ones correspond to the 2E
    if (technologyPlayer1->text().contains("{2E}")) {
      technologyPlayer1->setForeground(QColor(255, 255, 255));
      technologyPlayer1->setBackground(QColor(90, 90, 90));
    }

    if (technologyPlayer2->text().contains("{2E}")) {
      technologyPlayer2->setForeground(QColor(255, 255, 255));
      technologyPlayer2->setBackground(QColor(90, 90, 90));
    }

    ui.player1Technologies->addItem(technologyPlayer1);
    ui.player2Technologies->addItem(technologyPlayer2);
  }

  for (int eV = 0; eV < events.length(); eV++) {
    QListWidgetItem* eventPlayer1 = new QListWidgetItem(events[eV]);
    QListWidgetItem* eventPlayer2 = new QListWidgetItem(events[eV]);
    const QString    eventNameWithUnderscores
      = convertSpacesToUnderscores(events[eV]);

    if (eventNameWithUnderscores != "Back_From_A_Foreign_Land") {
      eventPlayer1->setData(
        Qt::CheckStateRole,
        m_player1Events.isActive(eventNameWithUnderscores) ? Qt::Checked
                                                           : Qt::Unchecked);
      eventPlayer2->setData(
        Qt::CheckStateRole,
        m_player2Events.isActive(eventNameWithUnderscores) ? Qt::Checked
                                                           : Qt::Unchecked);

      if (eventPlayer1->text().contains("{2E}")) {
        eventPlayer1->setForeground(QColor(255, 255, 255));
        eventPlayer1->setBackground(QColor(90, 90, 90));
      }

      if (eventPlayer2->text().contains("{2E}")) {
        eventPlayer2->setForeground(QColor(255, 255, 255));
        eventPlayer2->setBackground(QColor(90, 90, 90));
      }
    }
    else {
      if (
        (m_player1Events.isActive(
           "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier")
         == 1)
        || (m_player1Events.isActive("Back_From_A_Foreign_Land_Byz_HP_Bonus") == 1)
        || (m_player1Events.isActive("Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier") == 1)) {
        if (
          m_player1Events.isActive(
            "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier")
          == 1) {
          player1BackFromAForeignLandCivilizationBonusSelection
            = "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier";
        }
        else if (
          m_player1Events.isActive("Back_From_A_Foreign_Land_Byz_HP_Bonus")
          == 1) {
          player1BackFromAForeignLandCivilizationBonusSelection
            = "Back_From_A_Foreign_Land_Byz_HP_Bonus";
        }
        else if (
          m_player1Events.isActive(
            "Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier")
          == 1) {
          player1BackFromAForeignLandCivilizationBonusSelection
            = "Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier";
        }

        eventPlayer1->setData(Qt::CheckStateRole, Qt::Checked);

        isP1BackFromAForeignLandEventInPlay = true;
      }
      else {
        eventPlayer1->setData(Qt::CheckStateRole, Qt::Unchecked);

        isP1BackFromAForeignLandEventInPlay = false;
      }

      if (
        (m_player2Events.isActive(
           "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier")
         == 1)
        || (m_player2Events.isActive("Back_From_A_Foreign_Land_Byz_HP_Bonus") == 1)
        || (m_player2Events.isActive("Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier") == 1)) {
        if (
          m_player2Events.isActive(
            "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier")
          == 1) {
          player2BackFromAForeignLandCivilizationBonusSelection
            = "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier";
        }
        else if (
          m_player2Events.isActive("Back_From_A_Foreign_Land_Byz_HP_Bonus")
          == 1) {
          player2BackFromAForeignLandCivilizationBonusSelection
            = "Back_From_A_Foreign_Land_Byz_HP_Bonus";
        }
        else if (
          m_player2Events.isActive(
            "Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier")
          == 1) {
          player2BackFromAForeignLandCivilizationBonusSelection
            = "Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier";
        }

        eventPlayer2->setData(Qt::CheckStateRole, Qt::Checked);

        isP2BackFromAForeignLandEventInPlay = true;
      }
      else {
        eventPlayer2->setData(Qt::CheckStateRole, Qt::Unchecked);

        isP2BackFromAForeignLandEventInPlay = false;
      }
    }

    // Mark which ones I haven't implemented
    if (eventPlayer1->text().contains("(unimplemented)")) {
      eventPlayer1->setForeground(QColor(255, 0, 0));
    }

    if (eventPlayer2->text().contains("(unimplemented)")) {
      eventPlayer2->setForeground(QColor(255, 0, 0));
    }

    ui.player1Events->addItem(eventPlayer1);
    ui.player2Events->addItem(eventPlayer2);
  }

  QString monkIconFileName = returnUnitFilePathThatMatchesUnitName("Monk");

  QIcon monkIcon = QIcon(workingDirectory.absolutePath() + monkIconFileName);

  ui.player1BattleAssistantNames->addItem(monkIcon, "Monk", Qt::DisplayRole);
  ui.player2BattleAssistantNames->addItem(monkIcon, "Monk", Qt::DisplayRole);

  // These are like placeholder (lorem ipsum) values
  // Player 1 UI elements starting state
  ui.player1EntityNamesFilter->setText("");

  // Player 2 UI elements starting state
  ui.player2EntityNamesFilter->setText("");

  ui.player1EntityAssistantQuantity->setRange(0, 5);
  ui.player2EntityAssistantQuantity->setRange(0, 5);

  // Read in the .csv files and update the UI elements on the basis of this
  setInitialNames();
  selectInitialEntities();
  selectInitialAssistants();
  markInitialPlayerMedievalAge();
  initializeAnimations();

  // Set up palettes
  palettes.setPaletteValues();
  palettes.darkModeEnabled = false;

  setColorTheUIElements();

  // Filter list based on age player is in for both player 1 and 2
  filterBasedOnAgeAndCivilization("1");
  filterBasedOnAgeAndCivilization("2");
}

MainWindow::~MainWindow()
{
}

const QString& MainWindow::lastLine() const
{
  return m_lastLine;
}

// Run this when there's a call to play a button SFX
void SFXToPlay(QString filePath)
{
  // Play SFX if SFX is enabled
  if (soundEffectsEnabled == true) {
    playSound.fileLocation = workingDirectory.absolutePath() + filePath;
    playSound.playSoundEffect();
  }
  else {
    // Do nothing
  }
}

// Run this on click of the "Program" -> "Exit" button
void MainWindow::on_closeProgram_triggered()
{
  // Exit the program
  QApplication::quit();
}

// Run this on click of Help > About
void MainWindow::on_actionAbout_triggered()
{
  SFXToPlay(buttonPressedFilePath);

  // Open the about window
  aboutwindow aboutWindow;
  aboutWindow.setModal(true);
  aboutWindow.exec();
}

// Tooltip returner
QString MainWindow::tooltipReturner(QString name)
{
  QString tooltipForIt;

  if (name == "Charlamagne's Palace At Aix La'Chapelle (Briton)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Briton)";
  }
  else if (name == "Rock Of Cashel (Celt)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Celt)";
  }
  else if (name == "The Golden Tent Of The Great Khan (Mongol)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Mongol)";
  }
  else if (name == "The Palace Of Ctesiphon On The Tigris (Persian)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Persian)";
  }
  else if (name == "Tomb Of Theodoric (Goth)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Goth)";
  }
  else if (name == "Notre-Dame Cathedral (Frank)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Frank)";
  }
  else if (name == "Stave Church At Urnes (Viking)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Viking)";
  }
  else if (name == "The Great Temple At Nara (Japanese)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Japanese)";
  }
  else if (name == "The Great Mosque at Samarra (Saracen)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Saracen)";
  }
  /*
  else if (name == "Bombard Cannon"){
    tooltipForIt = "<b>Aliases:</b> BBC";
  }
  */
  else if (name == "Knight") {
    tooltipForIt = "<b>Aliases:</b> Kt";
  }
  else if (name == "Knight (Frank)") {
    tooltipForIt = "<b>Aliases:</b> Kt (Frank)";
  }
  else if (name == "Knight (Persian)") {
    tooltipForIt = "<b>Aliases:</b> Kt (Persian)";
  }
  else if (name == "Crossbowman") {
    tooltipForIt = "<b>Aliases:</b> Xbow";
  }
  else if (name == "Crossbowman (Saracen)") {
    tooltipForIt = "<b>Aliases:</b> Xbow (Saracen)";
  }
  else if (name == "Siege Onager") {
    tooltipForIt = "<b>Aliases:</b> SO";
  }
  else if (name == "Siege Onager (Celt)") {
    tooltipForIt = "<b>Aliases:</b> SO (Celt)";
  }
  /*
  else if (name == "Bombard Tower"){
    tooltipForIt = "<b>Aliases:</b> BBT";
  }
  */
  else if (name == "Town Center") {
    tooltipForIt = "<b>Aliases:</b> TC";
  }
  else if (name == "Town Center (Briton)") {
    tooltipForIt = "<b>Aliases:</b> TC (Briton)";
  }
  else if (name == "Town Center (Persian)") {
    tooltipForIt = "<b>Aliases:</b> TC (Persian)";
  }
  else {
    tooltipForIt = "";
  }

  return tooltipForIt;
}

// Run this when the text inside of the player 1 entities search field changes
void MainWindow::on_player1EntityNamesFilter_textChanged(
  const QString& textInsideOfElement)
{
  filterEntityNames(ui.player1EntityNames, textInsideOfElement, "1");
}

// Run this when the text inside of the player 2 entities search field changes
void MainWindow::on_player2EntityNamesFilter_textChanged(
  const QString& textInsideOfElement)
{
  filterEntityNames(ui.player2EntityNames, textInsideOfElement, "2");
}

// Run this on click of Help > Documentation > Developer guide
void MainWindow::on_actionDeveloper_guide_triggered()
{
  SFXToPlay(buttonPressedFilePath);

  // Set the path to it
  QString filePath
    = workingDirectory.absolutePath() + developerGuideDocumentFilePath;

  PdfWindow pdfWindow{filePath};
  pdfWindow.exec();
}

// Run this on click of Help > Documentation > Test cases
void MainWindow::on_actionDeveloper_test_cases_triggered()
{
  SFXToPlay(buttonPressedFilePath);

  // Set the path to it
  QString filePath
    = workingDirectory.absolutePath() + developerTestCasesDocumentFilePath;

  PdfWindow pdfWindow{filePath};
  pdfWindow.exec();
}

// Run this on click of Help > Documentation > Developer wishlist
void MainWindow::on_actionDeveloper_wishlist_triggered()
{
  SFXToPlay(buttonPressedFilePath);

  // Set the path to it
  QString filePath
    = workingDirectory.absolutePath() + developerWishlistDocumentFilePath;

  PdfWindow pdfWindow{filePath};
  pdfWindow.exec();
}

// Run this on click of Help > Documentation > User guide
void MainWindow::on_actionUser_guide_triggered()
{
  SFXToPlay(buttonPressedFilePath);

  // Set the path to it
  QString filePath
    = workingDirectory.absolutePath() + userGuideDocumentFilePath;

  PdfWindow pdfWindow{filePath};
  pdfWindow.exec();
}

// Run on click of the calculate results button
void MainWindow::on_calculateResultsButton_clicked()
{
  SFXToPlay(buttonPressedFilePath);

  ui.gameOutputTextEdit->setHtml("");

  // Update animation
  getEntityAnimationForSelectedEntity(
    ui.player1EntityNames->currentItem()->text(), "1", "_attack");
  getEntityAnimationForSelectedEntity(
    ui.player2EntityNames->currentItem()->text(), "2", "_attack");
  getAssistantEntityAnimationForSelectedAssistant(
    ui.player1BattleAssistantNames->currentText(), "1", "_attack");
  getAssistantEntityAnimationForSelectedAssistant(
    ui.player1BattleAssistantNames->currentText(), "2", "_attack");


  // Calculate the results of a battle
  runGame(
    m_database,
    [this](Player player, bool didAnAsssitantDie) {
      onPlayerEntityDeath(player, didAnAsssitantDie);
    },
    m_distanceBetweenTheBattleParticipants,
    EntityOutputConfig{
      m_showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput,
      m_showFurtherInformationAboutTheBattleParticipantsInTheOutput},
    monkAttackingPromptAnswer,
    PlayerSelectionMemory{
      m_p1VillagerMemory, m_p2VillagerMemory, m_p1FarmMemory, m_p2FarmMemory},
    workingDirectory.absolutePath().toStdString() + "/");

  updateDetatchedOutputWindow();
}

// Run this when the value inside of the player 1 entity quantities field
// changes
void MainWindow::on_player1EntityQuantity_valueChanged(int valueInsideOfField)
{
  // Get what entity quantity the user is entering
  player1EntityQuantity = valueInsideOfField;

  // Give an error to Console if quantity isn't right
  if (ui.player1EntityNames->currentItem()->text() != "Villager") {
    if (
      player1EntityQuantity != 1 && player1EntityQuantity != 2
      && player1EntityQuantity != 3 && player1EntityQuantity != 4
      && player1EntityQuantity != 5) {
      qDebug()
        << "Error: Player 1's entity quantity input should be between 1 and 5";
      std::terminate();
    }
  }
  else {
    if (player1EntityQuantity <= 0) {
      qDebug() << "Error: Player 1's entity quantity input should be > 0";
      std::terminate();
    }
  }

  m_entities.changePlayer1EntityQuantity(player1EntityQuantity);
}

// Run this when the value inside of the player 2 entity quantities field
// changes
void MainWindow::on_player2EntityQuantity_valueChanged(int valueInsideOfField)
{
  // Get what entity quantity the user is entering
  player2EntityQuantity = valueInsideOfField;

  // Give an error to Console if quantity isn't right
  if (ui.player2EntityNames->currentItem()->text() != "Villager") {
    // Give an error to Console if quantity isn't right
    if (
      player2EntityQuantity != 1 && player2EntityQuantity != 2
      && player2EntityQuantity != 3 && player2EntityQuantity != 4
      && player2EntityQuantity != 5) {
      qDebug()
        << "Error: Player 2's entity quantity input should be between 1 and 5";
    }
  }
  else {
    if (player2EntityQuantity <= 0) {
      qDebug() << "Error: Player 2's entity quantity input should be > 0";
      std::terminate();
    }
  }

  m_entities.changePlayer2EntityQuantity(player2EntityQuantity);
}

// Run on change of what battle assistant is selected by player 1
void MainWindow::on_player1BattleAssistantNames_textActivated(
  const QString& currentSelection)
{
  SFXToPlay(buttonPressedFilePath);

  player1BattleAssistantName = currentSelection;

  m_entities.changePlayer1AssistantName(player1BattleAssistantName);

  getAssistantEntityAnimationForSelectedAssistant(
    currentSelection, "1", "_idle");
}

// Run on change of what battle assistant is selected by player 2
void MainWindow::on_player2BattleAssistantNames_textActivated(
  const QString& currentSelection)
{
  SFXToPlay(buttonPressedFilePath);

  player2BattleAssistantName = currentSelection;

  m_entities.changePlayer2AssistantName(player2BattleAssistantName);

  getAssistantEntityAnimationForSelectedAssistant(
    currentSelection, "2", "_idle");
}

void MainWindow::on_player1EntityAssistantQuantity_valueChanged(
  int valueInsideOfField)
{
  player1AssistingEntityQuantity = valueInsideOfField;

  // Give an error to Console if quantity isn't right
  if (
    player1AssistingEntityQuantity != 0 && player1AssistingEntityQuantity != 1
    && player1AssistingEntityQuantity != 2
    && player1AssistingEntityQuantity != 3
    && player1AssistingEntityQuantity != 4
    && player1AssistingEntityQuantity != 5) {
    qDebug() << "Error: Player 1's assisting entity quantity input should be "
                "between 0 and 5";
  }

  m_entities.changePlayer1AssistantQuantity(player1AssistingEntityQuantity);
}

void MainWindow::on_player2EntityAssistantQuantity_valueChanged(
  int valueInsideOfField)
{
  player2AssistingEntityQuantity = valueInsideOfField;

  // Give an error to Console if quantity isn't right
  if (
    player2AssistingEntityQuantity != 0 && player2AssistingEntityQuantity != 1
    && player2AssistingEntityQuantity != 2
    && player2AssistingEntityQuantity != 3
    && player2AssistingEntityQuantity != 4
    && player2AssistingEntityQuantity != 5) {
    qDebug() << "Error: Player 2's assisting entity quantity input should be "
                "between 0 and 5";
  }

  m_entities.changePlayer2AssistantQuantity(player2AssistingEntityQuantity);
}

QListWidgetItem* MainWindow::findByEntityName(
  QListWidget* haystack,
  QString      needle) const
{
  needle = convertUnderscoresToSpaces(needle);
  const QList<QListWidgetItem*> foundItems{
    haystack->findItems(needle, Qt::MatchFixedString)};

  if (foundItems.empty()) {
    return nullptr;
  }

  return foundItems.front();
}

static bool isUnitWithOneToken(const QString& entity)
{
  return (entity.contains("RAM")) || (entity.contains("DEMOLITION_SHIP"))
         || (entity.contains("FIRE_SHIP")) || (entity.contains("FISHING_SHIP"))
         || (entity.contains("GALLEY")) || (entity.contains("HERO"))
         || (entity.contains("GALLEON"))
         || (entity.contains("LONGBOAT_(VIKING)"))
         || (entity.contains("MANGONEL")) || (entity.contains("ONAGER"))
         || (entity.contains("SCORPION")) || (entity.contains("TREBUCHET"))
         || (entity.contains("WAR_ELEPHANT_(PERSIAN)"));
}

void MainWindow::updateRangeAllowed(QString nameOfSelection, int playerNumber)
{
  nameOfSelection = nameOfSelection.toUpper();
  nameOfSelection = convertSpacesToUnderscores(nameOfSelection);

  QSpinBox* const entityQuantitySpinBox
    = playerNumber == 1 ? ui.player1EntityQuantity : ui.player2EntityQuantity;
  QSpinBox* const assistantQuantitySpinBox
    = playerNumber == 1 ? ui.player1EntityAssistantQuantity
                        : ui.player2EntityAssistantQuantity;

  const bool isSingleEntity
    = isBuilding(nameOfSelection) || isUnitWithOneToken(nameOfSelection);

  if (isSingleEntity) {
    entityQuantitySpinBox->setRange(1, 1);
    assistantQuantitySpinBox->setRange(0, 0);
  }
  else {
    if (nameOfSelection.contains("VILLAGER")) {
      entityQuantitySpinBox->setRange(1, 500);
    }
    else {
      entityQuantitySpinBox->setRange(1, 5);
    }

    assistantQuantitySpinBox->setRange(0, 5);
  }
}

// Run on change of what battle participant is selected by player 1
void MainWindow::on_player1EntityNames_itemClicked(
  QListWidgetItem* selectedItem)
{
  SFXToPlay(buttonPressedFilePath);

  updateRangeAllowed(selectedItem->text(), 1);

  QString currentSelectionFormatted
    = convertSpacesToUnderscores(selectedItem->text());
  currentSelectionFormatted = currentSelectionFormatted.toUpper();
  m_player1EntityName       = currentSelectionFormatted;

  m_entities.changePlayer1EntityName(m_player1EntityName);

  getEntityAnimationForSelectedEntity(selectedItem->text(), "1", "_idle");

  checkIfEitherPlayerHasBombardmentEntity();
}

void MainWindow::on_player2EntityNames_itemClicked(
  QListWidgetItem* selectedItem)
{
  SFXToPlay(buttonPressedFilePath);

  updateRangeAllowed(selectedItem->text(), 2);

  QString currentSelectionFormatted
    = convertSpacesToUnderscores(selectedItem->text());
  currentSelectionFormatted = currentSelectionFormatted.toUpper();
  m_player2EntityName       = currentSelectionFormatted;

  m_entities.changePlayer2EntityName(m_player2EntityName);

  getEntityAnimationForSelectedEntity(selectedItem->text(), "2", "_idle");

  checkIfEitherPlayerHasBombardmentEntity();
}

// Input validation. Superior technologies take the place of lesser technologies
void MainWindow::overrideTechnologies(
  QStringList   technologiesToCancelOut,
  Technologies* playerTechnologies)
{
  for (int i = 0; i < technologiesToCancelOut.size(); i++) {
    QString technologyNameBackend
      = convertSpacesToUnderscores(technologiesToCancelOut[i]);

    // Disable it in file
    if (playerTechnologies->isActive(technologyNameBackend)) {
      playerTechnologies->disable(technologyNameBackend);
    }

    // Disable it in the GUI
    QList<QListWidgetItem*> list = ui.player1Technologies->findItems(
      technologiesToCancelOut[i], Qt::MatchExactly);
    for (QListWidgetItem* item : list) {
      item->setCheckState(Qt::Unchecked);
    }
  }
}

// Should have the listCombinations (as input) in order of what overrides what
void MainWindow::technologyOverrider(
  QStringList   listCombinations,
  Technologies* playerTechnologies)
{
  QStringList combinationsFound;

  for (int i = 0; i < listCombinations.size(); i++) {
    if (playerTechnologies->isActive(
          convertSpacesToUnderscores(listCombinations[i]))) {
      // Get all other combinations
      for (int y = 0; y < listCombinations.size(); y++) {
        if (listCombinations[i] != listCombinations[y]) {
          combinationsFound.append(listCombinations[y]);
        }
      }

      overrideTechnologies(combinationsFound, playerTechnologies);
    }
  }
}

// Run on change of what technologies are toggled by player 1
void MainWindow::on_player1Technologies_itemChanged(
  QListWidgetItem* checkedItem)
{
  SFXToPlay(togglePressedFilePath);

  QString technology = checkedItem->text();
  technology         = convertSpacesToUnderscores(technology);

  if (checkedItem->checkState() == Qt::Checked) {
    m_player1Technologies.enable(technology);
  }
  else {
    m_player1Technologies.disable(technology);
  }

  /* Input validation - Up to 1 selected */
  technologyOverrider(
    {"Bracer", "Bodkin Arrow", "Fletching"}, &m_player1Technologies);
  technologyOverrider(
    {"Blast Furnace", "Iron Casting", "Forging"}, &m_player1Technologies);
  technologyOverrider(
    {"Plate Barding Armor", "Chain Barding Armor", "Scale Barding Armor"},
    &m_player1Technologies);
  technologyOverrider(
    {"Ring Archer Armor", "Leather Archer Armor", "Padded Archer Armor"},
    &m_player1Technologies);
}

// Run on change of what events are toggled by player 1
void MainWindow::on_player1Events_itemChanged(QListWidgetItem* checkedItem)
{
  SFXToPlay(togglePressedFilePath);

  QString event = checkedItem->text();
  event         = convertSpacesToUnderscores(event);

  if (
    (m_player1Events.isActive(
       "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier")
     == 1)
    || (m_player1Events.isActive("Back_From_A_Foreign_Land_Byz_HP_Bonus") == 1)
    || (m_player1Events.isActive("Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier") == 1)) {
    isP1BackFromAForeignLandEventInPlay = true;
  }
  else {
    isP1BackFromAForeignLandEventInPlay = false;
  }

  if (event == "Back_From_A_Foreign_Land") {
    if (isP1BackFromAForeignLandEventInPlay == false) {
      QInputDialog backFromAForeignLandEventDialog;
      QLabel       civilizationBonusSelectedLabel(
        palettes.getDialogBoxTextTags("Selected civilization bonus:"));
      backFromAForeignLandEventDialog.setLabelText(
        civilizationBonusSelectedLabel.text());
      backFromAForeignLandEventDialog.setInputMode(QInputDialog::TextInput);
      backFromAForeignLandEventDialog.setWindowTitle(
        "\"Back From A Foreign Land\" event card");
      backFromAForeignLandEventDialog.setStyleSheet(
        palettes.getDialogBoxStyling());
      backFromAForeignLandEventDialog.setComboBoxItems(
        backFromAForeignLandCivilizationBonuses);
      backFromAForeignLandEventDialog.exec();

      player1BackFromAForeignLandCivilizationBonusSelection
        = backFromAForeignLandEventDialog.textValue();
    }

    if (
      player1BackFromAForeignLandCivilizationBonusSelection
      == "Byzantine bonus: Monk healing rate has a +2 modifier") {
      event = "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier";
    }
    else if (
      player1BackFromAForeignLandCivilizationBonusSelection
      == "Byzantine bonus: All buildings get a HP bonus of + 10 HP per Age") {
      event = "Back_From_A_Foreign_Land_Byz_HP_Bonus";
    }
    else if (
      player1BackFromAForeignLandCivilizationBonusSelection
      == "Teuton bonus: Conversion rate modifier is -1") {
      event = "Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier";
    }
    else {
      event = player1BackFromAForeignLandCivilizationBonusSelection;
    }
  }

  if (checkedItem->checkState() == Qt::Checked) {
    m_player1Events.enable(event);
  }
  else {
    m_player1Events.disable(event);
  }
}

void MainWindow::on_player2Technologies_itemChanged(
  QListWidgetItem* checkedItem)
{
  SFXToPlay(togglePressedFilePath);

  QString technology = checkedItem->text();
  technology         = convertSpacesToUnderscores(technology);

  if (checkedItem->checkState() == Qt::Checked) {
    m_player2Technologies.enable(technology);
  }
  else {
    m_player2Technologies.disable(technology);
  }

  /* Input validation - Up to 1 selected */
  technologyOverrider(
    {"Bracer", "Bodkin Arrow", "Fletching"}, &m_player2Technologies);
  technologyOverrider(
    {"Blast Furnace", "Iron Casting", "Forging"}, &m_player2Technologies);
  technologyOverrider(
    {"Plate Barding Armor", "Chain Barding Armor", "Scale Barding Armor"},
    &m_player2Technologies);
  technologyOverrider(
    {"Ring Archer Armor", "Leather Archer Armor", "Padded Archer Armor"},
    &m_player2Technologies);
}

void MainWindow::on_player2Events_itemChanged(QListWidgetItem* checkedItem)
{
  SFXToPlay(togglePressedFilePath);

  QString event = checkedItem->text();
  event         = convertSpacesToUnderscores(event);

  if (
    (m_player2Events.isActive(
       "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier")
     == 1)
    || (m_player2Events.isActive("Back_From_A_Foreign_Land_Byz_HP_Bonus") == 1)
    || (m_player2Events.isActive("Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier") == 1)) {
    isP2BackFromAForeignLandEventInPlay = true;
  }
  else {
    isP2BackFromAForeignLandEventInPlay = false;
  }

  if (event == "Back_From_A_Foreign_Land") {
    if (isP2BackFromAForeignLandEventInPlay == false) {
      QInputDialog backFromAForeignLandEventDialog;
      QLabel       civilizationBonusSelectedLabel(
        palettes.getDialogBoxTextTags("Selected civilization bonus:"));
      backFromAForeignLandEventDialog.setLabelText(
        civilizationBonusSelectedLabel.text());
      backFromAForeignLandEventDialog.setInputMode(QInputDialog::TextInput);
      backFromAForeignLandEventDialog.setWindowTitle(
        "\"Back From A Foreign Land\" event card");
      backFromAForeignLandEventDialog.setStyleSheet(
        palettes.getDialogBoxStyling());
      backFromAForeignLandEventDialog.setComboBoxItems(
        backFromAForeignLandCivilizationBonuses);
      backFromAForeignLandEventDialog.exec();

      player2BackFromAForeignLandCivilizationBonusSelection
        = backFromAForeignLandEventDialog.textValue();
    }

    if (
      player2BackFromAForeignLandCivilizationBonusSelection
      == "Byzantine bonus: Monk healing rate has a +2 modifier") {
      event = "Back_From_A_Foreign_Land_Byz_Healing_Rate_Modifier";
    }
    else if (
      player2BackFromAForeignLandCivilizationBonusSelection
      == "Byzantine bonus: All buildings get a HP bonus of + 10 HP per Age") {
      event = "Back_From_A_Foreign_Land_Byz_HP_Bonus";
    }
    else if (
      player2BackFromAForeignLandCivilizationBonusSelection
      == "Teuton bonus: Conversion rate modifier is -1") {
      event = "Back_From_A_Foreign_Land_Teuton_Conversion_Rate_Modifier";
    }
    else {
      event = player2BackFromAForeignLandCivilizationBonusSelection;
    }
  }

  if (checkedItem->checkState() == Qt::Checked) {
    m_player2Events.enable(event);
  }
  else {
    m_player2Events.disable(event);
  }
}

void MainWindow::setColorTheToggleElements()
{
  if (palettes.darkModeEnabled == true) {
    // Have to do icons for the checkable items
    // No way to change color of actionEnableDisableDarkMode checkable via css
    // ui.menuOptions->setStyleSheet("QMenu::item:checked {color: rgb(255, 0,
    // 0);}"); Changes color of label but not of the checkbox
    // ui.actionEnableDisableDarkMode->setFont(); \\ Has no color option
    // Also have to remove checkable option as it interferes with the icons
    ui.actionEnableDisableDarkMode->setIcon(
      QIcon(workingDirectory.absolutePath() + checkedIconInvertedFilePath));
    ui.actionEnableDisableDarkMode->setIconVisibleInMenu(true);

    if (
      m_showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput
      == true) {
      ui.actionShow_the_total_instead_of_individual_values_of_battle_participants_in_the_output
        ->setIcon(
          QIcon(workingDirectory.absolutePath() + checkedIconInvertedFilePath));
      ui.actionShow_the_total_instead_of_individual_values_of_battle_participants_in_the_output
        ->setIconVisibleInMenu(true);
    }
    else {
      ui.actionShow_the_total_instead_of_individual_values_of_battle_participants_in_the_output
        ->setIconVisibleInMenu(false);
    }

    if (m_showFurtherInformationAboutTheBattleParticipantsInTheOutput == true) {
      ui.actionShow_further_information_about_the_battle_participants_in_the_output
        ->setIcon(
          QIcon(workingDirectory.absolutePath() + checkedIconInvertedFilePath));
      ui.actionShow_further_information_about_the_battle_participants_in_the_output
        ->setIconVisibleInMenu(true);
    }
    else {
      ui.actionShow_further_information_about_the_battle_participants_in_the_output
        ->setIconVisibleInMenu(false);
    }

    if (soundEffectsEnabled == false) {
      ui.actionEnableDisableSFX->setIcon(
        QIcon(workingDirectory.absolutePath() + checkedIconInvertedFilePath));
      ui.actionEnableDisableSFX->setIconVisibleInMenu(true);
    }
    else {
      ui.actionEnableDisableSFX->setIconVisibleInMenu(false);
    }
  }
  else if (palettes.darkModeEnabled == false) {
    ui.actionEnableDisableDarkMode->setIconVisibleInMenu(false);

    if (
      m_showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput
      == true) {
      ui.actionShow_the_total_instead_of_individual_values_of_battle_participants_in_the_output
        ->setIcon(QIcon(workingDirectory.absolutePath() + checkedIconFilePath));
      ui.actionShow_the_total_instead_of_individual_values_of_battle_participants_in_the_output
        ->setIconVisibleInMenu(true);
    }
    else {
      ui.actionShow_the_total_instead_of_individual_values_of_battle_participants_in_the_output
        ->setIconVisibleInMenu(false);
    }

    if (m_showFurtherInformationAboutTheBattleParticipantsInTheOutput == true) {
      ui.actionShow_further_information_about_the_battle_participants_in_the_output
        ->setIcon(QIcon(workingDirectory.absolutePath() + checkedIconFilePath));
      ui.actionShow_further_information_about_the_battle_participants_in_the_output
        ->setIconVisibleInMenu(true);
    }
    else {
      ui.actionShow_further_information_about_the_battle_participants_in_the_output
        ->setIconVisibleInMenu(false);
    }

    if (soundEffectsEnabled == false) {
      ui.actionEnableDisableSFX->setIcon(
        QIcon(workingDirectory.absolutePath() + checkedIconFilePath));
      ui.actionEnableDisableSFX->setIconVisibleInMenu(true);
    }
    else {
      ui.actionEnableDisableSFX->setIconVisibleInMenu(false);
    }
  }
}

void MainWindow::setColorTheUIElements()
{
  setColorTheToggleElements();

  if (palettes.darkModeEnabled == true) {
    selectedPalette = palettes.darkPalette;

    // Do the icons
    ui.closeProgram->setIcon(QIcon(
      workingDirectory.absolutePath() + closeProgramIconInvertedFilePath));

    ui.menuOptions->setIcon(QIcon(
      workingDirectory.absolutePath() + programOptionsIconInvertedFilePath));

    ui.menuDocumentation->setIcon(QIcon(
      workingDirectory.absolutePath() + documentationInvertedIconFilePath));

    ui.actionUser_guide->setIcon(QIcon(
      workingDirectory.absolutePath() + documentationInvertedIconFilePath));
    ui.actionDeveloper_guide->setIcon(QIcon(
      workingDirectory.absolutePath() + documentationInvertedIconFilePath));
    ui.actionDeveloper_wishlist->setIcon(QIcon(
      workingDirectory.absolutePath() + documentationInvertedIconFilePath));
    ui.actionDeveloper_test_cases->setIcon(QIcon(
      workingDirectory.absolutePath() + documentationInvertedIconFilePath));

    ui.actionAbout->setIcon(
      QIcon(workingDirectory.absolutePath() + aboutInvertedIconFilePath));

    ui.actionShow_database_window->setIcon(
      QIcon(workingDirectory.absolutePath() + databaseIconInvertedFilePath));

    ui.actionSet_player_1_Age->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));
    ui.actionSet_name_of_player_1->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));
    ui.actionSet_set_color_of_player_1->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));
    ui.actionSet_player_2_Age->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));
    ui.actionSet_name_of_player_2->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));
    ui.actionSet_set_color_of_player_2->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));

    ui.actionSet_civilization_of_player_1->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));
    ui.actionSet_civilization_of_player_2->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilePath));

    // Update the player names
    if (player1Color == "black" || player2Color == "black") {
      player1Color = "white";
      player2Color = "white";

      updatePlayerNames();
    }
  }
  else {
    selectedPalette = palettes.lightPalette;

    // Do the icons

    ui.closeProgram->setIcon(
      QIcon(workingDirectory.absolutePath() + closeProgramIconFilePath));
    ui.menuOptions->setIcon(
      QIcon(workingDirectory.absolutePath() + programOptionsIconFilePath));

    ui.menuDocumentation->setIcon(
      QIcon(workingDirectory.absolutePath() + documentationIconFilePath));

    ui.actionUser_guide->setIcon(
      QIcon(workingDirectory.absolutePath() + documentationIconFilePath));
    ui.actionDeveloper_guide->setIcon(
      QIcon(workingDirectory.absolutePath() + documentationIconFilePath));
    ui.actionDeveloper_wishlist->setIcon(
      QIcon(workingDirectory.absolutePath() + documentationIconFilePath));
    ui.actionDeveloper_test_cases->setIcon(
      QIcon(workingDirectory.absolutePath() + documentationIconFilePath));

    ui.actionAbout->setIcon(
      QIcon(workingDirectory.absolutePath() + aboutIconFilePath));

    ui.actionShow_database_window->setIcon(
      QIcon(workingDirectory.absolutePath() + databaseIconFilePath));

    ui.actionSet_player_1_Age->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));
    ui.actionSet_name_of_player_1->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));
    ui.actionSet_set_color_of_player_1->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));
    ui.actionSet_player_2_Age->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));
    ui.actionSet_name_of_player_2->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));
    ui.actionSet_set_color_of_player_2->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));

    ui.actionSet_civilization_of_player_1->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));
    ui.actionSet_civilization_of_player_2->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilePath));

    // Update the player names
    if (player1Color == "white" || player2Color == "white") {
      player1Color = "black";
      player2Color = "black";

      updatePlayerNames();
    }
  }

  // Do the colors

  ui.player1UnitsLabel->setPalette(selectedPalette);
  ui.player2UnitsLabel->setPalette(selectedPalette);

  ui.player1EntityNamesFilter->setPalette(selectedPalette);
  ui.player2EntityNamesFilter->setPalette(selectedPalette);

  ui.player1EntityNames->setPalette(selectedPalette);
  ui.player2EntityNames->setPalette(selectedPalette);

  ui.player1EntityQuantityLabel->setPalette(selectedPalette);
  ui.player2EntityQuantityLabel->setPalette(selectedPalette);

  ui.player1EntityQuantity->setPalette(selectedPalette);
  ui.player2EntityQuantity->setPalette(selectedPalette);

  ui.player1AssistingUnitsLabel->setPalette(selectedPalette);
  ui.player2AssistingUnitsLabel->setPalette(selectedPalette);

  ui.player1BattleAssistantNames->setStyleSheet(
    palettes.getSpinBoxStyling()); // Used for background color
  ui.player2BattleAssistantNames->setStyleSheet(
    palettes.getSpinBoxStyling()); // Used for background color

  ui.player1AssistingUnitsQuantityLabel->setPalette(selectedPalette);
  ui.player2AssistingUnitsQuantityLabel->setPalette(selectedPalette);

  ui.player1EntityAssistantQuantity->setPalette(selectedPalette);
  ui.player2EntityAssistantQuantity->setPalette(selectedPalette);

  ui.player1TechnologiesLabel->setPalette(selectedPalette);
  ui.player2TechnologiesLabel->setPalette(selectedPalette);

  ui.player1Technologies->setPalette(selectedPalette);
  ui.player2Technologies->setPalette(selectedPalette);

  ui.player1EventsLabel->setPalette(selectedPalette);
  ui.player2EventsLabel->setPalette(selectedPalette);

  ui.gameOutputLabel->setPalette(selectedPalette);
  ui.gameOutputTextEdit->setPalette(selectedPalette);

  ui.menubar->setStyleSheet(
    palettes.getMenuBarStyling());         // Used for background color
  ui.menubar->setPalette(selectedPalette); // For the text color

  ui.menuFile->setPalette(selectedPalette);
  ui.menuOptions->setPalette(selectedPalette);
  ui.menuPlayer_details->setPalette(selectedPalette);
  ui.menuDocumentation->setPalette(selectedPalette);

  ui.calculateResultsButton->setStyleSheet(
    palettes.getButtonBackgroundColor()); // Used for background color
  ui.calculateResultsButton->setPalette(selectedPalette); // For the text color

  this->setPalette(selectedPalette); // sets color of main window
}

// Run this when there's a call to update the names and colors of the players
void MainWindow::updatePlayerNames()
{
  // no underscores in UI
  player1Name = convertUnderscoresToSpaces(player1Name);
  player2Name = convertUnderscoresToSpaces(player2Name);

  ui.actionSet_player_1_Age->setText("Set " + player1Name + "'s medieval age");
  ui.actionSet_name_of_player_1->setText("Set " + player1Name + "'s name");
  ui.actionSet_set_color_of_player_1->setText(
    "Set " + player1Name + "'s color");
  ui.actionSet_player_2_Age->setText("Set " + player2Name + "'s medieval age");
  ui.actionSet_name_of_player_2->setText("Set " + player2Name + "'s name");
  ui.actionSet_set_color_of_player_2->setText(
    "Set " + player2Name + "'s color");

  ui.actionSet_civilization_of_player_1->setText(
    "Set " + player1Name + "'s civilization");
  ui.actionSet_civilization_of_player_2->setText(
    "Set " + player2Name + "'s civilization");

  ui.player1UnitsLabel->setText(
    "<font color=" + player1Color + ">" + player1Name + "'s" + "</font>" + " "
    + "battle participant");
  ui.player1AssistingUnitsLabel->setText(
    "<font color=" + player1Color + ">" + player1Name + "'s" + "</font>" + " "
    + "battle assistant");
  ui.player1TechnologiesLabel->setText(
    "<font color=" + player1Color + ">" + player1Name + "'s" + "</font>" + " "
    + "technologies");
  ui.player1EventsLabel->setText(
    "<font color=" + player1Color + ">" + player1Name + "'s" + "</font>" + " "
    + "event cards");
  ui.player2UnitsLabel->setText(
    "<font color=" + player2Color + ">" + player2Name + "'s" + "</font>" + " "
    + "battle participant");
  ui.player2AssistingUnitsLabel->setText(
    "<font color=" + player2Color + ">" + player2Name + "'s" + "</font>" + " "
    + "battle assistant");
  ui.player2TechnologiesLabel->setText(
    "<font color=" + player2Color + ">" + player2Name + "'s" + "</font>" + " "
    + "technologies");
  ui.player2EventsLabel->setText(
    "<font color=" + player2Color + ">" + player2Name + "'s" + "</font>" + " "
    + "event cards");

  // underscores in file
  player1Name = convertSpacesToUnderscores(player1Name);
  player2Name = convertSpacesToUnderscores(player2Name);

  // update file
  m_player_names.changePlayer1Name(player1Name);
  m_player_names.changePlayer2Name(player2Name);
}

// Run on change of "Options" > "Set player 1's name"
void MainWindow::on_actionSet_name_of_player_1_triggered()
{
  SFXToPlay(buttonPressedFilePath);

  QInputDialog nameDialog;

  QLabel player1NameLabel(palettes.getDialogBoxTextTags(
    convertUnderscoresToSpaces(player1Name) + "'s name:"));
  nameDialog.setLabelText(player1NameLabel.text());
  nameDialog.setInputMode(QInputDialog::TextInput);
  nameDialog.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player1Name) + "'s name");
  nameDialog.setStyleSheet(palettes.getDialogBoxStyling());
  nameDialog.exec();

  player1Name = nameDialog.textValue();

  // Validate the user input
  if (player1Name.isEmpty()) {
    player1Name = "Player 1";
  }

  updatePlayerNames();
}

// Run on change of "Options" > "Set player 2's name"
void MainWindow::on_actionSet_name_of_player_2_triggered()
{
  SFXToPlay(buttonPressedFilePath);

  QInputDialog nameDialog;
  QLabel       player2NameLabel(palettes.getDialogBoxTextTags(
    convertUnderscoresToSpaces(player2Name) + "'s name:"));
  nameDialog.setLabelText(player2NameLabel.text());
  nameDialog.setInputMode(QInputDialog::TextInput);
  nameDialog.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player2Name) + "'s name");
  nameDialog.setStyleSheet(palettes.getDialogBoxStyling());
  nameDialog.exec();

  player2Name = nameDialog.textValue();

  // Validate the user input
  if (player2Name.isEmpty()) {
    player2Name = "Player 2";
  }

  updatePlayerNames();
}

void MainWindow::onShowDatabaseWindowTriggered()
{
  DeveloperWindow developerWindow{m_database->getDatabase()};
  developerWindow.exec();
}

void MainWindow::initializeEntityAliases()
{
  m_aliases.add(
    "Charlamagne's Palace At Aix La'Chapelle (Briton)", "Wonder (Briton)");
  m_aliases.add("Rock Of Cashel (Celt)", "Wonder (Celt)");
  m_aliases.add(
    "The Golden Tent Of The Great Khan (Mongol)", "Wonder (Mongol)");
  m_aliases.add(
    "The Palace Of Ctesiphon On The Tigris (Persian)", "Wonder (Persian)");
  m_aliases.add("Tomb Of Theodoric (Goth)", "Wonder (Goth)");
  m_aliases.add("Notre-Dame Cathedral (Frank)", "Wonder (Frank)");
  m_aliases.add("Stave Church At Urnes (Viking)", "Wonder (Viking)");
  m_aliases.add("The Great Temple At Nara (Japanese)", "Wonder (Japanese)");
  m_aliases.add("The Great Mosque at Samarra (Saracen)", "Wonder (Saracen)");

  m_aliases.add("Knight", "Kt");
  m_aliases.add("Knight (Frank)", "Kt (Frank)");
  m_aliases.add("Knight (Persian)", "Kt (Persian)");
  m_aliases.add("Crossbowman", "Xbow");
  m_aliases.add("Crossbowman (Saracen)", "Xbow (Saracen)");
  m_aliases.add("Siege Onager", "SO");
  m_aliases.add("Siege Onager (Celt)", "SO (Celt)");
  m_aliases.add("Town Center", "TC");
  m_aliases.add("Town Center (Briton)", "TC (Briton)");
  m_aliases.add("Town Center (Persian)", "TC (Persian)");
}

static QList<QString> fetchNamesOf(
  const QListWidgetItem* item,
  const Aliases&         aliases)
{
  const QString  name{item->text()};
  QList<QString> names{aliases.aliasesOf(name)};
  names.push_front(name);
  return names;
}

static bool hasMatch(const QList<QString>& names, const QString& filter)
{
  return std::ranges::any_of(names, [&filter](const QString& name) {
    return name.contains(filter, Qt::CaseInsensitive);
  });
}

void MainWindow::filterEntityNames(
  QListWidget*   list,
  const QString& filter,
  QString        player)
{
  filterBasedOnAgeAndCivilization(player);

  for (int row{0}; row < list->count(); ++row) {
    QListWidgetItem* const element{list->item(row)};
    const QList<QString>   names{fetchNamesOf(element, m_aliases)};
    const bool             doesMatch{hasMatch(names, filter)};

    if (!element->isHidden() && !doesMatch) {
      element->setHidden(true);
    }
  }
}

void MainWindow::on_actionSet_player_1_Age_triggered()
{
  QInputDialog medievalAgeDialog;
  QLabel       ageLabel(palettes.getDialogBoxTextTags(
    convertUnderscoresToSpaces(player1Name) + "'s medieval age:"));
  medievalAgeDialog.setLabelText(ageLabel.text());
  medievalAgeDialog.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player1Name) + "'s medieval age");
  medievalAgeDialog.setStyleSheet(palettes.getDialogBoxStyling());
  medievalAgeDialog.setComboBoxItems(ages);
  medievalAgeDialog.exec();

  player1Age = medievalAgeDialog.textValue();

  if (player1Age == "Dark Age") {
    representationOfPlayer1Age = 1;
  }
  else if (player1Age == "Feudal Age") {
    representationOfPlayer1Age = 2;
  }
  else if (player1Age == "Castle Age") {
    representationOfPlayer1Age = 3;
  }
  else if (player1Age == "Imperial Age") {
    representationOfPlayer1Age = 4;
  }

  // update file
  m_player_medieval_age.changePlayer1MedievalAge(representationOfPlayer1Age);

  // filter list based on age and civilization
  filterBasedOnAgeAndCivilization("1");
}

void MainWindow::on_actionSet_player_2_Age_triggered()
{
  QInputDialog medievalAgeDialog;
  QLabel       ageLabel(palettes.getDialogBoxTextTags(
    convertUnderscoresToSpaces(player2Name) + "'s medieval age:"));
  medievalAgeDialog.setLabelText(ageLabel.text());
  medievalAgeDialog.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player2Name) + "'s medieval age");
  medievalAgeDialog.setStyleSheet(palettes.getDialogBoxStyling());
  medievalAgeDialog.setComboBoxItems(ages);
  medievalAgeDialog.exec();

  player2Age = medievalAgeDialog.textValue();

  if (player2Age == "Dark Age") {
    representationOfPlayer2Age = 1;
  }
  else if (player2Age == "Feudal Age") {
    representationOfPlayer2Age = 2;
  }
  else if (player2Age == "Castle Age") {
    representationOfPlayer2Age = 3;
  }
  else if (player2Age == "Imperial Age") {
    representationOfPlayer2Age = 4;
  }

  // update file
  m_player_medieval_age.changePlayer2MedievalAge(representationOfPlayer2Age);

  // filter list based on age and civilization
  filterBasedOnAgeAndCivilization("2");
}

void MainWindow::setInitialNames()
{
  const QString player1InitialName{m_player_names.play1Name().playerName()};
  const QString player2InitialName{m_player_names.play2Name().playerName()};

  player1Name = player1InitialName;
  player2Name = player2InitialName;

  updatePlayerNames();
}

void MainWindow::markInitialPlayerMedievalAge()
{
  representationOfPlayer1Age
    = m_player_medieval_age.player1MedievalAge().PlayerMedievalAge();
  representationOfPlayer2Age
    = m_player_medieval_age.player2MedievalAge().PlayerMedievalAge();

  if (representationOfPlayer1Age == 1) {
    player1Age = "Dark Age";
  }
  else if (representationOfPlayer1Age == 2) {
    player1Age = "Feudal Age";
  }
  else if (representationOfPlayer1Age == 3) {
    player1Age = "Castle Age";
  }
  else if (representationOfPlayer1Age == 4) {
    player1Age = "Imperial Age";
  }

  if (representationOfPlayer2Age == 1) {
    player2Age = "Dark Age";
  }
  else if (representationOfPlayer2Age == 2) {
    player2Age = "Feudal Age";
  }
  else if (representationOfPlayer2Age == 3) {
    player2Age = "Castle Age";
  }
  else if (representationOfPlayer2Age == 4) {
    player2Age = "Imperial Age";
  }

  filterBasedOnAgeAndCivilization("1");
  filterBasedOnAgeAndCivilization("2");
}

void MainWindow::selectInitialEntities()
{
  const QString          player1Entity{m_entities.player1Entity().entityName()};
  QListWidgetItem* const player1SelectedEntity{
    findByEntityName(ui.player1EntityNames, player1Entity)};
  const QString          player2Entity{m_entities.player2Entity().entityName()};
  QListWidgetItem* const player2SelectedEntity{
    findByEntityName(ui.player2EntityNames, player2Entity)};

  if (player1SelectedEntity != nullptr) {
    ui.player1EntityNames->setCurrentItem(player1SelectedEntity);
    ui.player1EntityNames->scrollToItem(player1SelectedEntity);

    ui.player1EntityQuantity->setValue(
      m_entities.player1Entity().entityQuantity());

    updateRangeAllowed(m_entities.player1Entity().entityName(), 1);
  }

  if (player2SelectedEntity != nullptr) {
    ui.player2EntityNames->setCurrentItem(player2SelectedEntity);
    ui.player2EntityNames->scrollToItem(player2SelectedEntity);

    ui.player2EntityQuantity->setValue(
      m_entities.player2Entity().entityQuantity());

    updateRangeAllowed(m_entities.player2Entity().entityName(), 2);
  }

  checkIfEitherPlayerHasBombardmentEntity();
}

static void selectAssistant(const QString& assistant, QComboBox* comboBox)
{
  const int assistantIndex{comboBox->findText(assistant, Qt::MatchFixedString)};
  assert(assistantIndex != -1);
  comboBox->setCurrentIndex(assistantIndex);
}

void MainWindow::selectInitialAssistants()
{
  selectAssistant(
    m_entities.player1Entity().assistantName(), ui.player1BattleAssistantNames);
  selectAssistant(
    m_entities.player2Entity().assistantName(), ui.player2BattleAssistantNames);
  ui.player1EntityAssistantQuantity->setValue(
    m_entities.player1Entity().assistantQuantity());
  ui.player2EntityAssistantQuantity->setValue(
    m_entities.player2Entity().assistantQuantity());
}

// Run on change of "Program" > "Options" > "Disable SFX" toggle
void MainWindow::on_actionEnableDisableSFX_triggered()
{
  SFXToPlay(togglePressedFilePath);

  soundEffectsEnabled = !soundEffectsEnabled;

  setColorTheToggleElements();
}

// Run on change of "Program" > "Options" > "Enable dark mode" toggle
void MainWindow::on_actionEnableDisableDarkMode_triggered()
{
  SFXToPlay(togglePressedFilePath);
  palettes.darkModeEnabled = !palettes.darkModeEnabled;
  setColorTheUIElements();
}

void MainWindow::on_p1ActiveTechnologiesToggle_toggled(bool checked)
{
  QListWidgetItem* p1TechnologiesListItem;

  for (int i = 0; i < ui.player1Technologies->count(); i++) {
    p1TechnologiesListItem = ui.player1Technologies->item(i);

    if (p1TechnologiesListItem->checkState() == Qt::Checked) {
      p1TechnologiesListItem->setHidden(false);
    }
    else {
      if (checked == true) {
        p1TechnologiesListItem->setHidden(true);
      }
      else if (checked == false) {
        p1TechnologiesListItem->setHidden(false);
      }
    }
  }
}

void MainWindow::on_p2ActiveTechnologiesToggle_toggled(bool checked)
{
  QListWidgetItem* p2TechnologiesListItem;

  for (int i = 0; i < ui.player2Technologies->count(); i++) {
    p2TechnologiesListItem = ui.player2Technologies->item(i);

    if (p2TechnologiesListItem->checkState() == Qt::Checked) {
      p2TechnologiesListItem->setHidden(false);
    }
    else {
      if (checked == true) {
        p2TechnologiesListItem->setHidden(true);
      }
      else if (checked == false) {
        p2TechnologiesListItem->setHidden(false);
      }
    }
  }
}

void MainWindow::on_p1ActiveEventsToggle_toggled(bool checked)
{
  QListWidgetItem* p1EventsListItem;

  for (int i = 0; i < ui.player1Events->count(); i++) {
    p1EventsListItem = ui.player1Events->item(i);

    if (p1EventsListItem->checkState() == Qt::Checked) {
      p1EventsListItem->setHidden(false);
    }
    else {
      if (checked == true) {
        p1EventsListItem->setHidden(true);
      }
      else if (checked == false) {
        p1EventsListItem->setHidden(false);
      }
    }
  }
}

void MainWindow::on_p2ActiveEventsToggle_toggled(bool checked)
{
  QListWidgetItem* p2EventsListItem;

  for (int i = 0; i < ui.player2Events->count(); i++) {
    p2EventsListItem = ui.player2Events->item(i);

    if (p2EventsListItem->checkState() == Qt::Checked) {
      p2EventsListItem->setHidden(false);
    }
    else {
      if (checked == true) {
        p2EventsListItem->setHidden(true);
      }
      else if (checked == false) {
        p2EventsListItem->setHidden(false);
      }
    }
  }
}

void MainWindow::on_actionSetDefaultAnswerToRetreatingPrompt_triggered()
{
  SFXToPlay(togglePressedFilePath);

  QStringList options
    = {"Ask each time", "Always retreating", "Never retreating"};

  QInputDialog defaultAnswerToRetreatingPromptDialog;

  QLabel dialogLabel(palettes.getDialogBoxTextTags("Players' default answer:"));
  defaultAnswerToRetreatingPromptDialog.setLabelText(dialogLabel.text());
  defaultAnswerToRetreatingPromptDialog.setWindowTitle(
    "Handling of the retreating prompt");
  defaultAnswerToRetreatingPromptDialog.setStyleSheet(
    palettes.getDialogBoxStyling());
  defaultAnswerToRetreatingPromptDialog.setComboBoxItems(options);
  defaultAnswerToRetreatingPromptDialog.exec();

  retreatingPromptAnswer = defaultAnswerToRetreatingPromptDialog.textValue();
}

void MainWindow::on_actionSetDefaultAnswerToConvertingHealingPrompt_triggered()
{
  SFXToPlay(togglePressedFilePath);

  QStringList options
    = {"Ask each time", "Always converting", "Always healing"};

  QInputDialog defaultAnswerToConversionHealingPromptDialog;
  QLabel dialogLabel(palettes.getDialogBoxTextTags("Players' default answer:"));
  defaultAnswerToConversionHealingPromptDialog.setLabelText(dialogLabel.text());
  defaultAnswerToConversionHealingPromptDialog.setWindowTitle(
    "Handling of the conversion/healing prompt");
  defaultAnswerToConversionHealingPromptDialog.setStyleSheet(
    palettes.getDialogBoxStyling());
  defaultAnswerToConversionHealingPromptDialog.setComboBoxItems(options);
  defaultAnswerToConversionHealingPromptDialog.exec();

  convertingHealingPromptAnswer
    = defaultAnswerToConversionHealingPromptDialog.textValue();
}

void MainWindow::onActionSetDefaultAnswerToAttackMonksPromptTriggered()
{
  SFXToPlay(togglePressedFilePath);

  QStringList options
    = {"Ask each time", "Always attack monks", "Never attack monks"};

  QInputDialog defaultAnswerToAttackMonksPromptDialog;

  QLabel dialogLabel(palettes.getDialogBoxTextTags("Players' default answer:"));
  defaultAnswerToAttackMonksPromptDialog.setLabelText(dialogLabel.text());
  defaultAnswerToAttackMonksPromptDialog.setWindowTitle(
    "Handling of the monk attacking prompt");
  defaultAnswerToAttackMonksPromptDialog.setStyleSheet(
    palettes.getDialogBoxStyling());
  defaultAnswerToAttackMonksPromptDialog.setComboBoxItems(options);
  defaultAnswerToAttackMonksPromptDialog.exec();

  monkAttackingPromptAnswer
    = defaultAnswerToAttackMonksPromptDialog.textValue();
}

void MainWindow::setUnitBuildingStyleBasedOnCivilizationSelected(
  QString* playerCivilization,
  QString* playerArchitecturalStyle,
  QString* playerUnitStyle)
{
  if (
    (*playerCivilization == "Ethiopians")
    || (*playerCivilization == "Malians")) {
    *playerArchitecturalStyle = "_african";
  }
  else if (
    (*playerCivilization == "Cumans") || (*playerCivilization == "Tatars")) {
    *playerArchitecturalStyle = "_central_asian";
    *playerUnitStyle          = "_asian";
  }
  else if (
    (*playerCivilization == "Goths") || (*playerCivilization == "Huns")
    || (*playerCivilization == "Teutons")
    || (*playerCivilization == "Vikings")) {
    *playerArchitecturalStyle = "_central_european";
    *playerUnitStyle          = "_western";
  }
  else if (
    (*playerCivilization == "Chinese") || (*playerCivilization == "Japanese")
    || (*playerCivilization == "Koreans") || (*playerCivilization == "Mongols")
    || (*playerCivilization == "Vietnamese")) {
    *playerArchitecturalStyle = "_east_asian";
    *playerUnitStyle          = "_asian";
  }
  else if (
    (*playerCivilization == "Bohemians")
    || (*playerCivilization == "Bulgarians")
    || (*playerCivilization == "Lithuanians")
    || (*playerCivilization == "Poles") || (*playerCivilization == "Slavs")) {
    *playerArchitecturalStyle = "_eastern_european";
    *playerUnitStyle          = "_asian";
  }
  else if (
    (*playerCivilization == "Armenians")
    || (*playerCivilization == "Byzantines")
    || (*playerCivilization == "Georgians")
    || (*playerCivilization == "Italians")
    || (*playerCivilization == "Portuguese")
    || (*playerCivilization == "Romans") || (*playerCivilization == "Italians")
    || (*playerCivilization == "Sicilians")
    || (*playerCivilization == "Spanish")) {
    *playerArchitecturalStyle = "_mediterranean";
    *playerUnitStyle          = "_western";
  }
  else if (
    (*playerCivilization == "Berbers") || (*playerCivilization == "Persians")
    || (*playerCivilization == "Saracens")
    || (*playerCivilization == "Turks")) {
    *playerArchitecturalStyle = "_middle_eastern";
  }
  else if (
    (*playerCivilization == "Aztecs") || (*playerCivilization == "Incas")
    || (*playerCivilization == "Mayans")) {
    *playerArchitecturalStyle = "_native_american";
    *playerUnitStyle          = "_mesoamerican";
  }
  else if (
    (*playerCivilization == "Bengalis") || (*playerCivilization == "Dravidians")
    || (*playerCivilization == "Gurjaras")
    || (*playerCivilization == "Hindustanis")) {
    *playerArchitecturalStyle = "_south_asian";
    *playerUnitStyle          = "_asian";
  }
  else if (
    (*playerCivilization == "Burmese") || (*playerCivilization == "Khmer")
    || (*playerCivilization == "Malay")) {
    *playerArchitecturalStyle = "_southeast_asian";
    *playerUnitStyle          = "_asian";
  }
  else if (
    (*playerCivilization == "Britons") || (*playerCivilization == "Burgundians")
    || (*playerCivilization == "Celts") || (*playerCivilization == "Franks")) {
    *playerArchitecturalStyle = "_western_european";
    *playerUnitStyle          = "_western";
  }
}

void MainWindow::on_actionSet_civilization_of_player_1_triggered()
{
  SFXToPlay(togglePressedFilePath);

  QInputDialog player1CivilizationSelection;

  QLabel dialogLabel(palettes.getDialogBoxTextTags(
    convertUnderscoresToSpaces(player1Name) + "'s civilization:"));
  player1CivilizationSelection.setLabelText(dialogLabel.text());
  player1CivilizationSelection.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player1Name) + "'s civilization");
  player1CivilizationSelection.setStyleSheet(palettes.getDialogBoxStyling());
  player1CivilizationSelection.setComboBoxItems(civilizations);

  player1CivilizationSelection.exec();

  player1Civilization = player1CivilizationSelection.textValue();

  filterBasedOnAgeAndCivilization("1");

  setUnitBuildingStyleBasedOnCivilizationSelected(
    &player1Civilization, &p1BuildingArchitecturalStyle, &p1UnitStyle);
}

void MainWindow::on_actionSet_civilization_of_player_2_triggered()
{
  QInputDialog player2CivilizationSelection;

  QLabel dialogLabel(palettes.getDialogBoxTextTags(
    convertUnderscoresToSpaces(player2Name) + "'s civilization:"));
  player2CivilizationSelection.setLabelText(dialogLabel.text());
  player2CivilizationSelection.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player2Name) + "'s civilization");
  player2CivilizationSelection.setStyleSheet(palettes.getDialogBoxStyling());
  player2CivilizationSelection.setComboBoxItems(civilizations);
  player2CivilizationSelection.exec();

  player2Civilization = player2CivilizationSelection.textValue();

  filterBasedOnAgeAndCivilization("2");

  setUnitBuildingStyleBasedOnCivilizationSelected(
    &player2Civilization, &p2BuildingArchitecturalStyle, &p2UnitStyle);
}

void MainWindow::initializeAnimations()
{
  getEntityAnimationForSelectedEntity(
    ui.player1EntityNames->currentItem()->text(), "1", "_idle");
  getEntityAnimationForSelectedEntity(
    ui.player2EntityNames->currentItem()->text(), "2", "_idle");
  getAssistantEntityAnimationForSelectedAssistant(
    ui.player1BattleAssistantNames->currentText(), "1", "_idle");
  getAssistantEntityAnimationForSelectedAssistant(
    ui.player1BattleAssistantNames->currentText(), "2", "_idle");
}

void MainWindow::getAssistantEntityAnimationForSelectedAssistant(
  QString currentSelection,
  QString player,
  QString assistantStatus)
{
  QLabel* theLabelOfTheCurrentPlayer = ui.p1AssistantAnimation;
  QString fileName;
  QString filePath;

  fileName = (convertSpacesToUnderscores(currentSelection)).toLower();
  fileName = fileName.remove(removeBracketedTextExpression);

  if (player == "1") {
    theLabelOfTheCurrentPlayer = ui.p1AssistantAnimation;
    filePath
      = animationFolderPath + fileName + p1UnitStyle + assistantStatus + ".gif";
  }
  else if (player == "2") {
    theLabelOfTheCurrentPlayer = ui.p2AssistantAnimation;
    filePath
      = animationFolderPath + fileName + p2UnitStyle + assistantStatus + ".gif";
  }

  QMovie* GifAnimation = new QMovie(workingDirectory.absolutePath() + filePath);

  theLabelOfTheCurrentPlayer->setMovie(GifAnimation);
  theLabelOfTheCurrentPlayer->setVisible(true);
  theLabelOfTheCurrentPlayer->resize(75, 75);
  theLabelOfTheCurrentPlayer->setScaledContents(true);

  GifAnimation->setSpeed(70); // 70% of original speed
  GifAnimation->setScaledSize(QSize().scaled(75, 75, Qt::KeepAspectRatio));
  GifAnimation->start();
}

void MainWindow::getEntityAnimationForSelectedEntity(
  QString currentSelection,
  QString player,
  QString entityStatus)
{
  QLabel* theLabelOfTheCurrentPlayer = ui.player1Animation;
  QString fileName;
  QString filePath;
  QString fileAge;

  bool unit     = false;
  bool building = false;

  if (unitNames.contains(currentSelection)) {
    unit = true;
  }

  if (buildingNames.contains(currentSelection)) {
    building = true;

    if (entityStatus == "_attack") {
      entityStatus = "_idle";
    }
  }

  // Manually correct it for edge cases
  if (
    (currentSelection.contains("Gold Mine"))
    || (currentSelection.contains("Stone Mine"))) {
    currentSelection = "Mining Camp";
  }

  if (currentSelection.contains("Farm")) {
    if (m_p1FarmMemory == 0) {
      m_p1FarmMemory = 1 + (rand() % 2);
    }

    if (m_p2FarmMemory == 0) {
      m_p2FarmMemory = 1 + (rand() % 2);
    }

    if (player == "1") {
      if (m_p1FarmMemory == 1) {
        currentSelection = "Farm Corn";
      }
      else if (m_p1FarmMemory == 2) {
        currentSelection = "Farm Grape";
      }
    }
    else if (player == "2") {
      if (m_p2FarmMemory == 1) {
        currentSelection = "Farm Corn";
      }
      else if (m_p2FarmMemory == 2) {
        currentSelection = "Farm Grape";
      }
    }
  }

  if (currentSelection.contains("Trebuchet")) {
    currentSelection = "Trebuchet Unpacked";
  }

  if (currentSelection.contains("Charlamagne")) {
    currentSelection = "Wonder Briton";
  }
  else if (currentSelection.contains("Rock Of Cashel")) {
    currentSelection = "Wonder Celt";
  }
  else if (currentSelection.contains("The Golden Tent Of The Great Khan")) {
    currentSelection = "Wonder Mongol";
  }
  else if (currentSelection.contains("The Palace Of Ctesiphon On The Tigris")) {
    currentSelection = "Wonder Persian";
  }
  else if (currentSelection.contains("Tomb Of Theodoric")) {
    currentSelection = "Wonder Goth";
  }
  else if (currentSelection.contains("Notre-Dame Cathedral")) {
    currentSelection = "Wonder Frank";
  }
  else if (currentSelection.contains("Stave Church At Urnes")) {
    currentSelection = "Wonder Viking";
  }
  else if (currentSelection.contains("The Great Temple At Nara")) {
    currentSelection = "Wonder Japanese";
  }
  else if (currentSelection.contains("The Great Mosque at Samarra")) {
    currentSelection = "Wonder Saracen";
  }

  if (
    currentSelection.contains("Elite") & (currentSelection.contains("("))
    & (currentSelection.contains(")"))

  )

  {
    currentSelection.remove(0, 6);
  }

  // Make the currentSelection string have the same name as it's corresponding
  // file
  fileName = (convertSpacesToUnderscores(currentSelection)).toLower();
  fileName = fileName.remove(removeBracketedTextExpression);

  // Work out what age the entity belongs in
  if (player == "1") {
    fileAge = (convertSpacesToUnderscores(player1Age)).toLower();
  }
  else if (player == "2") {
    fileAge = (convertSpacesToUnderscores(player2Age)).toLower();
  }

  // Manually correct it for edge cases
  bool isADarkAgeBuilding = false;

  if (building == true) {
    for (int i = 0; i < age1BuildingNames.length(); i++) {
      if (currentSelection.contains(age1BuildingNames[i])) {
        isADarkAgeBuilding = true;
        fileAge            = "dark_age";
      }
    }
  }

  if (currentSelection.contains("Mining Camp")) {
    if (player == "1") {
      if (p1BuildingArchitecturalStyle.contains("asian")) {
        p1BuildingArchitecturalStyle = "_asian";
      }
    }
    else if (player == "2") {
      if (p2BuildingArchitecturalStyle.contains("asian")) {
        p2BuildingArchitecturalStyle = "_asian";
      }
    }
  }

  if (currentSelection.contains("Lumber Camp")) {
    fileAge            = "feudal_age";
    isADarkAgeBuilding = false;
  }

  if (
    ((currentSelection.contains("Barracks"))
     || (currentSelection.contains("Archery Range"))
     || (currentSelection.contains("Blacksmith"))
     || (currentSelection.contains("Castle"))
     || (currentSelection.contains("Dock"))
     || (currentSelection.contains("House"))
     || (currentSelection.contains("Mill"))
     || (currentSelection.contains("Monastery"))
     || (currentSelection.contains("Fortified Gate"))
     || (currentSelection.contains("Fortified Wall"))
     || (currentSelection.contains("Stable"))
     || (currentSelection.contains("Siege Workshop"))

       )
    && (fileAge == "imperial_age")) {
    fileAge = "castle_age";
  }

  if (
    ((currentSelection.contains("Mining Camp"))
     || (currentSelection.contains("Stone Wall"))
     || (currentSelection.contains("Stone Gate"))
     || (currentSelection.contains("Watch Tower")))
    && ((fileAge == "castle_age") || (fileAge == "imperial_age"))) {
    fileAge = "feudal_age";
  }

  // Randomly make it choose between female and male villagers
  // Todo: Make it reset m_p1VillagerMemory and m_p2VillagerMemory values to 0
  // upon unit death So that it can randomly choose these values again
  bool useOldP1VillagerMemoryValueTemporarily = false;
  bool useOldP2VillagerMemoryValueTemporarily = false;

  if (unit == true) {
    if (currentSelection.contains("Villager")) {
      if (m_p1VillagerMemory == 0 && entityStatus == "_idle") {
        m_p1VillagerMemory  = 1 + (rand() % 2);
        p1VillagerMemoryOld = m_p1VillagerMemory;
      }
      else {
        useOldP1VillagerMemoryValueTemporarily = true;
      }

      if (m_p2VillagerMemory == 0 && entityStatus == "_idle") {
        m_p2VillagerMemory  = 1 + (rand() % 2);
        p2VillagerMemoryOld = m_p2VillagerMemory;
      }
      else {
        useOldP2VillagerMemoryValueTemporarily = true;
      }
    }
  }
  // Set which UI element is being modified
  // Set the path to the filename
  if (player == "1") {
    theLabelOfTheCurrentPlayer = ui.player1Animation;

    if (building == true) {
      if (isADarkAgeBuilding == false) {
        filePath = animationFolderPath + fileName + p1BuildingArchitecturalStyle
                   + "_" + fileAge + entityStatus + ".gif";
      }
      else {
        filePath
          = animationFolderPath + fileName + "_" + fileAge + entityStatus + ".gif";
      }
    }

    if (unit == true) {
      if (currentSelection.contains("Villager")) {
        if (useOldP1VillagerMemoryValueTemporarily == false) {
          if (m_p1VillagerMemory == 1) {
            fileName += "_male";
          }
          else if (m_p1VillagerMemory == 2) {
            fileName += "_female";
          }
        }
        else if (useOldP1VillagerMemoryValueTemporarily == true) {
          if (p1VillagerMemoryOld == 1) {
            fileName += "_male";
          }
          else if (p1VillagerMemoryOld == 2) {
            fileName += "_female";
          }
        }
      }

      filePath = animationFolderPath + fileName + entityStatus + ".gif";
    }
  }
  else if (player == "2") {
    theLabelOfTheCurrentPlayer = ui.player2Animation;

    if (building == true) {
      if (isADarkAgeBuilding == false) {
        filePath = animationFolderPath + fileName + p2BuildingArchitecturalStyle
                   + "_" + fileAge + entityStatus + ".gif";
      }
      else {
        filePath
          = animationFolderPath + fileName + "_" + fileAge + entityStatus + ".gif";
      }
    }

    if (unit == true) {
      if (currentSelection.contains("Villager")) {
        if (useOldP2VillagerMemoryValueTemporarily == false) {
          if (m_p2VillagerMemory == 1) {
            fileName += "_male";
          }
          else if (m_p2VillagerMemory == 2) {
            fileName += "_female";
          }
        }
        else if (useOldP2VillagerMemoryValueTemporarily == true) {
          if (p2VillagerMemoryOld == 1) {
            fileName += "_male";
          }
          else if (p2VillagerMemoryOld == 2) {
            fileName += "_female";
          }
        }
      }

      filePath = animationFolderPath + fileName + entityStatus + ".gif";
    }
  }

  /* Manual correction */
  if (currentSelection.contains("Monk")) {
    if (player == "1") {
      filePath
        = animationFolderPath + fileName + p1UnitStyle + entityStatus + ".gif";
    }
    else if (player == "2") {
      filePath
        = animationFolderPath + fileName + p2UnitStyle + entityStatus + ".gif";
    }
  }

  // qDebug() << "file path to animation file: " << filePath;

  QMovie* GifAnimation = new QMovie(workingDirectory.absolutePath() + filePath);

  theLabelOfTheCurrentPlayer->setMovie(GifAnimation);
  theLabelOfTheCurrentPlayer->setVisible(true);
  theLabelOfTheCurrentPlayer->resize(75, 75);
  theLabelOfTheCurrentPlayer->setScaledContents(true);

  if (
    (currentSelection.contains("ship", Qt::CaseInsensitive))
    || (currentSelection.contains("galle", Qt::CaseInsensitive))
    || (currentSelection.contains("boat", Qt::CaseInsensitive))
    || (currentSelection.contains("dock", Qt::CaseInsensitive))) {
    theLabelOfTheCurrentPlayer->setStyleSheet(
      "background-color: rgb(35,137,218);");
  }
  else {
    theLabelOfTheCurrentPlayer->setStyleSheet("background-color: none;");
  }

  GifAnimation->setSpeed(70); // 70% of original speed
  GifAnimation->setScaledSize(QSize().scaled(75, 75, Qt::KeepAspectRatio));
  GifAnimation->start();
}

void MainWindow::hideOrShowBasedOnAge(
  QString player,
  QString itemToCheckFor,
  bool    hideOrShow)
{
  QListWidget* theListOfTheCurrentPlayer;
  theListOfTheCurrentPlayer = ui.player1EntityNames;

  if (player == "1") {
    theListOfTheCurrentPlayer = ui.player1EntityNames;
  }
  else if (player == "2") {
    theListOfTheCurrentPlayer = ui.player2EntityNames;
  }

  for (int i = 0; i < theListOfTheCurrentPlayer->count(); i++) {
    if (theListOfTheCurrentPlayer->item(i)->text().contains(itemToCheckFor)) {
      theListOfTheCurrentPlayer->item(i)->setHidden(hideOrShow);
    }
  }
}

void MainWindow::onPlayerEntityDeath(Player player, bool didAnAssistantDie)
{
  if (didAnAssistantDie == false) {
    QListWidget* battleParticipantsListWidget = player == Player::Player1
                                                  ? ui.player1EntityNames
                                                  : ui.player2EntityNames;

    getEntityAnimationForSelectedEntity(
      battleParticipantsListWidget->currentItem()->text(),
      player == Player::Player1 ? "1" : "2",
      "_death");
  }
  else if (didAnAssistantDie == true) {
    QListWidget* assistantBattleParticipantsListWidget
      = player == Player::Player1 ? ui.player1EntityNames
                                  : ui.player2EntityNames;

    getAssistantEntityAnimationForSelectedAssistant(
      assistantBattleParticipantsListWidget->currentItem()->text(),
      player == Player::Player1 ? "1" : "2",
      "_death");
  }
}

void MainWindow::filterBasedOnAgeAndCivilization(QString player)
{
  const QString age = (player == "1") ? player1Age : player2Age;
  QString civ = (player == "1") ? player1Civilization : player2Civilization;
  QListWidget const* entities
    = (player == "1") ? ui.player1EntityNames : ui.player2EntityNames;
  QStringList otherCivs = civilizations;
  otherCivs.removeOne(civ);

  for (int i = 0; i < otherCivs.length(); ++i) {
    // Remove "s" character from end of civilization mame if it is present
    if (otherCivs[i].endsWith("s", Qt::CaseSensitive)) {
      otherCivs[i] = otherCivs[i].mid(0, otherCivs[i].length() - 1);
    }
  }

  if (civ.endsWith("s", Qt::CaseSensitive)) {
    civ = civ.mid(0, civ.length() - 1);
  }

  bool removeAge2ListElements = false;
  bool removeAge3ListElements = false;
  bool removeAge4ListElements = false;

  if (age == "Dark Age") {
    removeAge2ListElements = true;
    removeAge3ListElements = true;
    removeAge4ListElements = true;
  }
  else if (age == "Feudal Age") {
    removeAge2ListElements = false;
    removeAge3ListElements = true;
    removeAge4ListElements = true;
  }
  else if (age == "Castle Age") {
    removeAge2ListElements = false;
    removeAge3ListElements = false;
    removeAge4ListElements = true;
  }
  else if (age == "Imperial Age") {
    removeAge2ListElements = false;
    removeAge3ListElements = false;
    removeAge4ListElements = false;
  }

  for (int i = 0; i < entities->count(); ++i) {
    const QString& itemText = entities->item(i)->text();
    entities->item(i)->setHidden(false);

    // Hide it if it belongs exclusively to another civilization.
    if (std::ranges::any_of(otherCivs, [&itemText](const QString& otherCiv) {
          return itemText.contains(otherCiv);
        })) {
      entities->item(i)->setHidden(true);
    }
  }

  // Hide those that have an incorrect age.
  for (int i = 0; i < age2UnitNames.count(); i++) {
    QString age2UnitName = age2UnitNames[i];
    if (removeAge2ListElements) {
      hideOrShowBasedOnAge(player, age2UnitName, true);
    }
  }

  for (int i = 0; i < age2BuildingNames.count(); i++) {
    QString age2BuildingName = age2BuildingNames[i];
    if (removeAge2ListElements) {
      hideOrShowBasedOnAge(player, age2BuildingName, true);
    }
  }

  for (int i = 0; i < age3UnitNames.count(); i++) {
    QString age3UnitName = age3UnitNames[i];
    if (removeAge3ListElements) {
      hideOrShowBasedOnAge(player, age3UnitName, true);
    }
  }

  for (int i = 0; i < age3BuildingNames.count(); i++) {
    QString age3BuildingName = age3BuildingNames[i];
    if (removeAge3ListElements) {
      hideOrShowBasedOnAge(player, age3BuildingName, true);
    }
  }

  for (int i = 0; i < age4UnitNames.count(); i++) {
    QString age4UnitName = age4UnitNames[i];
    if (removeAge4ListElements) {
      hideOrShowBasedOnAge(player, age4UnitName, true);
    }
  }

  for (int i = 0; i < age4BuildingNames.count(); i++) {
    QString age4BuildingName = age4BuildingNames[i];
    if (removeAge4ListElements) {
      hideOrShowBasedOnAge(player, age4BuildingName, true);
    }
  }

  // Find those that have our civilization in parentheses and
  // remove the plain entities that are the same without parentheses.
  std::vector<QString> toHide;
  for (int i = 0; i < entities->count(); ++i) {
    const QString&  itemText = entities->item(i)->text();
    const QString   ending   = QString(" (%1)").arg(civ);
    const qsizetype index    = itemText.indexOf(ending);

    if (index != -1) {
      const QString entityToErase = itemText.mid(0, index);
      toHide.push_back(entityToErase);
    }
  }

  for (int i = 0; i < entities->count(); ++i) {
    if (std::ranges::any_of(toHide, [entities, i](const QString& hideMe) {
          return hideMe == entities->item(i)->text();
        })) {
      entities->item(i)->setHidden(true);
    }
  }
}

static bool isBombardmentUnitOrBuilding(const QString& entity)
{
  return (entity.contains("LONGBOAT")) || (entity.contains("GALLEON"))
         || (entity.contains("GALLEY_(JAPANESE)"))
         || (entity.contains("SCORPION")) || (entity.contains("MANGONEL"))
         || (entity.contains("ONAGER")) || (entity.contains("TOWER"))
         || (entity.contains("TREBUCHET"));
}

void MainWindow::checkIfEitherPlayerHasBombardmentEntity()
{
  QString player1Selection = ui.player1EntityNames->currentItem()->text();
  QString player2Selection = ui.player2EntityNames->currentItem()->text();

  player1Selection = player1Selection.toUpper();
  player1Selection = convertSpacesToUnderscores(player1Selection);

  player2Selection = player2Selection.toUpper();
  player2Selection = convertSpacesToUnderscores(player2Selection);

  const bool isBombardmentEntity
    = isBombardmentUnitOrBuilding(player1Selection)
      || isBombardmentUnitOrBuilding(player2Selection);

  if (isBombardmentEntity == true) {
    ui.distanceBetweenTheBattleParticipantsSlider->setMaximum(10);
    ui.distanceBetweenTheBattleParticipantsSlider->setMinimum(0);
    ui.distanceBetweenTheBattleParticipantsSlider->setEnabled(true);
  }
  else {
    ui.distanceBetweenTheBattleParticipantsSlider->setValue(0);
    ui.distanceBetweenTheBattleParticipantsSlider->setMaximum(0);
    ui.distanceBetweenTheBattleParticipantsSlider->setMinimum(0);
    ui.distanceBetweenTheBattleParticipantsSlider->setEnabled(false);
  }
}

void MainWindow::on_distanceBetweenTheBattleParticipantsSlider_valueChanged(
  int value)
{
  m_distanceBetweenTheBattleParticipants
    = value; // Used in combatCalculator.cpp
}

void MainWindow::
  on_actionShow_the_total_instead_of_individual_values_of_battle_participants_in_the_output_triggered(
    bool checked)
{
  m_showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput
    = !m_showTheTotalInsteadOfIndividualValuesOfBattleParticipantsInTheOutput; // Used in entity.cpp

  SFXToPlay(togglePressedFilePath);

  setColorTheToggleElements();
}

void MainWindow::
  on_actionShow_further_information_about_the_battle_participants_in_the_output_triggered(
    bool checked)
{
  m_showFurtherInformationAboutTheBattleParticipantsInTheOutput
    = !m_showFurtherInformationAboutTheBattleParticipantsInTheOutput; // Used in
                                                                      // entity.cpp

  SFXToPlay(togglePressedFilePath);

  setColorTheToggleElements();
}

void MainWindow::on_distanceBetweenTheBattleParticipantsSlider_sliderMoved(
  int position)
{
  QToolTip::showText(QCursor::pos(), QString("%1").arg(position), nullptr);
}

void MainWindow::updateDetatchedOutputWindow()
{
  if (m_myOutputWindowClass->returnIfWindowIsOpen() == true) {
    m_myOutputWindowClass->sendOutputToOutputWindow(ui.gameOutputTextEdit);
  }
}

void MainWindow::detachOutputWindow()
{
  m_myOutputWindowClass->sendOutputToOutputWindow(ui.gameOutputTextEdit);

  m_myOutputWindowClass->showOutputWindow();
}

void MainWindow::on_pushButton_clicked()
{
  detachOutputWindow();
}

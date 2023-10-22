// Included files
#include "mainwindow.hpp"     // This window
#include "aboutwindow.h"      // A window this window can open
#include "backend/run_game.h" // Age of Empires combat results calculator v1.2
#include "dialog_input.h"
#include "file_paths.h"

#include <QLabel>

#include "soundEffects.h" // Sound playing class

#include "cross-window_palette.h" // Coloring of the UI

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
#include <QColorDialog>
#include <QInputDialog>

// Librraries used for hotkeys
#include <QShortcut>

// Declaring class
SoundPlayer playSound;
bool        soundEffectsEnabled   = true;
bool        hasProgramInitialized = false;

// More global variables
bool    isP1BackFromAForeignLandEventInPlay;
QString player1BackFromAForeignLandCivilizationBonusSelection;

bool    isP2BackFromAForeignLandEventInPlay;
QString player2BackFromAForeignLandCivilizationBonusSelection;

// Declaring the variables, arrays for the UI elements
QStringList entityNames;
int         player1EntityQuantity;
int         player1AssistingEntityQuantity;
int         player2EntityQuantity;
int         player2AssistingEntityQuantity;
QString     player1Name;
QString     player2Name;
QString     player1Color;
QString     player2Color;
QString     player1BattleAssistantName;
QString     player2BattleAssistantName;
bool        expectingSingleEntityForPlayer1;
bool        expectingSingleEntityForPlayer2;

int representationOfPlayer1Age;
int representationOfPlayer2Age;

// Get what age the player is in
QString     player1Age;
QString     player2Age;
QStringList ages;

QStringList backFromAForeignLandCivilizationBonuses;

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow{parent}
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
  DialogInput::initialize(this);

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

  // Indicate that there's a hotkey for this in the tooltip
  ui.calculateResultsButton->setToolTip("<b>Hotkey:</b> R");

  // What the working directory is
  workingDirectory = QCoreApplication::applicationDirPath();

  // gets debug folder for some reason so go up a level
  workingDirectory.cdUp();

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

  // What the initial expectation is for number of entities allowed
  expectingSingleEntityForPlayer1 = false;
  expectingSingleEntityForPlayer2 = false;

  // What the initial player color of the players are
  player1Color = "black";
  player2Color = "black";

  initializeEntityAliases();
  entityNames << "Archer"
              << "Archer (Saracen)"
              << "Arbalest"
              << "Arbalest (Briton)"
              << "Arbalest (Saracen)"
              << "Archery Range"
              << "Barracks"
              << "Battering Ram"
              << "Battering Ram (Celt)"
              << "Berserk (Viking)"
              << "Blacksmith"
              << "Capped Ram"
              << "Capped Ram (Celt)"
              << "Castle"
              << "Camel"
              << "Castle (Frank)"
              << "Cavalier"
              << "Cavalier (Frank)"
              << "Cavalier (Persian)"
              << "Cavalry Archer"
              << "Cavalry Archer (Mongol)"
              << "Champion"
              << "Champion (Celt)"
              << "Champion (Goth)"
              << "Champion (Japanese)"
              << "Champion (Viking)"
              << "Charlamagne's Palace At Aix La'Chapelle (Briton)"
              << "Crossbowman"
              << "Crossbowman (Saracen)"
              << "Demolition Ship"
              << "Demolition Ship (Viking)"
              << "Dock"
              << "Dock (Persian)"
              << "Dock (Viking)"
              << "Elite Berserk (Viking)"
              << "Elite Huskarl (Goth)"
              << "Elite Longboat (Viking)"
              << "Elite Longbowman (Briton)"
              << "Elite Mameluke (Saracen)"
              << "Elite Mangudai (Mongol)"
              << "Elite Samurai (Japanese)"
              << "Elite Skirmisher"
              << "Elite Throwing Axeman (Frank)"
              << "Elite War Elephant (Persian)"
              << "Elite Woad Raider (Celt)"
              << "Farm"
              << "Fast Fire Ship"
              << "Fire Ship"
              << "Fishing Ship (Japanese)"
              << "Fishing Ship (Persian)"
              << "Fortified Wall"
              << "Galley"
              << "Galley (Japanese)"
              << "Galley (Saracen)"
              << "Galley (Viking)"
              << "Galleon"
              << "Galleon (Saracen)"
              << "Galleon (Viking)"
              << "Galleon (Japanese)"
              << "Gold Mine"
              << "Gold Mine (Japanese)"
              << "Heavy Camel"
              << "Heavy Cavalry Archer"
              << "Heavy Cavalry Archer (Mongol)"
              << "Heavy Cavalry Archer (Saracen)"
              << "Heavy Demolition Ship"
              << "Heavy Demolition Ship (Viking)"
              << "Heavy Scorpion"
              << "Heavy Scorpion (Celt)"
              << "House"
              << "Huskarl (Goth)"
              << "Knight"
              << "Knight (Frank)"
              << "Knight (Persian)"
              << "Light Cavalry"
              << "Light Cavalry (Mongol)"
              << "Long Swordsman"
              << "Long Swordsman (Celt)"
              << "Long Swordsman (Goth)"
              << "Long Swordsman (Japanese)"
              << "Long Swordsman (Viking)"
              << "Longboat (Viking)"
              << "Longbowman (Briton)"
              << "Lumber Camp"
              << "Lumber Camp (Japanese)"
              << "Mameluke (Saracen)"
              << "Man-at-Arms"
              << "Man-at-Arms (Viking)"
              << "Man-at-Arms (Celt)"
              << "Man-at-Arms (Goth)"
              << "Man-at-Arms (Japanese)"
              << "Mangonel"
              << "Mangonel (Celt)"
              << "Mangudai (Mongol)"
              << "Market"
              << "Market (Saracen)"
              << "Militia"
              << "Militia (Celt)"
              << "Militia (Goth)"
              << "Mill"
              << "Mill (Japanese)"
              << "Monastery"
              << "Notre-Dame Cathedral (Frank)"
              << "Onager"
              << "Onager (Celt)"
              << "Outpost"
              << "Paladin"
              << "Paladin (Persian)"
              << "Paladin (Frank)"
              << "Palisade Wall"
              << "Pikeman"
              << "Pikeman (Celt)"
              << "Pikeman (Goth)"
              << "Pikeman (Japanese)"
              << "Pikeman (Viking)"
              << "Rock Of Cashel (Celt)"
              << "Samurai (Japanese)"
              << "Scorpion"
              << "Scorpion (Celt)"
              << "Scout Cavalry"
              << "Siege Onager"
              << "Siege Onager (Celt)"
              << "Siege Ram"
              << "Siege Ram (Celt)"
              << "Siege Workshop"
              << "Skirmisher"
              << "Spearman"
              << "Spearman (Japanese)"
              << "Spearman (Viking)"
              << "Spearman (Celt)"
              << "Spearman (Goth)"
              << "Stable"
              << "Stave Church At Urnes (Viking)"
              << "Stone Gate"
              << "Stone Mine"
              << "Stone Mine (Japanese)"
              << "Stone Wall"
              << "The Golden Tent Of The Great Khan (Mongol)"
              << "The Great Temple At Nara (Japanese)"
              << "The Palace Of Ctesiphon On The Tigris (Persian)"
              << "Throwing Axeman (Frank)"
              << "Tomb Of Theodoric (Goth)"
              << "Town Center"
              << "Town Center (Briton)"
              << "Town Center (Persian)"
              << "Trebuchet"
              << "Two-handed Swordsman"
              << "Two-handed Swordsman (Celt)"
              << "Two-handed Swordsman (Goth)"
              << "Two-handed Swordsman (Japanese)"
              << "Two-handed Swordsman (Viking)"
              << "Villager"
              << "War Elephant (Persian)"
              << "War Galley"
              << "War Galley (Japanese)"
              << "War Galley (Saracen)"
              << "War Galley (Viking)"
              << "Watch Tower"
              << "Woad Raider (Celt)";

  // Sort the list in alphabetical order
  entityNames.sort();

  // What the possible names of technologies are
  // @Reference: What row in the .csv file it goes to
  QStringList technologies = {
    "Blast Furnace",        // [Row 1]
    "Bodkin Arrow",         // [Row 2]
    "Bracer",               // [Row 3]
    "Chain Barding Armor",  // [Row 4]
    "Chain Mail Armor",     // [Row 5]
    "Fletching",            // [Row 6]
    "Forging",              // [Row 7]
    "Hoardings",            // [Row 8]
    "Iron Casting",         // [Row 9]
    "Leather Archer Armor", // [Row 10]
    "Loom",                 // [Row 11]
    "Padded Archer Armor",  // [Row 12]
    "Plate Barding Armor",  // [Row 13]
    "Plate Mail Armor",     // [Row 14]
    "Ring Archer Armor",    // [Row 15]
    "Scale Barding Armor",  // [Row 16]
    "Scale Mail Armor",     // [Row 17]
    "Sanctity {2E}"};       // [Row 18]

  // Sort the list in alphabetical order
  technologies.sort();

  // What the possible names of event cards are
  // @Reference: What row in the .csv file it goes to
  QStringList events = {
    "A Just Cause",             // [Row 1]
    "Back From A Foreign Land", // [Row 2]  (Byzantine civ bonus: +2 healing
                                // rate modifier)
    // (has multiple slots in .csv file)
    "Barrel Of Grog",                // [Row 3]
    "Bone Shaft Arrows (Mongol)",    // [Row 4]
    "Caught From The Crow's Nest",   // [Row 5]
    "Celtic Battle Cry (Celt)",      // [Row 6]
    "Dangerous Times",               // [Row 7]
    "Fat Friar's Tavern O' Spirits", // [Row 8]
    "Field Testing",                 // [Row 9]
    "First Battle Jitters",          // [Row 10]
    "Flaming Arrows",                // [Row 11]
    "Fortune Favors The Foolish",    // [Row 12]
    "Gatherin' A Rowdy Bunch",       // [Row 13]
    "Gladitorial Games",             // [Row 14]
    "Hard To Starboard",             // [Row 15]
    "Heavy Tree Cover",              // [Row 16]
    "High Ground",                   // [Row 17]
    "Husbandry",                     // [Row 18]
    "It's A Miracle",                // [Row 19]
    "Listen To A Story",             // [Row 20]
    "Muddy Battlefield",             // [Row 21]
    "Non-Compos Mentis",             // [Row 22]
    // Back_From_A_Foreign_Land (Byzantine civ bonus:
    // All building get a HP bonus: Age I – 10 HP, Age II – 20 HP, Age III – 30
    // HP, Age IV – 40 HP) [Row 23]
    "Piety",                                     // [Row 24]
    "Black Knight",                              // [Row 25]
    "Rally The Workers",                         // [Row 26]
    "Relentless Attack",                         // [Row 27]
    "Retreat",                                   // [Row 28]
    "Holy War",                                  // [Row 29]
    "Shots In The Back (Briton)",                // [Row 30]
    "Soak The Timbers",                          // [Row 31]
    "Spirits Of The Ancestors",                  // [Row 32]
    "Squires",                                   // [Row 33]
    "Steady Hand",                               // [Row 34]
    "The Hammer's Cavalry (Franks)",             // [Row 35]
    "The Jester Is Dead Let's Get Them! (Celt)", // [Row 36]
    "Vengeance Is Mine!",                        // [Row 37]
    "While They're Sleeping",                    // [Row 38]
    "You Will Die! (Saracen)",                   // [Row 39]
    "Zealous Monks"};                            // [Row 40]
  // Back_From_A_Foreign_Land (Teuton civ bonus: Conversion rate modifier is -1)
  // [Row 41]

  // Sort the list in alphabetical order
  events.sort();

  // Populate the UI elements with elements
  // Both player 1 & 2 UI elements
  for (int i = 0; i < entityNames.size(); i++) {
    ui.player1EntityNames->addItem(entityNames[i]);
    ui.player2EntityNames->addItem(entityNames[i]);
  }

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

  ui.player1BattleAssistantNames->addItem("Monk");
  ui.player2BattleAssistantNames->addItem("Monk");

  // These are like placeholder (lorem ipsum) values
  // Player 1 UI elements starting state
  ui.player1EntityNamesFilter->setText("");
  ui.player1EntityQuantity->setValue(1);
  ui.player1EntityAssistantQuantity->setValue(0);

  // Player 2 UI elements starting state
  ui.player2EntityNamesFilter->setText("");
  ui.player2EntityQuantity->setValue(1);
  ui.player2EntityAssistantQuantity->setValue(0);

  ui.player1EntityAssistantQuantity->setRange(0, 5);
  ui.player2EntityAssistantQuantity->setRange(0, 5);

  // Read in the .csv files and update the UI elements on the basis of this
  setInitialNames();
  selectInitialEntities();
  selectInitialAssistants();
  markInitialPlayerMedievalAge();

  // Set up palettes
  palettes.setPaletteValues();
  palettes.darkModeEnabled = false;

  setColorTheUIElements();
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
  SFXToPlay("/sfx/ui/button_pressed.wav");

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
  // Get what entity names the user is entering
  QString player1EntityNamesFiltered = textInsideOfElement;

  // Clear what's in the list of entity names
  ui.player1EntityNames->clear();

  // Store name of filtered item
  QString nameOfFilteredItem;

  // Filter the list based on what entity name the user entered, factoring in
  // aliases for that entity name
  QStringList filteredList = filterEntityNames(player1EntityNamesFiltered);
  for (int y = 0; y < filteredList.size(); y++) {
    // Get the name of the filtered item
    nameOfFilteredItem = filteredList[y];

    // Add in the tooltips for the aliases so the user is aware of them
    QListWidgetItem* listWidgetItem = new QListWidgetItem(nameOfFilteredItem);
    QString listWidgetItemTooltip   = tooltipReturner(nameOfFilteredItem);
    if (listWidgetItemTooltip != "") {
      listWidgetItem->setToolTip(listWidgetItemTooltip);
    }

    ui.player1EntityNames->addItem(listWidgetItem);
  }
}

// Run this when the text inside of the player 2 entities search field changes
void MainWindow::on_player2EntityNamesFilter_textChanged(
  const QString& textInsideOfElement)
{
  // Get what entity names the user is entering
  QString player2EntityNamesFiltered = textInsideOfElement;

  // Clear what's in the list of entity names
  ui.player2EntityNames->clear();

  // Store name of filtered item
  QString nameOfFilteredItem;

  // Filter the list based on what entity name the user entered, factoring in
  // aliases for that entity name
  QStringList filteredList = filterEntityNames(player2EntityNamesFiltered);
  for (int y = 0; y < filteredList.size(); y++) {
    // Get the name of the filtered item
    nameOfFilteredItem = filteredList[y];

    // Add in the tooltips for the aliases so the user is aware of them
    QListWidgetItem* listWidgetItem = new QListWidgetItem(nameOfFilteredItem);
    QString listWidgetItemTooltip   = tooltipReturner(nameOfFilteredItem);
    if (listWidgetItemTooltip != "") {
      listWidgetItem->setToolTip(listWidgetItemTooltip);
    }

    ui.player2EntityNames->addItem(listWidgetItem);
  }
}

// Run this on click of Help > Documentation > Developer guide
void MainWindow::on_actionDeveloper_guide_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  // Set the path to it
  QString fileName = "/documentation/developer_guide.docx";
  QString filePath = workingDirectory.absolutePath() + fileName;

  // Open that path
  QDesktopServices::openUrl(filePath);
}

// Run this on click of Help > Documentation > Developer wishlist
void MainWindow::on_actionDeveloper_wishlist_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  // Set the path to it
  QString fileName = "/documentation/developer_wishlist.docx";
  QString filePath = workingDirectory.absolutePath() + fileName;

  // Open that path
  QDesktopServices::openUrl(filePath);
}

// Run this on click of Help > Documentation > User guide
void MainWindow::on_actionUser_guide_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  // Set the path to it
  QString fileName = "/documentation/user_guide.docx";
  QString filePath = workingDirectory.absolutePath() + fileName;

  // Open that path
  QDesktopServices::openUrl(filePath);
}

// Run on click of the calculate results button
void MainWindow::on_calculateResultsButton_clicked()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  ui.gameOutputTextEdit->setHtml("");

  // Calculate the results of a battle
  runGame();
}

// Run this when the value inside of the player 1 entity quantities field
// changes
void MainWindow::on_player1EntityQuantity_valueChanged(int valueInsideOfField)
{
  // Get what entity quantity the user is entering
  player1EntityQuantity = valueInsideOfField;

  // Give an error to Console if quantity isn't right
  if (
    player1EntityQuantity != 1 && player1EntityQuantity != 2
    && player1EntityQuantity != 3 && player1EntityQuantity != 4
    && player1EntityQuantity != 5) {
    qDebug()
      << "Error: Player 1's entity quantity input should be between 1 and 5";
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
  if (
    player2EntityQuantity != 1 && player2EntityQuantity != 2
    && player2EntityQuantity != 3 && player2EntityQuantity != 4
    && player2EntityQuantity != 5) {
    qDebug()
      << "Error: Player 2's entity quantity input should be between 1 and 5";
  }

  m_entities.changePlayer2EntityQuantity(player2EntityQuantity);
}

// Run on change of what battle assistant is selected by player 1
void MainWindow::on_player1BattleAssistantNames_textActivated(
  const QString& currentSelection)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  player1BattleAssistantName = currentSelection;

  m_entities.changePlayer1AssistantName(player1BattleAssistantName);
}

// Run on change of what battle assistant is selected by player 2
void MainWindow::on_player2BattleAssistantNames_textActivated(
  const QString& currentSelection)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  player2BattleAssistantName = currentSelection;

  m_entities.changePlayer2AssistantName(player2BattleAssistantName);
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

QString MainWindow::convertSpacesToUnderscores(QString text) const
{
  std::replace(text.begin(), text.end(), ' ', '_');
  return text;
}

QString MainWindow::convertUnderscoresToSpaces(QString text) const
{
  std::replace(text.begin(), text.end(), '_', ' ');
  return text;
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

void MainWindow::updateRangeAllowed(QString nameOfSelection, int playerNumber)
{
  nameOfSelection = nameOfSelection.toUpper();
  nameOfSelection = convertSpacesToUnderscores(nameOfSelection);

  if (
    (nameOfSelection.contains("ARCHERY_RANGE"))
    || (nameOfSelection.contains("BARRACKS"))
    || (nameOfSelection.contains("BLACKSMITH"))
    || (nameOfSelection.contains("RAM")) || (nameOfSelection.contains("CASTLE"))
    || (nameOfSelection.contains(
      "CHARLAMAGNE'S_PALACE_AT_AIX_LA'CHAPELLE_(BRITON)"))
    || (nameOfSelection.contains("ROCK_OF_CASHEL_(CELT)"))
    || (nameOfSelection.contains("THE_GOLDEN_TENT_OF_THE_GREAT_KHAN_(MONGOL)"))
    || (nameOfSelection.contains(
      "THE_PALACE_OF_CTESIPHON_ON_THE_TIGRIS_(PERSIAN)"))
    || (nameOfSelection.contains("TOMB_OF_THEODORIC_(GOTH)"))
    || (nameOfSelection.contains("NOTRE-DAME_CATHEDRAL_(FRANK)"))
    || (nameOfSelection.contains("STAVE_CHURCH_AT_URNES_(VIKING)"))
    || (nameOfSelection.contains("THE_GREAT_TEMPLE_AT_NARA_(JAPANESE)"))
    || (nameOfSelection.contains("DEMOLITION_SHIP"))
    || (nameOfSelection.contains("DOCK"))
    || (nameOfSelection.contains("FIRE_SHIP"))
    || (nameOfSelection.contains("FISHING_SHIP"))
    || (nameOfSelection.contains("WALL"))
    || (nameOfSelection.contains("GALLEY"))
    || (nameOfSelection.contains("HERO"))
    || // May implement this as an assisting unit
    (nameOfSelection.contains("GATE")) || (nameOfSelection.contains("GALLEON"))
    || (nameOfSelection.contains("GOLD_MINE"))
    || (nameOfSelection.contains("HOUSE"))
    || (nameOfSelection.contains("LONGBOAT_(VIKING)"))
    || (nameOfSelection.contains("LUMBER_CAMP"))
    || (nameOfSelection.contains("MANGONEL"))
    || (nameOfSelection.contains("MARKET"))
    || (nameOfSelection.contains("MILL"))
    || (nameOfSelection.contains("MONASTERY"))
    || (nameOfSelection.contains("ONAGER"))
    || (nameOfSelection.contains("OUTPOST"))
    || (nameOfSelection.contains("SCORPION"))
    || (nameOfSelection.contains("SIEGE_WORKSHOP"))
    || (nameOfSelection.contains("STABLE"))
    || (nameOfSelection.contains("STONE_MINE"))
    || (nameOfSelection.contains("TOWN_CENTER"))
    || (nameOfSelection.contains("TREBUCHET"))
    || (nameOfSelection.contains("WAR_ELEPHANT_(PERSIAN)"))
    || (nameOfSelection.contains("WATCH_TOWER"))) {
    if (playerNumber == 1) {
      expectingSingleEntityForPlayer1 = true;
    }
    if (playerNumber == 2) {
      expectingSingleEntityForPlayer2 = true;
    }
  }
  else {
    if (playerNumber == 1) {
      expectingSingleEntityForPlayer1 = false;
    }
    if (playerNumber == 2) {
      expectingSingleEntityForPlayer2 = false;
    }
  }

  if (expectingSingleEntityForPlayer1 == true) {
    ui.player1EntityQuantity->setRange(1, 1);
  }
  else {
    ui.player1EntityQuantity->setRange(1, 5);
  }

  if (expectingSingleEntityForPlayer2 == true) {
    ui.player2EntityQuantity->setRange(1, 1);
  }
  else {
    ui.player2EntityQuantity->setRange(1, 5);
  }
}

// Run on change of what battle participant is selected by player 1
void MainWindow::on_player1EntityNames_itemClicked(
  QListWidgetItem* selectedItem)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  updateRangeAllowed(selectedItem->text(), 1);

  QString currentSelectionFormatted
    = convertSpacesToUnderscores(selectedItem->text());
  currentSelectionFormatted = currentSelectionFormatted.toUpper();
  m_player1EntityName       = currentSelectionFormatted;

  m_entities.changePlayer1EntityName(m_player1EntityName);
}

void MainWindow::on_player2EntityNames_itemClicked(
  QListWidgetItem* selectedItem)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  updateRangeAllowed(selectedItem->text(), 2);

  QString currentSelectionFormatted
    = convertSpacesToUnderscores(selectedItem->text());
  currentSelectionFormatted = currentSelectionFormatted.toUpper();
  m_player2EntityName       = currentSelectionFormatted;

  m_entities.changePlayer2EntityName(m_player2EntityName);
}

// Input validation. Superior technologies take the place of lesser technologies
void MainWindow::overrideTechnologies(QStringList technologiesToCancelOut, Technologies * playerTechnologies){
  for(int i = 0; i < technologiesToCancelOut.size(); i++){
    QString technologyNameBackend = convertSpacesToUnderscores(technologiesToCancelOut[i]);

    // Disable it in file
    if(playerTechnologies->isActive(technologyNameBackend)){
      playerTechnologies->disable(technologyNameBackend);
    }

    // Disable it in the GUI
    QList<QListWidgetItem *> list = ui.player1Technologies->findItems(technologiesToCancelOut[i], Qt::MatchExactly);
    for ( QListWidgetItem *item : list ){
      item->setCheckState(Qt::Unchecked);
    }
  }
}

// Should have the listCombinations (as input) in order of what overrides what
void MainWindow::technologyOverrider(QStringList listCombinations, Technologies * playerTechnologies){
  QStringList combinationsFound;

  for(int i = 0; i < listCombinations.size(); i ++){
    if(playerTechnologies->isActive(convertSpacesToUnderscores(listCombinations[i]))){

      // Get all other combinations
      for (int y = 0; y < listCombinations.size(); y++){
        if(listCombinations[i] != listCombinations[y]){
          combinationsFound.append(listCombinations[y]);
        }
      }

      overrideTechnologies(combinationsFound, playerTechnologies);
    }
  }

  combinationsFound.clear(); // @Phillip: Is this even necessary?
}


// Run on change of what technologies are toggled by player 1
void MainWindow::on_player1Technologies_itemChanged(
  QListWidgetItem* checkedItem)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  QString technology = checkedItem->text();
  technology         = convertSpacesToUnderscores(technology);

  if (checkedItem->checkState() == Qt::Checked) {
    m_player1Technologies.enable(technology);
  }
  else {
    m_player1Technologies.disable(technology);
  }

  /* Input validation - Up to 1 selected */
  technologyOverrider({"Bracer", "Bodkin Arrow", "Fletching"}, &m_player1Technologies);
  technologyOverrider({"Blast Furnace", "Iron Casting", "Forging"}, &m_player1Technologies);
  technologyOverrider({"Plate Barding Armor", "Chain Barding Armor", "Scale Barding Armor"}, &m_player1Technologies);
  technologyOverrider({"Ring Archer Armor", "Leather Archer Armor", "Padded Archer Armor"}, &m_player1Technologies);
}

// Run on change of what events are toggled by player 1
void MainWindow::on_player1Events_itemChanged(QListWidgetItem* checkedItem)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

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
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  QString technology = checkedItem->text();
  technology         = convertSpacesToUnderscores(technology);

  if (checkedItem->checkState() == Qt::Checked) {
    m_player2Technologies.enable(technology);
  }
  else {
    m_player2Technologies.disable(technology);
  }

  /* Input validation - Up to 1 selected */
  technologyOverrider({"Bracer", "Bodkin Arrow", "Fletching"}, &m_player2Technologies);
  technologyOverrider({"Blast Furnace", "Iron Casting", "Forging"}, &m_player2Technologies);
  technologyOverrider({"Plate Barding Armor", "Chain Barding Armor", "Scale Barding Armor"}, &m_player2Technologies);
  technologyOverrider({"Ring Archer Armor", "Leather Archer Armor", "Padded Archer Armor"}, &m_player2Technologies);
}

void MainWindow::on_player2Events_itemChanged(QListWidgetItem* checkedItem)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

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
      QIcon(workingDirectory.absolutePath() + checkedIconInvertedFilename));
    ui.actionEnableDisableDarkMode->setIconVisibleInMenu(true);

    if (soundEffectsEnabled == false) {
      ui.actionEnableDisableSFX->setIcon(
        QIcon(workingDirectory.absolutePath() + checkedIconInvertedFilename));
      ui.actionEnableDisableSFX->setIconVisibleInMenu(true);
    }
    else {
      ui.actionEnableDisableSFX->setIconVisibleInMenu(false);
    }
  }
  else if (palettes.darkModeEnabled == false) {
    ui.actionEnableDisableDarkMode->setIconVisibleInMenu(false);

    if (soundEffectsEnabled == false) {
      ui.actionEnableDisableSFX->setIcon(
        QIcon(workingDirectory.absolutePath() + checkedIconFilename));
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
      workingDirectory.absolutePath() + closeProgramIconInvertedFilename));

    ui.menuOptions->setIcon(QIcon(
      workingDirectory.absolutePath() + programOptionsIconInvertedFilename));

    ui.menuDocumentation->setIcon(QIcon(
      workingDirectory.absolutePath() + documentationInvertedIconFilename));

    ui.actionAbout->setIcon(
      QIcon(workingDirectory.absolutePath() + aboutInvertedIconFilename));

    ui.actionSet_player_1_Age->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilename));
    ui.actionSet_name_of_player_1->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilename));
    ui.actionSet_set_color_of_player_1->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilename));
    ui.actionSet_player_2_Age->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilename));
    ui.actionSet_name_of_player_2->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilename));
    ui.actionSet_set_color_of_player_2->setIcon(QIcon(
      workingDirectory.absolutePath() + playerDetailsIconInvertedFilename));

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
      QIcon(workingDirectory.absolutePath() + closeProgramIconFilename));
    ui.menuOptions->setIcon(
      QIcon(workingDirectory.absolutePath() + programOptionsIconFilename));

    ui.menuDocumentation->setIcon(
      QIcon(workingDirectory.absolutePath() + documentationIconFilename));

    ui.actionAbout->setIcon(
      QIcon(workingDirectory.absolutePath() + aboutIconFilename));

    ui.actionSet_player_1_Age->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilename));
    ui.actionSet_name_of_player_1->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilename));
    ui.actionSet_set_color_of_player_1->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilename));
    ui.actionSet_player_2_Age->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilename));
    ui.actionSet_name_of_player_2->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilename));
    ui.actionSet_set_color_of_player_2->setIcon(
      QIcon(workingDirectory.absolutePath() + playerDetailsIconFilename));

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
  SFXToPlay("/sfx/ui/button_pressed.wav");

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
  SFXToPlay("/sfx/ui/button_pressed.wav");

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

// Run on change of "Options" > "Set player 1's color"
void MainWindow::on_actionSet_set_color_of_player_1_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QColorDialog colorDialog;
  colorDialog.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player1Name) + "'s player color");
  colorDialog.setStyleSheet(palettes.getColorDialogBoxStyling());
  colorDialog.exec();

  QColor color = colorDialog.currentColor();

  player1Color = color.name();

  updatePlayerNames();
}

// Run on change of "Options" > "Set player 2's color"
void MainWindow::on_actionSet_set_color_of_player_2_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QColorDialog colorDialog;
  colorDialog.setWindowTitle(
    "Enter " + convertUnderscoresToSpaces(player2Name) + "'s player color");
  colorDialog.setStyleSheet(palettes.getColorDialogBoxStyling());
  colorDialog.exec();

  QColor color = colorDialog.currentColor();

  player2Color = color.name();

  updatePlayerNames();
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

static std::vector<QString> findMatches(
  const QList<QString>& haystack,
  const QString&        needle)
{
  std::vector<QString> result{};
  result.reserve(haystack.size());
  std::copy_if(
    haystack.begin(),
    haystack.end(),
    std::back_inserter(result),
    [&needle](const QString& hay) {
      return hay.contains(needle, Qt::CaseInsensitive);
    });
  return result;
}

QStringList MainWindow::filterEntityNames(QString input) const
{
  std::vector<QString> directMatches{findMatches(entityNames, input)};
  QSet<QString>        filteredEntities(
    std::make_move_iterator(directMatches.begin()),
    std::make_move_iterator(directMatches.end()));
  const QList<QString>       aliases{m_aliases.getAllAliases()};
  const std::vector<QString> indirectMatches{findMatches(aliases, input)};

  for (const QString& alias : indirectMatches) {
    const QList<QString> entities{m_aliases.entityOf(alias)};

    for (const QString& entity : entities) {
      filteredEntities.insert(entity);
    }
  }

  QList<QString> result{filteredEntities.values()};
  result.sort();
  return result;
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

    //@Phillip: For some reason this returns 1
    qDebug() << m_entities.player1Entity().entityQuantity();

    updateRangeAllowed(m_entities.player1Entity().entityName(), 1);
  }

  if (player2SelectedEntity != nullptr) {
    ui.player2EntityNames->setCurrentItem(player2SelectedEntity);
    ui.player2EntityNames->scrollToItem(player2SelectedEntity);
    ui.player2EntityQuantity->setValue(
      m_entities.player2Entity().entityQuantity());

    updateRangeAllowed(m_entities.player2Entity().entityName(), 2);
  }
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
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  soundEffectsEnabled = !soundEffectsEnabled;

  setColorTheToggleElements();
}

// Run on change of "Program" > "Options" > "Enable dark mode" toggle
void MainWindow::on_actionEnableDisableDarkMode_triggered()
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");
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

// Included files
#include "mainwindow.hpp"     // This window
#include "aboutwindow.h"      // A window this window can open
#include "backend/run_game.h" // Age of Empires combat results calculator v1.2
#include "soundEffects.h"     // Sound playing class

// Libraries used for std::cout
#include <algorithm>
#include <iostream>

// Libraries used for debugging
#include <QDebug>

// Libraries used for accessing file paths
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>

// Libraries used for storing data
#include <QColor>
#include <QString>
#include <QStringList>

// Libraries for acquiring user input
#include <QColorDialog>
#include <QInputDialog>

// Declaring class
SoundPlayer playSound;
bool        soundEffectsEnabled = true;

// Declaring the file paths for @Phillip
QString entitiesFilename       = "/import/entities.csv";
QString eventsP1Filename       = "/import/events_p1.csv";
QString eventsP2Filename       = "/import/events_p2.csv";
QString playerDetailsFilename  = "/import/playerDetails.csv";
QString technologiesP1Filename = "/import/technologies_p1.csv";
QString technologiesP2Filename = "/import/technologies_p2.csv";

// Declaring the variables, arrays for the UI elements
QString     player1EntityNamesFiltered = "";
QStringList entityNames;
QString     player1EntityQuantity = "0";
QString     player1MonkQuantity   = "0";
QString     player1Name;
QString     player2Name;
QString     player1Color;
QString     player2Color;

// Declare working directory
QDir workingDirectory;

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow{parent}
  , m_gameOutputBuffer{}
  , m_streamBuffer{std::cout, [this](char ch) {
                     m_gameOutputBuffer.append(QString{QChar{ch}});
                     ui.gameOutputTextEdit->setText(m_gameOutputBuffer);
                   }}
{
  ui.setupUi(this);

  // What the working directory is
  workingDirectory = QCoreApplication::applicationDirPath();

  // gets debug folder for some reason so go up a level
  workingDirectory.cdUp();
  qDebug() << workingDirectory;

  // What the initial name of the players are
  player1Name = "Player 1";
  player2Name = "Player 2";

  // What the initial player color of the players are
  player1Color = "black";
  player2Color = "black";

  initializeEntityAliases();
  entityNames << "Archer"
              << "Archer_(Saracen)"
              << "Arbalest"
              << "Arbalest_(Briton)"
              << "Arbalest_(Saracen)"
              << "Archery_Range"
              << "Barracks"
              << "Battering_Ram"
              << "Battering_Ram_(Celt)"
              << "Berserk_(Viking)"
              << "Blacksmith"
              << "Capped_Ram"
              << "Capped_Ram_(Celt)"
              << "Castle"
              << "Camel"
              << "Castle_(Frank)"
              << "Cavalier"
              << "Cavalier_(Frank)"
              << "Cavalier_(Persian)"
              << "Cavalry_Archer"
              << "Cavalry_Archer_(Mongol)"
              << "Champion"
              << "Champion_(Celt)"
              << "Champion_(Goth)"
              << "Champion_(Japanese)"
              << "Champion_(Viking)"
              << "Charlamagne's_Palace_At_Aix_La'Chapelle_(Briton)"
              << "Crossbowman"
              << "Crossbowman_(Saracen)"
              << "Demolition_Ship"
              << "Demolition_Ship_(Viking)"
              << "Dock"
              << "Dock_(Persian)"
              << "Dock_(Viking)"
              << "Elite_Berserk_(Viking)"
              << "Elite_Huskarl_(Goth)"
              << "Elite_Longboat_(Viking)"
              << "Elite_Longbowman_(Briton)"
              << "Elite_Mameluke_(Saracen)"
              << "Elite_Manugdai_(Mongol)"
              << "Elite_Samurai_(Japanese)"
              << "Elite_Skirmisher"
              << "Elite_Throwing_Axeman_(Frank)"
              << "Elite_War_Elephant_(Persian)"
              << "Elite_Woad_Raider_(Celt)"
              << "Farm"
              << "Fast_Fire_Ship"
              << "Fire_Ship"
              << "Fishing_Ship_(Japanese)"
              << "Fishing_Ship_(Persian)"
              << "Fortified_Wall"
              << "Galley"
              << "Galley_(Japanese)"
              << "Galley_(Saracen)"
              << "Galley_(Viking)"
              << "Galleon"
              << "Galleon_(Saracen)"
              << "Galleon_(Viking)"
              << "Galleon_(Japanese)"
              << "Gold_Mine"
              << "Gold_Mine_(Japanese)"
              << "Heavy_Camel"
              << "Heavy_Cavalry_Archer"
              << "Heavy_Cavalry_Archer_(Mongol)"
              << "Heavy_Cavalry_Archer_(Saracen)"
              << "Heavy_Demolition_Ship"
              << "Heavy_Demolition_Ship_(Viking)"
              << "Heavy_Scorpion"
              << "Heavy_Scorpion_(Celt)"
              << "House"
              << "Huskarl_(Goth)"
              << "Knight"
              << "Knight_(Frank)"
              << "Knight_(Persian)"
              << "Light_Cavalry"
              << "Light_Cavalry_(Mongol)"
              << "Long_Swordsman"
              << "Long_Swordsman_(Celt)"
              << "Long_Swordsman_(Goth)"
              << "Long_Swordsman_(Japanese)"
              << "Long_Swordsman_(Viking)"
              << "Longboat_(Viking)"
              << "Longbowman_(Briton)"
              << "Lumber_Camp"
              << "Lumber_Camp_(Japanese)"
              << "Mameluke_(Saracen)"
              << "Man-at-Arms"
              << "Man-at-Arms_(Viking)"
              << "Man-at-Arms_(Celt)"
              << "Man-at-Arms_(Goth)"
              << "Man-at-Arms_(Japanese)"
              << "Mangonel"
              << "Mangonel_(Celt)"
              << "Mangudai_(Mongol)"
              << "Market"
              << "Market_(Saracen)"
              << "Militia"
              << "Militia_(Celt)"
              << "Militia_(Goth)"
              << "Mill"
              << "Mill_(Japanese)"
              << "Monastery"
              << "Onager"
              << "Onager_(Celt)"
              << "Outpost"
              << "Paladin"
              << "Paladin_(Persian)"
              << "Paladin_(Frank)"
              << "Palisade_Wall"
              << "Pikeman"
              << "Pikeman_(Celt)"
              << "Pikeman_(Goth)"
              << "Pikeman_(Japanese)"
              << "Pikeman_(Viking)"
              << "Samurai_(Japanese)"
              << "Scorpion"
              << "Scorpion_(Celt)"
              << "Scout_Cavalry"
              << "Siege_Onager"
              << "Siege_Onager_(Celt)"
              << "Siege_Ram"
              << "Siege_Ram_(Celt)"
              << "Siege_Workshop"
              << "Skirmisher"
              << "Spearman"
              << "Spearman_(Japanese)"
              << "Spearman_(Viking)"
              << "Spearman_(Celt)"
              << "Spearman_(Goth)"
              << "Stable"
              << "Stone_Gate"
              << "Stone_Mine"
              << "Stone_Mine_(Japanese)"
              << "Stone_Wall"
              << "Throwing_Axeman_(Frank)"
              << "Town_Center"
              << "Town_Center_(Briton)"
              << "Town_Center_(Persian)"
              << "Trebuchet"
              << "Two-handed_Swordsman"
              << "Two-handed_Swordsman_(Celt)"
              << "Two-handed_Swordsman_(Goth)"
              << "Two-handed_Swordsman_(Japanese)"
              << "Two-handed_Swordsman_(Viking)"
              << "Villager"
              << "War_Elephant_(Persian)"
              << "War_Galley"
              << "War_Galley_(Japanese)"
              << "War_Galley_(Saracen)"
              << "War_Galley_(Viking)"
              << "Watch_Tower"
              << "Woad_Raider_(Celt)";

  // Sort the list in alphabetical order
  entityNames.sort();

  // What the possible names of technologies are
  QStringList technologies = {
    "Blast Furnace",
    "Bodkin Arrow",
    "Bracer",
    "Chain Barding Armor",
    "Chain Mail Armor",
    "Fletching",
    "Forging",
    "Hoardings",
    "Iron Casting",
    "Leather Archer Armor",
    "Loom",
    "Padded Archer Armor",
    "Plate Barding Armor",
    "Plate Mail Armor",
    "Ring Archer Armor",
    "Scale Barding Armor",
    "Scale Mail Armor",
    "Sanctity (unofficial)"};

  // What the possible names of event cards are
  QStringList events = {
    "A Just Cause",
    "Back From A Foreign Land (unimplemented)",
    "Barrel Of Grog",
    "Bone Shaft Arrows (Mongol)",
    "Caught From The Crow's Nest",
    "Celtic Battle Cry (Celt)",
    "Dangerous Times",
    "Fat Friar's Tavern O' Spices",
    "Field Testing",
    "First Battle Jitters",
    "Flaming Arrows",
    "Fortune Favors The Foolish",
    "Gatherin' A Rowdy Bunch",
    "Gladitorial Games",
    "Hard To Starboard",
    "Heavy Tree Cover",
    "High Ground",
    "Husbandry",
    "It's A Miracle",
    "Listen To A Story",
    "Muddy Battlefield",
    "Non-Compos Mentis",
    "Spare slot",
    "Piety",
    "Black Knight (unimplemented)",
    "Rally The Workers",
    "Relentless Attack",
    "Retreat",
    "Holy War (unimplemented)",
    "Shots In The Back (Briton)",
    "Soak The Timbers",
    "Spirits Of The Ancestors",
    "Squires",
    "Steady Hand",
    "The Hammer's Cavalry (Franks)",
    "The Jester Is Dead Let's Get Them! (Celt)",
    "Vengeance Is Mine!",
    "While They're Sleeping",
    "You Will Die! (Saracen)",
    "Zealous Monks"};

  // Set the initial state of the UI elements
  // These are like placeholder (lorem ipsum) values

  // Both player 1 & 2 UI elements
  for (int i = 0; i < entityNames.size(); i++) {
    ui.player1EntityNames->addItem(entityNames[i]);
    // add ui.player2EntityNames->addItem(entityNames[i]);
  }

  for (int z = 0; z < technologies.length(); z++) {
    QListWidgetItem* technology = new QListWidgetItem(technologies[z]);
    technology->setData(Qt::CheckStateRole, Qt::Unchecked);
    ui.player1Technologies->addItem(technology);
    // add ui.player1Technologies->addItem(technology);
  }

  for (int eV = 0; eV < events.length(); eV++) {
    QListWidgetItem* event = new QListWidgetItem(events[eV]);
    event->setData(Qt::CheckStateRole, Qt::Unchecked);
    ui.player1Events->addItem(event);
    // add ui.player2Events->addItem(event);
  }

  // Player 1 UI elements
  ui.player1EntityQuantity->setText("1");
  ui.player1EntityNamesFilter->setText("");
  ui.player1MonkQuantity->setText("0");
  ui.player1BattleAssistantNames->addItem("Monk");

  // Player 2 UI elements
}

MainWindow::~MainWindow()
{
}

// Run this when there's a call to play a button SFX
void SFXToPlay(QString filePath)
{
  // Play SFX if SFX is enabled
  if (soundEffectsEnabled == true) {
    playSound.fileLocation = workingDirectory.absolutePath() + filePath;
    // playSound.playSoundEffect();
  }
  else {
    // Do nothing
  }
}

// Run this on click of the exit button
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

// Run this when the text inside of the player 1 entities search field changes
void MainWindow::on_player1EntityNamesFilter_textChanged()
{
  // Get what entity names the user is entering
  player1EntityNamesFiltered = ui.player1EntityNamesFilter->text();

  // Clear what's in the list of entity names
  ui.player1EntityNames->clear();

  // Filter the list based on what entity name the user entered
  QStringList filteredList = filterEntityNames(player1EntityNamesFiltered);
  for (int y = 0; y < filteredList.size(); y++) {
    ui.player1EntityNames->addItem(filteredList[y]);
  }

  // TODO: Add something to filter the entityNames, considering the aliases.
  // TODO: Add Charlemagne's thing for Wonder
}

// Run this when the text inside of the player 1 quantities field changes
void MainWindow::on_player1EntityQuantity_textChanged()
{
  // Get what entity quantity the user is entering
  player1EntityQuantity = ui.player1EntityQuantity->text();

  // Give an error to Console if quantity isn't right
  if (
    player1EntityQuantity != "1" && player1EntityQuantity != "2"
    && player1EntityQuantity != "3" && player1EntityQuantity != "4"
    && player1EntityQuantity != "5") {
    qDebug()
      << "Error: Player 1's entity quantity input should be between 1 and 4";
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

  ui.gameOutputTextEdit->setPlainText("");

  // Calculate the results of a battle
  runGame();
}

// Run on change of what battle assistant is selected by player 1
void MainWindow::on_player1BattleAssistantNames_activated(int index)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");
}

// Run on change of what battle participant is selected by player 1
void MainWindow::on_player1EntityNames_itemClicked(QListWidgetItem* item)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");
}

// Run on change of what technologies are toggled by player 1
void MainWindow::on_player1Technologies_itemChanged(QListWidgetItem* item)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");
}

// Run on change of what events are toggled by player 1
void MainWindow::on_player1Events_itemChanged(QListWidgetItem* item)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");
}

// Run on change of "Options" > "Disable SFX" toggle
void MainWindow::on_actionDisable_SFX_triggered()
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  if (soundEffectsEnabled == true) {
    soundEffectsEnabled = false;
  }
  else {
    soundEffectsEnabled = true;
  }
}

// Run this when there's a call to update the names and colors of the players
void MainWindow::updatePlayerNames(
  QString updatedPlayer1Name,
  QString updatedPlayer2Name)
{
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
}

// Run on change of "Options" > "Set player 1's name"
void MainWindow::on_actionSet_name_of_player_1_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  bool ok;
  player1Name = QInputDialog::getText(
    this,
    tr("Enter player 1's name"),
    tr("Player 1's name:"),
    QLineEdit::Normal,
    "",
    &ok);

  // Validate the user input
  if (player1Name.isEmpty()) {
    player1Name = "Player 1";
  }

  updatePlayerNames(player1Name, player2Name);
}

// Run on change of "Options" > "Set player 1's color"
void MainWindow::on_actionSet_set_color_of_player_1_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QColor color = QColorDialog::getColor();
  player1Color = color.name();

  updatePlayerNames(player1Name, player2Name);
}

// Run on change of "Options" > "Set player 2's name"
void MainWindow::on_actionSet_name_of_player_2_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  bool ok;
  player2Name = QInputDialog::getText(
    this,
    tr("Enter player 2's name"),
    tr("Player 2's name:"),
    QLineEdit::Normal,
    "",
    &ok);

  // Validate the user input
  if (player2Name.isEmpty()) {
    player2Name = "Player 2";
  }

  updatePlayerNames(player1Name, player2Name);
}

// Run on change of "Options" > "Set player 2's color"
void MainWindow::on_actionSet_set_color_of_player_2_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QColor color = QColorDialog::getColor();
  player2Color = color.name();

  updatePlayerNames(player1Name, player2Name);
}

void MainWindow::initializeEntityAliases()
{
  m_entityAliases.insert(
    "CHARLAMAGNE'S_PALACE_AT_AIX_LA'CHAPELLE_(BRITON)",
    QStringList{"WONDER_(BRITON)"});
  m_entityAliases.insert("ROCK_OF_CASHEL_(CELT)", QStringList{"WONDER_(CELT)"});
  m_entityAliases.insert(
    "THE_GOLDEN_TENT_OF_THE_GREAT_KHAN_(MONGOL)",
    QStringList{"WONDER_(MONGOL)"});
  m_entityAliases.insert(
    "THE_PALACE_OF_CTESIPHON_ON_THE_TIGRIS_(PERSIAN)",
    QStringList{"WONDER_(PERSIAN)"});
  m_entityAliases.insert(
    "TOMB_OF_THEODORIC_(GOTH)", QStringList{"WONDER_(GOTH)"});
  m_entityAliases.insert(
    "NOTRE-DAME_CATHEDRAL_(FRANK)", QStringList{"WONDER_(FRANK)"});
  m_entityAliases.insert(
    "STAVE_CHURCH_AT_URNES_(VIKING)", QStringList{"WONDER_(VIKING)"});
  m_entityAliases.insert(
    "THE_GREAT_TEMPLE_AT_NARA_(JAPANESE)", QStringList{"WONDER_(JAPANESE)"});
}

QStringList MainWindow::filterEntityNames(QString input) const
{
  QStringList filteredEntities{};

  for (const QString& entity : entityNames) {
    const QHash<QString, QStringList>::const_iterator it{
      m_entityAliases.find(entity.toUpper())};
    const QStringList aliases{[this, it] {
      if (it == m_entityAliases.end()) {
        return QStringList{};
      }

      return it.value();
    }()};

    const bool isDirectMatch{entity.contains(input, Qt::CaseInsensitive)};
    const bool hasAliases{!aliases.isEmpty()};
    const bool hasAliasMatch{
      hasAliases
      && std::any_of(
        aliases.begin(), aliases.end(), [&input](const QString& alias) {
          return alias.contains(input, Qt::CaseInsensitive);
        })};

    if (isDirectMatch || hasAliasMatch) {
      filteredEntities << entity;
    }
  }

  return filteredEntities;
}

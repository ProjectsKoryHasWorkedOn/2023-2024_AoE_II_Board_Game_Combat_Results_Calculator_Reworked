// Included files
#include "mainwindow.hpp"     // This window
#include "aboutwindow.h"      // A window this window can open
#include "backend/run_game.h" // Age of Empires combat results calculator v1.2
#include "soundEffects.h"     // Sound playing class

// Libraries used for std::cout
#include <algorithm>
#include <iostream>
#include <vector>

// Libraries used for debugging
#include <QDebug>
#include <QSet>

// Libraries used for accessing file paths
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>

// Libraries used for storing data
#include <QColor>
#include <QListWidgetItem>
#include <QString>
#include <QStringList>

// Libraries for acquiring user input
#include <QColorDialog>
#include <QInputDialog>

// Declaring class
SoundPlayer playSound;
bool        soundEffectsEnabled = true;

// Declaring the file paths for @Phillip (unused for now)
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
  , m_aliases{}
{
  ui.setupUi(this);

  // What the working directory is
  workingDirectory = QCoreApplication::applicationDirPath();

  // gets debug folder for some reason so go up a level
  workingDirectory.cdUp();

  // What the initial name of the players are
  player1Name = "Player 1";
  player2Name = "Player 2";

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
              << "Elite Manugdai (Mongol)"
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

MainWindow::~MainWindow() {}

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
    if (
      nameOfFilteredItem
      == "Charlamagne's Palace At Aix La'Chapelle (Briton)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Briton)");
    }
    else if (nameOfFilteredItem == "Rock Of Cashel (Celt)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Celt)");
    }
    else if (
      nameOfFilteredItem == "The Golden Tent Of The Great Khan (Mongol)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Mongol)");
    }
    else if (
      nameOfFilteredItem == "The Palace Of Ctesiphon On The Tigris (Persian)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Persian)");
    }
    else if (nameOfFilteredItem == "Tomb Of Theodoric (Goth)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Goth)");
    }
    else if (nameOfFilteredItem == "Notre-Dame Cathedral (Frank)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Frank)");
    }
    else if (nameOfFilteredItem == "Stave Church At Urnes (Viking)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Viking)");
    }
    else if (nameOfFilteredItem == "The Great Temple At Nara (Japanese)") {
      listWidgetItem->setToolTip("<b>Aliases:</b> Wonder (Japanese)");
    }

    ui.player1EntityNames->addItem(listWidgetItem);
  }
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
  m_gameOutputBuffer.clear();

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

  if (soundEffectsEnabled == true) { soundEffectsEnabled = false; }
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
  if (player1Name.isEmpty()) { player1Name = "Player 1"; }

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
  if (player2Name.isEmpty()) { player2Name = "Player 2"; }

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

    for (const QString& entity : entities) { filteredEntities.insert(entity); }
  }

  QList<QString> result{filteredEntities.values()};
  result.sort();
  return result;
}

// Included files
#include "mainwindow.hpp"     // This window
#include "aboutwindow.h"      // A window this window can open
#include "backend/run_game.h" // Age of Empires combat results calculator v1.2
#include "soundEffects.h"     // Sound playing class

// Libraries used for std::cout
#include <iostream>

// Libraries used for std::copy_if
#include <algorithm>

// Libraries used for debugging
#include <QDebug>

// Libraries used for accessing file paths
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>

// Libraries used for storing data
#include <QColor> // Prefers RGB input over HEX
#include <QListWidgetItem>
#include <QString>
#include <QStringList>
#include <QSet>
#include <vector>

// Libraries for acquiring user input
#include <QColorDialog>
#include <QInputDialog>

// Librraries used for hotkeys
#include <QShortcut>

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
QStringList entityNames;
int     player1EntityQuantity;
int     player1AssistingEntityQuantity;
int     player2EntityQuantity;
int     player2AssistingEntityQuantity;
QString     player1Name;
QString     player2Name;
QString     player1Color;
QString     player2Color;
QString player1BattleAssistantName;
QString player2BattleAssistantName;

// Get what age the player is in
QString player1Age;
QString player2Age;
QStringList ages;

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

  QIntValidator myName;
  myName.setRange(100, 999);


  // create shortcut
  QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_R), this);

  // connect its 'activated' signal to the 'on_calculateResultsButton_clicked' function
  QObject::connect(shortcut,    &QShortcut::activated,
                   this,        &MainWindow::on_calculateResultsButton_clicked);

  // Indicate that there's a hotkey for this in the tooltip
  ui.calculateResultsButton->setToolTip("<b>Hotkey:</b> R");

  // What the working directory is
  workingDirectory = QCoreApplication::applicationDirPath();

  // gets debug folder for some reason so go up a level
  workingDirectory.cdUp();

  // What the ages are
  ages << tr("Dark Age") << tr("Feudal Age") << tr("Castle Age") << tr("Imperial Age");

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
  // @Reference: What row in the .csv file it goes to
  QStringList technologies = {
    "Blast Furnace", // [Row 1]
    "Bodkin Arrow", // [Row 2]
    "Bracer", // [Row 3]
    "Chain Barding Armor", // [Row 4]
    "Chain Mail Armor", // [Row 5]
    "Fletching", // [Row 6]
    "Forging", // [Row 7]
    "Hoardings", // [Row 8]
    "Iron Casting", // [Row 9]
    "Leather Archer Armor", // [Row 10]
    "Loom", // [Row 11]
    "Padded Archer Armor", // [Row 12]
    "Plate Barding Armor", // [Row 13]
    "Plate Mail Armor", // [Row 14]
    "Ring Archer Armor", // [Row 15]
    "Scale Barding Armor", // [Row 16]
    "Scale Mail Armor", // [Row 17]
    "Sanctity {2E}"}; // [Row 18]

  // Sort the list in alphabetical order
  technologies.sort();

  // What the possible names of event cards are
  // @Reference: What row in the .csv file it goes to
  QStringList events = {
    "A Just Cause", // [Row 1]
    "Back From A Foreign Land", // [Row 2]  (Byzantine civ bonus: +2 healing rate modifier)
    // (has multiple slots in .csv file)
    "Barrel Of Grog", // [Row 3]
    "Bone Shaft Arrows (Mongol)", // [Row 4]
    "Caught From The Crow's Nest", // [Row 5]
    "Celtic Battle Cry (Celt)", // [Row 6]
    "Dangerous Times", // [Row 7]
    "Fat Friar's Tavern O' Spices", // [Row 8]
    "Field Testing", // [Row 9]
    "First Battle Jitters", // [Row 10]
    "Flaming Arrows", // [Row 11]
    "Fortune Favors The Foolish", // [Row 12]
    "Gatherin' A Rowdy Bunch", // [Row 13]
    "Gladitorial Games", // [Row 14]
    "Hard To Starboard", // [Row 15]
    "Heavy Tree Cover", // [Row 16]
    "High Ground", // [Row 17]
    "Husbandry", // [Row 18]
    "It's A Miracle", // [Row 19]
    "Listen To A Story", // [Row 20]
    "Muddy Battlefield", // [Row 21]
    "Non-Compos Mentis", // [Row 22]
    // Back_From_A_Foreign_Land (Byzantine civ bonus:
    // All building get a HP bonus: Age I – 10 HP, Age II – 20 HP, Age III – 30 HP, Age IV – 40 HP) [Row 23]
    "Piety", // [Row 24]
    "Black Knight", // [Row 25]
    "Rally The Workers", // [Row 26]
    "Relentless Attack", // [Row 27]
    "Retreat", // [Row 28]
    "Holy War", // [Row 29]
    "Shots In The Back (Briton)", // [Row 30]
    "Soak The Timbers", // [Row 31]
    "Spirits Of The Ancestors", // [Row 32]
    "Squires", // [Row 33]
    "Steady Hand", // [Row 34]
    "The Hammer's Cavalry (Franks)", // [Row 35]
    "The Jester Is Dead Let's Get Them! (Celt)", // [Row 36]
    "Vengeance Is Mine!", // [Row 37]
    "While They're Sleeping", // [Row 38]
    "You Will Die! (Saracen)", // [Row 39]
    "Zealous Monks"}; // [Row 40]
    // Back_From_A_Foreign_Land (Teuton civ bonus: Conversion rate modifier is -1) [Row 41]

  // Sort the list in alphabetical order
  events.sort();

  // Populate the UI elements with elements
  // Both player 1 & 2 UI elements
  for (int i = 0; i < entityNames.size(); i++) {
    ui.player1EntityNames->addItem(entityNames[i]);
    ui.player2EntityNames->addItem(entityNames[i]);
  }

  // Can only have one list widget item per list
  for (int tE = 0; tE < technologies.length(); tE++) {
    QListWidgetItem* technologyPlayer1 = new QListWidgetItem(technologies[tE]);
    QListWidgetItem* technologyPlayer2 = new QListWidgetItem(technologies[tE]);

    technologyPlayer1->setData(Qt::CheckStateRole, Qt::Unchecked);
    technologyPlayer2->setData(Qt::CheckStateRole, Qt::Unchecked);

    // Mark which ones correspond to the 2E
    if(technologyPlayer1->text().contains("{2E}")){
      technologyPlayer1->setForeground(QColor(255,255,255));
      technologyPlayer1->setBackground(QColor(90, 90, 90));
    }

    if(technologyPlayer2->text().contains("{2E}")){
      technologyPlayer2->setForeground(QColor(255,255,255));
      technologyPlayer2->setBackground(QColor(90, 90, 90));
    }

    ui.player1Technologies->addItem(technologyPlayer1);
    ui.player2Technologies->addItem(technologyPlayer2);
  }

  for (int eV = 0; eV < events.length(); eV++) {
    QListWidgetItem* eventPlayer1 = new QListWidgetItem(events[eV]);
    QListWidgetItem* eventPlayer2 = new QListWidgetItem(events[eV]);

    eventPlayer1->setData(Qt::CheckStateRole, Qt::Unchecked);
    eventPlayer2->setData(Qt::CheckStateRole, Qt::Unchecked);

    // Mark which ones I haven't implemented
    if(eventPlayer1->text().contains("(unimplemented)")){
      eventPlayer1->setForeground(QColor(255, 0, 0));
    }

    if(eventPlayer2->text().contains("(unimplemented)")){
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

  // Setting up the validators
  ui.player1EntityQuantity->setRange(1, 5);
  ui.player2EntityQuantity->setRange(1, 5);
  ui.player1EntityAssistantQuantity->setRange(0, 5);
  ui.player2EntityAssistantQuantity->setRange(0, 5);
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

// Tooltip returner
QString MainWindow::tooltipReturner(QString name){
  QString tooltipForIt;

  if (
    name
    == "Charlamagne's Palace At Aix La'Chapelle (Briton)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Briton)";
  }
  else if (name == "Rock Of Cashel (Celt)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Celt)";
  }
  else if (
    name == "The Golden Tent Of The Great Khan (Mongol)") {
    tooltipForIt = "<b>Aliases:</b> Wonder (Mongol)";
  }
  else if (
    name == "The Palace Of Ctesiphon On The Tigris (Persian)") {
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
  else if (name == "Knight"){
    tooltipForIt = "<b>Aliases:</b> Kt";
  }
  else if (name == "Knight (Frank)"){
    tooltipForIt = "<b>Aliases:</b> Kt (Frank)";
  }
  else if (name == "Knight (Persian)"){
    tooltipForIt = "<b>Aliases:</b> Kt (Persian)";
  }
  else if (name == "Crossbowman"){
    tooltipForIt = "<b>Aliases:</b> Xbow";
  }
  else if (name == "Crossbowman (Saracen)"){
    tooltipForIt = "<b>Aliases:</b> Xbow (Saracen)";
  }
  else if (name == "Siege Onager"){
    tooltipForIt = "<b>Aliases:</b> SO";
  }
  else if (name == "Siege Onager (Celt)"){
    tooltipForIt = "<b>Aliases:</b> SO (Celt)";
  }
  /*
  else if (name == "Bombard Tower"){
    tooltipForIt = "<b>Aliases:</b> BBT";
  }
  */
  else if (name == "Town Center"){
    tooltipForIt = "<b>Aliases:</b> TC";
  }
  else if (name == "Town Center (Briton)"){
    tooltipForIt = "<b>Aliases:</b> TC (Briton)";
  }
  else if (name == "Town Center (Persian)"){
    tooltipForIt = "<b>Aliases:</b> TC (Persian)";
  }
  else{
    tooltipForIt ="";
  }

  return tooltipForIt;
}

// Run this when the text inside of the player 1 entities search field changes
void MainWindow::on_player1EntityNamesFilter_textChanged(const QString &textInsideOfElement)
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
    QString listWidgetItemTooltip = tooltipReturner(nameOfFilteredItem);
    if(listWidgetItemTooltip != ""){
      listWidgetItem->setToolTip(listWidgetItemTooltip);
    }

    ui.player1EntityNames->addItem(listWidgetItem);
  }
}

// Run this when the text inside of the player 2 entities search field changes
void MainWindow::on_player2EntityNamesFilter_textChanged(const QString &textInsideOfElement)
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
    QString listWidgetItemTooltip = tooltipReturner(nameOfFilteredItem);
    if(listWidgetItemTooltip != ""){
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




// Run this when the value inside of the player 1 entity quantities field changes
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

  // @Phillip: Pass player1EntityQuantity to entities.csv
}

// Run this when the value inside of the player 2 entity quantities field changes
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

         // @Phillip: Pass player2EntityQuantity to entities.csv
}

// Run on change of what battle assistant is selected by player 1
void MainWindow::on_player1BattleAssistantNames_textActivated(const QString &currentSelection)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  player1BattleAssistantName = currentSelection;

  // @Phillip: Pass player1BattleAssistantName into entities.csv
}

// Run on change of what battle assistant is selected by player 2
void MainWindow::on_player2BattleAssistantNames_textActivated(const QString &currentSelection)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  player2BattleAssistantName = currentSelection;

  // @Phillip: Pass player2BattleAssistantName into entities.csv
}

void MainWindow::on_player1EntityAssistantQuantity_valueChanged(int valueInsideOfField)
{

  player1AssistingEntityQuantity = valueInsideOfField;

         // Give an error to Console if quantity isn't right
  if (player1AssistingEntityQuantity != 0 && player1AssistingEntityQuantity != 1 && player1AssistingEntityQuantity != 2
    && player1AssistingEntityQuantity != 3 && player1AssistingEntityQuantity != 4
    && player1AssistingEntityQuantity != 5) {
    qDebug()
      << "Error: Player 1's assisting entity quantity input should be between 0 and 5";
  }

         // @Phillip: Pass player1AssistingEntityQuantity to entities.csv

}


void MainWindow::on_player2EntityAssistantQuantity_valueChanged(int valueInsideOfField)
{
  player2AssistingEntityQuantity = valueInsideOfField;

         // Give an error to Console if quantity isn't right
  if (player2AssistingEntityQuantity != 0 && player2AssistingEntityQuantity != 1 && player2AssistingEntityQuantity != 2
      && player2AssistingEntityQuantity != 3 && player2AssistingEntityQuantity != 4
      && player2AssistingEntityQuantity != 5) {
    qDebug()
      << "Error: Player 2's assisting entity quantity input should be between 0 and 5";
  }

         // @Phillip: Pass player2AssistingEntityQuantity to entities.csv

}

QString MainWindow::convertSpacesToUnderscores(QString text){
  std::replace(text.begin(), text.end(), ' ', '_');
  return text;
}

// Run on change of what battle participant is selected by player 1
void MainWindow::on_player1EntityNames_itemClicked(QListWidgetItem * selectedItem)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QString currentSelectionFormatted = convertSpacesToUnderscores(selectedItem->text());

  // @Phillip: Pass currentSelectionFormatted into entities.csv
}

void MainWindow::on_player2EntityNames_itemClicked(QListWidgetItem *selectedItem)
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QString currentSelectionFormatted = convertSpacesToUnderscores(selectedItem->text());

  // @Phillip: Pass currentSelectionFormatted into entities.csv
}

// Run on change of what technologies are toggled by player 1
void MainWindow::on_player1Technologies_itemChanged(QListWidgetItem* checkedItem)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  if(checkedItem->checkState() == Qt::Checked){
    QString activeTechnology = checkedItem->text();
    // @Phillip: Pass a 1 in row of activeTechnology into technologies_p1.csv
    // @Phillip refer to @Reference
  }
  else{
    QString inactiveTechnology = checkedItem->text();
    // @Phillip: Pass a 0 in row of activeTechnology into technologies_p1.csv
    // @Phillip refer to @Reference
  }
}

// Run on change of what events are toggled by player 1
void MainWindow::on_player1Events_itemChanged(QListWidgetItem* checkedItem)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  if(checkedItem->checkState() == Qt::Checked){
    QString activeEvent = checkedItem->text();
    // @Phillip: Pass a 1 in row of activeEvent into events_p1.csv
    // @Phillip refer to @Reference
  }
  else{
    QString inactiveEvent = checkedItem->text();
    // @Phillip: Pass a 0 in row of inactiveEvent into events_p1.csv
    // @Phillip refer to @Reference
  }
}

void MainWindow::on_player2Technologies_itemChanged(QListWidgetItem *checkedItem)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  if(checkedItem->checkState() == Qt::Checked){
    QString activeTechnology = checkedItem->text();
    // @Phillip: Pass a 1 in row of activeTechnology into technologies_p2.csv
    // @Phillip refer to @Reference
  }
  else{
    QString inactiveTechnology = checkedItem->text();
    // @Phillip: Pass a 0 in row of activeTechnology into technologies_p2.csv
    // @Phillip refer to @Reference
  }
}

void MainWindow::on_player2Events_itemChanged(QListWidgetItem *checkedItem)
{
  SFXToPlay("/sfx/ui/toggle_pressed_sfx.wav");

  if(checkedItem->checkState() == Qt::Checked){
    QString activeEvent = checkedItem->text();
    // @Phillip: Pass a 1 in row of activeEvent into events_p2.csv
    // @Phillip refer to @Reference
  }
  else{
    QString inactiveEvent = checkedItem->text();
    // @Phillip: Pass a 0 in row of inactiveEvent into events_p2.csv
    // @Phillip refer to @Reference
  }
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
void MainWindow::updatePlayerNames(){
  ui.actionSet_player_1_Age->setText("Set " + player1Name + "'s medieval age");
  ui.actionSet_name_of_player_1->setText("Set " + player1Name + "'s name");
  ui.actionSet_set_color_of_player_1->setText("Set " + player1Name + "'s color");
  ui.actionSet_player_2_Age->setText("Set " + player2Name + "'s medieval age");
  ui.actionSet_name_of_player_2->setText("Set " + player2Name + "'s name");
  ui.actionSet_set_color_of_player_2->setText("Set " + player2Name + "'s color");

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

  updatePlayerNames();
}

// Run on change of "Options" > "Set player 1's color"
void MainWindow::on_actionSet_set_color_of_player_1_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QColor color = QColorDialog::getColor();
  player1Color = color.name();

  updatePlayerNames();
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

  updatePlayerNames();
}

// Run on change of "Options" > "Set player 2's color"
void MainWindow::on_actionSet_set_color_of_player_2_triggered()
{
  SFXToPlay("/sfx/ui/button_pressed.wav");

  QColor color = QColorDialog::getColor();
  player2Color = color.name();

  updatePlayerNames();
}

void MainWindow::initializeEntityAliases()
{
  m_aliases.add("Charlamagne's Palace At Aix La'Chapelle (Briton)", "Wonder (Briton)");
  m_aliases.add("Rock Of Cashel (Celt)", "Wonder (Celt)");
  m_aliases.add("The Golden Tent Of The Great Khan (Mongol)", "Wonder (Mongol)");
  m_aliases.add("The Palace Of Ctesiphon On The Tigris (Persian)", "Wonder (Persian)");
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

    for (const QString& entity : entities) { filteredEntities.insert(entity); }
  }

  QList<QString> result{filteredEntities.values()};
  result.sort();
  return result;
}

void MainWindow::on_actionSet_player_1_Age_triggered()
{
  bool ok;
  player1Age = QInputDialog::getItem(this, tr("Enter player 1's medieval age"),
                                       tr("Age:"), ages, 0, false, &ok);
}

void MainWindow::on_actionSet_player_2_Age_triggered()
{
  bool ok;
  player2Age = QInputDialog::getItem(this, tr("Enter player 2's medieval age"),
                                             tr("Age:"), ages, 0, false, &ok);
}





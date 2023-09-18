// Included files
#include "backend/run_game.h"
#include "mainwindow.hpp"
#include "aboutwindow.h"

// Included libaries
#include <iostream>
#include <QStringList>
#include <QString>
#include <QTextEdit>
#include <QDebug>
#include <QDesktopServices>
#include <QCoreApplication>
#include <QDir>
// #include <QSoundEffect>

// Declaring the variables, arrays
QString player1EntityNamesFiltered = "";
QStringList entityNames;
QString player1EntityQuantity = "0";
QString player1MonkQuantity = "0";


QString entitiesFilename = "/import/entities.csv";
QString eventsP1Filename = "/import/events_p1.csv";
QString eventsP2Filename = "/import/events_p2.csv";
QString playerDetailsFilename = "/import/playerDetails.csv";
QString technologiesP1Filename = "/import/technologies_p1.csv";
QString technologiesP2Filename = "/import/technologies_p2.csv";


// Declare working directory
QDir workingDirectory(QCoreApplication::applicationDirPath());




MainWindow::MainWindow(QWidget* parent)
  : QMainWindow{parent}
  , m_gameOutputBuffer{}
  , m_streamBuffer{std::cout, [this](char ch) {
                     m_gameOutputBuffer.append(QString{QChar{ch}});
                     ui.gameOutputTextEdit->setText(m_gameOutputBuffer);
                   }}
{
  ui.setupUi(this);
  connect(
    ui.calculateResultsButton,
    &QAbstractButton::clicked,
    this,
    &MainWindow::onCalculateResultsButtonClicked);

    // Supply the names of the entities
    // @Phillip: Can we have aliases? For example, "CHARLAMAGNE'S_PALACE_AT_AIX_LA'CHAPELLE_(BRITON)" = "WONDER_(BRITON)"
    entityNames << "Archer" << "Archer_(Saracen)" << "Arbalest" << "Arbalest_(Briton)" << "Arbalest_(Saracen)" << "Archery_Range"
              << "Barracks" << "Battering_Ram" << "Battering_Ram_(Celt)" << "Berserk_(Viking)" << "Blacksmith"
              << "Capped_Ram" << "Capped_Ram_(Celt)" << "Castle" << "Camel" << "Castle_(Frank)" << "Cavalier" << "Cavalier_(Frank)" << "Cavalier_(Persian)" << "Cavalry_Archer" << "Cavalry_Archer_(Mongol)" << "Champion" << "Champion_(Celt)" << "Champion_(Goth)" << "Champion_(Japanese)" << "Champion_(Viking)" << "Charlamagne's_Palace_At_Aix_La'Chapelle_(Briton)" << "Crossbowman" << "Crossbowman_(Saracen)"
              << "Demolition_Ship" << "Demolition_Ship_(Viking)" << "Dock" << "Dock_(Persian)" << "Dock_(Viking)"
              << "Elite_Berserk_(Viking)" << "Elite_Huskarl_(Goth)" << "Elite_Longboat_(Viking)" << "Elite_Longbowman_(Briton)" << "Elite_Mameluke_(Saracen)" << "Elite_Manugdai_(Mongol)" << "Elite_Samurai_(Japanese)" << "Elite_Skirmisher" << "Elite_Throwing_Axeman_(Frank)" << "Elite_War_Elephant_(Persian)" << "Elite_Woad_Raider_(Celt)"
              << "Farm" << "Fast_Fire_Ship" << "Fire_Ship" << "Fishing_Ship_(Japanese)" << "Fishing_Ship_(Persian)" << "Fortified_Wall" << "Galley" << "Galley_(Japanese)" << "Galley_(Saracen)" << "Galley_(Viking)"
              << "Galleon" << "Galleon_(Saracen)" << "Galleon_(Viking)" << "Galleon_(Japanese)" << "Gold_Mine" << "Gold_Mine_(Japanese)"
              << "Heavy_Camel" << "Heavy_Cavalry_Archer" << "Heavy_Cavalry_Archer_(Mongol)" << "Heavy_Cavalry_Archer_(Saracen)" << "Heavy_Demolition_Ship" << "Heavy_Demolition_Ship_(Viking)" << "Heavy_Scorpion" << "Heavy_Scorpion_(Celt)" << "House" << "Huskarl_(Goth)"
              << "Knight" << "Knight_(Frank)" << "Knight_(Persian)"
              << "Light_Cavalry" << "Light_Cavalry_(Mongol)" << "Long_Swordsman" << "Long_Swordsman_(Celt)" << "Long_Swordsman_(Goth)" << "Long_Swordsman_(Japanese)" << "Long_Swordsman_(Viking)" << "Longboat_(Viking)" << "Longbowman_(Briton)" << "Lumber_Camp" << "Lumber_Camp_(Japanese)"
              << "Mameluke_(Saracen)" << "Man-at-Arms" << "Man-at-Arms_(Viking)" << "Man-at-Arms_(Celt)" << "Man-at-Arms_(Goth)" << "Man-at-Arms_(Japanese)" << "Mangonel" << "Mangonel_(Celt)" << "Mangudai_(Mongol)" << "Market" << "Market_(Saracen)" << "Militia" << "Militia_(Celt)" << "Militia_(Goth)" << "Mill" << "Mill_(Japanese)" << "Monastery"
              << "Onager" << "Onager_(Celt)" << "Outpost"
              << "Paladin" << "Paladin_(Persian)" << "Paladin_(Frank)" << "Palisade_Wall" << "Pikeman" << "Pikeman_(Celt)" << "Pikeman_(Goth)" << "Pikeman_(Japanese)" << "Pikeman_(Viking)"
              << "Samurai_(Japanese)" << "Scorpion" << "Scorpion_(Celt)" << "Scout_Cavalry" << "Siege_Onager" << "Siege_Onager_(Celt)" << "Siege_Ram" << "Siege_Ram_(Celt)" << "Siege_Workshop" << "Skirmisher" << "Spearman" << "Spearman_(Japanese)" << "Spearman_(Viking)" << "Spearman_(Celt)" << "Spearman_(Goth)" << "Stable" << "Stone_Gate" << "Stone_Mine" << "Stone_Mine_(Japanese)" << "Stone_Wall"
              <<"Throwing_Axeman_(Frank)" << "Town_Center" << "Town_Center_(Briton)" << "Town_Center_(Persian)" << "Trebuchet" << "Two-handed_Swordsman" << "Two-handed_Swordsman_(Celt)" << "Two-handed_Swordsman_(Goth)" << "Two-handed_Swordsman_(Japanese)" << "Two-handed_Swordsman_(Viking)" << "Villager"
              << "War_Elephant_(Persian)" << "War_Galley" << "War_Galley_(Japanese)" << "War_Galley_(Saracen)" << "War_Galley_(Viking)" << "Watch_Tower" << "Woad_Raider_(Celt)"
      ;


  // Sort the list in alphabetical order
  entityNames.sort();

  // Set the placeholder values for player 1 & 2
  for(int i = 0; i < entityNames.size(); i++){
      ui.player1EntityNames->addItem(entityNames[i]);
  }
  ui.player1EntityQuantity->setText("1");
  ui.player1EntityNamesFilter->setText("");
  ui.player1MonkQuantity->setText("0");


  // gets debug folder for some reason so go up a level
  // workingDirectory.cdUp();


    // See if sound plays
  QString fileName = "/sfx/wonder_destroyed_sfx.wav";
  QString filePath = workingDirectory.absolutePath() + fileName;
  qDebug() << filePath;

  // QSoundEffect effect;
  // effect.setSource(filePath);
  // effect.setVolume(0.25f);
  // effect.play();
}

MainWindow::~MainWindow()
{


}

// Calculate the results of a battle
void MainWindow::onCalculateResultsButtonClicked()
{
  // @Phillip How do we clear this? This doesn't seem to do it
    ui.gameOutputTextEdit->clear();

  runGame();



}


// Run this on click of the exit button
void MainWindow::on_closeProgram_triggered()
{
  // Exit the program
  QApplication::quit();
}

// Run this on click of the about button
void MainWindow::on_actionAbout_triggered()
{
  aboutwindow aboutWindow;
  aboutWindow.setModal(true);
  aboutWindow.exec();
}

// Run this when the text inside of the player 1 entities search field changes
void MainWindow::on_player1EntityNamesFilter_textChanged()
{
  player1EntityNamesFiltered = ui.player1EntityNamesFilter->text();
  // See if it is changing the value of this variable
  // qDebug() << player1EntityNamesFiltered;

  // Clear what's in it
  ui.player1EntityNames->clear();

  // Filter the list based on user input
  QStringList filteredList = entityNames.filter(player1EntityNamesFiltered);
  for(int y = 0; y < filteredList.size(); y++){
      ui.player1EntityNames->addItem(filteredList[y]);
  }
}

// Run this when the text inside of the player 1 quantities field changes
void MainWindow::on_player1EntityQuantity_textChanged()
{
  player1EntityQuantity = ui.player1EntityQuantity->text();

  // Give an error to Console if quantity isn't right
  if(player1EntityQuantity != "1" && player1EntityQuantity != "2" && player1EntityQuantity != "3" && player1EntityQuantity != "4" && player1EntityQuantity != "5"){
      qDebug() << "Error: Player 1's entity quantity input should be between 1 and 4";
  }
}





void MainWindow::on_actionDeveloper_guide_triggered()
{
  QString fileName = "/documentation/developer_guide.docx";
  QString filePath = workingDirectory.absolutePath() + fileName;
  QDesktopServices::openUrl(filePath);
}


void MainWindow::on_actionUser_guide_triggered()
{
  QString fileName = "/documentation/user_guide.docx";
  QString filePath = workingDirectory.absolutePath() + fileName;
  QDesktopServices::openUrl(filePath);
}


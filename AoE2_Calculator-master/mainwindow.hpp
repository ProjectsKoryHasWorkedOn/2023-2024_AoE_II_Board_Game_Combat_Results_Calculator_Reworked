#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include "./ui_mainwindow.h"
#include "aliases.h"
#include "csv/entities.h"
#include "csv/player_names.h"
#include "csv/player_medieval_ages.h"
#include "stream_buffer.h"

#include <QDir>
#include <QHash>
#include <QMainWindow>
#include <QString>

extern const QString entitiesFilename;
extern const QString eventsP1Filename;
extern const QString eventsP2Filename;
extern const QString playerMedievalAgesFilename;
extern const QString playerNamesFilename;
extern const QString technologiesP1Filename;
extern const QString technologiesP2Filename;
extern QDir          workingDirectory;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  // Delete redundant references to functions or it'll re-create them in
  // moc_mainwindow.cpp under slots and throw up an error Add new references to
  // functions if it hasn't added them automatically or it'll throw up an error
private slots:

  void on_closeProgram_triggered();

  void on_actionAbout_triggered();

  void on_player1EntityNamesFilter_textChanged(
    const QString& textInsideOfElement);

  void on_actionDeveloper_guide_triggered();

  void on_actionUser_guide_triggered();

  void on_calculateResultsButton_clicked();

  void on_player1EntityNames_itemClicked(QListWidgetItem* selectedItem);

  void on_player1Technologies_itemChanged(QListWidgetItem* checkedItem);

  void on_player1Events_itemChanged(QListWidgetItem* checkedItem);

  void on_actionDisable_SFX_triggered();

  void on_actionSet_name_of_player_2_triggered();

  void on_actionSet_name_of_player_1_triggered();

  void on_actionSet_set_color_of_player_1_triggered();

  void updatePlayerNames();

  void on_actionSet_set_color_of_player_2_triggered();

  void on_player2EntityNamesFilter_textChanged(
    const QString& textInsideOfElement);

  void on_player2Technologies_itemChanged(QListWidgetItem* checkedItem);

  void on_player2Events_itemChanged(QListWidgetItem* checkedItem);

  void on_player2EntityNames_itemClicked(QListWidgetItem* selectedItem);

  void on_actionSet_player_1_Age_triggered();

  void on_actionSet_player_2_Age_triggered();

  void on_player2EntityQuantity_valueChanged(int valueInsideOfField);

  void on_player1EntityQuantity_valueChanged(int valueInsideOfField);

  void on_player1EntityAssistantQuantity_valueChanged(int valueInsideOfField);

  void on_player2EntityAssistantQuantity_valueChanged(int valueInsideOfField);

  void on_player1BattleAssistantNames_textActivated(
    const QString& currentSelection);

  void on_player2BattleAssistantNames_textActivated(
    const QString& currentSelection);

  void on_actionDeveloper_wishlist_triggered();

private:
  void initializeEntityAliases();

  QStringList filterEntityNames(QString input) const;

  QString tooltipReturner(QString name);

  QString convertSpacesToUnderscores(QString text) const;

  QString convertUnderscoresToSpaces(QString text) const;

  QListWidgetItem* findByEntityName(QListWidget* haystack, QString needle)
    const;

  void selectInitialEntities();

  void setInitialNames();

  void markInitialPlayerMedievalAge();

  void updateRangeAllowed(QString nameOfSelection, int playerNumber);


  Ui::MainWindow ui;
  QString        m_gameOutputBuffer;
  StreamBuffer   m_streamBuffer;
  Aliases        m_aliases;
  QString        m_player1EntityName;
  QString        m_player2EntityName;
  Entities       m_entities;
  PlayerName     m_player_names;
  playerMedievalAges  m_player_medieval_age;



};
#endif // MAINWINDOW_HPP

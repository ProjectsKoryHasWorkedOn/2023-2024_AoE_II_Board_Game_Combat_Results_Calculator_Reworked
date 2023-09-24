#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include "./ui_mainwindow.h"
#include "aliases.h"
#include "stream_buffer.h"

#include <QHash>
#include <QMainWindow>
#include <QString>

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

  void on_player1EntityNamesFilter_textChanged();

  void on_player1EntityQuantity_textChanged();

  void on_actionDeveloper_guide_triggered();

  void on_actionUser_guide_triggered();

  void on_calculateResultsButton_clicked();

  void on_player1BattleAssistantNames_activated(int index);

  void on_player1EntityNames_itemClicked(QListWidgetItem* item);

  void on_player1Technologies_itemChanged(QListWidgetItem* item);

  void on_player1Events_itemChanged(QListWidgetItem* item);

  void on_actionDisable_SFX_triggered();

  void on_actionSet_name_of_player_2_triggered();

  void on_actionSet_name_of_player_1_triggered();

  void on_actionSet_set_color_of_player_1_triggered();

  void updatePlayerNames(
    QString updatedPlayer1Name,
    QString updatedPlayer2Name);

  void on_actionSet_set_color_of_player_2_triggered();

private:
  void initializeEntityAliases();

  QStringList filterEntityNames(QString input) const;

  Ui::MainWindow ui;
  QString        m_gameOutputBuffer;
  StreamBuffer   m_streamBuffer;
  Aliases        m_aliases;
};
#endif // MAINWINDOW_HPP

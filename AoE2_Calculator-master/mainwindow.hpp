#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include "./ui_mainwindow.h"
#include "stream_buffer.h"

#include <QMainWindow>
#include <QString>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();


  // @ Kory make sure you delete old references, or it'll re-create them in moc_mainwindow.cpp in slots and be annoying

private slots:
  void onCalculateResultsButtonClicked();

    void on_closeProgram_triggered();

  void on_actionAbout_triggered();

    void on_player1EntityNamesFilter_textChanged();

  void on_player1EntityQuantity_textChanged();


    void on_actionDeveloper_guide_triggered();

  void on_actionUser_guide_triggered();


  private:
  Ui::MainWindow ui;
  QString        m_gameOutputBuffer;
  StreamBuffer   m_streamBuffer;
};
#endif // MAINWINDOW_HPP

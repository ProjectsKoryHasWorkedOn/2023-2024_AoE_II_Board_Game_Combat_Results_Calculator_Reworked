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

private slots:
  void onCalculateResultsButtonClicked();

    void on_closeProgram_triggered();

  void on_actionAbout_triggered();

    void on_player1EntityNamesFilter_textChanged();

  void on_player1EntityQuantity_textChanged();

    void on_player1EntityQuantity_textChanged(const QString &arg1);

  void on_player1EntityNamesFilter_textChanged(const QString &arg1);

    void on_actionDeveloper_guide_triggered();

  void on_actionUser_guide_triggered();

  private:
  Ui::MainWindow ui;
  QString        m_gameOutputBuffer;
  StreamBuffer   m_streamBuffer;
};
#endif // MAINWINDOW_HPP

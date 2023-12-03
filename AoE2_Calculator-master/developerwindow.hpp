#ifndef DEVELOPERWINDOW_HPP
#define DEVELOPERWINDOW_HPP

#include "ui_developerwindow.h"

QT_FORWARD_DECLARE_CLASS(QSqlDatabase)

class DeveloperWindow : public QDialog {
  Q_OBJECT

public:
  explicit DeveloperWindow(
    const QSqlDatabase& database,
    QWidget*            parent = nullptr);

public slots:
  void showTable(const QString& table);

private slots:
  void onListItemClicked(QListWidgetItem* item);

private:
  void setTitle();

  void loadTableNamesIntoListWidget();

  void selectItem(const QString& tableName);

  Ui::DeveloperWindow ui;
  const QSqlDatabase& m_database;
};

#endif // DEVELOPERWINDOW_HPP

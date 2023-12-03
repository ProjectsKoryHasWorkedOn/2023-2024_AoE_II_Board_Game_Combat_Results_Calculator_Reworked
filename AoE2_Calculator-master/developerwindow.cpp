#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlTableModel>

#include "developerwindow.hpp"

DeveloperWindow::DeveloperWindow(const QSqlDatabase& database, QWidget* parent)
  : QDialog{parent}, m_database{database}
{
  ui.setupUi(this);
  setTitle();
  showTable("Units");
}

void DeveloperWindow::showTable(const QString& table)
{
  QSqlTableModel* model = new QSqlTableModel(ui.tableView, m_database);
  model->setEditStrategy(QSqlTableModel::OnFieldChange);
  model->setTable(
    m_database.driver()->escapeIdentifier(table, QSqlDriver::TableName));
  model->select();

  if (model->lastError().type() != QSqlError::NoError) {
    const QString errorMessage{model->lastError().text()};
    // TODO: Handle error.
  }

  ui.tableView->setModel(model);
  ui.tableView->setEditTriggers(
    QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}

void DeveloperWindow::setTitle()
{
  setWindowTitle("Developer window");
}

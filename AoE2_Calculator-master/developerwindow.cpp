#include <cassert>

#include "developerwindow.hpp"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlTableModel>

DeveloperWindow::DeveloperWindow(const QSqlDatabase& database, QWidget* parent)
  : QDialog{parent}, m_database{database}
{
  ui.setupUi(this);
  setTitle();

  connect(
    ui.tablesListWidget,
    &QListWidget::itemClicked,
    this,
    &DeveloperWindow::onListItemClicked);

  loadTableNamesIntoListWidget();

  selectItem("Ages");
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

void DeveloperWindow::onListItemClicked(QListWidgetItem* item)
{
  if (item == nullptr) {
    return;
  }

  showTable(item->text());
}

void DeveloperWindow::setTitle()
{
  setWindowTitle("Database");
}

void DeveloperWindow::loadTableNamesIntoListWidget()
{
  QStringList tableNames{m_database.driver()->tables(QSql::Tables)};

  // Put in ascending order
  std::sort(
    tableNames.begin(), tableNames.end()); // Has to be non-const var to sort it

  // Remove bizarre "sqlite_sequence" table
  tableNames.removeAll("sqlite_sequence");

  ui.tablesListWidget->addItems(tableNames);
}

void DeveloperWindow::selectItem(const QString& tableName)
{
  const QList<QListWidgetItem*> items{
    ui.tablesListWidget->findItems(tableName, Qt::MatchExactly)};
  assert(items.size() == 1 && "Unexpected item count");
  ui.tablesListWidget->setCurrentItem(items.front());
  ui.tablesListWidget->itemClicked(items.front());
}

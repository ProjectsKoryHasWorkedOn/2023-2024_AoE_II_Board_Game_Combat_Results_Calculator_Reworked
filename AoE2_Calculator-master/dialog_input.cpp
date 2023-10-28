#include <QMessageBox>

#include "dialog_input.h"

std::unique_ptr<DialogInput> din{};

void DialogInput::initialize(MainWindow* mainWindow)
{
  din = std::make_unique<DialogInput>(mainWindow);
}

DialogInput::DialogInput(MainWindow* mainWindow) : m_mainWindow{mainWindow}
{
}

DialogInput::MonkAction DialogInput::queryForMonkAction(
  const std::string& playerName) const
{
  QMessageBox messageBox{
    /* icon */ QMessageBox::Icon::Question,
    /* title */ "Monk action query",
    /* text */
    QString{"Hey, %1! Do you want your monks to convert or heal?"}.arg(
      QString::fromStdString(playerName))};
  QAbstractButton* convertButton{new QPushButton{"Convert", &messageBox}};
  messageBox.addButton(convertButton, QMessageBox::YesRole);
  QAbstractButton* healButton{new QPushButton{"Heal", &messageBox}};
  messageBox.addButton(healButton, QMessageBox::DestructiveRole);
  messageBox.exec();
  QAbstractButton* clickedButton{messageBox.clickedButton()};

  if (clickedButton == convertButton) {
    return DialogInput::MonkAction::Convert;
  }
  else if (clickedButton == healButton) {
    return DialogInput::MonkAction::Heal;
  }

  return DialogInput::MonkAction::Heal;
}

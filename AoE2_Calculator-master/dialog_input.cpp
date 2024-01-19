#include <QMessageBox>

#include "dialog_input.h"

std::unique_ptr<DialogInput> din{};

void DialogInput::initialize(
  MainWindow* mainWindow,
  QString&    retreatingPromptAnswer,
  QString&    convertingHealingPromptAnswer)
{
  din = std::make_unique<DialogInput>(
    mainWindow, retreatingPromptAnswer, convertingHealingPromptAnswer);
}

DialogInput::DialogInput(
  MainWindow* mainWindow,
  QString&    retreatingPromptAnswer,
  QString&    convertingHealingPromptAnswer)
  : m_mainWindow{mainWindow}
  , m_retreatingPromptAnswer{retreatingPromptAnswer}
  , m_convertingHealingPromptAnswer{convertingHealingPromptAnswer}
{
}

DialogInput::MonkAction DialogInput::queryForMonkAction(
  const std::string& playerName)
{
  if (m_convertingHealingPromptAnswer == "Always converting") {
    return MonkAction::Convert; // Converting selected
  }

  if (m_convertingHealingPromptAnswer == "Always healing") {
    return MonkAction::Heal; // Healing selected
  }

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
    return DialogInput::MonkAction::Convert;     // Converting selected
  }
  else if (clickedButton == healButton) {
    return DialogInput::MonkAction::Heal;     // Healing selected
  }

  assert(false && "It went wrong.");
  return DialogInput::MonkAction::Heal;
}

bool DialogInput::queryForIsRetreating()
{
  if (m_retreatingPromptAnswer == "Always retreating") {
    return true; // Yes to retreating
  }

  if (m_retreatingPromptAnswer == "Never retreating") {
    return false; // No to reteating
  }

  bool result{};
  *this >> result;
  return result;
}

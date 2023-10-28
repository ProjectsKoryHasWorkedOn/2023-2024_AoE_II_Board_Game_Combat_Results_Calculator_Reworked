#ifndef DIALOG_INPUT_H
#define DIALOG_INPUT_H
#include <QInputDialog>
#include <QMessageBox>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "mainwindow.hpp"

template<typename Any>
bool readFromDialog(MainWindow* mainWindow, Any& any)
{
  using UnCvRef = std::remove_cv_t<std::remove_reference_t<Any>>;
  bool          ok{};
  const QString title{"Data entry prompt"};
  const QString label{mainWindow->lastLine()};

  if constexpr (std::is_same_v<UnCvRef, bool>) {
    const QMessageBox::StandardButton buttonPressed{
      QMessageBox::question(mainWindow, title, label)};
    any = buttonPressed == QMessageBox::Yes;
    std::cout << (any ? "Yes" : "No") << "<br>";
    return true;
  }
  else if constexpr (std::is_same_v<UnCvRef, double>) {
    const double result{QInputDialog::getDouble(
      /* parent */ mainWindow,
      /* title */ title,
      /* label */ label,
      /* value */ 0.0,
      /* min */ -2147483647,
      /* max */ 2147483647,
      /* decimals */ 1,
      /* ok */ &ok)};

    if (!ok) {
      return false;
    }

    any = result;
    std::cout << result << "<br>";
    return true;
  }
  else if (std::is_same_v<UnCvRef, int>) {
    const int result{QInputDialog::getInt(
      /* parent */ mainWindow,
      /* title */ title,
      /* label */ label,
      /* value */ 0,
      /* min */ -2147483647,
      /* max */ 2147483647,
      /* sept */ 1,
      /* ok */ &ok)};

    if (!ok) {
      return false;
    }

    any = result;
    std::cout << result << "<br>";
    return true;
  }
  else {
    const QString userInput = QInputDialog::getText(
      /* parent */ mainWindow,
      /* title */ title,
      /* label */ label,
      /* mode */ QLineEdit::Normal,
      /* text */ QString{},
      /* ok */ &ok);

    if (!ok) {
      return false;
    }

    std::istringstream iss{userInput.toStdString()};
    if (!(iss >> any)) {
      return false;
    }

    std::cout << userInput.toStdString() << "<br>";
    return true;
  }
}

class DialogInput {
public:
  enum class MonkAction { Heal, Convert };

  static void initialize(
    MainWindow* mainWindow,
    QString&    retreatingPromptAnswer,
    QString&    convertingHealingPromptAnswer);

  template<typename Any>
  friend DialogInput& operator>>(DialogInput& din, Any& any)
  {
    while (!readFromDialog(din.m_mainWindow, any)) {
      QMessageBox::information(
        din.m_mainWindow,
        "Incorrect input",
        "The data entered was not valid, please try again.");
    }

    return din;
  }

  explicit DialogInput(
    MainWindow* mainWindow,
    QString&    retreatingPromptAnswer,
    QString&    convertingHealingPromptAnswer);

  MonkAction queryForMonkAction(const std::string& playerName);

  bool queryForIsRetreating();

private:
  MainWindow* m_mainWindow;
  QString&    m_retreatingPromptAnswer;
  QString&    m_convertingHealingPromptAnswer;
};

extern std::unique_ptr<DialogInput> din;

#define DIN (*din)

#endif // DIALOG_INPUT_H

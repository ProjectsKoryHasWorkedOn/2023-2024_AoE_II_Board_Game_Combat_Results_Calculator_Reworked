#ifndef DIALOG_INPUT_H
#define DIALOG_INPUT_H
#include <QInputDialog>
#include <QMessageBox>

#include <iostream>
#include <memory>
#include <sstream>
#include <type_traits>

#include "mainwindow.hpp"

template<typename Any>
bool readFromDialog(MainWindow* mainWindow, Any& any)
{
  using UnCvRef = std::remove_cv_t<std::remove_reference_t<Any>>;
  bool          ok{};
  const QString title{"Data entry prompt"};
  const QString label{mainWindow->lastLine()};

  if constexpr (std::is_same_v<UnCvRef, double>) {
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
  static void initialize(MainWindow* mainWindow);

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

  explicit DialogInput(MainWindow* mainWindow);

private:
  MainWindow* m_mainWindow;
};

extern std::unique_ptr<DialogInput> din;

#define DIN (*din)

#endif // DIALOG_INPUT_H

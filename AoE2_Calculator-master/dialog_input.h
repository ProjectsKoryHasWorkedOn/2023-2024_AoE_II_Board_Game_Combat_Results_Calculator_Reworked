#ifndef DIALOG_INPUT_H
#define DIALOG_INPUT_H
#include <QInputDialog>

#include <memory>
#include <sstream>

#include "mainwindow.hpp"

class DialogInput {
public:
  static void initialize(MainWindow* mainWindow);

  template<typename Any>
  friend DialogInput& operator>>(DialogInput& din, Any& any)
  {
    // TODO: Funny business to detect the type of any.

    bool          ok{};
    const QString userInput = QInputDialog::getText(
      /* parent */ din.m_mainWindow,
      /* title */ "Data entry prompt",
      /* label */ "PLACEHOLDER",
      /* mode */ QLineEdit::Normal,
      /* text */ QString{},
      /* ok */ &ok);

    if (!ok) {
      qFatal() << "Couldn't get text from user in DialogInput::operator>>";
      return din;
    }

    std::istringstream iss{userInput.toStdString()};
    if (!(iss >> any)) {
      qFatal() << "Couldn't write to variable in DialogInput::operator>>";
      return din;
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

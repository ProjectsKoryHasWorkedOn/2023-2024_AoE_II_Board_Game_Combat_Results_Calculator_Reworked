#ifndef DIALOG_INPUT_H
#define DIALOG_INPUT_H
#include <QInputDialog>

#include <memory>
#include <sstream>
#include <type_traits>

#include "mainwindow.hpp"

class DialogInput {
public:
  static void initialize(MainWindow* mainWindow);

  template<typename Any>
  friend DialogInput& operator>>(DialogInput& din, Any& any)
  {
    using UnCvRef = std::remove_cv_t<std::remove_reference_t<Any>>;
    bool          ok{};
    const QString title{"Data entry prompt"};
    const QString label{"PLACEHOLDER"};

    if constexpr (std::is_same_v<UnCvRef, double>) {
      const double result{QInputDialog::getDouble(
        /* parent */ din.m_mainWindow,
        /* title */ title,
        /* label */ label,
        /* value */ 0.0,
        /* min */ -2147483647,
        /* max */ 2147483647,
        /* decimals */ 1,
        /* ok */ &ok)};

      if (!ok) {
        qFatal() << "Couldn't get double from user in DialogInput::operator>>";
        return din;
      }

      any = result;
      return din;
    }
    else if (std::is_same_v<UnCvRef, int>) {
      const int result{QInputDialog::getInt(
        /* parent */ din.m_mainWindow,
        /* title */ title,
        /* label */ label,
        /* value */ 0,
        /* min */ -2147483647,
        /* max */ 2147483647,
        /* sept */ 1,
        /* ok */ &ok)};

      if (!ok) {
        qFatal() << "Couldn't get int from user in DialogInput::operator>>";
        return din;
      }

      any = result;
      return din;
    }
    else {
      const QString userInput = QInputDialog::getText(
        /* parent */ din.m_mainWindow,
        /* title */ title,
        /* label */ label,
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
  }

  explicit DialogInput(MainWindow* mainWindow);

private:
  MainWindow* m_mainWindow;
};

extern std::unique_ptr<DialogInput> din;

#define DIN (*din)

#endif // DIALOG_INPUT_H

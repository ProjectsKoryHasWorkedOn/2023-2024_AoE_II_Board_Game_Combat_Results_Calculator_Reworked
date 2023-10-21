#include <iostream>

#include <QInputDialog>

#include "mainwindow.hpp"
#include "user_input_handler.h"

// TODO: Put nice text in there from cout.
class RedirectingStreamBuf : public std::streambuf {
public:
  explicit RedirectingStreamBuf(MainWindow* mainWindow)
    : m_mainWindow{mainWindow}, m_string{}, m_index{0}, m_buffer{}
  {
    // buffer is initially full
    setg(&m_buffer, &m_buffer + 1, &m_buffer + 1);
  }

protected:
  int underflow() override
  {
    if (m_string.empty()) {
      bool          ok{};
      const QString userInput{QInputDialog::getText(
        m_mainWindow, "Title", "Label", QLineEdit::Normal, QString{}, &ok)};

      if (ok && !userInput.isEmpty()) {
        m_index  = 0;
        m_buffer = '\0';
        m_string = userInput.toStdString();
      }
    }

    if (m_index == m_string.size()) {
      m_string.clear();
      m_index  = 0;
      m_buffer = '\0';
      return traits_type::eof();
    }

    m_buffer = m_string[m_index];
    setg(&m_buffer, &m_buffer, &m_buffer + 1);
    m_index++;
    return traits_type::to_int_type(m_buffer);
  }

private:
  MainWindow*            m_mainWindow;
  std::string            m_string;
  std::string::size_type m_index;
  char                   m_buffer;
};

UserInputHandler::UserInputHandler(MainWindow* mainWindow)
  : m_mainWindow{mainWindow}
  , m_cinStreamBuf{std::cin.rdbuf()}
  , m_redirectingStreamBuf{std::make_unique<RedirectingStreamBuf>(m_mainWindow)}
{
  std::cin.rdbuf(m_redirectingStreamBuf.get());
}

UserInputHandler::~UserInputHandler()
{
  std::cin.rdbuf(m_cinStreamBuf);
  m_redirectingStreamBuf = nullptr;
}

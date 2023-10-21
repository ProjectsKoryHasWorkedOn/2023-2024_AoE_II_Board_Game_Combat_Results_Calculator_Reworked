#include <iostream>

#include <QInputDialog>

#include "mainwindow.hpp"
#include "user_input_handler.h"

// TODO: Put nice text in there from cout.
class RedirectingStreamBuf : public std::streambuf {
public:
  RedirectingStreamBuf(
    MainWindow*        mainWindow,
    std::stringstream& stringStream,
    std::streambuf*    stringStreamBuf)
    : m_mainWindow{mainWindow}
    , m_stringStream{stringStream}
    , m_stringStreamBuf{stringStreamBuf}
    , m_buffer{}
    , m_string{}
    , m_index{0}
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
        m_stringStream << m_string;
      }
    }

    const traits_type::int_type i{m_stringStreamBuf->sbumpc()};

    if (!traits_type::eq_int_type(i, traits_type::eof())) {
      // Success
      m_buffer = traits_type::to_char_type(i);
      setg(&m_buffer, &m_buffer, &m_buffer + 1);
      ++m_index;

      if (m_index == m_string.size()) {
        m_string.clear();
        m_index  = 0;
        m_buffer = '\0';
      }

      return i;
    }

    // Failure
    return i;

    /*
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
*/
  }

private:
  MainWindow*            m_mainWindow;
  std::stringstream&     m_stringStream;
  std::streambuf*        m_stringStreamBuf;
  char                   m_buffer;
  std::string            m_string;
  std::string::size_type m_index;
};

UserInputHandler::UserInputHandler(MainWindow* mainWindow)
  : m_mainWindow{mainWindow}
  , m_stringStream{}
  , m_stringStreamBuf{m_stringStream.rdbuf()}
  , m_cinStreamBuf{std::cin.rdbuf()}
  , m_redirectingStreamBuf{std::make_unique<RedirectingStreamBuf>(
      m_mainWindow,
      m_stringStream,
      m_stringStreamBuf)}
{
  std::cin.rdbuf(m_redirectingStreamBuf.get());
}

UserInputHandler::~UserInputHandler()
{
  std::cin.rdbuf(m_cinStreamBuf);
  m_redirectingStreamBuf = nullptr;
}

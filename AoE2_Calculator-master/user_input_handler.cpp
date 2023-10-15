#include <iostream>

#include "user_input_handler.h"

UserInputHandler::UserInputHandler()
  : m_stringStream{}
  , m_stringStreamBuf{m_stringStream.rdbuf()}
  , m_cinStreamBuf{std::cin.rdbuf()}
{
  std::cin.rdbuf(m_stringStreamBuf);

  // m_stringStream << "1";
}

UserInputHandler::~UserInputHandler()
{
  std::cin.rdbuf(m_cinStreamBuf);
}

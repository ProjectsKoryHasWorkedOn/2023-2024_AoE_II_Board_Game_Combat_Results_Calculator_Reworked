#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H
#include <memory>
#include <sstream>

class UserInputHandler {
public:
  template<typename Any>
  friend UserInputHandler& operator<<(
    UserInputHandler& userInputHandler,
    const Any&        any)
  {
    userInputHandler.m_stringStream << any;
  }

  UserInputHandler();

  UserInputHandler(const UserInputHandler&) = delete;

  UserInputHandler& operator=(const UserInputHandler&) = delete;

  ~UserInputHandler();

private:
  std::stringstream               m_stringStream;
  std::streambuf*                 m_stringStreamBuf;
  std::streambuf*                 m_cinStreamBuf;
  std::unique_ptr<std::streambuf> m_redirectingStreamBuf;
};

#endif // USER_INPUT_HANDLER_H
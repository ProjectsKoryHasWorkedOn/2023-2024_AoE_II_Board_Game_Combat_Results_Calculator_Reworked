#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H
#include <memory>
#include <sstream>

class MainWindow;

class UserInputHandler {
public:
  explicit UserInputHandler(MainWindow* mainWindow);

  UserInputHandler(const UserInputHandler&) = delete;

  UserInputHandler& operator=(const UserInputHandler&) = delete;

  ~UserInputHandler();

private:
  MainWindow*                     m_mainWindow;
  std::stringstream               m_stringStream;
  std::streambuf*                 m_stringStreamBuf;
  std::streambuf*                 m_cinStreamBuf;
  std::unique_ptr<std::streambuf> m_redirectingStreamBuf;
};

#endif // USER_INPUT_HANDLER_H

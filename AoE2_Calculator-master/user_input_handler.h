#ifndef USER_INPUT_HANDLER_H
#define USER_INPUT_HANDLER_H
#include <memory>

class MainWindow;

class UserInputHandler {
public:
  explicit UserInputHandler(MainWindow* mainWindow);

  UserInputHandler(const UserInputHandler&) = delete;

  UserInputHandler& operator=(const UserInputHandler&) = delete;

  ~UserInputHandler();

private:
  MainWindow*                     m_mainWindow;
  std::streambuf*                 m_cinStreamBuf;
  std::unique_ptr<std::streambuf> m_redirectingStreamBuf;
};

#endif // USER_INPUT_HANDLER_H

#include "dialog_input.h"

std::unique_ptr<DialogInput> din{};

void DialogInput::initialize(MainWindow* mainWindow)
{
  din = std::make_unique<DialogInput>(mainWindow);
}

DialogInput::DialogInput(MainWindow* mainWindow) : m_mainWindow{mainWindow}
{
}

#include <cassert>

#include <algorithm>

#include <QFile>

#include "csv/events.h"
#include "mainwindow.hpp"

namespace {
QString getEventFilePath(Player player)
{
  return workingDirectory.absolutePath()
         + (player == Player::Player1 ? eventsP1Filename : eventsP2Filename);
}
} // anonymous namespace

Events::Events(Player player)
  : m_events{CsvEvent::readFile<CsvEvent>(getEventFilePath(player))}
  , m_player{player}
{
}

void Events::enable(const QString& name)
{
  changeEntry(name, 1);
}

void Events::disable(const QString& name)
{
  changeEntry(name, 0);
}

void Events::toggle(const QString& name)
{
  CsvEvent& event{find(name)};
  event.setCount(event.count() == 0 ? 1 : 0);
  CsvEvent::writeFile<CsvEvent>(getEventFilePath(m_player), m_events);
}

bool Events::isActive(const QString& name) const
{
  const CsvEvent& event{find(name)};
  return event.count() == 1;
}

const std::vector<CsvEvent>& Events::events() const
{
  return m_events;
}

void Events::changeEntry(const QString& name, int value)
{
  CsvEvent& event{find(name)};
  event.setCount(value);
  CsvEvent::writeFile<CsvEvent>(getEventFilePath(m_player), m_events);
}

CsvEvent& Events::find(const QString& name)
{
  const std::vector<CsvEvent>::iterator it{std::find_if(
    m_events.begin(), m_events.end(), [&name](const CsvEvent& event) {
      return event.text().contains(name, Qt::CaseInsensitive);
    })};
  assert((it != m_events.end()) && "Couldn't find something that matches name");
  return *it;
}

const CsvEvent& Events::find(const QString& name) const
{
  return const_cast<Events*>(this)->find(name);
}

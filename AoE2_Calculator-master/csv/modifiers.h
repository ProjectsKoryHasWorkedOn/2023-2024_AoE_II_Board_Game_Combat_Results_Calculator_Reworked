#ifndef MODIFIERS_H
#define MODIFIERS_H
#include <cassert>

#include <type_traits>
#include <vector>

#include <QString>

#include "csv/csv_event.h"
#include "csv/csv_technology.h"
#include "file_paths.h"
#include "player.h"

namespace {
template<typename Modifier>
QString getFilePath(Player player)
{
  if constexpr (std::is_same_v<Modifier, CsvEvent>) {
    return workingDirectory.absolutePath()
           + ((
             player == Player::Player1 ? eventsP1Filename : eventsP2Filename));
  }
  else if constexpr (std::is_same_v<Modifier, CsvTechnology>) {
    return workingDirectory.absolutePath()
           + ((
             player == Player::Player1 ? technologiesP1Filename
                                       : technologiesP2Filename));
  }
  else {
    assert(false && "Only CsvEvent and CsvTechnology work here.");
    return "";
  }
}
} // anonymous namespace

template<typename Modifier>
class Modifiers {
public:
  explicit Modifiers(Player player)
    : m_modifiers{CsvCountWithText::readFile<Modifier>(
      getFilePath<Modifier>(player))}
    , m_player{player}
  {
  }

  void enable(const QString& name)
  {
    changeEntry(name, 1);
  }

  void disable(const QString& name)
  {
    changeEntry(name, 0);
  }

  void toggle(const QString& name)
  {
    Modifier& modifier{find(name)};
    modifier.setCount(modifier.count() == 0 ? 1 : 0);
    CsvCountWithText::writeFile<Modifier>(
      getFilePath<Modifier>(m_player), m_modifiers);
  }

  bool isActive(const QString& name) const
  {
    const Modifier& modifier{find(name)};
    return modifier.count() == 1;
  }

  const std::vector<Modifier>& modifiers() const
  {
    return m_modifiers;
  }

private:
  void changeEntry(const QString& name, int value)
  {
    Modifier& modifier{find(name)};
    modifier.setCount(value);
    CsvCountWithText::writeFile<Modifier>(
      getFilePath<Modifier>(m_player), m_modifiers);
  }

  Modifier& find(const QString& name)
  {
    const typename std::vector<Modifier>::iterator it{std::find_if(
      m_modifiers.begin(),
      m_modifiers.end(),
      [&name](const Modifier& modifier) {

        return modifier.text().contains(name, Qt::CaseInsensitive);
      })};
    assert(
      (it != m_modifiers.end()) && "Couldn't find something that matches name");
    return *it; // Check if file in build directory has right name
  }

  const Modifier& find(const QString& name) const
  {
    return const_cast<Modifiers*>(this)->find(name);
  }

  std::vector<Modifier> m_modifiers;
  Player                m_player;
};

#endif // MODIFIERS_H

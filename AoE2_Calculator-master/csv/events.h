#ifndef EVENTS_H
#define EVENTS_H
#include <vector>

#include "csv/csv_event.h"
#include "player.h"

class Events {
public:
  explicit Events(Player player);

  void enable(const QString& name);

  void disable(const QString& name);

  void toggle(const QString& name);

  bool isActive(const QString& name) const;

  const std::vector<CsvEvent>& events() const;

private:
  void changeEntry(const QString& name, int value);

  CsvEvent& find(const QString& name);

  const CsvEvent& find(const QString& name) const;

  std::vector<CsvEvent> m_events;
  Player                m_player;
};

#endif // EVENTS_H

#ifndef ENTITIES_H
#define ENTITIES_H
#include <vector>

#include <QString>

#include "csv/csv_entity.h"

class Entities {
public:
  Entities();

  void changePlayer1EntityQuantity(int newQuantity);

  void changePlayer2EntityQuantity(int newQuantity);

  CsvEntity& player1Entity();

  CsvEntity& player2Entity();

  const CsvEntity& player1Entity() const;

  const CsvEntity& player2Entity() const;

private:
  std::vector<CsvEntity> m_entities;
};

#endif // ENTITIES_H

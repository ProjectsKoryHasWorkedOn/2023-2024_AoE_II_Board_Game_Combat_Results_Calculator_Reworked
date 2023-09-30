#include <stdexcept>

#include <QFile>
#include <QTextStream>

#include "csv/entities.h"
#include "mainwindow.hpp"

namespace {
std::vector<CsvEntity> readEntities()
{
  QFile file{entitiesFilename};
  if (!file.open(
        QIODeviceBase::ReadOnly | QIODeviceBase::Text,
        QFileDevice::ReadOther)) {
    throw std::runtime_error{
      "Could not open \"" + entitiesFilename.toStdString() + "\""};
  }

  QTextStream            stream{&file};
  std::vector<CsvEntity> entities{};

  for (QString line{}; !(line = stream.readLine()).isNull();) {
    entities.push_back(CsvEntity::fromLine(line));
  }

  return entities;
}

void writeEntities(const std::vector<CsvEntity>& entities)
{
  QFile file{entitiesFilename};
  if (
    !file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text),
    QFileDevice::WriteOther) {
    throw std::runtime_error{
      "Could not open \"" + entitiesFilename.toStdString() + "\""};
  }

  QTextStream stream{&file};

  for (const CsvEntity& csvEntity : entities) {
    stream << csvEntity.toString() << '\n';
  }
}
} // anonymous namespace

Entities::Entities() : m_entities{readEntities()} {}

void Entities::changePlayer1EntityQuantity(int newQuantity)
{
  player1Entity().setEntityQuantity(newQuantity);
  writeEntities(m_entities);
}

void Entities::changePlayer2EntityQuantity(int newQuantity)
{
  player2Entity().setEntityQuantity(newQuantity);
  writeEntities(m_entities);
}

CsvEntity& Entities::player1Entity() { return m_entities[0]; }

CsvEntity& Entities::player2Entity() { return m_entities[1]; }

const CsvEntity& Entities::player1Entity() const { return m_entities[0]; }

const CsvEntity& Entities::player2Entity() const { return m_entities[1]; }

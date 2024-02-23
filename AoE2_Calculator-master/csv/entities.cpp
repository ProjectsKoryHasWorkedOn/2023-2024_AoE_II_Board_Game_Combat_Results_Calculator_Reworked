#include <stdexcept>

#include <QFile>
#include <QTextStream>

#include "csv/entities.h"
#include "mainwindow.hpp"

namespace {
std::vector<CsvEntity> readEntities()
{
  QFile file{workingDirectory.absolutePath() + entitiesFilePath};
  if (!file.open(
        QIODeviceBase::ReadOnly | QIODeviceBase::Text,
        QFileDevice::ReadOther)) {
    throw std::runtime_error{
      "Could not open \"" + entitiesFilePath.toStdString() + "\""};
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
  QFile file{workingDirectory.absolutePath() + entitiesFilePath};
  if (!file.open(
        QIODeviceBase::WriteOnly | QIODeviceBase::Text,
        QFileDevice::WriteOther)) {
    throw std::runtime_error{
      "Could not open \"" + entitiesFilePath.toStdString() + "\""};
  }

  QTextStream stream{&file};

  for (const CsvEntity& csvEntity : entities) {
    stream << csvEntity.toString() << '\n';
  }
}
} // anonymous namespace

Entities::Entities() : m_entities{readEntities()}
{
}

void Entities::changePlayer1EntityName(QString entityName)
{
  player1Entity().setEntityName(entityName);
  writeEntities(m_entities);
}

void Entities::changePlayer2EntityName(QString entityName)
{
  player2Entity().setEntityName(entityName);
  writeEntities(m_entities);
}

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

void Entities::changePlayer1AssistantName(QString newName)
{
  player1Entity().setAssistantName(newName);
  writeEntities(m_entities);
}

void Entities::changePlayer2AssistantName(QString newName)
{
  player2Entity().setAssistantName(newName);
  writeEntities(m_entities);
}

void Entities::changePlayer1AssistantQuantity(int newQuantity)
{
  player1Entity().setAssistantQuantity(newQuantity);
  writeEntities(m_entities);
}

void Entities::changePlayer2AssistantQuantity(int newQuantity)
{
  player2Entity().setAssistantQuantity(newQuantity);
  writeEntities(m_entities);
}

CsvEntity& Entities::player1Entity()
{
  return m_entities[0];
}

CsvEntity& Entities::player2Entity()
{
  return m_entities[1];
}

const CsvEntity& Entities::player1Entity() const
{
  return m_entities[0];
}

const CsvEntity& Entities::player2Entity() const
{
  return m_entities[1];
}

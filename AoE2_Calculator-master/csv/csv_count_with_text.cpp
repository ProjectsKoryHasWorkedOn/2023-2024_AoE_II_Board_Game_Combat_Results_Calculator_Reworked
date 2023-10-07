#include <cassert>

#include <QTextStream>

#include "csv/csv_count_with_text.hpp"

CsvCountWithText::CsvCountWithText(int count, QString text)
  : m_count{count}, m_text{text}
{
}

void CsvCountWithText::setCount(int newCount)
{
  m_count = newCount;
}

void CsvCountWithText::setText(QString text)
{
  m_text = text;
}

int CsvCountWithText::count() const
{
  return m_count;
}

const QString& CsvCountWithText::text() const
{
  return m_text;
}

QString CsvCountWithText::toString() const
{
  QString     buffer{};
  QTextStream stream{&buffer};

  stream << m_count << ' ' << m_text;
  return buffer;
}

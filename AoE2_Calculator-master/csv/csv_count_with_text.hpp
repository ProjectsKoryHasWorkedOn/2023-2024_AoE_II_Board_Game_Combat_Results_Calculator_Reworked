#ifndef CSV_COUNT_WITH_TEXT_HPP
#define CSV_COUNT_WITH_TEXT_HPP
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <vector>

class CsvCountWithText {
public:
  template<typename Derived>
  static Derived fromLine(const QString& line)
  {
    const QStringList parts{line.split(QStringLiteral(" "))};
    assert((parts.size() == 2) && "Unexpected count of parts");
    const QString& countString{parts[0]};
    const QString& name{parts[1]};
    bool           ok{false};
    const int      count{countString.toInt(&ok)};

    if (!ok) {
      throw std::runtime_error{
        "Unexpected input: " + countString.toStdString() + " is not numeric"};
    }

    return Derived{count, name};
  }

  template<typename Derived>
  static std::vector<Derived> readFile(const QString& filePath)
  {
    QFile file{filePath};
    if (!file.open(
          QIODeviceBase::ReadOnly | QIODeviceBase::Text,
          QFileDevice::ReadOther)) {
      throw std::runtime_error{
        "Could not open \"" + filePath.toStdString() + "\""};
    }

    QTextStream          stream{&file};
    std::vector<Derived> elements{};

    for (QString line{}; !(line = stream.readLine()).isNull();) {
      elements.push_back(fromLine<Derived>(line));
    }

    return elements;
  }

  template<typename Derived>
  static void writeFile(
    const QString&              filePath,
    const std::vector<Derived>& elements)
  {
    QFile file{filePath};
    if (!file.open(
          QIODeviceBase::WriteOnly | QIODeviceBase::Text,
          QFileDevice::WriteOther)) {
      throw std::runtime_error{
        "Could not open \"" + filePath.toStdString() + "\""};
    }

    QTextStream stream{&file};

    for (const Derived& element : elements) {
      stream << element.toString() << '\n';
    }
  }

  CsvCountWithText(int count, QString text);

  void setCount(int newCount);

  void setText(QString text);

  int count() const;

  const QString& text() const;

  QString toString() const;

private:
  int     m_count;
  QString m_text;
};

#endif // CSV_COUNT_WITH_TEXT_HPP

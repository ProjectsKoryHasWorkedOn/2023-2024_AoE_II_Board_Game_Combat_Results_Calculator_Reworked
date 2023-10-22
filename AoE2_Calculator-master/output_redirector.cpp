#include <cassert>

#include <ostream>

#include <QTextEdit>

#include "output_redirector.h"

const QString htmlLineBreak{"<br>"};

static void setLastLine(QString& lastLine, QString buffer)
{
  const qsizetype index{buffer.lastIndexOf(htmlLineBreak)};
  assert((index != -1) && "Broken");
  buffer.remove(index, htmlLineBreak.size());
  buffer += "\n";
  lastLine = buffer;
}

OutputRedirector::OutputRedirector(
  std::ostream& ostream,
  QTextEdit*&   textEdit,
  QString&      lastLine)
  : m_lastLine{lastLine}
  , m_ostream{ostream}
  , m_buffer{}
  , m_textEdit{textEdit}
  , m_streamBuffer{m_ostream, [this](char ch) {
                     m_buffer.append(QString{QChar{ch}});

                     if (m_buffer.endsWith(htmlLineBreak)) {
                       setLastLine(m_lastLine, m_buffer);
                       m_textEdit->insertHtml(m_buffer);
                       m_buffer.clear();
                     }
                   }}
{
}

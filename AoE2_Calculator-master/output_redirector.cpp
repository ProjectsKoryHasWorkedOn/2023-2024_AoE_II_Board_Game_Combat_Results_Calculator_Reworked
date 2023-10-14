#include <ostream>

#include <QTextEdit>

#include "output_redirector.h"

OutputRedirector::OutputRedirector(std::ostream& ostream, QTextEdit*& textEdit)
  : m_ostream{ostream}
  , m_buffer{}
  , m_textEdit{textEdit}
  , m_streamBuffer{m_ostream, [this](char ch) {
                     m_buffer.append(QString{QChar{ch}});

                     if (m_buffer.endsWith("<br>")) {
                       m_textEdit->insertHtml(m_buffer);
                       m_buffer.clear();
                     }
                   }}
{
}

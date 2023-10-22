#ifndef OUTPUT_REDIRECTOR_H
#define OUTPUT_REDIRECTOR_H
#include <iosfwd>

#include <QString>

#include "stream_buffer.h"

QT_FORWARD_DECLARE_CLASS(QTextEdit)

class OutputRedirector {
public:
  OutputRedirector(
    std::ostream& ostream,
    QTextEdit*&   textEdit,
    QString&      lastLine);

private:
  QString&      m_lastLine;
  std::ostream& m_ostream;
  QString       m_buffer;
  QTextEdit*&   m_textEdit;
  StreamBuffer  m_streamBuffer;
};

#endif // OUTPUT_REDIRECTOR_H

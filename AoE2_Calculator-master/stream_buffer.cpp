#include <ostream>
#include <utility>

#include "stream_buffer.h"


StreamBuffer::StreamBuffer(
  std::ostream&             destination,
  std::function<void(char)> callback)
  : m_destination{destination.rdbuf()}
  , m_ostream{&destination}
  , m_callback{std::move(callback)}
{
  m_ostream->rdbuf(this);
}

StreamBuffer::~StreamBuffer()
{
  if (m_ostream != nullptr) {
    m_ostream->rdbuf(m_destination);
  }
}

int StreamBuffer::overflow(int ch)
{
  m_callback(ch);
  return m_destination->sputc(ch);
}

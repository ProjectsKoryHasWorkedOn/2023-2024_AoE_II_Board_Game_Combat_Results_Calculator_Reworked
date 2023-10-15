#include <iostream>

#include "user_input_handler.h"

class RedirectingStreamBuf : public std::streambuf {
public:
  explicit RedirectingStreamBuf(std::streambuf* buf)
    : m_string{"1"}, m_index{0}, m_src{buf}, m_buffer{}
  {
    // TODO: HERE: No idea about this thing.

    // buffer is initially full
    setg(&m_buffer, &m_buffer + 1, &m_buffer + 1);
  }

protected:
  int underflow() override
  {
    if (m_index == m_string.size()) {
      m_string.clear();
      m_index  = 0;
      m_buffer = '\0';
      return traits_type::eof();
    }

    m_buffer = m_string[m_index];
    setg(&m_buffer, &m_buffer, &m_buffer + 1);
    m_index++;
    return traits_type::to_int_type(m_buffer);

    /*
    const traits_type::int_type i{m_src->sbumpc()};

    if (!traits_type::eq_int_type(i, traits_type::eof())) {
      m_buffer = traits_type::to_char_type(i);

      // Makes one read position available.
      setg(&m_buffer, &m_buffer, &m_buffer + 1);
    }

    return i;
*/
  }

private:
  std::string            m_string;
  std::string::size_type m_index;
  std::streambuf*        m_src;
  char                   m_buffer;
};

UserInputHandler::UserInputHandler()
  : m_stringStream{}
  , m_stringStreamBuf{m_stringStream.rdbuf()}
  , m_cinStreamBuf{std::cin.rdbuf()}
  , m_redirectingStreamBuf{
      std::make_unique<RedirectingStreamBuf>(m_stringStreamBuf)}
{
  std::cin.rdbuf(m_redirectingStreamBuf.get());

  // m_stringStream << "1";
}

UserInputHandler::~UserInputHandler()
{
  std::cin.rdbuf(m_cinStreamBuf);
  m_redirectingStreamBuf = nullptr;
}

#ifndef STREAM_BUFFER_H
#define STREAM_BUFFER_H
#include <functional>
#include <streambuf>

class StreamBuffer : public std::streambuf {
public:
  StreamBuffer(std::ostream& destination, std::function<void(char)> callback);

  ~StreamBuffer();

protected:
  int overflow(int ch) override;

private:
  std::streambuf*           m_destination;
  std::ostream*             m_ostream;
  std::function<void(char)> m_callback;
};

#endif // STREAM_BUFFER_H

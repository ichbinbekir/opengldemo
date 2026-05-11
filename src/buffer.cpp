#include "buffer.hpp"

Buffer::Buffer() {
  glCreateBuffers(1, &m_id);
}

Buffer::~Buffer() {
  glDeleteBuffers(1, &m_id);
}

void Buffer::setData(const void* data, uint32_t size, uint32_t usage) {
  glNamedBufferData(m_id, size, data, usage);
}

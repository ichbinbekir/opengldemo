#include "buffer.hpp"
#include <utility>

Buffer::Buffer() {
  glCreateBuffers(1, &m_id);
}

Buffer::~Buffer() {
  if (m_id != 0) {
    glDeleteBuffers(1, &m_id);
  }
}

Buffer::Buffer(Buffer&& other) noexcept : m_id(other.m_id) {
  other.m_id = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  if (this != &other) {
    if (m_id != 0) {
      glDeleteBuffers(1, &m_id);
    }
    m_id = other.m_id;
    other.m_id = 0;
  }
  return *this;
}

void Buffer::setData(const void* data, uint32_t size, uint32_t usage) {
  glNamedBufferData(m_id, size, data, usage);
}

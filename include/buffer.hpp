#pragma once

#include <cstdint>
#include <glad/glad.h>

class Buffer {
public:
  Buffer();
  ~Buffer();

  void setData(const void* data, uint32_t size, uint32_t usage = GL_STATIC_DRAW);
  
  uint32_t getID() const { return m_id; }

private:
  uint32_t m_id;
};

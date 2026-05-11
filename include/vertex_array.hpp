#pragma once

#include <cstdint>
#include <glad/glad.h>
#include "buffer.hpp"

class VertexArray {
public:
  VertexArray();
  ~VertexArray();

  void bind() const;
  void unbind() const;

  void enableAttribute(uint32_t index);
  void setAttributeBinding(uint32_t attrib_index, uint32_t binding_index);
  void setAttributeFormat(uint32_t index, int32_t size, uint32_t type, bool normalized, uint32_t relative_offset);
  void setVertexBuffer(uint32_t binding_index, const Buffer& buffer, uint32_t offset, uint32_t stride);

  uint32_t getID() const { return m_id; }

private:
  uint32_t m_id;
};

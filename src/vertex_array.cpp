#include "vertex_array.hpp"

VertexArray::VertexArray() {
  glCreateVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const {
  glBindVertexArray(m_id);
}

void VertexArray::unbind() const {
  glBindVertexArray(0);
}

void VertexArray::enableAttribute(uint32_t index) {
  glEnableVertexArrayAttrib(m_id, index);
}

void VertexArray::setAttributeBinding(uint32_t attrib_index, uint32_t binding_index) {
  glVertexArrayAttribBinding(m_id, attrib_index, binding_index);
}

void VertexArray::setAttributeFormat(uint32_t index, int32_t size, uint32_t type, bool normalized, uint32_t relative_offset) {
  glVertexArrayAttribFormat(m_id, index, size, type, normalized, relative_offset);
}

void VertexArray::setVertexBuffer(uint32_t binding_index, const Buffer& buffer, uint32_t offset, uint32_t stride) {
  glVertexArrayVertexBuffer(m_id, binding_index, buffer.getID(), offset, stride);
}

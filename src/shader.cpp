#include "shader.hpp"
#include <stdexcept>
#include <vector>
#include <format>

#include <glad/glad.h>

std::string_view getShaderTypeName(ShaderType type)
{
  switch (type)
  {
  case ShaderType::Vertex:
    return "Vertex";
  case ShaderType::Fragment:
    return "Fragment";
  default:
    return "Unknown";
  }
}

Shader::Shader(ShaderType type)
{
  m_type = type;
  switch (m_type)
  {
  case ShaderType::Vertex:
    m_id = glCreateShader(GL_VERTEX_SHADER);
    break;
  case ShaderType::Fragment:
    m_id = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  default:
    throw std::runtime_error("Unknown shader type");
  }
}

Shader::~Shader()
{
  glDeleteShader(m_id);
}

void Shader::compile(std::initializer_list<std::string_view> source) const
{
  auto len = source.size();
  std::vector<const char *> ptrs;
  ptrs.reserve(len);
  std::vector<int32_t> lengths;
  lengths.reserve(len);

  for (const auto &part : source)
  {
    ptrs.push_back(part.data());
    lengths.push_back(part.size());
  }

  glShaderSource(m_id, len, ptrs.data(), lengths.data());

  glCompileShader(m_id);

  int32_t success;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    int32_t len;
    glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &len);

    std::vector<char> log(len);
    glGetShaderInfoLog(m_id, len, nullptr, log.data());
    throw std::runtime_error(std::format("{} shader could not be compile: {}", getShaderTypeName(m_type), log.data()));
  }
}

#include "program.hpp"
#include <vector>
#include <format>
#include <stdexcept>

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

Program::Program(ShaderType type, const std::string &source)
{
  auto ptr = source.c_str();
  switch (type)
  {
  case ShaderType::Vertex:
    m_id = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &ptr);
    break;
  case ShaderType::Fragment:
    m_id = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &ptr);
    break;
  default:
    throw std::runtime_error("Unknown shader type");
  }
  m_type = type;

  int32_t success;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success)
  {
    int32_t len;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);

    std::vector<char> log(len);
    glGetProgramInfoLog(m_id, len, nullptr, log.data());

    throw std::runtime_error(std::format("{} shader could not be compile: {}", getShaderTypeName(type), log.data()));
  }
}

Program::~Program()
{
  glDeleteProgram(m_id);
}

int Program::getUniformLocation(const std::string &name) const
{
  return glGetUniformLocation(m_id, name.c_str());
}

void Program::setUniformMatrix2fv(int location, const float *data, uint32_t count, bool transpose) const
{
  glProgramUniformMatrix2fv(m_id, location, count, transpose, data);
}

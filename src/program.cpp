#include "program.hpp"
#include "shader.hpp"
#include <vector>
#include <format>
#include <stdexcept>

#include <glad/glad.h>

Program::Program()
{
  m_id = glCreateProgram();
}

Program::~Program()
{
  glDeleteProgram(m_id);
}

void Program::link(bool separable, std::initializer_list<std::reference_wrapper<const Shader>> shaders)
{
  if (isSeparable() != separable)
  {
    m_separable = separable;
    glProgramParameteri(m_id, GL_PROGRAM_SEPARABLE, m_separable);
  }

  for (auto shader : shaders)
  {
    const auto &ref = shader.get();
    auto type = ref.getType();
    if (m_attached.find(type) != m_attached.end())
    {
      if (m_attached[type] == ref.getID())
      {
        continue;
      }
      glDetachShader(m_id, m_attached[type]);
    }

    glAttachShader(m_id, ref.getID());
    m_attached[type] = ref.getID();
  }

  glLinkProgram(m_id);

  int32_t success;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success)
  {
    int32_t len;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);

    std::vector<char> log(len);
    glGetProgramInfoLog(m_id, len, nullptr, log.data());
    throw std::runtime_error(std::format("Program could not be link: {}", log.data()));
  }
}

void Program::detach(std::initializer_list<ShaderType> types)
{
  for (auto type : types)
  {
    auto it = m_attached.find(type);
    if (it != m_attached.end())
    {
      glDetachShader(m_id, it->second);
      m_attached.erase(it);
    }
  }
}

void Program::use() const
{
  glUseProgram(m_id);
}

uint32_t Program::getUniformLocation(const std::string &name) const
{
  return glGetUniformLocation(m_id, name.c_str());
}

void Program::setUniformMatrix2fv(int location, const float *data, uint32_t count, bool transpose) const
{
  glProgramUniformMatrix2fv(m_id, location, count, transpose, data);
}

std::vector<ShaderType> Program::getAttachedTypes() const
{
  std::vector<ShaderType> types;
  types.reserve(m_attached.size());
  for (const auto &[type, id] : m_attached)
  {
    types.push_back(type);
  }
  return types;
}

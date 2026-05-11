#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <format>
#include <fstream>
#include <sstream>

#include "FileWatch.hpp"

// TODO
const std::string readFile(const std::string &path)
{
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  file.open(path);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

void compileShaderFromFile(uint32_t shader, const std::string &path)
{
  auto source = readFile(path);
  auto ptr = source.c_str();
  glShaderSource(shader, 1, &ptr, nullptr);

  glCompileShader(shader);

  int32_t success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    int32_t len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    std::vector<char> log(len);
    glGetShaderInfoLog(shader, len, nullptr, log.data());
    throw std::runtime_error(std::format("Shader could not be compile: {}", log.data()));
  }
}

void linkProgram(uint32_t program)
{
  glLinkProgram(program);

  int32_t success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    int32_t len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

    std::vector<char> log(len);
    glGetProgramInfoLog(program, len, nullptr, log.data());
    throw std::runtime_error(std::format("Program could not be link: {}", log.data()));
  }
}

class Shader
{
private:
  uint32_t m_id;
  uint32_t m_vert;
  uint32_t m_frag;
  bool m_vert_changed;
  bool m_frag_changed;

public:
  Shader(const std::string &vertPath, const std::string &fragPath);
  ~Shader();

  void use() const;
  // void check();

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat2(const std::string &name, const float *value) const;
};

Shader::Shader(const std::string &vertPath, const std::string &fragPath)
{
  m_vert = glCreateShader(GL_VERTEX_SHADER);
  m_frag = glCreateShader(GL_FRAGMENT_SHADER);

  compileShaderFromFile(m_vert, vertPath);
  compileShaderFromFile(m_frag, fragPath);

  m_id = glCreateProgram();

  glAttachShader(m_id, m_vert);
  glAttachShader(m_id, m_frag);

  // auto prog = m_id;
  // auto shader = m_vert;
  // filewatch::FileWatch<std::string> vertWatch(
  //     vertPath,
  //     [prog, shader](const std::string &path, const filewatch::Event change_type)
  //     {
  //       if (change_type == filewatch::Event::modified)
  //       {
  //         try
  //         {
  //           compileShaderFromFile(shader, path);
  //           linkProgram(prog);
  //         }
  //         catch (const std::exception &e)
  //         {
  //           spdlog::warn(std::format("{} shader reload failed: {}", path, e.what()));
  //         }
  //         spdlog::info(std::format("{} shader reloaded", path));
  //       }
  //     });
  // shader = m_frag;
  // filewatch::FileWatch<std::string> fragWatch(
  //     fragPath,
  //     [prog, shader](const std::string &path, const filewatch::Event change_type)
  //     {
  //       if (change_type == filewatch::Event::modified)
  //       {
  //         try
  //         {
  //           compileShaderFromFile(shader, path);
  //           linkProgram(prog);
  //         }
  //         catch (const std::exception &e)
  //         {
  //           spdlog::warn(std::format("{} shader reload failed: {}", path, e.what()));
  //         }
  //         spdlog::info(std::format("{} shader reloaded", path));
  //       }
  //     });

  linkProgram(m_id);

  // hemen delete olmayacaklar, dispose aşamasında detach oldukları zaman ancak delete olabilirler ya da program delete olduğunda detach olacakları için yine olabilirler // TODO: ing yorum
  glDeleteShader(m_vert);
  glDeleteShader(m_frag);
}

Shader::~Shader()
{
  glDeleteProgram(m_id);
}

inline void Shader::use() const
{
  glUseProgram(m_id);
}

// inline void Shader::check()
// {
//   auto mustLink = m_vert_changed || m_frag_changed;
//   if (m_vert_changed)
//   {
//     compileShaderFromFile(m_vert, );
//     m_vert_changed = false;
//   }
//   if (m_frag_changed)
//   {
//   }

//   if (mustLink)
//   {
//     linkProgram(m_id);
//   }
// }

inline void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

inline void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

inline void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

inline void Shader::setMat2(const std::string &name, const float *value) const
{
  glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, false, value);
}

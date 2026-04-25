#pragma once
#include <cstdint>
#include <string>
#include <string_view>

enum class ShaderType
{
  Vertex,
  Fragment,
};

std::string_view getShaderTypeName(ShaderType type);

class Program
{
private:
  uint32_t m_id;
  ShaderType m_type;

public:
  Program(ShaderType type, const std::string &source);
  ~Program();

  uint32_t getID() const { return m_id; }
  ShaderType getType() const { return m_type; }

  int getUniformLocation(const std::string &name) const;
  void setUniformMatrix2fv(int location, const float *data, uint32_t count = 1, bool transpose = false) const;
};

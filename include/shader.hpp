#pragma once
#include <string>
#include <cstdint>

enum class ShaderType
{
  Vertex,
  Fragment,
};

std::string_view getShaderTypeName(ShaderType type);

class Shader
{
private:
  uint32_t m_id;
  ShaderType m_type;

public:
  Shader(ShaderType type);
  ~Shader();

  void compile(std::initializer_list<std::string_view> source) const;

  uint32_t getID() const { return m_id; }
  ShaderType getType() const { return m_type; }
};

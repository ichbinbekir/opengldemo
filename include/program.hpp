#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <map>
#include <vector>

enum class ShaderType;

class Shader;

class Program
{
private:
  uint32_t m_id;
  bool m_separable;
  std::map<ShaderType, uint32_t> m_attached;

public:
  Program();
  ~Program();

  void link(bool separable, std::initializer_list<std::reference_wrapper<const Shader>> shaders);
  void detach(std::initializer_list<ShaderType> types);
  void use() const;

  uint32_t getUniformLocation(const std::string &name) const;
  void setUniformMatrix2fv(int location, const float *data, uint32_t count = 1, bool transpose = false) const;

  uint32_t getID() const { return m_id; }
  bool isSeparable() const { return m_separable; };
  std::vector<ShaderType> getAttachedTypes() const;
};

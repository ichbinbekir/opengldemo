#pragma once
#include <cstdint>
#include <vector>

enum class ShaderType;

class Program;

class ProgramPipeline
{
private:
  uint32_t m_id;

public:
  ProgramPipeline();
  ~ProgramPipeline();

  void useStages(const Program &program, const std::vector<ShaderType> &stages) const;
  void bind() const;

  uint32_t getID() const { return m_id; }
};

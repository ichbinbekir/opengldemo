#pragma once
#include <cstdint>

class Program;

class ProgramPipeline
{
private:
  uint32_t m_id;

public:
  ProgramPipeline();
  ~ProgramPipeline();

  uint32_t getID() const { return m_id; }
  void setBit(const Program &program) const;
  void bind() const;
};

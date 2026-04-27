#include "program_pipeline.hpp"
#include "program.hpp"
#include "shader.hpp"
#include <stdexcept>

#include <glad/glad.h>

ProgramPipeline::ProgramPipeline()
{
  glCreateProgramPipelines(1, &m_id);
}

ProgramPipeline::~ProgramPipeline()
{
  glDeleteProgramPipelines(1, &m_id);
}

void ProgramPipeline::useStages(const Program &program, const std::vector<ShaderType> &stages) const
{
  if (!program.isSeparable())
  {
    throw std::runtime_error("Program is not separable");
  }

  if (!program.hasShaders(stages))
  {
    throw std::runtime_error("Program does not have required shaders");
  }

  GLbitfield bits = 0;
  for (auto stage : stages)
  {
    switch (stage)
    {
    case ShaderType::Vertex:
      bits |= GL_VERTEX_SHADER_BIT;
      break;
    case ShaderType::Fragment:
      bits |= GL_FRAGMENT_SHADER_BIT;
      break;
    default:
      break;
    }
  }

  glUseProgramStages(m_id, bits, program.getID());
}

void ProgramPipeline::bind() const
{
  glBindProgramPipeline(m_id);
}


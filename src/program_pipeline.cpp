#include "program_pipeline.hpp"
#include "program.hpp"

#include <glad/glad.h>

ProgramPipeline::ProgramPipeline()
{
  glCreateProgramPipelines(1, &m_id);
}

ProgramPipeline::~ProgramPipeline()
{
  glDeleteProgramPipelines(1, &m_id);
}

void ProgramPipeline::setBit(const Program &program) const
{
  // switch (program.getType())
  // {
  // case ShaderType::Vertex:
  //   glUseProgramStages(m_id, GL_VERTEX_SHADER_BIT, program.getID());
  //   break;
  // case ShaderType::Fragment:
  //   glUseProgramStages(m_id, GL_FRAGMENT_SHADER_BIT, program.getID());
  //   break;
  // }
}

void ProgramPipeline::bind() const
{
  glBindProgramPipeline(m_id);
}

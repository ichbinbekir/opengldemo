#include "asset_manager.hpp"
#include "util.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>

std::shared_ptr<Program> AssetManager::createProgram(const std::string &name, bool separable, const std::vector<std::pair<std::string, ShaderType>> &shaders, std::function<void(Program &)> onRelink)
{
  auto program = std::make_shared<Program>();
  ProgramInfo progInfo;
  progInfo.program = program;
  progInfo.onRelink = onRelink;

  std::vector<std::reference_wrapper<const Shader>> shaderRefs;

  for (const auto &[path, type] : shaders)
  {
    if (m_shaders.find(path) == m_shaders.end())
    {
      ShaderInfo info;
      info.path = path;
      info.type = type;
      info.shader = std::make_shared<Shader>(type);

      reloadShader(info);

      info.watch = std::make_unique<filewatch::FileWatch<std::string>>(
          path, [this, path](const std::string &p, const filewatch::Event change_type)
          {
            if (change_type == filewatch::Event::modified)
            {
              std::lock_guard<std::mutex> lock(m_mutex);
              if (std::find(m_dirtyShaders.begin(), m_dirtyShaders.end(), path) == m_dirtyShaders.end())
              {
                m_dirtyShaders.push_back(path);
              }
            }
          });

      m_shaders[path] = std::move(info);
    }
    progInfo.shaderPaths.push_back(path);
    shaderRefs.push_back(*m_shaders[path].shader);
  }

  try
  {
    program->link(separable, shaderRefs);
    if (onRelink)
    {
      onRelink(*program);
    }
  }
  catch (const std::exception &e)
  {
    spdlog::error("Initial link failed for program {}: {}", name, e.what());
  }

  m_programs[name] = std::move(progInfo);
  return program;
}

void AssetManager::update()
{
  std::vector<std::string> dirty;
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::swap(dirty, m_dirtyShaders);
  }

  if (dirty.empty())
    return;

  std::vector<std::string> affectedPrograms;

  for (const auto &path : dirty)
  {
    auto it = m_shaders.find(path);
    if (it != m_shaders.end())
    {
      try
      {
        reloadShader(it->second);
        spdlog::info("Shader reloaded: {}", path);

        for (auto &[progName, progInfo] : m_programs)
        {
          if (std::find(progInfo.shaderPaths.begin(), progInfo.shaderPaths.end(), path) != progInfo.shaderPaths.end())
          {
            progInfo.needsRelink = true;
          }
        }
      }
      catch (const std::exception &e)
      {
        spdlog::error("Shader reload failed: {}: {}", path, e.what());
      }
    }
  }

  for (auto &[progName, progInfo] : m_programs)
  {
    if (progInfo.needsRelink)
    {
      try
      {
        std::vector<std::reference_wrapper<const Shader>> shaderRefs;
        for (const auto &path : progInfo.shaderPaths)
        {
          shaderRefs.push_back(*m_shaders[path].shader);
        }
        progInfo.program->link(progInfo.program->isSeparable(), shaderRefs);
        if (progInfo.onRelink)
        {
          progInfo.onRelink(*progInfo.program);
        }
        spdlog::info("Program relinked: {}", progName);
      }
      catch (const std::exception &e)
      {
        spdlog::error("Program relink failed: {}: {}", progName, e.what());
      }
      progInfo.needsRelink = false;
    }
  }
}

void AssetManager::reloadShader(ShaderInfo &info)
{
  auto source = readFile(info.path);
  info.shader->compile({source});
}

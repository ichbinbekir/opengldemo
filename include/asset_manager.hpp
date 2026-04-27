#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <atomic>
#include <mutex>
#include "shader.hpp"
#include "program.hpp"
#include "FileWatch.hpp"

class AssetManager
{
public:
  struct ShaderInfo
  {
    std::string path;
    ShaderType type;
    std::shared_ptr<Shader> shader;
    std::unique_ptr<filewatch::FileWatch<std::string>> watch;
  };

  struct ProgramInfo
  {
    std::shared_ptr<Program> program;
    std::vector<std::string> shaderPaths;
    std::function<void(Program &)> onRelink;
    bool needsRelink = false;
  };

  static AssetManager &instance()
  {
    static AssetManager inst;
    return inst;
  }

  std::shared_ptr<Program> createProgram(const std::string &name, bool separable, const std::vector<std::pair<std::string, ShaderType>> &shaders, std::function<void(Program &)> onRelink = nullptr);

  void update();

private:
  AssetManager() = default;

  std::map<std::string, ShaderInfo> m_shaders;
  std::map<std::string, ProgramInfo> m_programs;
  std::mutex m_mutex;
  std::vector<std::string> m_dirtyShaders;

  void reloadShader(ShaderInfo &info);
};

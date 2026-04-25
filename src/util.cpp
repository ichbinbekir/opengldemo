#include "util.hpp"

#include <fstream>
#include <sstream>

const std::string readFile(const std::string &path)
{
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  file.open(path);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

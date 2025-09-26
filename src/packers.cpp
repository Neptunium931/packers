#include "executor/executor.hpp"
#include "executor/service.hpp"
#include "project/file.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
auto
readFile(std::string const &fileName) -> std::optional<std::string>
{
  std::ifstream file(fileName);
  if (!file.is_open())
  {
    return std::nullopt;
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
}

auto
main() -> int
{
  auto const fileContent = readFile("packers.toml").value();
  auto const package = packers::file::parseProject(fileContent);
  if (!package.has_value())
  {
    std::cerr << "Failed to parse project file\n";
    return 1;
  }
  try
  {
    std::filesystem::create_directories(std::filesystem::path{
                                          package->build.dir,
                                        } /
                                        "debug");
  }
  catch (std::filesystem::filesystem_error const &e)
  {
    std::cerr << "Failed to create build directory\n";
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}

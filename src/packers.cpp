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
  constexpr auto configFileName = "packers.toml";
  if (!std::filesystem::exists(configFileName))
  {
    std::cerr << "Project file " << configFileName << " does not exist\n";
    return 1;
  }
  auto const fileContent = readFile(configFileName).value_or("");
  auto const packageOpt = packers::file::parseProject(fileContent);
  if (!packageOpt.has_value())
  {
    std::cerr << "Failed to parse project file\n";
    return 1;
  }
  auto const &package = packageOpt.value();
  try
  {
    std::filesystem::create_directories(std::filesystem::path{
                                          package.build.dir,
                                        } /
                                        "debug");
  }
  catch (std::filesystem::filesystem_error const &exception)
  {
    std::cerr << "Failed to create build directory\n";
    std::cerr << exception.what() << '\n';
    return 1;
  }
  if (!std::filesystem::exists("src"))
  {
    std::cerr << "src directory does not exist\n";
    return 1;
  }
  auto runner = packers::Executor::Service{};
  auto objFiles = std::vector<std::string>{};
  for (auto const &file : std::filesystem::directory_iterator("src"))
  {
    std::cout << "Processing " << file.path().filename() << '\n';
    if (file.is_directory())
    {
      continue;
    }
    auto const fileName = file.path().filename().string();
    auto buildArgs = []() -> std::string
    {
      return std::string{ "clang++" }
        .append(" -Isrc")
        .append(" -Iinclude")
        .append(" -Wall")
        .append(" -Wextra")
        .append(" -O0")
        .append(" -g");
    };
    if (fileName.ends_with(".cpp"))
    {
      runner.submit(
        buildArgs()
          .append(" -std=" + package.standard)
          .append(" -o " + package.build.dir + "/" + fileName + ".o")
          .append(" -c src/" + fileName));
      objFiles.push_back(package.build.dir + "/" + fileName + ".o");
      std::cout << "Compiled " << fileName << " to " << fileName << ".o\n";
    }
  }
  runner.runAwait();
  auto linkCommand = std::string{ "clang++" }
                       .append(" -std=" + package.standard)
                       .append(" -o " + package.build.dir + "/" + package.name)
                       .append(" -Wall")
                       .append(" -Wextra")
                       .append(" -O0")
                       .append(" -g");
  std::cout << linkCommand << '\n';
  for (auto const &objFile : objFiles)
  {
    linkCommand.append(" " + objFile);
  }
  auto linkResult = packers::Executor::runSync(linkCommand);
  if (linkResult.getCode() != 0)
  {
    std::cerr << "Failed to link executable\n";
    std::cerr << linkResult.getError() << '\n';
    return 1;
  }
  std::cout << "Successfully built executable\n";
  std::cout << "Executable: " << package.build.dir + "/" + package.name
            << '\n';
  return 0;
}

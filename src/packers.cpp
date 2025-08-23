#include "executor/executor.hpp"
#include "project/file.hpp"
#include <iostream>
#include <string>
auto
main() -> int
{
  std::string const fileContent = R"(
    [package]
    name = "test"
    version = "0.1.0"
    description = "test"
    authors = "neptunium"

    [build]
    build-dir = "build"
  )";
  auto const package = packers::file::parseProject(fileContent);
  auto const result = packers::Executor::run("echo 'hello'");
  if (result)
  {
    std::cout << result->getOutput() << "\n";
  }
  return 0;
}

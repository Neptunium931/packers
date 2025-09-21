#include "executor/executor.hpp"
#include "executor/service.hpp"
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
  auto service = packers::Executor::Service({
    "cat /etc/os-release",
    "echo 'hello'",
  });
  service.run();
  service.await();

  auto const result =
    packers::Executor::runSync(std::string{ "cat /etc/ssh/sshd_config" });
  std::cout << result.getOutput() << "\n";
  std::cout << result.getError() << "\n";
  std::cout << result.getCode() << "\n";
  return 0;
}

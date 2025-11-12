#include "executor/executor.hpp"
#include <criterion/criterion.h>
#include <filesystem>
#include <iostream>
#include <string>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic add "-Wall"
#pragma clang diagnostic add "-Wextra"
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

const auto baseComand = [](const std::filesystem::path &path) -> std::string
{
  constexpr auto packersPath = "packers";
  const auto relativePathCommand =
    path.root_path().lexically_relative(path).string();
  auto command = std::string{ "cd " };
  command.append(path.string());
  command.append(" && ");
  command.append(relativePathCommand);
  command.append("/");
  command.append(packersPath);
  return command;
};

Test(example, hello)
{
  auto path = std::filesystem::path{ "example/hello" };
  auto const command = baseComand(path);

  auto const result = packers::Executor::runSync(std::string{ command });
  cr_assert(result.getCode() == 0);
}

Test(example, multiFile)
{
  auto path = std::filesystem::path{ "example/mutilFile" };
  auto const command = baseComand(path);

  auto const result = packers::Executor::runSync(std::string{ command });
  cr_assert(result.getCode() == 0);
}

Test(example, noPackers_toml)
{
  auto path = std::filesystem::path{ "example/noPackers_toml" };
  auto const command = baseComand(path);

  auto const result = packers::Executor::runSync(std::string{ command });
  cr_assert(
    result.getOutput().compare("Project file packers.toml does not exist\n"));
  cr_assert(result.getCode() == 1);
}

Test(example, emptyPackers_toml)
{
  auto path = std::filesystem::path{ "example/emptyPackers_toml" };
  auto const command = baseComand(path);

  auto const result = packers::Executor::runSync(std::string{ command });
  cr_assert(result.getOutput().compare("Failed to parse project file\n"));
  cr_assert(result.getCode() == 1);
}

#pragma GCC diagnostic pop

#ifdef __clang__
#pragma clang diagnostic pop
#endif

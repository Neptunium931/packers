#include "project/buildArgs.hpp"

namespace packers::file
{
auto
parseBuildArgs([[maybe_unused]] const std::string_view &fileContent)
  -> BuildArgs
{
  return BuildArgs{ .all = "-Wall -Wextra" };
}
}

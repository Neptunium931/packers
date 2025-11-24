#ifndef PACKERS_PROJECT_BUILDARGS_HPP
#define PACKERS_PROJECT_BUILDARGS_HPP
#include <string>
#include <string_view>

namespace packers::file
{
struct BuildArgs
{
  std::string all;
};

auto parseBuildArgs(const std::string_view &fileContent) -> BuildArgs;
}

#endif // PACKERS_PROJECT_BUILDARGS_HPP

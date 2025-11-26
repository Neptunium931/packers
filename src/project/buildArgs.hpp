#ifndef PACKERS_PROJECT_BUILDARGS_HPP
#define PACKERS_PROJECT_BUILDARGS_HPP
#include <string>
#include <toml++/toml.hpp>

namespace packers::file
{
struct BuildArgs
{
  std::string all;
};

auto parseBuildArgs(const toml::table &toml) -> BuildArgs;
}

#endif // PACKERS_PROJECT_BUILDARGS_HPP

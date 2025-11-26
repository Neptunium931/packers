#include "project/buildArgs.hpp"
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>

namespace packers::file
{
auto
parseBuildArgs(const toml::table &toml) -> BuildArgs
{
  auto all = toml["build"]["args"]["all"];
  std::string const allArgs =
    std::accumulate(all.as_array()->begin(),
                    all.as_array()->end(),
                    std::string{},
                    [](const auto &accumulator, const auto &value) -> auto
                    {
                      if (accumulator.empty())
                      {
                        return std::string{ value.value_or("") };
                      }
                      return accumulator + " " + value.value_or("");
                    });

  if (allArgs.empty())
  {
    return BuildArgs{ .all = "-Wall -Wextra" };
  }
  return BuildArgs{ .all = allArgs };
}
}

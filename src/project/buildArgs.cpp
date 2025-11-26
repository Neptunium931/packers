#include "project/buildArgs.hpp"
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>

namespace packers::file
{
auto
parseBuildArgs([[maybe_unused]] const std::string_view &fileContent)
  -> BuildArgs
{
  toml::table toml;
  try
  {
    toml = toml::parse(fileContent);
  }
  catch (const toml::parse_error &e)
  {
    std::cerr << "Error parsing package file: " << e << "\n";
  }
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

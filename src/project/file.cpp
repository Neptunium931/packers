#include "project/file.hpp"
#include "project/buildArgs.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>

namespace
{
auto
parseBuild(const toml::table &toml) -> packers::file::Build
{
  auto buildDir = toml["build"]["build-dir"].value_or<std::string>("build");
  auto buildArgs = packers::file::parseBuildArgs(toml);
  packers::file::Build buildInstance = { .dir = buildDir, .args = buildArgs };

  return buildInstance;
}
}

namespace packers::file
{

auto
parseProject(const std::string_view &fileContent) -> std::optional<Package>
{
  toml::table toml;
  try
  {
    toml = toml::parse(fileContent);
  }
  catch (const toml::parse_error &e)
  {
    std::cerr << "Error parsing package file: " << e << "\n";
    return std::nullopt;
  }
  auto name = toml["package"]["name"].value<std::string>();
  auto description = toml["package"]["description"].value<std::string>();
  auto version = toml["package"]["version"].value<std::string>();
  auto authors = toml["package"]["authors"].value<std::string>();
  auto standard = toml["package"]["standard"].value<std::string>();
  if (!name || name->empty())
  {
    std::cerr << "Package name is empty\n";
    return std::nullopt;
  }
  if (!version || version->empty())
  {
    std::cerr << "Package version is empty\n";
    return std::nullopt;
  }
  auto const build = parseBuild(toml);
  auto packers = Package{ .name = name.value(),
                          .description = description.value_or(""),
                          .version = version.value(),
                          .authors = authors.value_or(""),
                          .standard = standard.value_or("c++23"),
                          .build = build };
  return std::optional<Package>{ packers };
}
} // namespace packers::file

#include "project/file.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>

namespace packers::file
{
auto
parsePackage(const std::string_view &fileContent) -> std::optional<Package>
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
  return Package{ .name = name.value_or(""),
                  .description = description.value_or(""),
                  .version = version.value_or(""),
                  .authors=authors.value_or("") };
}
} // namespace packers::file

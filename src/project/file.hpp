#ifndef PROJECT_CONFIG_HPP
#define PROJECT_CONFIG_HPP
#include <optional>
#include <string>
#include <string_view>
namespace packers::file
{
struct Build
{
  std::string dir;
};

struct Package
{
  std::string name;
  std::string description;
  std::string version;
  std::string authors;
  Build build;
};

auto parseProject(const std::string_view &fileContent)
  -> std::optional<Package>;
} // namespace packers::file
#endif

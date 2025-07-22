#include "project/file.hpp"
#include <string>
auto
main() -> int
{
  std::string const fileContent = "";
  auto const package = packers::file::parsePackage(fileContent);
  return 0;
}

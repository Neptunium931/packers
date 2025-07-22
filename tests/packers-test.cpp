#include <criterion/criterion.h>
#include <string>
#include "project/file.hpp"

Test(files, parseEmptyFile)
{
  std::string const fileContent;
  auto const package = packers::file::parsePackage(fileContent);
  cr_assert(not(package.has_value()));
}

Test(files, parsePackageSection)
{
  std::string const fileContent = R"(
    [package]
    name = "name of package"
    description = ""
    version = "0.1.0"
    authors = ""
  )";
  auto const package = packers::file::parsePackage(fileContent);
  cr_assert(package.has_value());
  auto const &pkg = package.value();
  cr_assert_eq(pkg.name, "name of package");
  cr_assert_eq(pkg.description, "");
  cr_assert_eq(pkg.version, "0.1.0");
  cr_assert_eq(pkg.authors, "");
}

Test(files, parsePackageSectionOptional)
{
  std::string const fileContent = R"(
    [package]
    name = "name of package"
    version = "0.1.0"
  )";
  auto const package = packers::file::parsePackage(fileContent);
  cr_assert(package.has_value());
  auto const &pkg = package.value();
  cr_assert_eq(pkg.name, "name of package");
  cr_assert_eq(pkg.version, "0.1.0");
}

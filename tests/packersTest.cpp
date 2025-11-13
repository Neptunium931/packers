#include "project/file.hpp"
#include <criterion/criterion.h>
#include <string>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic add "-Wall"
#pragma clang diagnostic add "-Wextra"
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
Test(files, parseEmptyFile)
{
  std::string const fileContent;
  auto const package = packers::file::parseProject(fileContent);
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
  auto const package = packers::file::parseProject(fileContent);
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
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(package.has_value());
  auto const &pkg = package.value();
  cr_assert_eq(pkg.name, "name of package");
  cr_assert_eq(pkg.version, "0.1.0");
  cr_assert_eq(pkg.description, "");
  cr_assert_eq(pkg.authors, "");
}

Test(files, parseBuildDirDefault)
{
  std::string const fileContent = R"(
    [package]
    name = "name of package"
    version = "0.1.0"
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(package.has_value());
  auto const &pkg = package.value();
  cr_assert_eq(pkg.build.dir, "build");
}

Test(files, parseBuildDirNotDefault)
{
  std::string const fileContent = R"(
    [package]
    name = "name of package"
    version = "0.1.0"
    [build]
    build-dir = "buildDir"
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(package.has_value());
  auto const &pkg = package.value();
  cr_assert_eq(pkg.build.dir, "buildDir");
}

Test(files, parsePackageStandard)
{
  std::string const fileContent = R"(
    [package]
    name = "name of package"
    description = ""
    version = "0.1.0"
    authors = ""
    standard = "c++17"
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(package.has_value());
  auto const &pkg = package.value();
  cr_assert_eq(pkg.name, "name of package");
  cr_assert_eq(pkg.description, "");
  cr_assert_eq(pkg.version, "0.1.0");
  cr_assert_eq(pkg.authors, "");
  cr_assert_eq(pkg.standard, "c++17");
}

Test(files, parseEmptySection)
{
  std::string const fileContent = R"(
    [package]
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(not(package.has_value()));
}

Test(files, parseNotName)
{
  std::string const fileContent = R"(
    [package]
    version = "0.1.0"
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(not(package.has_value()));
}

Test(files, parseEmptyName)
{
  std::string const fileContent = R"(
    [package]
    name = ""
    version = "0.1.0"
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(not(package.has_value()));
}

Test(files, parseNotVersion)
{
  std::string const fileContent = R"(
    [package]
    name = "name of package"
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(not(package.has_value()));
}

Test(files, parseEmptyVersion)
{
  std::string const fileContent = R"(
    [package]
    name = "name of package"
    version = ""
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(not(package.has_value()));
}

Test(files, parseInvalidToml)
{
  std::string const fileContent = R"(
    [package]
    name =
  )";
  auto const package = packers::file::parseProject(fileContent);
  cr_assert(not(package.has_value()));
}

#pragma GCC diagnostic pop
#ifdef __clang__
#pragma clang diagnostic pop
#endif

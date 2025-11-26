#include "project/buildArgs.hpp"
#include <criterion/criterion.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic add "-Wall"
#pragma clang diagnostic add "-Wextra"
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
Test(buildArgs, simpleAllArgs)
{
  constexpr std::string_view fileContent = R"(
    [package]
    name = "name of package"
    description = ""
    version = "0.1.0"
    authors = ""
    [build.args]
    all = ["-Wall", "-Wextra"]
  )";

  auto const package = packers::file::parseBuildArgs(fileContent);
  cr_assert_eq(package.all, "-Wall -Wextra");
}

Test(buildArgs, aOtherSimpleAllArgs)
{
  constexpr std::string_view fileContent = R"(
    [package]
    name = "name of package"
    description = ""
    version = "0.1.0"
    authors = ""
    [build.args]
    all = ["-Wall", "-Wextra", "-Werror", "-pedantic", "-g"]
  )";

  auto const package = packers::file::parseBuildArgs(fileContent);
  cr_assert_eq(package.all, "-Wall -Wextra -Werror -pedantic -g");
}

Test(buildArgs, defaultAllArgs)
{
  constexpr std::string_view fileContent = R"(
    [package]
    name = "name of package"
    description = ""
    version = "0.1.0"
    authors = ""
    [build.args]
    all = []
  )";

  auto const package = packers::file::parseBuildArgs(fileContent);
  cr_assert_eq(package.all, "-Wall -Wextra");
}

#pragma GCC diagnostic pop
#ifdef __clang__
#pragma clang diagnostic pop
#endif

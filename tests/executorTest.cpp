#include "executor/executor.hpp"
#include <criterion/criterion.h>

Test(executor, initExecutorWithOutput)
{
  auto const result = packers::Executor::run(std::string{ "echo 'hello'" });
  cr_assert_eq(result.getOutput(), "hello\n");
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, initExecutorWithError)
{
  auto const result =
    packers::Executor::run(std::string{ "echo 'hello' 1>&2" });
  cr_assert_eq(result.getOutput(), "");
  cr_assert_eq(result.getError(), "hello\n");
  cr_assert_eq(result.getCode(), 0);
}

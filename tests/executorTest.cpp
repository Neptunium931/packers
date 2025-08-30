#include "executor/executor.hpp"
#include <criterion/criterion.h>

Test(executor, initExecutorWithOutput)
{
  auto const result =
    packers::Executor::runSync(std::string{ "echo 'hello'" });
  cr_assert_eq(result.getOutput(), "hello\n");
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, initExecutorWithError)
{
  auto const result =
    packers::Executor::runSync(std::string{ "echo 'hello' 1>&2" });
  cr_assert_eq(result.getOutput(), "");
  cr_assert_eq(result.getError(), "hello\n");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, initExecutorWithExitCode)
{
  auto const result = packers::Executor::runSync(std::string{ "cat /" });
  cr_assert_eq(result.getCode(), 1);
}

Test(executor, initExecutorWithOutputAndError)
{
  auto const result = packers::Executor::runSync(
    std::string{ "echo 'hello' 1>&2; echo 'world'" });
  cr_assert_eq(result.getOutput(), "world\n");
  cr_assert_eq(result.getError(), "hello\n");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, ExecutorAsync)
{
  auto const process =
    packers::Executor::runAsync(std::string{ "echo 'hello'" });
  auto const result = packers::Executor::waitToFinish(process);
  cr_assert_eq(result.getOutput(), "hello\n");
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
}

#include "executor/executor.hpp"
#include <criterion/criterion.h>

Test(executor, ExecutorWithOutput)
{
  auto const result =
    packers::Executor::runSync(std::string{ "echo 'hello'" });
  cr_assert_eq(result.getOutput(), "hello\n");
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, ExecutorWithError)
{
  auto const result =
    packers::Executor::runSync(std::string{ "echo 'hello' 1>&2" });
  cr_assert_eq(result.getOutput(), "");
  cr_assert_eq(result.getError(), "hello\n");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, ExecutorWithExitCode)
{
  auto const result = packers::Executor::runSync(std::string{ "cat /" });
  cr_assert_eq(result.getCode(), 1);
}

Test(executor, ExecutorWithOutputAndError)
{
  auto const result = packers::Executor::runSync(
    std::string{ "echo 'hello' 1>&2; echo 'world'" });
  cr_assert_eq(result.getOutput(), "world\n");
  cr_assert_eq(result.getError(), "hello\n");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, ExecutorAsync)
{
  auto process = packers::Executor::runAsync(std::string{ "echo 'hello'" });
  auto const result = packers::Executor::waitToFinish(process);
  cr_assert_eq(result.getOutput(), "hello\n");
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
  cr_assert(process.isFinished());
}

Test(executor, ExecutorWithLongOutput)
{
  const auto *fileContent =
    R"(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
)";
  auto const result =
    packers::Executor::runSync(std::string{ "cat tests/longFile.txt" });
  cr_assert_not(result.getOutput().compare(fileContent));
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, noOutput)
{
  auto const result =
    packers::Executor::runSync(std::string{ "pwd > /dev/null" });
  cr_assert_eq(result.getOutput(), "");
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
}

Test(executor, longRunningCommand)
{
  auto const result =
    packers::Executor::runSync(std::string{ "sleep 2 && echo 'hello'" });
  cr_assert_eq(result.getOutput(), "hello\n");
  cr_assert_eq(result.getError(), "");
  cr_assert_eq(result.getCode(), 0);
}

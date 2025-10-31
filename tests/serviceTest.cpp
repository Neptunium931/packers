#include "executor/service.hpp"
#include <criterion/criterion.h>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
Test(service, ServiceRunAndAwait)
{
  auto service = packers::Executor::Service(std::string{ "echo 'hello'" });
  service.run();
  service.await();
  auto completedProcesses = service.getCompletedProcesses();
  cr_assert_eq(completedProcesses.size(), 1);
  cr_assert_eq(completedProcesses[0].getCode(), 0);
  cr_assert_eq(completedProcesses[0].getOutput(), "hello\n");
  cr_assert_eq(completedProcesses[0].getError(), "");
}

Test(service, ServiceRunAwait)
{
  auto service = packers::Executor::Service(std::string{ "echo 'hello'" });
  service.runAwait();
  auto completedProcesses = service.getCompletedProcesses();
  cr_assert_eq(completedProcesses.size(), 1);
  cr_assert_eq(completedProcesses[0].getCode(), 0);
  cr_assert_eq(completedProcesses[0].getOutput(), "hello\n");
  cr_assert_eq(completedProcesses[0].getError(), "");
}
#pragma GCC diagnostic pop

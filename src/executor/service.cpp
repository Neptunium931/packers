#include "service.hpp"

namespace packers::Executor
{
Service::Service(std::vector<std::string> commands)
  : submittedCommands(std::move(commands))
{
}

auto
Service::submit(const std::string &command) -> void
{
  submittedCommands.push_back(command);
}

auto
Service::getCompletedProcesses()
  -> std::vector<packers::Executor::CompletedProcess>
{
  std::vector<packers::Executor::CompletedProcess> completedProcesses;
  while (!processesCompleted.empty())
  {
    completedProcesses.push_back(processesCompleted.front());
    processesCompleted.pop();
  }
  return completedProcesses;
}

auto
Service::run() -> void
{
  for (auto const &command : submittedCommands)
  {
    auto process = packers::Executor::runAsync(command);
    processesRuning.push_back(std::move(process));
  }
}

auto
Service::await() -> void
{
  for (auto const &process : processesRuning)
  {
    auto result = packers::Executor::waitToFinish(process);
    processesCompleted.push(std::move(result));
  }
}

auto
Service::runAwait() -> void
{
  run();
  await();
}
}

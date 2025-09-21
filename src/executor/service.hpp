#ifndef SERVICE_HPP
#define SERVICE_HPP
#include "executor.hpp"
#include <queue>
#include <vector>

namespace packers::Executor
{
class Service
{
  std::vector<std::string> submittedCommands;
  std::vector<packers::Executor::RunningProcess> processesRuning;
  std::queue<packers::Executor::CompletedProcess> processesCompleted;

public:
  explicit Service(std::vector<std::string> commands);
  auto submit(const std::string &command) -> void;
  auto getCompletedProcesses()
    -> std::vector<packers::Executor::CompletedProcess>;
  auto run() -> void;
  auto await() -> void;
  auto runAwait() -> void;
};
}
#endif // SERVICE_HPP

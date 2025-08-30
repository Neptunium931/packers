#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP
#include <string>
#include <string_view>
namespace packers::Executor
{
class CompletedProcess
{
  std::string output;
  std::string error;
  int code;

public:
  explicit CompletedProcess(std::string output, std::string error, int code)
    : output(std::move(output))
    , error(std::move(error))
    , code(code)
  {
  }
  [[nodiscard]] auto getOutput() const -> std::string_view;
  [[nodiscard]] auto getError() const -> std::string_view;
  [[nodiscard]] auto getCode() const -> int;
};

class RunningProcess
{
  std::string command;
  int pid;
  bool isFinish = false;
  int stdoutFd;
  int stderrFd;

public:
  explicit RunningProcess(std::string command,
                          int pid,
                          int stdoutFd,
                          int stderrFd)
    : command(std::move(command))
    , pid(pid)
    , stdoutFd(stdoutFd)
    , stderrFd(stderrFd)
  {
  }
  [[nodiscard]] auto getCommand() const -> std::string_view;
  [[nodiscard]] auto getPid() const -> int;
  [[nodiscard]] auto isFinished() -> bool;
  [[nodiscard]] auto isFinished() const -> bool;
  [[nodiscard]] auto getStdoutFd() const -> int;
  [[nodiscard]] auto getStderrFd() const -> int;
};

auto runSync(const std::string &command) -> CompletedProcess;
auto runAsync(const std::string &command) -> RunningProcess;
auto waitToFinish(const RunningProcess &process) -> CompletedProcess;
};
#endif // EXECUTOR_HPP

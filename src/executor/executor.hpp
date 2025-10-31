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
  explicit CompletedProcess(std::string tOutput, std::string tError, int tCode)
    : output(std::move(tOutput))
    , error(std::move(tError))
    , code(tCode)
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
  int stdoutFd;
  int stderrFd;
  bool isFinish = false;

public:
  explicit RunningProcess(std::string tCommand,
                          int tPid,
                          int tStdoutFd,
                          int tStderrFd)
    : command(std::move(tCommand))
    , pid(tPid)
    , stdoutFd(tStdoutFd)
    , stderrFd(tStderrFd)
  {
  }
  explicit RunningProcess(RunningProcess &&) noexcept;
  auto operator=(RunningProcess &&) noexcept -> RunningProcess &;
  RunningProcess(const RunningProcess &) = delete;
  auto operator=(const RunningProcess &) -> RunningProcess & = delete;
  ~RunningProcess();
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

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
  CompletedProcess(std::string output, std::string error, int code)
    : output(std::move(output))
    , error(std::move(error))
    , code(code)
  {
  }
  [[nodiscard]] auto getOutput() const -> std::string_view;
  [[nodiscard]] auto getError() const -> std::string_view;
  [[nodiscard]] auto getCode() const -> int;
};
auto runSync(const std::string &command) -> RunningProcess;
};
#endif // EXECUTOR_HPP

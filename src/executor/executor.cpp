#include "executor.hpp"
#include "util.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <fcntl.h>
#include <spawn.h>
// NOLINTNEXTLINE
#include <stdlib.h>
#include <string>
#include <string_view>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>

namespace
{
auto
readFd(int fileDescriptor) -> std::string
{
  constexpr size_t bufferSize = 1024;
  std::array<char, bufferSize> buffer{};
  size_t bytesRead{};
  std::string out;
  while ((bytesRead = static_cast<size_t>(
            read(fileDescriptor, buffer.data(), bufferSize - 1))) > 0)
  {
    buffer.at(bytesRead) = '\0';
    out.append(buffer.data());
    if (bytesRead < sizeof(buffer) - 1)
    {
      break;
    }
  }
  return out;
}
}

namespace packers::Executor
{
auto
CompletedProcess::getOutput() const -> std::string_view
{
  return output;
}
auto
CompletedProcess::getError() const -> std::string_view
{
  return error;
}
auto
CompletedProcess::getCode() const -> int
{
  return code;
}

RunningProcess::RunningProcess(RunningProcess &&other) noexcept
  : command(std::move(other.command))
  , pid(other.pid)
  , stdoutFd(other.stdoutFd)
  , stderrFd(other.stderrFd)
{
  other.pid = -1;
  other.stdoutFd = -1;
  other.stderrFd = -1;
}

auto
RunningProcess::operator=(RunningProcess &&other) noexcept -> RunningProcess &
{
  this->command = std::move(other.command);
  this->pid = other.pid;
  this->stdoutFd = other.stdoutFd;
  this->stderrFd = other.stderrFd;
  other.pid = -1;
  other.stdoutFd = -1;
  other.stderrFd = -1;
  return *this;
}

RunningProcess::~RunningProcess()
{
  if (stdoutFd != -1)
  {
    close(stdoutFd);
  }
  if (stderrFd != -1)
  {
    close(stderrFd);
  }
}

auto
RunningProcess::getCommand() const -> std::string_view
{
  return command;
}

auto
RunningProcess::getPid() const -> int
{
  return pid;
}

auto
RunningProcess::isFinished() -> bool
{
  if (isFinish)
  {
    return true;
  }
  int status{};
  waitpid(pid, &status, 0);
  auto isFinished = WIFEXITED(status) || WIFSIGNALED(status);
  this->isFinish = isFinished;
  return isFinished;
}

auto
RunningProcess::isFinished() const -> bool
{
  if (isFinish)
  {
    return true;
  }
  int status{};
  waitpid(pid, &status, 0);
  return WIFEXITED(status) || WIFSIGNALED(status);
}

auto
RunningProcess::getStdoutFd() const -> int
{
  return stdoutFd;
}

auto
RunningProcess::getStderrFd() const -> int
{
  return stderrFd;
}

auto
runSync(const std::string &command) -> CompletedProcess
{
  auto const process = runAsync(command);
  return waitToFinish(process);
}

auto
runAsync(const std::string &command) -> RunningProcess
{

  pid_t pid{};
  // NOLINTBEGIN(*-avoid-c-arrays)
  int stdoutFd[2]{};
  int stderrFd[2]{};
  // NOLINTEND(*-avoid-c-arrays)
  // NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  pipe(stdoutFd);
  pipe(stderrFd);
  // NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

  posix_spawn_file_actions_t action;
  int err = posix_spawn_file_actions_init(&action);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_init failed %s", strerror(err));
  }
  err = posix_spawn_file_actions_addopen(&action, 0, "/dev/null", O_RDONLY, 0);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_addopen failed %s", strerror(err));
  }

  err = posix_spawn_file_actions_adddup2(&action, stdoutFd[1], 1);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_adddup2 failed %s", strerror(err));
  }
  err = posix_spawn_file_actions_adddup2(&action, stderrFd[1], 2);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_adddup2 failed %s", strerror(err));
  }

  err = posix_spawn_file_actions_addclose(&action, stdoutFd[1]);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_addclose failed %s", strerror(err));
  }
  err = posix_spawn_file_actions_addclose(&action, stdoutFd[0]);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_addclose failed %s", strerror(err));
  }
  err = posix_spawn_file_actions_addclose(&action, stderrFd[1]);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_addclose failed %s", strerror(err));
  }
  err = posix_spawn_file_actions_addclose(&action, stderrFd[0]);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_addclose failed %s", strerror(err));
  }

  // NOLINTNEXTLINE(*-avoid-c-arrays)
  const char *spawnedArgs[] = { "/bin/sh", "-c", command.c_str(), nullptr };
  err = posix_spawn(&pid,
                    "/bin/sh",
                    &action,
                    nullptr,
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
                    const_cast<char **>(spawnedArgs),
                    environ);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn failed %s", strerror(err));
  }
  err = posix_spawn_file_actions_destroy(&action);
  if (err != 0)
  {
    // concurrency-mt-unsafe error fix in clang-tidy 21
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    fatal("posix_spawn_file_actions_destroy failed %s", strerror(err));
  }
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  return RunningProcess{ command, pid, *stdoutFd, *stderrFd };
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
auto
waitToFinish(const RunningProcess &process) -> CompletedProcess
{

  int status{};
  do // NOLINT(cppcoreguidelines-avoid-do-while)
  {
    waitpid(process.getPid(), &status, 0);
  } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(process.getStdoutFd(), &readfds);
  FD_SET(process.getStderrFd(), &readfds);
  int const maxFd = std::max(process.getStdoutFd(), process.getStderrFd()) + 1;
  struct timeval timeout{};
  constexpr auto timeoutInUsec = 10;
  timeout.tv_usec = timeoutInUsec;
  select(maxFd, &readfds, nullptr, nullptr, &timeout);
  std::string outString{};
  std::string errString{};
  if (FD_ISSET(process.getStdoutFd(), &readfds))
  {
    outString = readFd(process.getStdoutFd());
  }
  if (FD_ISSET(process.getStderrFd(), &readfds))
  {
    errString = readFd(process.getStderrFd());
  }
  return CompletedProcess{ outString, errString, WEXITSTATUS(status) };
}
#pragma GCC diagnostic pop
}

#include "executor.hpp"
#include "util.hpp"
#include <array>
#include <cstring>
#include <spawn.h>
#include <string_view>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace
{
auto
readFd(int fileDescriptor) -> std::string
{
  constexpr size_t bufferSize = 1024;
  std::array<char, bufferSize> buffer{};
  size_t bytesRead{};
  std::string out;
  while ((bytesRead = read(fileDescriptor, buffer.data(), bufferSize - 1)) > 0)
  {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    buffer[bytesRead] = '\0';
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

auto
runSync(const std::string &command) -> CompletedProcess
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
  err = posix_spawn_file_actions_destroy(&action);
  int status{};
  do // NOLINT(cppcoreguidelines-avoid-do-while)
  {
    waitpid(pid, &status, 0);
  } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(stdoutFd[0], &readfds);
  FD_SET(stderrFd[0], &readfds);
  int maxFd = std::max(stdoutFd[0], stderrFd[0]) + 1;
  select(maxFd, &readfds, nullptr, nullptr, nullptr);
  std::string outString{};
  std::string errString{};
  if (FD_ISSET(stdoutFd[0], &readfds))
  {
    outString = readFd(stdoutFd[0]);
  }
  if (FD_ISSET(stderrFd[0], &readfds))
  {
    errString = readFd(stderrFd[0]);
  }
  return { outString, errString, WEXITSTATUS(status) };
}
}

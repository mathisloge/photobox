// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CrashCollection.hpp"

#include <array>
#include <csignal>
#include <expected>
#include <span>
#include <string_view>
#include <system_error>

#include <cpptrace/utils.hpp>
#include <sys/wait.h>
#include <unistd.h>

namespace
{

class FileDescriptor
{
  public:
    explicit FileDescriptor(int fd = -1) noexcept
        : fd_(fd)
    {}

    ~FileDescriptor() noexcept
    {
        if (fd_ >= 0)
        {
            ::close(fd_);
        }
    }

    FileDescriptor(const FileDescriptor &) = delete;
    FileDescriptor &operator=(const FileDescriptor &) = delete;

    FileDescriptor(FileDescriptor &&other) noexcept
        : fd_(other.fd_)
    {
        other.fd_ = -1;
    }
    FileDescriptor &operator=(FileDescriptor &&other) noexcept
    {
        if (this != &other)
        {
            if (fd_ >= 0)
            {
                ::close(fd_);
            }
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }

    [[nodiscard]] int get() const noexcept
    {
        return fd_;
    }

  private:
    int fd_;
};

void safe_write(int fd, std::string_view msg) noexcept
{
    ::write(fd, msg.data(), msg.size());
}

void crash_handler(int /*signo*/, siginfo_t * /*info*/, void * /*context*/) noexcept
{
    safe_write(STDERR_FILENO, "SIGSEGV occurred:\n");

    std::array<cpptrace::frame_ptr, 100> buffer{};
    const auto count = cpptrace::safe_generate_raw_trace(buffer.data(), buffer.size());

    std::array<int, 2> pipe_fds{};
    if (::pipe(pipe_fds.data()) == -1)
    {
        return;
    }

    FileDescriptor read_fd(pipe_fds[0]);
    FileDescriptor write_fd(pipe_fds[1]);

    const pid_t pid = ::fork();
    if (pid == -1)
    {
        return;
    }

    if (pid == 0)
    {
        ::dup2(read_fd.get(), STDIN_FILENO);
        ::execl("PhotoboxSignalTracer", "PhotoboxSignalTracer", nullptr);
        safe_write(STDERR_FILENO,
                   "exec(PhotoboxSignalTracer) failed: Make sure the PhotoboxSignalTracer "
                   "executable is in the current working directory.\n");
        ::_exit(1);
    }

    for (const auto frame_ptr : std::span(buffer.data(), count))
    {
        cpptrace::safe_object_frame frame{};
        cpptrace::get_safe_object_frame(frame_ptr, &frame);
        ::write(write_fd.get(), &frame, sizeof(frame));
    }

    ::waitpid(pid, nullptr, 0);
    ::_exit(1);
}

void warmup_cpptrace() noexcept
{
    std::array<cpptrace::frame_ptr, 10> buffer{};
    const auto count = cpptrace::safe_generate_raw_trace(buffer.data(), buffer.size());
    if (count > 0)
    {
        cpptrace::safe_object_frame frame{};
        cpptrace::get_safe_object_frame(buffer[0], &frame);
    }
}

} // namespace

namespace Pbox
{

std::expected<void, std::error_code> install_crash_handler() noexcept
{
    cpptrace::absorb_trace_exceptions(false);
    cpptrace::register_terminate_handler();
    warmup_cpptrace();

    struct sigaction action{};
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = crash_handler;

    if (::sigaction(SIGSEGV, &action, nullptr) == -1)
    {
        return std::unexpected(std::error_code(errno, std::system_category()));
    }

    return {};
}
} // namespace Pbox

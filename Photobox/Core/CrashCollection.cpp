// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CrashCollection.hpp"
#include <array>
#include <csignal>
#include <cstring>
#include <cpptrace/utils.hpp>
#include <sys/wait.h>
#include <unistd.h>

namespace
{
struct pipe_t
{
    union {
        struct
        {
            int read_end;
            int write_end;
        };
        int data[2];
    };
};
static_assert(sizeof(pipe_t) == 2 * sizeof(int), "Unexpected struct packing");

void handler(int /*signo*/, siginfo_t * /*info*/, void * /*context*/)
{
    const char *message = "SIGSEGV occurred:\n";
    write(STDERR_FILENO, message, strlen(message));
    std::array<cpptrace::frame_ptr, 100> buffer{};
    std::size_t count = cpptrace::safe_generate_raw_trace(buffer.data(), buffer.size());
    pipe_t input_pipe{};
    pipe(input_pipe.data);
    const pid_t pid = fork();
    if (pid == -1)
    {
        return;
    }
    if (pid == 0)
    { // child
        dup2(input_pipe.read_end, STDIN_FILENO);
        close(input_pipe.read_end);
        close(input_pipe.write_end);
        execl("PhotoboxSignalTracer", "PhotoboxSignalTracer", nullptr);
        const char *exec_failure_message =
            "exec(PhotoboxSignalTracer) failed: Make sure the PhotoboxSignalTracer executable is in "
            "the current working directory and the binary's permissions are correct.\n";
        write(STDERR_FILENO, exec_failure_message, strlen(exec_failure_message));
        _exit(1);
    }
    for (std::size_t i = 0; i < count; i++)
    {
        cpptrace::safe_object_frame frame{};
        cpptrace::get_safe_object_frame(buffer[i], &frame);
        write(input_pipe.write_end, &frame, sizeof(frame));
    }
    close(input_pipe.read_end);
    close(input_pipe.write_end);
    waitpid(pid, nullptr, 0);
    _exit(1);
}

void warmup_cpptrace()
{
    std::array<cpptrace::frame_ptr, 10> buffer{};
    std::size_t count = cpptrace::safe_generate_raw_trace(buffer.data(), buffer.size());
    cpptrace::safe_object_frame frame{};
    cpptrace::get_safe_object_frame(buffer[0], &frame);
}

} // namespace

namespace Pbox
{
void install_crash_handler()
{
    cpptrace::absorb_trace_exceptions(false);
    cpptrace::register_terminate_handler();
    warmup_cpptrace();

    struct sigaction action = {0};
    action.sa_flags = 0;
    action.sa_sigaction = &handler;
    if (sigaction(SIGSEGV, &action, NULL) == -1)
    {
        perror("sigaction");
        std::exit(EXIT_FAILURE);
    }
}
} // namespace Pbox

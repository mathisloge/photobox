#pragma once
#include <exec/async_scope.hpp>
#include <stdexec/execution.hpp>

namespace Pbox
{
inline void cleanup_async_scope(exec::async_scope &scope)
{
    auto cleanup_sender = scope.on_empty();
    stdexec::sync_wait(std::move(cleanup_sender));
}
} // namespace Pbox

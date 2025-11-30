// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>
#include <iostream>
#include <cpptrace/cpptrace.hpp>
#include <unistd.h>

int main()
{
    cpptrace::object_trace trace;
    while (true)
    {
        cpptrace::safe_object_frame frame{};
        std::size_t res = fread(&frame, sizeof(frame), 1, stdin);
        if (res == 0)
        {
            break;
        }
        if (res != 1)
        {
            std::cerr << "Oops, size mismatch " << res << " " << sizeof(frame) << std::endl;
            break;
        }
        trace.frames.emplace_back(frame.resolve());
    }
    trace.resolve().print();
}

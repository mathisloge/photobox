// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/PrintLicenseInfo.hpp"
#include <print>

namespace Pbox
{
void printLicenseInfo()
{
    //! TODO: implement subcommands.
    std::println("Photobox Copyright (C) 2024  Mathis Logemann");
    std::println("This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.");
    std::println("This is free software, and you are welcome to redistribute it");
    std::println("under certain conditions; type `show c' for details.");
}
} // namespace Pbox

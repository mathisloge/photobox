// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QtClassHelperMacros>
#include <filesystem>

class QPrinter;
namespace Pbox
{
class CollageRenderer;
class CollagePrinter
{
  public:
    Q_DISABLE_COPY_MOVE(CollagePrinter);
    explicit CollagePrinter(const std::filesystem::path &settings);
    ~CollagePrinter();

    void print(CollageRenderer &renderer);

  private:
    std::unique_ptr<QPrinter> printer_;
};
} // namespace Pbox
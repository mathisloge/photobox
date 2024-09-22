#pragma once
#include <filesystem>

class QPrinter;
namespace Pbox
{
class CollageRenderer;
class CollagePrinter
{
  public:
    explicit CollagePrinter(const std::filesystem::path &settings);

    void print(CollageRenderer &renderer);

  private:
    std::unique_ptr<QPrinter> printer_;
};
} // namespace Pbox
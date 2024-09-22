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
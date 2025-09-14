#pragma once
#include <memory>
namespace Pbox
{
template <typename T>
using Instance = std::shared_ptr<T>;
}

#pragma once
#include <stdexcept>

namespace Pbox::GPhoto2
{
class GPhoto2Exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
} // namespace Pbox::GPhoto2

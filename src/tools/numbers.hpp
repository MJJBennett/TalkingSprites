#ifndef TS_NUMBERTOOLS_HPP
#define TS_NUMBERTOOLS_HPP

#include <array>

namespace ts::tools
{
template <typename T, typename DT>
auto coordinate_div(T x, T y, DT d) -> std::array<T, 2>
{
    if (x < 0 && x % d != 0) x -= d;
    if (y < 0 && y % d != 0) y -= d;
    return {x / d, y / d};
}
}  // namespace ts::tools

#endif  // TS_NUMBERTOOLS_HPP

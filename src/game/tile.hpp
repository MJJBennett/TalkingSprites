#ifndef TS_GAMETILE_HPP
#define TS_GAMETILE_HPP

namespace ts
{
struct Tile
{
    enum class Type
    {
        grass = 0,
        rock
    } type = Type::grass;
};
}  // namespace ts

#endif  // TS_GAMETILE_HPP

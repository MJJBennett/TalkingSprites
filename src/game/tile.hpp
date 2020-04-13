#ifndef TS_GAMETILE_HPP
#define TS_GAMETILE_HPP

#include <string>

namespace ts
{
struct Tile
{
    enum class Type
    {
        grass = 0,
        rock,
        cave,
        unloaded,
    } type = Type::grass;
};

inline std::string to_string(::ts::Tile::Type type)
{
    using Ty = ts::Tile::Type;
    switch(type)
    {
        case Ty::grass: return "grass";
        case Ty::rock: return "rock";
        case Ty::cave: return "cave";
        case Ty::unloaded: return "unloaded";
        default: return "error";
    }
}

inline std::string to_string(::ts::Tile tile)
{
    return to_string(tile.type);
}
}  // namespace ts

#endif  // TS_GAMETILE_HPP

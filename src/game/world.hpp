#ifndef TS_WORLD_HPP
#define TS_WORLD_HPP

#include "tile.hpp"
#include "tools/serializable.hpp"

#include <array>
#include <random>
#include <unordered_map>
#include <vector>

namespace ts
{
using area_size_t                      = unsigned short;
using area_component_t                 = long;
using area_pos_t                       = std::pair<area_component_t, area_component_t>;
static constexpr area_size_t area_size = 8;

struct AreaPositionHash
{
    std::size_t operator()(const area_pos_t& area_pos) const
    {
        return std::hash<area_component_t>()(area_pos.first) ^
               std::hash<area_component_t>()(area_pos.second);
    }
};

struct Area
{
    std::array<ts::Tile, area_size>& operator[](area_size_t i) { return area[i]; }
    const std::array<ts::Tile, area_size>& operator[](area_size_t i) const { return area[i]; }
    std::array<std::array<ts::Tile, area_size>, area_size> area;
};

using world_t = std::unordered_map<area_pos_t, Area, AreaPositionHash>;

class World : public ::ts::serializable
{
public:
    World(long seed_ = 0) : seed(seed_), prng(seed_), distribution(0, 15) {}

    /* World generation */
    ts::Area generate_area(long x, long y);
    void load_area(long x, long y)
    {
        if (world.find({x, y}) != world.end()) return;
        world.emplace(std::pair{x, y}, generate_area(x, y));
    }
    void unload_area(long x, long y) { world.erase({x, y}); }

    std::string get_string() override { return ""; }

    const world_t& get() const { return world; }

private:
    world_t world;
    long seed;
    std::mt19937 prng;
    std::uniform_int_distribution<int> distribution;
};
}  // namespace ts

#endif  // TS_WORLD_HPP

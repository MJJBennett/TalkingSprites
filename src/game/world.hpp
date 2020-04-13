#ifndef TS_WORLD_HPP
#define TS_WORLD_HPP

#include "tile.hpp"
#include "tools/serializable.hpp"
#include "tools/numbers.hpp"

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
    World(long seed_ = 0) : seed(seed_), prng(seed_), distribution(0, 15), rare_distribution(0, 100) {}

    static std::array<long, 2> tile_to_area(long x, long y)
    {
        return ts::tools::coordinate_div(x, y, area_size);
    }

    static std::array<long, 2> tile_mod(long x, long y)
    {
        x %= area_size;
        y %= area_size;
        return {x, y};
    }

    // Expects tile coordinates
    ts::Tile::Type tile_type_at(std::array<long, 2> arr) const
    {
        const auto [tx, ty] = arr; 
        const auto [ax, ay] = tile_to_area(tx, ty); 
        if (const auto it = world.find({ax, ay}); it != world.end())
        {
            const auto [ix, iy] = tile_mod(tx, ty);
            return it->second[ix][iy].type;
        }
        return ts::Tile::Type::grass;
    }

    /* World generation */
    ts::Area generate_area(long x, long y);
    void load_area(long x, long y);
    void unload_area(long x, long y) { world.erase({x, y}); }
    void unload_areas(long x, long y);

    std::string get_string() const override;
    std::string from_string(std::string str) override;

    const world_t& get() const { return world; }

    void print_debug() const;

    long get_seed() const { return seed; }
    void set_seed(long _seed) { seed = _seed; world.clear(); }

private:
    world_t world;
    long seed;
    std::mt19937 prng;
    std::uniform_int_distribution<int> distribution;
    std::uniform_int_distribution<int> rare_distribution;
};
}  // namespace ts

#endif  // TS_WORLD_HPP

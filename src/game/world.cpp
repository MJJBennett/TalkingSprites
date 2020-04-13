#include "world.hpp"

#include "tools/debug.hpp"
#include "tools/string.hpp"

// Expects tile coordinates
ts::Tile::Type ts::World::tile_type_at(std::array<long, 2> arr) const
{
    const auto [tx, ty] = arr;
    const auto [ax, ay] = tile_to_area(tx, ty);
    if (const auto it = world.find({ax, ay}); it != world.end())
    {
        //ts::log::message<4>("thing > ", ax, ", ", ay);
        const auto [ix, iy] = tile_mod(tx, ty);
        //ts::log::message<4>("thing > ", ix, ", ", iy);
        return it->second[ix][iy].type;
    }
    return ts::Tile::Type::unloaded;
}

ts::Area ts::World::generate_area(long x, long y)
{
    // https://stackoverflow.com/questions/22923551/generating-number-0-1-using-mersenne-twister-c
    prng.seed(seed ^ x ^ y);
    distribution(prng);
    rare_distribution(prng);

    Area a{};

    for (area_size_t x_pos = 0; x_pos < area_size; x_pos++)
    {
        for (area_size_t y_pos = 0; y_pos < area_size; y_pos++)
        {
            if (rare_distribution(prng) == 0)
            {
                a[x_pos][y_pos].type = ts::Tile::Type::cave;
                continue;
            }
            a[x_pos][y_pos].type =
                (distribution(prng) > 3 ? ts::Tile::Type::grass : ts::Tile::Type::rock);
        }
    }

    return a;
}

void ts::World::load_area(long x, long y)
{
    if (world.find({x, y}) != world.end()) return;
    ts::log::message<1>("World: Loading area at ", x, ", ", y);
    world.emplace(std::pair{x, y}, generate_area(x, y));
}

void ts::World::unload_areas(long x, long y)
{
    for (auto itr = world.begin(); itr != world.end();)
    {
        const auto [kx, ky] = itr->first;
        if (kx + 2 < x || kx - 2 > x || ky + 2 < y || ky - 2 > y)
        {
            ts::log::message<1>("World: Unloading area at ", kx, ", ", ky);
            itr = world.erase(itr);
            ts::log::message<1>("World: Unloaded area at ", kx, ", ", ky);
        }
        else
            itr++;
    }
}

std::string ts::World::get_string() const
{
    constexpr char sep = '|';
    // Serialize seed
    const auto seed_str = std::to_string(seed);
    // Send everything
    return seed_str + sep;
}

std::string ts::World::from_string(std::string str)
{
    constexpr char sep = '|';
    // Split into values
    const auto [seed_str, _rem] = ts::tools::splitn<2>(str, sep);
    // Deserialize
    if (const auto seed_opt = ts::tools::stol(seed_str); seed_opt)
    {
        seed = *seed_opt;
    }

    for (auto& [k, v] : world)
    {
        v = generate_area(k.first, k.second);
    }

    return _rem;
}

void ts::World::print_debug() const
{
    ts::log::message<10>("World => Debug Information:");
    ts::log::message<10>("\tSeed: ", seed);
    ts::log::message<10>("\tAreas loaded (", world.size(), "):");
    std::string areas = "\t[ ";
    int count         = 1;
    for (const auto [k, v] : world)
    {
        if (count++ % 8 == 0) areas += "\n\t  ";
        areas += "(" + std::to_string(k.first) + "," + std::to_string(k.second) + "), ";
    }
    if (areas.size() > 4)
    {
        areas.pop_back();
        areas.pop_back();
        areas += " ";
    }
    areas += "]";
    ts::log::message<10>(areas);
}

#include "world.hpp"

#include "tools/string.hpp"

ts::Area ts::World::generate_area(long x, long y)
{
    // https://stackoverflow.com/questions/22923551/generating-number-0-1-using-mersenne-twister-c
    prng.seed(seed ^ x ^ y);
    distribution(prng);

    Area a{};

    for (area_size_t x_pos = 0; x_pos < area_size; x_pos++)
    {
        for (area_size_t y_pos = 0; y_pos < area_size; y_pos++)
        {
            a[x_pos][y_pos].type =
                (distribution(prng) > 3 ? ts::Tile::Type::grass : ts::Tile::Type::rock);
        }
    }

    return a;
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

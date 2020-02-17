#include "world.hpp"

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

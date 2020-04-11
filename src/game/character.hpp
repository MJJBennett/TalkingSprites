#ifndef TS_GAMECHARACTER_HPP
#define TS_GAMECHARACTER_HPP

#include "fwd/sfml.hpp"
#include "graphics/resource.hpp"
#include "tools/numbers.hpp"
#include "tools/serializable.hpp"

#include <array>

namespace ts
{
class Character : public ::ts::serializable
{
public:
    void set_position(int x_, int y_)
    {
        if (x_ == x && y_ == y) return;
        x       = x_;
        y       = y_;
        updated = true;
    }
    void set_tile_position(int x_, int y_)
    {
        set_position(move_delta * x_, move_delta * y_);
    }
    std::array<int, 2> get_position() const { return {x, y}; }
    std::array<long, 2> get_tile() const { return {x / move_delta, y / move_delta}; }
    void move_left()
    {
        x -= move_delta;
        updated = true;
    }
    void move_right()
    {
        x += move_delta;
        updated = true;
    }
    void move_up()
    {
        y -= move_delta;
        updated = true;
    }
    void move_down()
    {
        y += move_delta;
        updated = true;
    }

    void set_sprite(ts::SpriteHandle h) { handle = h; }
    ts::SpriteHandle get_sprite() { return handle; }

    void sync(sf::Sprite& sprite);

    // Serialization
    std::string get_string() const override;
    std::string from_string(std::string) override;
    virtual ~Character() = default;

    char id = ' ';
    bool updated{false};

private:
    const int move_delta{32};
    int x{0};
    int y{0};
    ts::SpriteHandle handle{0};
};
}  // namespace ts

#endif  // TS_GAMECHARACTER_HPP

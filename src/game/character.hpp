#ifndef TS_GAMECHARACTER_HPP
#define TS_GAMECHARACTER_HPP

#include "fwd/sfml.hpp"
#include "graphics/resource.hpp"

#include <array>

namespace ts
{
class Character
{
public:
    void set_position(int x_, int y_)
    {
        if (x_ == x && y_ == y) return;
        x       = x_;
        y       = y_;
        updated = true;
    }
    std::array<int, 2> get_position() const { return {x, y}; }
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

#ifndef TS_GAMECHARACTER_HPP
#define TS_GAMECHARACTER_HPP

#include "fwd/sfml.hpp"
#include "graphics/resource.hpp"

namespace ts
{
class Character
{
public:
    void set_position(int x_, int y_)
    {
        x = x_;
        y = y_;
    }
    void move_left() { x -= move_delta; }
    void move_right() { x += move_delta; }
    void move_up() { y -= move_delta; }
    void move_down() { y += move_delta; }

    void set_sprite(ts::SpriteHandle h) { handle = h; }
    ts::SpriteHandle get_sprite() { return handle; }

    void sync(sf::Sprite& sprite);

private:
    const int move_delta{32};
    int x{0};
    int y{0};
    ts::SpriteHandle handle{0};
};
}  // namespace ts

#endif  // TS_GAMECHARACTER_HPP

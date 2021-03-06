#include "character.hpp"

#include "tools/debug.hpp"
#include "tools/string.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <cassert>

void ts::Character::sync(sf::Sprite& sprite)
{
    sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
}

std::string ts::Character::from_string(std::string str)
{
    const auto [string_id, string_x, string_y, _rem] = ts::tools::splitn<4>(str, '|');
    const auto opt_x                                 = ts::tools::stoi(string_x);
    const auto opt_y                                 = ts::tools::stoi(string_y);
    assert(opt_x && opt_y);  // Failure
    x  = *opt_x;
    y  = *opt_y;
    id = string_id[0];
    // We might be subclassed; return additional
    if (!_rem.empty()) ts::log::message<1>("Character: Returning extra string data: ", _rem);
    return _rem;
}

std::string ts::Character::get_string() const
{
    const auto [char_x, char_y] = get_position();
    const auto string_x         = std::to_string(char_x);
    const auto string_y         = std::to_string(char_y);
    return std::string{id} + '|' + string_x + '|' + string_y + '|';
}

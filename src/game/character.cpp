#include "character.hpp"

#include "tools/string.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <cassert>

void ts::Character::sync(sf::Sprite& sprite)
{
    sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
}

void ts::Character::from_string(std::string str)
{
    const auto [string_id, string_x, string_y, other] = ts::tools::splitn<4>(str, '|');
    const auto opt_x               = ts::tools::stoi(string_x);
    const auto opt_y               = ts::tools::stoi(string_y);
    if (!opt_x || !opt_y) assert(false);  // Failure, really
    x = *opt_x;
    y = *opt_y;
    id = string_id[0];
    // Ignore other
}

std::string ts::Character::get_string() const
{
    const auto [char_x, char_y] = get_position();
    const auto string_x = std::to_string(char_x);
    const auto string_y = std::to_string(char_y);
    return std::string{id} + "|" + string_x + '|' + string_y + '|';
}

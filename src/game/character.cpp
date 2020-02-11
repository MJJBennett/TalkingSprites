#include "character.hpp"

#include <SFML/Graphics/Sprite.hpp>

void ts::Character::sync(sf::Sprite& sprite)
{
    sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
}

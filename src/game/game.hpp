#ifndef TS_GAME_HPP
#define TS_GAME_HPP

#include "game/keyboard.hpp"

#include <SFML/Window/Event.hpp>

namespace ts
{
class Game
{
public:
    enum class Response
    {
        none,
        close_window,
    };
public:
    Response handle_keyevent(const sf::Event& e);
    Keyboard keyboard{"resources/config/keyfile.dump"};
};
}  // namespace ts

#endif  // TS_GAME_HPP

#ifndef TS_GAME_HPP
#define TS_GAME_HPP

#include "game/keyboard.hpp"
#include "game/player.hpp"

#include <SFML/Window/Event.hpp>

namespace ts
{
class Renderer;

class Game
{
public:
    enum class Response
    {
        none,
        close_window,
    };

public:
    Game(ts::Renderer& r);

    void render();

    Response handle_keyevent(const sf::Event& e);

private:
    ts::Player player;
    ts::Renderer& renderer;
    Keyboard keyboard{"resources/config/keyfile.dump"};
};
}  // namespace ts

#endif  // TS_GAME_HPP

#ifndef TS_GAME_HPP
#define TS_GAME_HPP

#include "game/keyboard.hpp"
#include "game/state.hpp"

#include <SFML/Window/Event.hpp>

namespace ts
{
class Renderer;
class GameClient;

class Game
{
public:
    enum class Response
    {
        none,
        close_window,
    };

public:
    Game(ts::Renderer& r, ts::GameClient& c);

    void render();
    void update();

    Response handle_keyevent(const sf::Event& e);

    void update_player(const std::string& str);

private:
    ts::GameState state;
    ts::TextureHandle default_player{};
    ts::Renderer& renderer;
    ts::GameClient& client;
    Keyboard keyboard{"resources/config/keyfile.dump"};
};
}  // namespace ts

#endif  // TS_GAME_HPP

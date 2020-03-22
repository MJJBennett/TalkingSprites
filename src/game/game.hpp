#ifndef TS_GAME_HPP
#define TS_GAME_HPP

#include "game/keyboard.hpp"
#include "game/state.hpp"

#include <SFML/Window/Event.hpp>

namespace ts
{
class Renderer;
class GameClient;
class World;
struct Config;

class Game
{
public:
    enum class Response
    {
        none,
        close_window,
    };

public:
    Game(ts::Renderer&, ts::GameClient&, ts::World&, ts::Config&);

    void render();
    void update();

    Response handle_keyevent(const sf::Event& e);

    void update_player(const std::string& str);
    void update_world(const std::string& str);

    bool debug{false};

private:
    ts::GameState state;
    ts::TextureHandle default_player{};
    ts::TextureHandle local_player{};
    ts::Renderer& renderer;
    ts::GameClient& client;
    ts::World& world;
    ts::Config& config;
    Keyboard keyboard{"resources/config/keyfile.dump"};
};
}  // namespace ts

#endif  // TS_GAME_HPP

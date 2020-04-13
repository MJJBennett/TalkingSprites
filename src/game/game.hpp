#ifndef TS_GAME_HPP
#define TS_GAME_HPP

#include "game/keyboard.hpp"
#include "game/state.hpp"
#include "widgets/balance.hpp"

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
    void draw_widgets(); // widgets only

    Response handle_keyevent(const sf::Event& e);
    void handle_command(const std::string& cmd);

    void update_player(const std::string& str);
    void update_world(const std::string& str);

    bool debug{false};
    bool focus_chat{true};

    std::array<long, 2> get_player_tile();
    std::array<long, 2> get_player_position();

    std::function<void(const std::string&)> chat_focus_callback;
    std::function<void()> chat_close_toggle_callback;

private:
    ts::Player& get_player() { return state.players[0]; }
    ts::Tile::Type get_player_tile_type();

private:
    ts::GameState state;
    ts::TextureHandle default_player{};
    ts::TextureHandle local_player{};
    ts::Renderer& renderer;
    ts::GameClient& client;
    ts::World& world;
    ts::Config& config;
    Keyboard keyboard{"resources/config/keyfile.dump"};
    Balance balance;
};
}  // namespace ts

#endif  // TS_GAME_HPP

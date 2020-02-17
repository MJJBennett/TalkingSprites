#include "game.hpp"

#include "graphics/renderer.hpp"

ts::Game::Game(ts::Renderer& r) : renderer(r)
{
    player.set_sprite(
        renderer.load_sprite(renderer.load_texture("resources/sprites/player_1.png")));
}

ts::Game::Response ts::Game::handle_keyevent(const sf::Event& e)
{
    const bool press = e.type == sf::Event::KeyPressed;
    const auto key   = keyboard.get_key(e.key.code);

    if (press) return Response::none;

    if (keyboard.rebind(e.key.code)) return Response::none;
    switch (key)
    {
        case ts::key::close_game: return Response::close_window;
        case ts::key::path_right:
            player.move_right();
            player.sync(renderer.get_sprite(player.get_sprite()));
            return Response::none;
        case ts::key::path_up:
            player.move_up();
            player.sync(renderer.get_sprite(player.get_sprite()));
            return Response::none;
        case ts::key::path_down:
            player.move_down();
            player.sync(renderer.get_sprite(player.get_sprite()));
            return Response::none;
        case ts::key::path_left:
            player.move_left();
            player.sync(renderer.get_sprite(player.get_sprite()));
            return Response::none;
        default: return Response::none;
    }
}

void ts::Game::render() { renderer.render(player.get_sprite()); }

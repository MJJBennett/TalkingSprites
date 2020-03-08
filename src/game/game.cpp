#include "game.hpp"

#include "game/client.hpp"
#include "graphics/renderer.hpp"
#include "tools/string.hpp"

ts::Game::Game(ts::Renderer& r, ts::GameClient& c) : renderer(r), client(c)
{
    state.players.emplace_back();
    default_player = renderer.load_texture("resources/sprites/player_1.png");
    state.players[0].set_sprite(renderer.load_sprite(default_player));
}

void ts::Game::update()
{
    while (!client.game_updates.empty())
    {
        const auto update = client.game_updates.back();
        switch (update[1])
        {
            case 'P':
            {
                const auto [id, nxs, nys, other] = ts::tools::splitn<4>(update, '|');
                bool found                       = false;
                const auto nxo                   = ts::tools::stoi(nxs);
                const auto nyo                   = ts::tools::stoi(nys);
                if (!nxo || !nyo) break;
                for (auto&& p : state.players)
                {
                    if (p.id == id[0])
                    {
                        found = true;
                        p.set_position(*nxo, *nyo);
                        p.sync(renderer.get_sprite(p.get_sprite()));
                    }
                }
                if (!found)
                {
                    state.players.emplace_back();
                    state.players.back().id = id[0];
                    state.players.back().set_position(*nxo, *nyo);
                    state.players.back().set_sprite(renderer.load_sprite(default_player));
                    state.players.back().sync(
                        renderer.get_sprite(state.players.back().get_sprite()));
                }
                break;
            }
            default: break;
        }
        client.game_updates.pop();
    }
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
            state.players[0].move_right();
            state.players[0].sync(renderer.get_sprite(state.players[0].get_sprite()));
            break;
        case ts::key::path_up:
            state.players[0].move_up();
            state.players[0].sync(renderer.get_sprite(state.players[0].get_sprite()));
            break;
        case ts::key::path_down:
            state.players[0].move_down();
            state.players[0].sync(renderer.get_sprite(state.players[0].get_sprite()));
            break;
        case ts::key::path_left:
            state.players[0].move_left();
            state.players[0].sync(renderer.get_sprite(state.players[0].get_sprite()));
            break;
        default: return Response::none;
    }

    if (state.players[0].updated)
    {
        const auto [x, y] = state.players[0].get_position();
        client.send(ts::player_update_str + '|' + std::to_string(x) + '|' + std::to_string(y) +
                    '|');
    }
    return Response::none;
}

void ts::Game::render()
{
    for (auto& p : state.players)
    {
        renderer.render(p.get_sprite());
    }
}

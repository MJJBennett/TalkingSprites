#include "game.hpp"

#include "game/client.hpp"
#include "game/world.hpp"
#include "graphics/renderer.hpp"
#include "tools/debug.hpp"
#include "tools/string.hpp"
#include "config.hpp"

ts::Game::Game(ts::Renderer& r, ts::GameClient& c, ts::World& w, ts::Config& config_) : renderer(r), client(c), world(w), config(config_)
{
    state.players.emplace_back();
    default_player = renderer.load_texture("resources/sprites/player_01.png");
    local_player = renderer.load_texture(config.avatar);
    state.players[0].set_sprite(renderer.load_sprite(local_player));
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
                update_player(update.substr(ts::player_update_str.size()));
                break;
            }
            case 'U':
            {
                state.players[0].id = update.at(ts::username_request_str.size());
                break;
            }
            case 'S':
            {
                // Game status, we need to load many things!

                // We could make this a lot more efficient but we'll do it the fast
                // way to get this working.
                const auto str  = update.substr(ts::status_response_str.size());
                const auto strs = ts::tools::splitv(str, '\n');
                ts::log::message<1>("Game: Performing ", strs.size(), " status updates.");
                for (const auto& s : strs)
                {
                    if (s.empty()) continue;
                    if (ts::tools::startswith(s, ts::stat_world))
                    {
                        update_world(s.substr(ts::stat_world.size()));
                        continue;
                    }
                    if (ts::tools::startswith(s, ts::stat_player))
                    {
                        update_player(s.substr(ts::stat_player.size()));
                        continue;
                    }
                }
                break;
            }
            default:
            {
                ts::log::warn("Game: Ignored update:\n", update);
                break;
            }
        }
        client.game_updates.pop();
    }
}

void ts::Game::update_world(const std::string& update_substr)
{
    // The fact that this is so simple makes me pretty happy
    // In the future, proper serialization-deserialization might make
    // this style of networked game really simple to implement, once
    // serialization is taken care of.
    world.from_string(update_substr);
}

void ts::Game::update_player(const std::string& update_substr)
{
    const auto [id, nxs, nys, other] = ts::tools::splitn<4>(update_substr, '|');
    bool found                       = false;
    const auto nxo                   = ts::tools::stoi(nxs);
    const auto nyo                   = ts::tools::stoi(nys);
    if (!nxo || !nyo) return;
    for (auto&& p : state.players)
    {
        if (p.id == id[0])
        {
            ts::log::message<1>("Game: Updating player at id(", id[0], ") and position(", *nxo,
                                ", ", *nyo, ")");
            found = true;
            p.from_string(update_substr);
            p.sync(renderer.get_sprite(p.get_sprite()));
            break;
        }
    }
    if (!found)
    {
        ts::log::message<1>("Game: Adding new player at id(", id[0], ") and position(", *nxo, ", ",
                            *nyo, ")");
        state.players.emplace_back();
        state.players.back().from_string(update_substr);
        state.players.back().set_sprite(renderer.load_sprite(default_player));
        state.players.back().sync(renderer.get_sprite(state.players.back().get_sprite()));
    }
}

ts::Game::Response ts::Game::handle_keyevent(const sf::Event& e)
{
    const bool press = e.type == sf::Event::KeyPressed;
    const auto key   = keyboard.get_key(e.key.code);

    if (!press) return Response::none;

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
        case ts::key::close_chat: break;
        case ts::key::open_chat: break;
        case ts::key::toggle_debug:
        {
            debug = !debug;
            ts::log::message<1>("Set debug status to ", debug ? "true" : "false");
            break;
        }
        default: return Response::none;
    }

    if (state.players[0].updated)
    {
        if (debug)
            ts::log::message<1>("Local player was updated due to: ", to_string(key), " keypress.");
        client.send(ts::player_update_str + state.players[0].get_string());
        state.players[0].updated = false;
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

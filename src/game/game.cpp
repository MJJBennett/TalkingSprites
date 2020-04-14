#include "game.hpp"

#include "config.hpp"
#include "game/client.hpp"
#include "game/world.hpp"
#include "graphics/renderer.hpp"
#include "tools/debug.hpp"
#include "tools/string.hpp"

ts::Game::Game(ts::Renderer& r, ts::GameClient& c, ts::World& w, ts::Config& config_)
    : renderer(r), client(c), world(w), config(config_)
{
    state.players.emplace_back(config.avatar);
    state.players.back().username = config.username;  // simple fix
    default_player                = renderer.load_texture("resources/sprites/player_01.png");
    local_player                  = renderer.load_texture(config.avatar);
    state.players[0].set_sprite(renderer.load_sprite(local_player));
    challenge.challenge_callback = [this](const std::string& str) { run_challenge(str); };
}

void ts::Game::update()
{
    while (!client.game_updates.empty())
    {
        const auto update = client.game_updates.back();
        if (update[0] == '/')
        {
            handle_command(update);
            client.game_updates.pop();
            continue;
        }
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
                // Send the player to the remote server to load things properly
                client.send(ts::player_init_str + config.username + '|' +
                            state.players[0].get_string());
                ts::log::message<1>(
                    "Game: Responding to username request with initialization string.");
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
                ts::log::message<1>("-> Completed status updates.");
                break;
            }
            case 'L':
            {
                // Should be challenge completion!
                const auto [stat, enemy] = ts::tools::splitn<2>(update.substr(ts::challenge_str.size()), '|');
                size_t i = 1;
                for (; i < state.players.size(); i++)
                {
                    if (state.players[i].username == enemy)
                    {
                        break;
                    }
                }
                if (stat == "tie")
                {
                    client.chat_local("Your match against " + enemy + " ended in a draw.");
                }
                if (stat == "lose")
                {
                    client.chat_local("Your match against " + enemy + " ended in a loss :(");
                    state.players[i].balance += get_player().balance;
                    get_player().balance = 0;
                }
                if (stat == "win")
                {
                    client.chat_local("Your match against " + enemy + " ended in a win!");
                    get_player().balance += state.players[i].balance;
                    state.players[i].balance = 0;
                }
                challenge.reset();
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
    const auto id = ts::Player::make_from_string(update_substr).id;
    // const auto [id, nxs, nys, other] = ts::tools::splitn<4>(update_substr, '|');
    // const auto nxo                   = ts::tools::stoi(nxs);
    // const auto nyo                   = ts::tools::stoi(nys);
    // if (!nxo || !nyo) return;
    for (auto&& p : state.players)
    {
        if (p.id == id)
        {
            ts::log::message<1>("Game: Updating player: ", p.get_string());
            ts::log::message<1>("-> Update: ", update_substr);
            p.from_string(update_substr);
            p.sync(renderer.get_sprite(p.get_sprite()));
            return;
        }
    }
    ts::log::message<2>("Game: Adding new player: ", update_substr);
    state.players.emplace_back();
    state.players.back().from_string(update_substr);
    state.players.back().set_sprite(
        renderer.load_sprite(renderer.load_texture(state.players.back().avatar_str)));
    state.players.back().sync(renderer.get_sprite(state.players.back().get_sprite()));
    challenge.update(state);
}

std::array<long, 2> ts::Game::get_player_tile() { return state.players[0].get_tile(); }

ts::Tile::Type ts::Game::get_player_tile_type()
{
    return world.tile_type_at(get_player().get_tile());
}

std::array<long, 2> ts::Game::get_player_position()
{
    auto [ix, iy] = state.players[0].get_position();
    return {long(ix), long(iy)};
}

void ts::Game::handle_command(const std::string& cmd)
{
    using namespace ts::tools;
    const auto [command, args] = splitn<2>(cmd, ' ');

    if (startswith(command, "/tp"))
    {
        // We only receive this command in offline mode,
        // so we're okayed to just move the player.
        if (const auto [x, y] = all_stol<2>(splitn<2>(args, ' ')); !!x && !!y)
        {
            state.players[0].set_tile_position(*x, *y);
            state.players[0].sync(renderer.get_sprite(state.players[0].get_sprite()));
        }
        return;
    }
    if (startswith(command, "/bal"))
    {
        const auto [arg, val] = splitn<2>(args, ' ');
        if (const auto iv = ts::tools::stol(val); iv)
        {
            if (arg == "set") get_player().balance = *iv;
            if (arg == "add") get_player().balance += *iv;
        }
    }
    if (startswith(command, "/seed"))
    {
        // We only receive this command in offline mode,
        // so we're okayed to set the seed if we need to.
        if (startswith(args, "set"))
        {
            const auto new_seed = ts::tools::stol(chop_first<1>(args, ' '));
            if (new_seed)
            {
                world.set_seed(*new_seed);
            }
        }
        else
        {
            client.chat_local("[Game] Local Seed: " + std::to_string(world.get_seed()));
        }
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
        case ts::key::focus_chat: chat_focus_callback(""); break;
        case ts::key::toggle_chat: chat_close_toggle_callback(); break;
        case ts::key::toggle_debug:
        {
            debug = !debug;
            ts::log::message<1>("Game: Set debug status to ", debug ? "true" : "false");
            break;
        }
        case ts::key::print_debug:
        {
            const auto [px, py] = get_player_position();
            ts::log::message<10>("Game => Debug information: ", "\n\tPlayer Position: ", px, ", ",
                                 py);
            const auto [ptx, pty] = get_player_tile();
            ts::log::message<10>("\tPlayer Tile: ", ptx, ", ", pty);
            ts::log::message<10>("\t-> Type: ", ts::to_string(get_player_tile_type()));
            const auto [pax, pay] = ts::World::tile_to_area(ptx, pty);
            ts::log::message<10>("\tPlayer Area: ", pax, ", ", pay);
            ts::log::message<10>("\tPlayer Balance: ", get_player().balance);
            ts::log::message<10>("\tPlayer Avatar: ", get_player().avatar_str);
            ts::log::message<10>("Players Online:");
            for (const auto& p : state.players) ts::log::message<10>("\t- ", p.username);
            world.print_debug();
            break;
        }
        case ts::key::start_command:
        {
            chat_focus_callback("/");
            break;
        }
        default: return Response::none;
    }

    if (state.players[0].updated)
    {
        if (debug)
            ts::log::message<1>("[Game] Local player was updated due to: ", to_string(key),
                                " keypress.");
        if (get_player_tile_type() == ts::Tile::Type::cave)
        {
            ts::log::message<1>("[Game] Player's balance rose!");
            get_player().balance += 1;  // rich get richer
            balance.set(get_player().balance);
        }
        challenge.update(state);
        if (challenge.to_challenge)
        {
            client.send(ts::challenge_str + "start" + '|' + *challenge.to_challenge);
        }
        client.send(ts::player_update_str + state.players[0].get_string());
        state.players[0].updated = false;
    }
    return Response::none;
}

void ts::Game::run_challenge(const std::string& username)
{
    client.chat_local("Starting challenge of " + username);
}

void ts::Game::render()
{
    for (auto& p : state.players)
    {
        renderer.render(p.get_sprite());
    }
}

void ts::Game::draw_widgets()
{
    balance.draw();
    challenge.draw();
}

#include "game/server.hpp"

#include "game/update.hpp"
#include "tools/debug.hpp"
#include "tools/file.hpp"
#include "tools/string.hpp"

ts::GameServer::~GameServer() { ts::log::message<3>("Destroying game server."); }

void ts::GameServer::launch()
{
    server.on_connect([this](web::UserID id) { this->on_connect(id); });
    server.on_read([this](web::UserID id, std::string str) { this->on_read(id, std::move(str)); });
    server.launch((local_address.empty() ? "127.0.0.1" : local_address),
                  (local_port == 0 ? ts::web::default_port : local_port));
}

void ts::GameServer::on_connect(web::UserID id)
{
    ts::log::message<0>("The id ", id, " connected on address ", local_address, "!");

    // We've connected this user, we can add them to our user vector
    // Each user has a unique session ID which is the web::UserID,
    // plus an identifying ID based on who they login as.
    // For now, as we're not dealing with that many simultaneous connections,
    // we'll maintain all users in a single vector.

    users.try_emplace(id, ts::User(id));
    users.at(id).pos = state.players.size();
    state.players.emplace_back();
    state.players.back().id = '0' + static_cast<int>(state.players.size());
    // Now we need to respond with a message to request their username
    server.write({id}, username_request_str + state.players.back().id);
}

void ts::GameServer::on_read(web::UserID id, std::string message)
{
    ts::log::message<1>("Game Server: The id ", id, " sent a message: ", message);
    if (users.find(id) == users.end())
        ts::log::warn("Game Server: The id ", id, " is not attached to a user.");

    if (message.size() < 2) return;

    switch (message[1])
    {
        case 'I':
        {
            const auto [username, player] =
                ts::tools::splitn<2>(message.substr(ts::player_init_str.size()), '|');
            ts::log::message<1>("Game Server: Setting username for user id ", id, " to '",
                                username, "'");
            users.at(id).username = username;
            // Initialize the player
            update_player(id, player, true);
            respond_status(id);
            break;
        }
        case 'U':
        {
            server.write({id}, "Warning: Your client is out of date! Update now! (#1)");
            break;
        }
        case 'C':
        {
            // const auto chat_message = message.substr(ts::chat_update_str.size());
            if (message.size() <= ts::chat_update_str.size()) break;  // Empty message
            if (message[ts::chat_update_str.size()] == '/')
            {
                run_command(id, message.substr(ts::chat_update_str.size()));
                break;
            }
            ts::log::message<1>("Game Server: Forwarding message from username ",
                                users.at(id).username);
            ts::web::UserSet targets;
            targets.reserve(users.size() - 1);
            for (const auto& p : users)
            {
                if (p.first != id) targets.push_back(p.first);
            }
            server.write(targets, message);
            break;
        }
        case 'P':
        {
            // Player update
            update_player(id, message.substr(ts::player_update_str.size()));
            break;
        }
        case 'S':
        {
            // Client requests [S]tatus, we send [S]tatus back!
            // respond_status(id);
            server.write({id}, "Warning: Your client is out of date! Update now! (#2)");
            break;
        }
        case 'L':
        {
            // Challenge!
            // This is fully server-side for obvious reasons.
            challenge(id, message.substr(ts::challenge_str.size()));
        }
        default: ts::log::warn("Game Server: Ignoring message."); break;
    }
}

void ts::GameServer::send_all(std::string message)
{
    ts::web::UserSet targets;
    targets.reserve(users.size());
    for (const auto& p : users)
    {
        targets.push_back(p.first);
    }
    server.write(targets, message);
}

void ts::GameServer::run_command(web::UserID id, std::string command)
{
    using namespace ts::tools;

    ts::log::message<2>("Game Server: Parsing command [User: ", id, "]: ", command);

    // Can be executed without a user
    if (startswith(command, "/list"))
    {
        std::string message = ts::chat_update_str + "Users Online: ";
        for (const auto& p : users)
        {
            const auto& u = p.second;
            message += u.username;
            if (u.op) message += " (OP)";
            message += ", ";
        }
        message.pop_back();
        message.pop_back();
        server.write({id}, message);
        return;
    }
    // Cannot be executed without a user
    auto user_itr = users.find(id);
    if (user_itr == users.end()) return;
    if (startswith(command, "/nick"))
    {
        const auto nick = splitn<2>(command, ' ')[1];
        if (!nick.empty())
        {
            user_itr->second.username = nick;
        }
        else
            ts::log::warn("Game Server: Found empty nickname update request.");
        return;
    }
    if (startswith(command, "/upgrade"))
    {
        if (const auto& itr = misc_config.find("password"); itr != misc_config.end())
        {
            if (itr->second == "" || splitn<2>(command, ' ')[1] == itr->second)
            {
                // Correct password
                user_itr->second.op = true;
            }
        }
        return;
    }

    // Cannot be executed without permissions
    if (!user_itr->second.op) return;
    if (startswith(command, "/seed"))
    {
        if (startswith(command, "/seed set"))
        {
            const auto new_seed = ts::tools::stol(chop_first<2>(command, ' '));
            if (new_seed)
            {
                world.set_seed(*new_seed);
                update_world();
            }
        }
        else
        {
            server.write({id}, ts::chat_update_str + "Seed: " + std::to_string(world.get_seed()));
        }
        return;
    }
    if (startswith(command, "/tp"))
    {
        server.write({id}, "/tp is not enabled on this server.");
    }
    if (startswith(command, "/stop"))
    {
        send_all(ts::chat_update_str + "Server: Shutting down.");
        server.shutdown();
        return;
    }
    if (startswith(command, "/say"))
    {
        send_all(ts::chat_update_str + "Server: " + splitn<2>(command, ' ')[1]);
        return;
    }
}

void ts::GameServer::challenge(web::UserID id, std::string str)
{
    const auto [cmd, enemy] = ts::tools::splitn<2>(str, '|');
    const auto u = users.at(id);
    if (u.username == enemy)
    {
        ts::log::warn("Player ", u.username, " tried to challenge themselves!");
        return;
    }
    for (size_t i = 0; i < challenges.size(); i++)
    {
        auto& chal = challenges[i];
        if (!chal.is_right_challenge(u.username, enemy)) continue;
        // else we have a challenge pre-existing
        if (cmd == "start")
        ts::log::message<1>("Player ", u.username, " & ", enemy, " have a challenge already!");
        else 
        {
            chal.set(u.username, cmd);
            if (chal.is_complete())
            {
                web::UserID enemyid;
                ts::Player* enemyp{nullptr};
                // challenge complete!
                for (const auto& user : users)
                {
                    if (user.second.username == enemy)
                    {
                        enemyid = user.second.uid;
                        enemyp = &state.players[user.second.pos];
                        break;
                    }
                }
                if (enemyp == nullptr) ts::log::error("[Game Server] Could not get enemy player for challenge!");
                if (const auto winner = chal.winner(); winner == enemy)
                {
                    server.write({enemyid}, ts::challenge_str + "win" + "|" + u.username);
                    server.write({id}, ts::challenge_str + "lose" + "|" + enemy);
                }
                else if (winner == "")
                {
                    server.write({enemyid}, ts::challenge_str + "tie" + "|" + u.username);
                    server.write({id}, ts::challenge_str + "tie" + "|" + enemy);
                }
                else
                {
                    server.write({enemyid}, ts::challenge_str + "lose" + "|" + u.username);
                    server.write({id}, ts::challenge_str + "win" + "|" + enemy);
                }
                challenges.erase(challenges.begin() + i);
            }
        }
        return;
    }
    for (const auto& user : users)
    {
        if (user.second.username == enemy)
        {
            server.write({user.second.uid}, ts::chat_update_str + u.username + " has challenged you!");
            challenges.emplace_back(enemy, u.username);
            break;
        }
    }
}

void ts::GameServer::update_world() { send_all(construct_status()); }

void ts::GameServer::update_player(web::UserID id, std::string str, bool force)
{
    ts::log::message<1>("Game Server: Received player update: ", str);

    const auto pos     = users.at(id).pos;
    ts::Player& player = state.players[pos];

    // Previous player data
    const auto [x, y]  = player.get_position();
    const auto prev_id = player.id;  // We need to preserve this

    // This is pretty hacky but it's a very quick way to get things running
    player.from_string(str);
    player.id           = prev_id;
    const auto [nx, ny] = player.get_position();
    player.set_position(x, y);
    if (nx != x || ny != y)
    {
        // Make sure the player isn't cheating!
        if (nx > x) player.move_right();
        if (nx < x) player.move_left();
        // this works this way because of rendering and things
        if (ny < y) player.move_up();
        if (ny > y) player.move_down();
        player_tile_update(player);
    }
    if (player.updated || force)
    {
        // Now we can send this update back, as we're only parsing positions
        const std::string update_str = ts::player_update_str + player.get_string();
        ts::log::message<1>("Game Server: Sending out update string: ", update_str);
        send_all(update_str);
        player.updated = false;
    }
}

void ts::GameServer::player_tile_update(ts::Player& player)
{
    if (world.tile_type_at(player.get_tile()) == ts::Tile::Type::cave)
    {
        // Okay, so basically, if this wasn't just a demo/prototype
        // we wouldn't trust what the client said about balance
        // and would force it to whatever we determine here instead.
        // But because this is a prototype there's no real reason to implement
        // this, and it helps with testing, so.
        // player.balance +=
    }
}

std::string ts::GameServer::construct_status() const
{
    // This is where things start to get a bit crazy
    std::string resp = ts::status_response_str;
    auto push        = [&resp](std::string str) { resp += std::move(str) + '\n'; };

    for (const auto& p : state.players)
    {
        push(ts::stat_player + p.get_string());
    }
    push(ts::stat_world + world.get_string());
    return resp;
}

void ts::GameServer::respond_status(web::UserID id) { server.write({id}, construct_status()); }

/**
 * Server configuration for the rest of this file.
 */

ts::GameServer::GameServer(const std::string& config_path)
{
    configure(ts::readlines(config_path));
}

void ts::GameServer::configure(const std::vector<std::string>& opts)
{
    for (const auto& o : opts)
    {
        const auto& [k, v] = ts::tools::splitn<2>(o, '=');
        configure(k, v);
    }
}

void ts::GameServer::configure(const std::string& key, const std::string& val)
{
    // This is so hard to look at, it's tempting to factor it
    // out even further. Factory classes, hm...
    if (std::find_if_not(key.begin(), key.end(), ts::tools::is_whitespace) == key.end()) return;
    if (key == "address")
    {
        if (val.size() > 6)
        {
            local_address = val;
        }
        else
        {
            ts::log::warn("Ignored configuration of address to: ", val);
            return;
        }
    }
    else if (key == "port")
    {
        // If there were more integer configuration arguments,
        // this could be put into a configure_int function
        // to avoid repeating this header here.
        if (val.size() == 0) return;
        const auto p_o = ts::tools::stoi(val);
        if (p_o && p_o <= web::port_max)
        {
            local_port = static_cast<web::port_t>(*p_o);
        }
        else
            return;
    }
    else if (key == "password")
    {
        if (val.size() == 0) return;
        misc_config["password"] = val;
    }
    else
    {
        ts::log::warn("Ignored configuration of ", key, " to ", val);
        return;
    }
    ts::log::message<1>("Configured ", key, " to ", val);
}

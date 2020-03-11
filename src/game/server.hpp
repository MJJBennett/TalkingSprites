#ifndef TS_GAMESERVER_HPP
#define TS_GAMESERVER_HPP

#include "web/server.hpp"
#include "game/user.hpp"
#include "game/state.hpp"
#include "game/world.hpp"

#include <vector>
#include <unordered_map>

namespace ts
{
class GameServer
{
public:
    // Configuration
    GameServer(const std::string& config_path = "server.conf");
    void configure(const std::vector<std::string>& opts);
    void configure(const std::string& key, const std::string& val);

    ~GameServer();
    GameServer(const GameServer&) = delete;
    GameServer(GameServer&&) = delete;

    // Blocking call
    void launch();

    void run_command(web::UserID id, std::string command);
    void update_player(web::UserID id, std::string str);
    void respond_status(web::UserID id);

private:
    void on_connect(web::UserID id);
    void on_read(web::UserID id, std::string message);

    void send_all(std::string message);

private:
    ts::web::port_t local_port{0};
    std::string local_address{};

    web::WebServer server;

    std::unordered_map<web::UserID, ts::User> users;
    std::unordered_map<std::string, std::string> misc_config;

    ts::GameState state;
    ts::World world{42};
};
}  // namespace ts

#endif  // TS_GAMESERVER_HPP

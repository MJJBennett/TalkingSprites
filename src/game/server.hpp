#ifndef TS_GAMESERVER_HPP
#define TS_GAMESERVER_HPP

#include "web/server.hpp"

#include <vector>

namespace ts
{
class GameServer
{
public:
    // Configuration
    GameServer(const std::string& config_path = "server.conf");
    void configure(const std::vector<std::string>& opts);
    void configure(const std::string& key, const std::string& val);

    // Blocking call
    void launch();

private:
    ts::web::port_t local_port{0};
    std::string local_address{};

private:
    web::WebServer server;
};
}  // namespace ts

#endif  // TS_GAMESERVER_HPP

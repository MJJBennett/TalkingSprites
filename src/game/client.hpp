#ifndef TS_GAMECLIENT_HPP
#define TS_GAMECLIENT_HPP

#include "game/update.hpp"
#include "web/client.hpp"

#include <queue>
#include <thread>

namespace ts
{
class Chat;
struct Config;

class GameClient
{
public:
    GameClient(ts::Chat&, ts::Config&);

    void send_chat(const std::string& message);
    void chat_local(const std::string& message);

    void send(std::string message) { client.send(std::move(message)); }

    void send_player_init(const std::string& player);

    void on_read(std::string message);

    void poll();

    void shutdown()
    {
        client.shutdown();
        if (up) th.join();
        up = false;
    }

    std::queue<std::string> game_updates;

private:
    web::Client client;
    bool up{false};
    std::thread th;
    ts::Chat& chat;
    ts::Config& config;
};
}  // namespace ts

#endif  // TS_GAMECLIENT_HPP

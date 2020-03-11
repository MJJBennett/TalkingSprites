#ifndef TS_GAMECLIENT_HPP
#define TS_GAMECLIENT_HPP

#include "game/update.hpp"
#include "web/client.hpp"

#include <queue>
#include <thread>

namespace ts
{
class Chat;

class GameClient
{
public:
    GameClient(ts::Chat&);

    void send_chat(const std::string& message);

    void send(std::string message) { client.send(std::move(message)); }

    void on_read(std::string message);

    void poll();

    void shutdown()
    {
        client.shutdown();
        th.join();
    }

    std::queue<std::string> game_updates;

private:
    web::Client client;
    std::thread th;
    ts::Chat& chat;
};
}  // namespace ts

#endif  // TS_GAMECLIENT_HPP

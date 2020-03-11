#include "client.hpp"

#include "tools/debug.hpp"
#include "tools/string.hpp"
#include "widgets/chat.hpp"

void ts::GameClient::on_read(std::string message)
{
    if (message.size() < 2) return;

    switch (message[1])
    {
        case 'C':
        {
            const auto chat_msg = message.substr(ts::chat_update_str.size());
            chat.push(std::move(chat_msg));
            break;
        }
        case 'U':  // Specification: Username request to start connection
            client.send(ts::username_update_str + "MyUsername");
            game_updates.push(message);
            client.send(ts::status_request_str);
            break;
        default:
            game_updates.push(message);
            break;
            //        case 'G':
            //            game_updates.push(ts::GameUpdate(ts::GameUpdate::from_stream,
            //            std::move(message))); break;
            // default: ts::log::warn("Game client: Ignored message: ", message); break;
    }
}

ts::GameClient::GameClient(ts::Chat& chat_)
    : client{"127.0.0.1"}, th{}, chat(chat_)
{
}

void ts::GameClient::poll()
{
    auto q = client.read();
    while (!q.empty())
    {
        on_read(q.front());
        q.pop();
    }
}

void ts::GameClient::send_chat(const std::string& message)
{
    using namespace ts::tools;
    if (startswith(message, "/connect"))
    {
        client.shutdown();
        th = std::thread{&ts::web::Client::launch, &client};
        return;
    }
    client.send(ts::chat_update_str + message);
}

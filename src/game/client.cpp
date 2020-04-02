#include "client.hpp"

#include "tools/debug.hpp"
#include "tools/string.hpp"
#include "widgets/chat.hpp"
#include "config.hpp"

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
        {
            // client.send(ts::username_update_str + config.username);
            game_updates.push(message);
            //client.send(ts::status_request_str);
            break;
        }
        default:
            game_updates.push(message);
            break;
            //        case 'G':
            //            game_updates.push(ts::GameUpdate(ts::GameUpdate::from_stream,
            //            std::move(message))); break;
            // default: ts::log::warn("Game client: Ignored message: ", message); break;
    }
}

ts::GameClient::GameClient(ts::Chat& chat_, ts::Config& config_)
    : client{config_.ip}, th{}, chat(chat_), config(config_)
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
    if (startswith(message, "/co")) // /connect
    {
        client.shutdown();
        if (up) th.join();
        if (const auto url = splitn<2>(lstrip(splitn<2>(message, ' ')[1], ' '), ' '); !url[0].empty())
        {
            client.set_url(url[0]); 
            if (!url[1].empty()) client.set_port(url[1]);
        }
        th = std::thread{&ts::web::Client::launch, &client};
        up = true;
        return;
    }
    else if (startswith(message, "/tp"))
    {
        if (!up)
        {
            game_updates.push(message);
            return;
        }
    }
    else if (startswith(message, "/ni")) // /nick
    {
        const auto [cmd, arg] = ts::tools::splitn<2>(message, ' ');
        if (!arg.empty())
        {
            config.username = arg;
        }
    }
    else if (startswith(message, "/pr")) // /profile
    {
        const auto [cmd, args] = ts::tools::splitn<2>(message, ' ');
    }
    client.send(ts::chat_update_str + message);
}

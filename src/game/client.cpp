#include "client.hpp"

#include "config.hpp"
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
        {
            // client.send(ts::username_update_str + config.username);
            game_updates.push(message);
            // client.send(ts::status_request_str);
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

void ts::GameClient::chat_local(const std::string& message) { chat.push(message); }

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
    if (startswith(message, "/co"))  // /connect
    {
        client.shutdown();
        if (up) th.join();
        if (const auto url = splitn<2>(lstrip(splitn<2>(message, ' ')[1], ' '), ' ');
            !url[0].empty())
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
    else if (startswith(message, "/bal")) // /balance
    {
        if (!up)
        {
            game_updates.push(message);
            return;
        }
    }
    else if (startswith(message, "/seed"))
    {
        if (!up)
        {
            game_updates.push(message);
            return;
        }
    }
    else if (startswith(message, "/ni"))  // /nick
    {
        const auto [cmd, arg] = ts::tools::splitn<2>(message, ' ');
        if (!arg.empty())
        {
            config.username = arg;
            //game_updates.push(message);
        }
    }
    else if (startswith(message, "/pr"))  // /profile
    {
        const auto [cmd, args] = ts::tools::splitn<2>(message, ' ');
    }
    else if (startswith(message, "/help"))  // /help
    {
        if (!up)
        {
            const auto [cmd, arg] = ts::tools::splitn<2>(message, ' ');
            if (arg.size() == 0)
            {
                chat.push(
                    "[Local] Available Commands:\n\t/connect [args...] - Connect to a "
                    "server.\n\t/help [arg] - Get help.\n\t/nick - Set "
                    "username\n\t/seed - Get world seed\n\t/tp - Set player position");
            }
            else
            {
                if (startswith(lstrip(arg, '/'), "seed"))
                {
                    chat.push(
                        "[Local] /seed\n\tGets the integer seed which\n\tuniquely generates this "
                        "world.");
                }
                else if (startswith(lstrip(arg, '/'), "connect"))
                {
                    chat.push(
                        "[Local] /connect [IP] [Port]\n\tConnect to a server.\n\tIP - IP of "
                        "remote server.\n\t  Defaults to 127.0.0.1\n\tPort - Port of remote "
                        "server.\n\t  Defaults to 47747.");
                }
            }
            return;
        }
    }
    client.send(ts::chat_update_str + message);
}

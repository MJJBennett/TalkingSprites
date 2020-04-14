#ifndef TS_GAMEUPDATE_HPP
#define TS_GAMEUPDATE_HPP

#include <string>

namespace ts
{
const std::string username_update_str = "[U] Username: ";
const std::string player_init_str = "[I] (Username | Player): ";
const std::string username_request_str = "[U] Username Request: ";
const std::string chat_update_str = "[C] Chat Message: ";
const std::string player_update_str = "[P] ";
const std::string status_request_str = "[S] Status Request";
const std::string status_response_str = "[S] Status: ";
const std::string challenge_str = "[L] Challenge: ";

const std::string stat_player = "A";
const std::string stat_world = "B";

struct GameUpdate
{
    enum StringType
    {
        from_stream = 0,
        to_stream   = 1
    };

    const std::string game_update_str = "[G] Game Update: ";

    GameUpdate(StringType st, std::string str)
    {
        switch (st)
        {
            case from_stream: update = str.substr(game_update_str.size()); break;
            case to_stream: update = game_update_str; break;
        }
    }

    std::string update;
};
}  // namespace ts

#endif  // TS_GAMEUPDATE_HPP

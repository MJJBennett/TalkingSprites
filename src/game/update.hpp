#ifndef TS_GAMEUPDATE_HPP
#define TS_GAMEUPDATE_HPP

#include <string>

namespace ts
{
const std::string username_update_str = "[U] Username: ";
const std::string chat_update_str = "[C] Chat Message: ";
const std::string player_update_str = "[P] ";

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

#include "player.hpp"

#include "config.hpp"
#include "tools/debug.hpp"
#include "tools/string.hpp"

std::string ts::Player::from_string(std::string str)
{
    const auto [string_av, _rem] =
        ts::tools::splitn<2>(ts::Character::from_string(std::move(str)), '|');
    ts::log::message<1>("Player: Parsed avatar string: ", string_av);
    avatar_str = string_av;
    return _rem;
}

std::string ts::Player::get_string() const
{
    return ts::Character::get_string() + avatar_str + '|';
}

ts::Player ts::Player::make_from_string(std::string str)
{
    Player p;
    p.from_string(str);
    return p;
}

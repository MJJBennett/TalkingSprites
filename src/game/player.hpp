#ifndef TS_PLAYER_HPP
#define TS_PLAYER_HPP

#include "character.hpp"

namespace ts
{
struct Config;

class Player : public Character
{
public:
    Player(std::string avatar_str_ = "") : avatar_str(std::move(avatar_str_)) {}

    std::string from_string(std::string) override;
    std::string get_string() const override;

    std::string avatar_str;

    static Player make_from_string(std::string);
};
}  // namespace ts

#endif  // TS_PLAYER_HPP

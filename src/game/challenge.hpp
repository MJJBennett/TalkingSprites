#ifndef TS_CHALLENGE_HPP
#define TS_CHALLENGE_HPP

#include <optional>
#include <string>

namespace ts
{
struct ChallengePlayer
{
    ChallengePlayer(const std::string& _name) : name(_name) {}
    std::string name;
    bool is_in{false};
    std::optional<std::string> choice{};
};

struct Challenge
{
    Challenge(const std::string& p1name, const std::string& p2name)
        : player_one(p1name), player_two(p2name)
    {
    }
    ChallengePlayer player_one;
    ChallengePlayer player_two;

    bool is_right_challenge(const std::string& one, const std::string& two) const
    {
        if (one != player_one.name && one != player_two.name) return false;
        if (two == player_one.name || two == player_two.name) return true;
        return false;
    }

    void set(const std::string& name, const std::string& val)
    {
        if (name == player_one.name) player_one.choice = val;
        if (name == player_two.name) player_two.choice = val;
    }

    bool is_complete() const
    {
        return (!!player_one.choice && !!player_two.choice);
    }

    std::string winner() const
    {
        if (*player_one.choice == *player_two.choice) return "";
        auto p1c = *player_one.choice;
        auto p2c = *player_two.choice;
        auto p1u = player_one.name;
        auto p2u = player_two.name;
        if (p1c == "Rock" && p2c == "Scissors") return p1u;
        if (p1c == "Paper" && p2c == "Rock") return p1u;
        if (p1c == "Scissors" && p2c == "Paper") return p1u;
        if (p2c == "Rock" && p1c == "Scissors") return p2u;
        if (p2c == "Paper" && p1c == "Rock") return p2u;
        if (p2c == "Scissors" && p1c == "Paper") return p2u;
        return "";
    }
};
}  // namespace ts

#endif  // TS_CHALLENGE_HPP

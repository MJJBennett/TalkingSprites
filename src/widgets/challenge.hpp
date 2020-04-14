#ifndef TS_CHALLENGE_HPP
#define TS_CHALLENGE_HPP

#include "game/player.hpp"
#include "widget.hpp"

#include <optional>
#include <string>
#include <vector>

namespace ts
{
struct GameState;

class ChallengeWidget : public ts::WidgetBase
{
public:
    void draw_self(bool debug = false) override;
    void draw_challenge();
    void update(ts::GameState& state);

    std::function<void(const std::string&)> challenge_callback;

    std::optional<std::string> to_challenge{};
    std::optional<std::string> selected{};
    void reset()
    {
        to_challenge = {};
        selected     = {};
    }

private:
    bool listing{false};
    std::vector<std::string> names;
};
}  // namespace ts

#endif  // TS_CHALLENGE_HPP

#include "challenge.hpp"

// Only necessary for now
#include <cmath>
#include <array>
#include "game/state.hpp"
#include "tools/debug.hpp"

#include "imgui.h"

void ts::ChallengeWidget::draw_self(bool)
{
    if (names.empty()) return;
    if (to_challenge)
    {
        draw_challenge();
        return;
    }
    ImGui::Begin("Challenge", &always_open,
                 ImGuiWindowFlags_NoResize |
                     /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Challenge Player?");
    for (const auto& str : names)
        if (ImGui::Button(str.c_str())) to_challenge = str;
    ImGui::End();
    return;
}

void ts::ChallengeWidget::draw_challenge()
{
    const static std::array<std::string, 3> choices = {"Rock", "Paper", "Scissors"};
    ImGui::Begin("Challenge!", &always_open,
                 ImGuiWindowFlags_NoResize |
                     /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_AlwaysAutoResize);
    if (selected)
    {
        ImGui::TextUnformatted(std::string{"You have chosen: " + *selected}.c_str());
    }
    else
    {
        for (const auto& c : choices)
        {
            if (ImGui::Button(c.c_str()))
            {
                selected = c;
                break;
            }
        }
    }
    ImGui::End();
}

void ts::ChallengeWidget::update(ts::GameState& state)
{
    const ts::Player& player = state.players[0];
    const auto [px, py]      = player.get_tile();
    for (size_t i = 1; i < state.players.size(); i++)
    {
        const auto& p2        = state.players[i];
        const auto [p2x, p2y] = p2.get_tile();
        if (std::labs(px - p2x) < 2 && std::labs(py - p2y) < 2)
        {
            if (!listing)
            {
                names.clear();
                listing = true;
            }
            names.push_back(p2.username);
        }
    }
    if (!listing) names.clear();
    listing = false;
}

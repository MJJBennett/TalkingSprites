#include "balance.hpp"

// Only necessary for now
#include "tools/debug.hpp"

#include "imgui.h"

void ts::Balance::draw_self(bool)
{
    ImGui::Begin(
        "Balance", &always_open,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("%s", balance.c_str());
    ImGui::End();
    return;
}

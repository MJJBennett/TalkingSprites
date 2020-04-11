#include "chat.hpp"

// Only necessary for now
#include "tools/debug.hpp"

#include "imgui.h"

ts::Chat::Chat() { std::fill(input_buffer.begin(), input_buffer.end(), '\0'); }

std::optional<std::string> ts::Chat::chat(bool debug)
{
    if (!visible) return {};
    ImGui::Begin("Chat", &open, ImGuiWindowFlags_NoResize);
    for (auto&& s : chat_buffer) ImGui::TextWrapped("%s", s.c_str());
    ImGui::Text(">");
    ImGui::SameLine();
    bool new_chat{false};
    if (ImGui::InputText("##chat input", input_buffer.data(), 128, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (debug) ts::log::message<1>("Got chat input: ", input_buffer.data());
        chat_buffer.emplace_back(input_buffer.data());
        std::fill(input_buffer.begin(), input_buffer.end(), '\0');
        ImGui::SetScrollY(10000.f);
        new_chat = true;
    }
    if (focus_next) 
    {
        if (input_buffer[0] == '\0')
        {
            std::copy(focus_str.begin(), focus_str.end(), input_buffer.begin());
        }
        ImGui::SetKeyboardFocusHere();
        focus_next = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Send") && input_buffer[0] != '\0')
    {
        if (debug) ts::log::message<1>("Got chat send key.");
        chat_buffer.emplace_back(input_buffer.data());
        std::fill(input_buffer.begin(), input_buffer.end(), '\0');
        ImGui::SetScrollY(10000.f);
        new_chat = true;
    }
    ImGui::End();

    if (new_chat) return chat_buffer.back();

    return {};
}

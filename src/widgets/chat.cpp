#include "chat.hpp"

#include "imgui.h"

ts::Chat::Chat() { std::fill(input_buffer.begin(), input_buffer.end(), '\0'); }

std::optional<std::string> ts::Chat::chat()
{
    ImGui::Begin("Chat", &open, ImGuiWindowFlags_NoResize);
    for (auto&& s : chat_buffer) ImGui::TextUnformatted(s.data());
    ImGui::Text(">");
    ImGui::SameLine();
    bool new_chat{false};
    if (ImGui::InputText("##chat input", input_buffer.data(), 128, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        chat_buffer.emplace_back(input_buffer.data());
        std::fill(input_buffer.begin(), input_buffer.end(), '\0');
        ImGui::SetScrollY(10000.f);
        new_chat = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Send") && input_buffer[0] != '\0')
    {
        chat_buffer.emplace_back(input_buffer.data());
        std::fill(input_buffer.begin(), input_buffer.end(), '\0');
        ImGui::SetScrollY(10000.f);
        new_chat = true;
    }
    ImGui::End();

    if (new_chat) return chat_buffer.back();

    return {};
}

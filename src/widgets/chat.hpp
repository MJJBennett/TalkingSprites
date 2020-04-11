#ifndef TS_CHAT_HPP
#define TS_CHAT_HPP

#include <array>
#include <optional>
#include <string>
#include <vector>

namespace ts
{
class Chat
{
public:
    Chat();
    std::optional<std::string> chat(bool debug = false);
    void push(std::string msg) { chat_buffer.emplace_back(std::move(msg)); }
    void focus(const std::string& str) { focus_str = str; focus_next = true; }
    void toggle_visible() { visible = !visible; }

private:
    std::vector<std::string> chat_buffer;
    std::array<char, 128> input_buffer;
    bool open{true};
    bool focus_next{false};
    std::string focus_str{};
    bool visible{true};
};
}  // namespace ts

#endif  // TS_CHAT_HPP

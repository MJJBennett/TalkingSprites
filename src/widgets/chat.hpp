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

private:
    std::vector<std::string> chat_buffer;
    std::array<char, 128> input_buffer;
    bool open{true};
};
}  // namespace ts

#endif  // TS_CHAT_HPP

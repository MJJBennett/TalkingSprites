#ifndef TS_CHAT_HPP
#define TS_CHAT_HPP

#include <optional>
#include <string>
#include <vector>
#include <array>

namespace ts
{
class Chat
{
public:
    Chat();
    std::optional<std::string> chat();
private:
    std::vector<std::string> chat_buffer;
    std::array<char, 128> input_buffer;
    bool open{true};
};
}  // namespace ts

#endif  // TS_CHAT_HPP

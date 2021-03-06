#ifndef TS_KEYBOARD_HPP
#define TS_KEYBOARD_HPP

#include <SFML/Window/Keyboard.hpp>
#include <optional>
#include <string>
#include <unordered_map>

namespace ts
{
enum class key
{
    close_game = 0,
    path_left,
    path_right,
    path_down,
    path_up,
    rebind_key,
    focus_chat,
    toggle_chat,
    toggle_debug,
    print_debug,
    start_command,
    unknown,
};

inline std::string to_string(const ::ts::key& k)
{
    switch (k)
    {
        case ::ts::key::close_game: return "close_game";
        case ::ts::key::path_left: return "path_left";
        case ::ts::key::path_right: return "path_right";
        case ::ts::key::path_down: return "path_down";
        case ::ts::key::path_up: return "path_up";
        case ::ts::key::rebind_key: return "rebind_key";
        case ::ts::key::focus_chat: return "focus_chat";
        case ::ts::key::toggle_chat: return "toggle_chat";
        case ::ts::key::print_debug: return "print_debug";
        case ::ts::key::toggle_debug: return "toggle_debug";
        case ::ts::key::start_command: return "start_command";
        case ::ts::key::unknown: return "unknown";
        default: return "unset";
    }
}

class Keyboard
{
public:
    Keyboard(const std::string& file);

    bool rebind(const sf::Keyboard::Key k);

    ts::key get_key(const sf::Keyboard::Key& k);
    void default_key(ts::key internal_key, sf::Keyboard::Key sfml_key);

    void set_map(ts::key internal_key, sf::Keyboard::Key sfml_key);

    void save();

private:
    std::unordered_map<sf::Keyboard::Key, ts::key> keymap;
    std::unordered_map<ts::key, sf::Keyboard::Key> bound;
    std::string filename;

    std::optional<sf::Keyboard::Key> original_key;

    bool rebind_started{false};
};
}  // namespace ts

#endif  // TS_KEYBOARD_HPP

#ifndef TS_KEYBOARD_HPP
#define TS_KEYBOARD_HPP

#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <unordered_map>

namespace ts
{
enum class key
{
    close_game,
    path_left,
    path_right,
    path_down,
    path_up,
    unknown,
};

class Keyboard
{
public:
    Keyboard(const std::string& file);

    ts::key get_key(const sf::Keyboard::Key& k);
    void default_key(ts::key internal_key, sf::Keyboard::Key sfml_key);

    void save();

private:
    std::unordered_map<sf::Keyboard::Key, ts::key> keymap;
    std::string filename;
};
}  // namespace ts

#endif  // TS_KEYBOARD_HPP

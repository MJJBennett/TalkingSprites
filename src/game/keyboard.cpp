#include "keyboard.hpp"

#include "tools/debug.hpp"
#include "tools/file.hpp"
#include "tools/string.hpp"

ts::Keyboard::Keyboard(const std::string& file) : filename(file)
{
    for (const auto& km : readlines(file))
    {
        const auto [keyid, keycode] = ts::tools::splitn<2>(km, '=');
        if (keyid.size() < 1 || keycode.size() < 1) continue;
        const auto internal_id = ts::tools::stoi(keyid);
        const auto sfml_id     = ts::tools::stoi(keycode);
        if (!internal_id || !sfml_id) continue;
        // Now we have a keymapping to actually deal with
        keymap[static_cast<sf::Keyboard::Key>(*sfml_id)] = static_cast<ts::key>(*internal_id);
    }

    // Now we have a few necessary defaults
    default_key(ts::key::close_game, sf::Keyboard::Key::Escape);
    default_key(ts::key::path_down, sf::Keyboard::Key::S);
    default_key(ts::key::path_up, sf::Keyboard::Key::W);
    default_key(ts::key::path_left, sf::Keyboard::Key::A);
    default_key(ts::key::path_right, sf::Keyboard::Key::D);

    save();
}

ts::key ts::Keyboard::get_key(const sf::Keyboard::Key& k)
{
    if (keymap.find(k) == keymap.end()) return key::unknown;
    return keymap[k];
}

void ts::Keyboard::default_key(ts::key internal_key, sf::Keyboard::Key sfml_key)
{
    if (keymap.find(sfml_key) == keymap.end()) keymap.emplace(sfml_key, internal_key);
}

void ts::Keyboard::save()
{
    ts::log::message<2>("Saving keymap to file: ", filename, " with ", keymap.size(),
                        " mappings.");
    std::ofstream ofs(filename, std::ios_base::out);
    if (!ofs.good())
    {
        ts::log::warn("Could not open output file for editing.");
        return;
    }
    for (const auto& mapping : keymap)
    {
        ofs << static_cast<int>(mapping.second) << "=" << mapping.first << "\n";
    }
}

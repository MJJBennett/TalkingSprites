#include "keyboard.hpp"

#include "tools/debug.hpp"
#include "tools/file.hpp"
#include "tools/string.hpp"

ts::Keyboard::Keyboard(const std::string& file) : filename(file)
{
    for (const auto& km : readlines(filename))
    {
        const auto [keyid, keycode, _comment] = ts::tools::splitn<3>(km, '=');
        if (keyid.size() < 1 || keycode.size() < 1) continue;
        const auto internal_id = ts::tools::stoi(keyid);
        const auto sfml_id     = ts::tools::stoi(keycode);
        if (!internal_id || !sfml_id) continue;
        // Now we have a keymapping to actually deal with
        set_map(static_cast<ts::key>(*internal_id), static_cast<sf::Keyboard::Key>(*sfml_id));
    }

    // Now we have a few necessary defaults
    default_key(ts::key::path_down, sf::Keyboard::Key::S);
    default_key(ts::key::path_up, sf::Keyboard::Key::W);
    default_key(ts::key::path_left, sf::Keyboard::Key::A);
    default_key(ts::key::path_right, sf::Keyboard::Key::D);
    default_key(ts::key::rebind_key, sf::Keyboard::Key::P);
    default_key(ts::key::open_chat, sf::Keyboard::Key::C);
    default_key(ts::key::close_chat, sf::Keyboard::Key::Escape);
    default_key(ts::key::toggle_debug, sf::Keyboard::Key::G);

    save();
}

ts::key ts::Keyboard::get_key(const sf::Keyboard::Key& k)
{
    if (keymap.find(k) == keymap.end()) return key::unknown;
    return keymap[k];
}

void ts::Keyboard::default_key(ts::key internal_key, sf::Keyboard::Key sfml_key)
{
    if (keymap.find(sfml_key) == keymap.end() && bound.find(internal_key) == bound.end())
    {
        ts::log::message<1>("Adding default key bind to: ", to_string(internal_key));
        set_map(internal_key, sfml_key);
    }
}

void ts::Keyboard::set_map(ts::key internal_key, sf::Keyboard::Key sfml_key)
{
    /** This is where things get a little interesting.
     * The behaviour of the function depends a lot on the case.
     * We don't want to make any changes if the keyboard key
     * is already bound - that's dangerous, currently.
     * If the ts::key is already bound, we need to update the
     * mapping, NOT just write a new one.
     */
    // Keyboard key already bound
    if (keymap.find(sfml_key) != keymap.end()) return;

    if (bound.find(internal_key) != bound.end())
    {
        // Internal key already has a bind, unbind it first
        keymap.erase(bound[internal_key]);
        bound.erase(internal_key);
    }
    bound.emplace(internal_key, sfml_key);
    keymap.emplace(sfml_key, internal_key);
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
        ofs << static_cast<int>(mapping.second) << "=" << mapping.first << "=" << to_string(mapping.second) << "\n";
    }
}

bool ts::Keyboard::rebind(const sf::Keyboard::Key k)
{
    if (!rebind_started)
    {
        if (get_key(k) == ts::key::rebind_key)
        {
            ts::log::message<2>("Starting to rebind a key.");
            rebind_started = true;
            return true;
        }
        return false;
    }

    if (original_key)
    {
        if (auto itr = keymap.find(*original_key);
            itr != keymap.end() && keymap.find(k) == keymap.end())
        {
            const ts::key dest = itr->second;
            keymap.erase(itr);
            keymap.emplace(k, dest);
            ts::log::message<2>("Rebinding key successful. Rebound to: ", k);
            save();
        }
        original_key.reset();
        rebind_started = false;
    }
    else
    {
        if (keymap.find(k) == keymap.end())
        {
            rebind_started = false;
        }
        else
        {
            ts::log::message<2>("Rebinding key: ", k);
            original_key = k;
        }
    }

    return true;
}

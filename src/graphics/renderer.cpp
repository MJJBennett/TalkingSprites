#include "renderer.hpp"

#include "tools/debug.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

void ts::Renderer::render(sf::Drawable& d) { window.draw(d); }

void ts::Renderer::render(const SpriteHandle& d)
{
    assert(d.handle < sprite_map.size());
    window.draw(sprite_map[d.handle]);
}

sf::Sprite& ts::Renderer::get_sprite(ts::SpriteHandle s)
{
    assert(s.handle < sprite_map.size());
    return sprite_map[s.handle];
}

sf::Sprite ts::Renderer::get_sprite(ts::TextureHandle t)
{
    assert(t.handle < texture_map.size());
    sf::Sprite ret;
    ret.setTexture(*texture_map[t.handle]);
    return ret;
}

ts::TextureHandle ts::Renderer::load_texture(const std::string& path)
{
    texture_map.emplace_back(std::make_unique<sf::Texture>());
    // later - make a map of preloaded paths
    if (!texture_map.back()->loadFromFile(path))
    {
        ts::log::warn("Could not load path: ", path, '\n');
        return {ts::TextureHandle::None};
    }
    ts::log::message<1>("Loaded texture at path: ", path, " with ID: ", texture_map.size() - 1);
    return {texture_map.size() - 1};
}

ts::SpriteHandle ts::Renderer::load_sprite(const ts::TextureHandle& t)
{
    assert(t.handle < texture_map.size());
    ts::log::message<1>("Loading sprite from texture ID: ", t.handle);
    sprite_map.emplace_back();
    sprite_map.back().setTexture(*texture_map[t.handle]);
    return {sprite_map.size() - 1};
}

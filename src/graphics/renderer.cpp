#include "renderer.hpp"

#include "game/world.hpp"
#include "tools/debug.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

void ts::Renderer::render(sf::Drawable& d) { window.draw(d); }

void ts::Renderer::render(const SpriteHandle& d)
{
    assert(d.handle < sprite_map.size());
    window.draw(sprite_map[d.handle]);
}

void ts::Renderer::render(const ts::World& w)
{
    for (const auto& kv : w.get())
    {
        const auto [area_x, area_y] = kv.first;
        for (area_size_t x_pos = 0; x_pos < area_size; x_pos++)
        {
            for (area_size_t y_pos = 0; y_pos < area_size; y_pos++)
            {
                const auto tp_int = static_cast<int>(kv.second[x_pos][y_pos].type);

                if (size_t(tp_int) < tile_map.size())
                {
                    auto& tile    = tile_map[tp_int];
                    const float x = 32 * (area_x * ts::area_size + x_pos);
                    const float y = 32 * (area_y * ts::area_size + y_pos);
                    tile.setPosition(x, y);
                    window.draw(tile);
                }
            }
        }
    }
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

void ts::Renderer::load_tile(const ts::TextureHandle& t)
{
    assert(t.handle < texture_map.size());
    ts::log::message<1>("Loading tile from texture ID: ", t.handle);
    tile_map.emplace_back();
    tile_map.back().setTexture(*texture_map[t.handle]);
}

#ifndef TS_RENDERER_HPP
#define TS_RENDERER_HPP

#include "fwd/sfml.hpp"
#include "resource.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <string>
#include <vector>

namespace ts
{
class Renderer
{
public:
    Renderer(sf::RenderWindow& w) : window(w)
    {
        texture_map.emplace_back();
        sprite_map.emplace_back();
    }

    void render(sf::Drawable& d);
    void render(const SpriteHandle& d);

    sf::Sprite& get_sprite(ts::SpriteHandle s);
    sf::Sprite get_sprite(ts::TextureHandle t);

    [[nodiscard]] TextureHandle load_texture(const std::string& path);
    [[nodiscard]] SpriteHandle load_sprite(const ts::TextureHandle& t);

private:
    sf::RenderWindow& window;

    std::vector<std::unique_ptr<sf::Texture>> texture_map;
    std::vector<sf::Sprite> sprite_map;
};
}  // namespace ts

#endif  // TS_RENDERER_HPP

#ifndef TS_RENDERER_HPP
#define TS_RENDERER_HPP

#include "fwd/sfml.hpp"

namespace ts
{
class Renderer
{
public:
    Renderer(sf::RenderWindow& w) : window(w) {}

    void render(sf::Drawable& d); 

private:
    sf::RenderWindow& window;
};
}  // namespace ts

#endif  // TS_RENDERER_HPP

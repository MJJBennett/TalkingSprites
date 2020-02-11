#include "renderer.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

void ts::Renderer::render(sf::Drawable& d) { window.draw(d); }

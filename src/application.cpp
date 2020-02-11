#include "application.hpp"

#include "game/game.hpp"
#include "tools/debug.hpp"
#include "tools/string.hpp"
#include "widgets/chat.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "graphics/renderer.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

namespace isf = ImGui::SFML;

int ts::Application::launch()
{
    // Full application logic is all launched from
    // this function.
    sf::RenderWindow window(sf::VideoMode(800, 600), "Talking Sprites");
    window.setFramerateLimit(120);
    ts::Renderer renderer(window);
    const auto player_id =
        renderer.load_sprite(renderer.load_texture("resources/sprites/player_0.png"));
    const auto grass_id =
        renderer.load_sprite(renderer.load_texture("resources/sprites/grass_0.png"));
    renderer.get_sprite(player_id).setPosition(50, 50);
    start_imgui(window);

    // Testing code
    sf::RectangleShape rect({80.f, 80.f});
    rect.setFillColor(sf::Color::Green);

    ts::Chat chat;
    ts::Game game;

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            // First let ImGui-SFML process event
            isf::ProcessEvent(e);

            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            if (!ImGui::GetIO().WantCaptureKeyboard)
            {
                if (e.type == sf::Event::KeyPressed || e.type == sf::Event::KeyReleased)
                {
                    switch (game.handle_keyevent(e))
                    {
                        case ts::Game::Response::close_window: window.close(); break;
                        default: break;
                    }
                }
            }
        }

        isf::Update(window, delta.restart());

        chat.chat();

        window.clear();
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                renderer.get_sprite(grass_id).setPosition(x * 32, y * 32);
                renderer.render(grass_id);
            }
        }
        renderer.render(rect);
        renderer.render(player_id);
        isf::Render(window);  // Render ImGui last
        window.display();
    }

    return 0;
}

void ts::Application::configure(std::vector<std::string> args)
{
    for (const auto& arg : args)
    {
        configure(arg);
    }
}

void ts::Application::configure(const std::string& arg)
{
    using namespace ts::tools;
    const auto [one, two] = splitn<2>(arg);
    // Deal with args now
}

ts::Application::~Application()
{
    if (imgui_launched) isf::Shutdown();
}

void ts::Application::start_imgui(sf::RenderWindow& w)
{
    isf::Init(w);
    imgui_launched = true;
}

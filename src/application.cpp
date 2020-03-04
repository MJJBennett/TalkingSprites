#include "application.hpp"

#include "game/game.hpp"
#include "game/world.hpp"
#include "tools/debug.hpp"
#include "tools/string.hpp"
#include "widgets/chat.hpp"
#include "web/client.hpp"

#include <thread>
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
    const auto grass_text = renderer.load_texture("resources/sprites/grass_0.png");
    // const auto grass_id =
    //   renderer.load_sprite(grass_text);
    renderer.load_tile(grass_text);
    renderer.load_tile(renderer.load_texture("resources/sprites/rocks_0.png"));
    World w;
    w.load_area(0, 0);
    w.load_area(1, 1);
    w.load_area(1, 0);
    w.load_area(0, 1);
    start_imgui(window);

    ts::Chat chat;
    ts::Game game(renderer);

    ts::web::Client c{"127.0.0.1"};
    std::thread th{&ts::web::Client::launch, &c};

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

        if (auto cm = chat.chat(); cm)
        {
            c.send(*cm);
        }

        window.clear();
        // for (int x = 0; x < 10; x++)
        //{
        //   for (int y = 0; y < 10; y++)
        //  {
        //     renderer.get_sprite(grass_id).setPosition(x * 32, y * 32);
        //    renderer.render(grass_id);
        //}
        //}
        renderer.render(w);
        game.render();
        isf::Render(window);  // Render ImGui last
        window.display();
    }

    ts::log::message<1>("Application: Shutting down web client.");
    c.shutdown();
    th.join();

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

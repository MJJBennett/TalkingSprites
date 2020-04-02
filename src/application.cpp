#include "application.hpp"

#include "config.hpp"
#include "game/client.hpp"
#include "game/game.hpp"
#include "game/world.hpp"
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

int ts::Application::launch(int argc, char* argv[])
{
    ts::Config config(argc, argv);
    // Full application logic is all launched from
    // this function.
    sf::RenderWindow window(sf::VideoMode(800, 600), config.name);
    window.setFramerateLimit(120);
    ts::Renderer renderer(window);
    const auto grass_text = renderer.load_texture("resources/sprites/grass_0.png");
    // const auto grass_id =
    //   renderer.load_sprite(grass_text);
    renderer.load_tile(grass_text);
    renderer.load_tile(renderer.load_texture("resources/sprites/rocks_0.png"));
    World world;
    world.load_area(0, 0);
    world.load_area(1, 1);
    world.load_area(1, 0);
    world.load_area(0, 1);
    world.load_area(2, 2);
    world.load_area(2, 1);
    world.load_area(1, 2);
    world.load_area(0, 2);
    world.load_area(2, 0);
    start_imgui(window);

    ts::Chat chat;

    ts::GameClient c(chat, config);
    ts::Game game(renderer, c, world, config);

    game.chat_close_toggle_callback = [&chat]() { chat.toggle_visible(); };
    game.chat_focus_callback        = [&chat]() { chat.focus(); };

    sf::View world_view = window.getDefaultView();
    //const auto area_load_radius = 1; // It's still a prototype, so this is (okay?)
    auto [prev_x, prev_y] = game.get_player_position();
    world_view.setCenter(prev_x, prev_y);
    window.setView(world_view);

    while (true)
    {
        /**
         * Event management
         */

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
                    if (game.debug) ts::log::message<1>("Application: Found key event.");
                    switch (game.handle_keyevent(e))
                    {
                        case ts::Game::Response::close_window: window.close(); break;
                        default: break;
                    }
                }
            }
        }
        if (!window.isOpen()) break;

        /**
         * Priority updates
         */

        isf::Update(window, delta.restart());

        if (auto cm = chat.chat(game.debug); cm)
        {
            c.send_chat(*cm);
        }

        /**
         * Game updates
         */

        c.poll();  // Gets web updates
        game.update();

        const auto [ptx, pty] = game.get_player_tile();
        const auto [pax, pay] = world.tile_to_area(ptx, pty);
        world.load_area(pax, pay);

        // Clear window prior to render
        window.clear();

        /**
         * Rendering
         */
       
        if (const auto [pxp, pyp] = game.get_player_position(); pxp != prev_x || pyp != prev_y)
        {
            world_view.setCenter(pxp, pyp);
            window.setView(world_view);
        }
        renderer.render(world); // Render the world
        game.render(); // Render the game
        isf::Render(window);  // Render ImGui last

        window.display();
    }

    ts::log::message<1>("Application: Shutting down web client.");
    c.shutdown();

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
    const auto [one, two] = splitn<2>(arg, '=');
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

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
    renderer.load_tile(renderer.load_texture("resources/sprites/grass_0.png"));
    renderer.load_tile(renderer.load_texture("resources/sprites/rocks_0.png"));
    renderer.load_tile(renderer.load_texture("resources/sprites/Cave_01.png"));
    World world;
    // Load initial regions
    for (int xi = -1; xi < 2; xi++) for (int yi = -1; yi < 2; yi++) world.load_area(xi, yi);
    start_imgui(window);

    ts::Chat chat;

    ts::GameClient c(chat, config);
    ts::Game game(renderer, c, world, config);

    game.chat_close_toggle_callback = [&chat]() { chat.toggle_visible(); };
    game.chat_focus_callback        = [&chat](const std::string& s) { chat.focus(s); };

    sf::View world_view = window.getDefaultView();
    //const auto area_load_radius = 1; // It's still a prototype, so this is (okay?)
    auto [prev_x, prev_y] = game.get_player_position();
    world_view.setCenter(prev_x, prev_y);
    window.setView(world_view);

    size_t tick_counter = 0;
    size_t rare_tick = 128;

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
        if (--rare_tick == 0) 
        {
            world.unload_areas(pax, pay);
            rare_tick = 128;
        }
        if (tick_counter % 8 == 0) 
        {
            for (int xi = -2; xi < 3; xi++) for (int yi = -2; yi < 3; yi++) world.load_area(pax + xi, pay + yi);
        }

        // Clear window prior to render
        window.clear(sf::Color(83, 159, 28));

        /**
         * Rendering
         */
       
        if (const auto [pxp, pyp] = game.get_player_position(); pxp != prev_x || pyp != prev_y)
        {
            world_view.setCenter(pxp, pyp);
            window.setView(world_view);
            prev_x = pxp;
            prev_y = pyp;
        }
        renderer.render(world); // Render the world
        game.render(); // Render the game
        isf::Render(window);  // Render ImGui last

        window.display();

        // Increment tick counter
        tick_counter++;
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

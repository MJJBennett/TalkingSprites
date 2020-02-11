#include "game.hpp"

ts::Game::Response ts::Game::handle_keyevent(const sf::Event& e)
{
    const bool press = e.type == sf::Event::KeyPressed;
    const auto key = keyboard.get_key(e.key.code);

    switch(key)
    {
        case ts::key::close_game:
            return (press ? Response::none : Response::close_window);
        default:
            return Response::none;
    }
}

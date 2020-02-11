#ifndef TS_APPLICATION_HPP
#define TS_APPLICATION_HPP

#include "fwd/sfml.hpp"

#include <string>
#include <vector>
#include <SFML/System/Clock.hpp>

namespace ts
{
class Application
{
public:
    int launch();

    // For commandline configuration
    void configure(std::vector<std::string> args);
    void configure(const std::string& arg);

    ~Application();

private:
    void start_imgui(sf::RenderWindow& w);

private:
    bool imgui_launched{false};
    sf::Clock delta{};
};
}  // namespace ts

#endif  // TS_APPLICATION_HPP

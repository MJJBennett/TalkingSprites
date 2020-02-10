#include "application.hpp"

#include "tools/string.hpp"
#include <iostream>

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
    const auto [one,two] = splitn<2>(arg);
    std::cout << one << "|" << two << std::endl;
}

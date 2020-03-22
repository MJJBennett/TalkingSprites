#ifndef TS_CONFIG_HPP
#define TS_CONFIG_HPP

#include <string>

namespace ts
{
struct Config
{
    Config(int argc, char* argv[]); 

    std::string name{"Talking Sprites"};
    std::string ip{"127.0.0.1"};
    std::string username{"Steven"};
    std::string avatar{"resources/sprites/player_01.png"};
};
}

#endif // TS_CONFIG_HPP

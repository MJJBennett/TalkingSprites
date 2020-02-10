#ifndef TS_GAMESTATE_HPP
#define TS_GAMESTATE_HPP

#include "game/tile.hpp"
#include "game/char.hpp"

#include <vector>

/**
 * This file exists to handle data structure definitions
 * for data related to storing the state of the game.
 */

namespace ts
{
struct TileState
{
    std::vector<CharID> characters;
};

class GameState
{
public:
     

private:

};
}

#endif // TS_GAMESTATE_HPP

#ifndef TS_GAMESTATE_HPP
#define TS_GAMESTATE_HPP

#include "game/character.hpp"
#include "game/player.hpp"
#include "game/tile.hpp"

#include <vector>

/**
 * This file exists to handle data structure definitions
 * for data related to storing the state of the game.
 */

namespace ts
{
struct TileState
{
    //    std::vector<CharID> characters;
};

struct GameState
{
    std::vector<ts::Player> players;
};
}  // namespace ts

#endif  // TS_GAMESTATE_HPP

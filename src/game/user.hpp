#ifndef TS_GAME_USER_HPP
#define TS_GAME_USER_HPP

#include <string>
#include "web/server.hpp"

namespace ts
{
struct User
{
    User(web::UserID id) : uid(id) {}

    web::UserID uid;
    std::string username;

    bool op{false};

    // Player information
    size_t pos{0};
};
}

#endif // TS_GAME_USER_HPP

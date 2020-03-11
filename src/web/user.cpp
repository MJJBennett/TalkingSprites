#include "web/user.hpp"

#include "tools/string.hpp"

void ts::User::from_string(std::string str)
{
    using namespace ts::tools;
    const auto [id_str, name_str] = splitn<2>(str, '=');
    const auto id_opt = ts::tools::stol(id_str);
    if (id_opt) id = *id_opt;
    name = name_str;
}

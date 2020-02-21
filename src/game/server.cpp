#include "game/server.hpp"

#include "tools/debug.hpp"
#include "tools/file.hpp"
#include "tools/string.hpp"

void ts::GameServer::launch()
{
    server.launch((local_address.empty() ? "127.0.0.1" : local_address),
                  (local_port == 0 ? ts::web::default_port : local_port));
}

/**
 * Server configuration for the rest of this file.
 */

ts::GameServer::GameServer(const std::string& config_path)
{
    configure(ts::readlines(config_path));
}

void ts::GameServer::configure(const std::vector<std::string>& opts)
{
    for (const auto& o : opts)
    {
        const auto& [k, v] = ts::tools::splitn<2>(o, '=');
        configure(k, v);
    }
}

void ts::GameServer::configure(const std::string& key, const std::string& val)
{
    // This is so hard to look at, it's tempting to factor it
    // out even further. Factory classes, hm...
    if (std::find_if_not(key.begin(), key.end(), ts::tools::is_whitespace) == key.end()) return;
    if (key == "address")
    {
        if (val.size() > 6)
        {
            local_address = val;
        }
        else
        {
            ts::log::warn("Ignored configuration of address to: ", val);
            return;
        }
    }
    else if (key == "port")
    {
        // If there were more integer configuration arguments,
        // this could be put into a configure_int function
        // to avoid repeating this header here.
        if (val.size() == 0) return;
        const auto p_o = ts::tools::stoi(val);
        if (p_o && p_o <= web::port_max)
        {
            local_port = static_cast<web::port_t>(*p_o);
        }
        else
            return;
    }
    else
    {
        ts::log::warn("Ignored configuration of ", key, " to ", val);
        return;
    }
    ts::log::message<1>("Configured ", key, " to ", val);
}

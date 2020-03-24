#include "config.hpp"

#include "tools/debug.hpp"
#include "tools/file.hpp"
#include "tools/string.hpp"

ts::Config::Config(int argc, char* argv[])
{
    using namespace ts::tools;
    for (int i = 1; i < argc; i++)
    {
        parse_arg(argv[i]);
    }
}

void ts::Config::from_file(const std::string& filename)
{
    auto lines = ts::readlines(filename);
    if (lines.size() == 0)
        ts::log::warn("Configuration file provided (", filename,
                      ") was either empty or did not exist!");
    ts::log::message<1>("Configuration: Loading from file: ", filename);
    for (const auto& line : lines)
    {
        parse_arg(line);
    }
}

void ts::Config::parse_arg(const std::string& arg)
{
    using namespace ts::tools;
    if (startswithignore(arg, "n", '-'))
    {
        name = chop_first<1>(arg, '=');
        return;
    }
    if (startswithignore(arg, "i", '-'))
    {
        ip = chop_first<1>(arg, '=');
        return;
    }
    if (startswithignore(arg, "c", '-'))
    {
        from_file(chop_first<1>(arg, '='));
        return;
    }
    if (startswithignore(arg, "u", '-'))
    {
        username = chop_first<1>(arg, '=');
        return;
    }
    if (startswithignore(arg, "a", '-'))
    {
        avatar = chop_first<1>(arg, '=');
        return;
    }
}

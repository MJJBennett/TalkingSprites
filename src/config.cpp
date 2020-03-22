#include "config.hpp"

#include "tools/string.hpp"

ts::Config::Config(int argc, char* argv[])
{
    using namespace ts::tools;
    for (int i = 1; i < argc; i++)
    {
        if (startswith(argv[i], "--n"))
        {
            name = chop_first<1>(argv[i], '=');
            continue;
        }
        if (startswith(argv[i], "--i"))
        {
            ip = chop_first<1>(argv[i], '=');
            continue;
        }
        if (startswith(argv[i], "--c"))
        {
            //config = chop_first<1>(argv[i], '=');
            continue;
        }
        if (startswith(argv[i], "--u"))
        {
            username = chop_first<1>(argv[i], '=');
            continue;
        }
        if (startswith(argv[i], "--a"))
        {
            avatar = chop_first<1>(argv[i], '=');
            continue;
        }
    }
}

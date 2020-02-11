#ifndef TS_FILE_HPP
#define TS_FILE_HPP

#include <fstream>
#include <string>
#include <vector>

namespace ts
{
inline std::vector<std::string> readlines(const std::string& file)
{
    std::vector<std::string> ret;
    if (std::ifstream ifs(file); ifs.good())
    {
        std::string in;
        while (std::getline(ifs, in))
        {
            ret.emplace_back(std::move(in));
        }
    }
    return ret;
}
}  // namespace ts

#endif  // TS_FILE_HPP

#ifndef TS_STRINGTOOLS_HPP
#define TS_STRINGTOOLS_HPP

#include <algorithm>
#include <array>
#include <optional>
#include <string>

namespace ts::tools
{
template <int N>
auto splitn(const std::string& str, const char delim = '=') -> std::array<std::string, N>
{
    std::array<std::string, N> ret;

    const auto is_delim = [delim](const char arg) { return arg == delim; };

    int count = 0;
    for (auto itr = str.begin(); itr != str.end() && count < N; count++)
    {
        itr                   = std::find_if_not(itr, str.end(), is_delim);
        const auto substr_end = std::find_if(itr, str.end(), is_delim);
        ret[count]            = {itr, (count == N - 1) ? str.end() : substr_end};
        itr                   = substr_end;
    }

    return ret;
}

inline std::optional<int> stoi(const std::string& s)
{
    try
    {
        return std::stoi(s);
    }
    catch (const std::invalid_argument& e)
    {
        return {};
    }
}

inline bool is_whitespace(const char in)
{
    return in == ' ' || in == '\n' || in == '\r';
}
}  // namespace ts::tools

#endif  // TS_STRINGTOOLS_HPP

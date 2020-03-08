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

template <int N>
auto split_get_nth(const std::string& str, const char delim = '=') -> std::string
{
    const auto arr = ::ts::tools::splitn<N + 2>(str, delim); 
    return arr[N];
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

// Does a start with b?
inline bool startswith(const std::string& a, const std::string& b)
{
    return a.size() >= b.size() && a.substr(0, b.size()) == b;
}
}  // namespace ts::tools

#endif  // TS_STRINGTOOLS_HPP

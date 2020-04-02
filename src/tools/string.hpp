#ifndef TS_STRINGTOOLS_HPP
#define TS_STRINGTOOLS_HPP

#include <algorithm>
#include <array>
#include <optional>
#include <string>
#include <vector>

namespace ts::tools
{
inline auto is_char(char c)
{
    return [c](char c_in) { return c == c_in; };
}

inline std::string lstrip(const std::string& str, const char strip)
{
    return {std::find_if_not(str.begin(), str.end(), is_char(strip)), str.end()};
}

template <int N>
auto splitn(const std::string& str, const char delim) -> std::array<std::string, N>
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

inline std::vector<std::string> splitv(const std::string& str, const char delim)
{
    std::vector<std::string> ret;

    const auto is_delim = [delim](const char arg) { return arg == delim; };

    for (auto itr = str.begin(); itr != str.end();)
    {
        itr                   = std::find_if_not(itr, str.end(), is_delim);
        const auto substr_end = std::find_if(itr, str.end(), is_delim);
        ret.emplace_back(itr, substr_end);
        itr = substr_end;
    }

    return ret;
}

// Splits on the delimiter, then removes the first N strings, then returns the remainder as a
// string
template <int N>
auto chop_first(const std::string& str, const char delim) -> std::string
{
    const std::array<std::string, N + 1> split = ::ts::tools::splitn<N + 1>(str, delim);
    return split[N];
}

template <int N>
auto split_get_nth(const std::string& str, const char delim) -> std::string
{
    const auto arr = ::ts::tools::splitn<N + 2>(str, delim);
    return arr[N];
}

inline std::optional<long> stol(const std::string& s)
{
    try
    {
        return std::stol(s);
    }
    catch (const std::invalid_argument& e)
    {
        return {};
    }
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

template <int N>
auto all_stol(std::array<std::string, N> strs) -> std::array<std::optional<long>, N>
{
    std::array<std::optional<long>, N> r;
    for (int i = 0; i < N; i++)
    {
        r[i] = ::ts::tools::stol(strs[i]);
    }
    return r;
}

inline bool is_whitespace(const char in) { return in == ' ' || in == '\n' || in == '\r'; }

// Does a start with b?
inline bool startswith(const std::string& a, const std::string& b)
{
    return a.size() >= b.size() && a.substr(0, b.size()) == b;
}
inline bool startswithignore(const std::string& a_in, const std::string& b_in, const char ignore)
{
    const auto a = ::ts::tools::lstrip(a_in, ignore);
    const auto b = ::ts::tools::lstrip(b_in, ignore);
    return a.size() >= b.size() && a.substr(0, b.size()) == b;
}
}  // namespace ts::tools

#endif  // TS_STRINGTOOLS_HPP

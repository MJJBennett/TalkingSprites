#ifndef TS_DEBUG_HPP
#define TS_DEBUG_HPP

#include <iostream>

namespace ts::log
{
template <typename T>
void print(const T& t)
{
    std::cout << t;
}

template <typename T, typename... Tn>
void print(const T& t, const Tn&... tn)
{
    ::ts::log::print(t);
    ::ts::log::print(std::forward<const Tn&>(tn)...); 
}

template <typename... Tn>
void warn(const Tn&... tn)
{
    ::ts::log::print("Warning: ", std::forward<const Tn&>(tn)...);
    std::cout << std::endl;
}

template <int priority, typename T, typename... Tn>
void message(const T& t, const Tn&... tn)
{
    if constexpr (priority < 1) return;
    ::ts::log::print(t);
    ::ts::log::print(std::forward<const Tn&>(tn)...); 
    std::cout << std::endl;
}
}  // namespace ts::log

#endif  // TS_DEBUG_HPP

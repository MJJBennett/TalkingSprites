#ifndef TS_SERIALIZABLE_HPP
#define TS_SERIALIZABLE_HPP

#include <string>

namespace ts
{
struct serializable
{
    virtual std::string get_string() = 0;
};
}  // namespace ts

#endif  // TS_SERIALIZABLE_HPP

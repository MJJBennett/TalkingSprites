#ifndef TS_USER_HPP
#define TS_USER_HPP

#include "tools/serializable.hpp"

#include <string>

namespace ts
{
class User : public ::ts::serializable
{
public:
    User(std::string name_, long id_) : name(std::move(name_)), id(id_) {}

    std::string get_string() const override { return std::to_string(id) + "=" + name; }
    void from_string(std::string str) override;

protected:
    std::string name;
    long id;
};
}  // namespace ts

#endif  // TS_USER_HPP

#ifndef TS_APPLICATION_HPP
#define TS_APPLICATION_HPP

#include <string>
#include <vector>

namespace ts
{
class Application
{
public:
    void launch();
    void configure(std::vector<std::string> args);
    void configure(const std::string& arg);
};
}  // namespace ts

#endif  // TS_APPLICATION_HPP

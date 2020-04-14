#ifndef TS_BALANCE_HPP
#define TS_BALANCE_HPP

#include <string>

#include "widget.hpp"

namespace ts
{
class Balance : public ts::WidgetBase
{
public:
    void set(long bal) { balance = "Balance: " + std::to_string(bal); }
    void draw_self(bool debug = false) override;

private:
    std::string balance{"Balance: 0"};
};
}  // namespace ts

#endif  // TS_BALANCE_HPP

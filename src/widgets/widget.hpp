#ifndef TS_WIDGET_HPP
#define TS_WIDGET_HPP

namespace ts
{
static bool always_open = true;

class WidgetBase
{
public:
    void toggle_visible() { visible = !visible; }

protected:
    bool visible{true};
};
}  // namespace ts

#endif  // TS_WIDGET_HPP

#ifndef TS_WIDGET_HPP
#define TS_WIDGET_HPP

namespace ts
{
static bool always_open = true;

class WidgetBase
{
public:
    void toggle_visible() { visible = !visible; }
    void draw(bool debug = false) { if (visible) draw_self(debug); }
    virtual void draw_self(bool) = 0;

protected:
    bool visible{true};
};
}  // namespace ts

#endif  // TS_WIDGET_HPP

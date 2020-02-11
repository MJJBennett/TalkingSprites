#ifndef TS_RESOURCE_HPP
#define TS_RESOURCE_HPP

namespace ts
{
struct SpriteHandle
{
    unsigned long handle{0};
    static constexpr unsigned long None = 0;
};
struct TextureHandle
{
    unsigned long handle{0};
    static constexpr unsigned long None = 0;
};
}  // namespace ts

#endif  // TS_RESOURCE_HPP

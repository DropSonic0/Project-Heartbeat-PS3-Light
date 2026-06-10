#ifndef COMPAT_VECTOR2_HPP
#define COMPAT_VECTOR2_HPP

namespace godot {
class Vector2 {
public:
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float p_x, float p_y) : x(p_x), y(p_y) {}
    Vector2 rotated(float p_by) const { return *this; }
    Vector2 operator+(const Vector2& p_v) const { return Vector2(x + p_v.x, y + p_v.y); }
    Vector2 operator*(float p_s) const { return Vector2(x * p_s, y * p_s); }
};
}

#endif

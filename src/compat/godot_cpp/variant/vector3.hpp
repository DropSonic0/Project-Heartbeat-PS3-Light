#ifndef COMPAT_VECTOR3_HPP
#define COMPAT_VECTOR3_HPP

#include <cmath>

namespace godot {

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float p_x, float p_y, float p_z) : x(p_x), y(p_y), z(p_z) {}

    Vector3 operator+(const Vector3& p_v) const { return Vector3(x + p_v.x, y + p_v.y, z + p_v.z); }
    Vector3& operator+=(const Vector3& p_v) { x += p_v.x; y += p_v.y; z += p_v.z; return *this; }
    Vector3 operator*(float p_s) const { return Vector3(x * p_s, y * p_s, z * p_s); }

    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 normalized() const {
        float l = length();
        if (l == 0) return Vector3();
        return Vector3(x / l, y / l, z / l);
    }
};

}

#endif

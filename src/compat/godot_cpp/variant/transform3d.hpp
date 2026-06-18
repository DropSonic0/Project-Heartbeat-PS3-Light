#ifndef COMPAT_TRANSFORM3D_HPP
#define COMPAT_TRANSFORM3D_HPP

#include "vector3.hpp"
#include <cmath>

namespace godot {

struct Transform3D {
    float basis[9]; // 3x3 matrix (row-major)
    Vector3 origin;

    Transform3D() {
        for (int i = 0; i < 9; i++) basis[i] = 0.0f;
        basis[0] = 1.0f;
        basis[4] = 1.0f;
        basis[8] = 1.0f;
    }

    static Transform3D rotated(const Vector3& p_axis, float p_angle) {
        Transform3D res;
        float c = std::cos(p_angle);
        float s = std::sin(p_angle);
        float t = 1.0f - c;
        Vector3 axis = p_axis.normalized();
        float x = axis.x, y = axis.y, z = axis.z;

        res.basis[0] = t*x*x + c;
        res.basis[1] = t*x*y - s*z;
        res.basis[2] = t*x*z + s*y;

        res.basis[3] = t*x*y + s*z;
        res.basis[4] = t*y*y + c;
        res.basis[5] = t*y*z - s*x;

        res.basis[6] = t*x*z - s*y;
        res.basis[7] = t*y*z + s*x;
        res.basis[8] = t*z*z + c;

        return res;
    }

    Transform3D translated(const Vector3& p_offset) const {
        Transform3D res = *this;
        res.origin.x += basis[0] * p_offset.x + basis[1] * p_offset.y + basis[2] * p_offset.z;
        res.origin.y += basis[3] * p_offset.x + basis[4] * p_offset.y + basis[5] * p_offset.z;
        res.origin.z += basis[6] * p_offset.x + basis[7] * p_offset.y + basis[8] * p_offset.z;
        return res;
    }

    Transform3D operator*(const Transform3D& p_other) const {
        Transform3D res;
        // Basis multiplication (Row-major)
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                res.basis[i * 3 + j] = 0;
                for (int k = 0; k < 3; k++) {
                    res.basis[i * 3 + j] += basis[i * 3 + k] * p_other.basis[k * 3 + j];
                }
            }
        }
        // Origin transformation: T1 * T2 = (B1, O1) * (B2, O2) = (B1*B2, B1*O2 + O1)
        res.origin.x = basis[0] * p_other.origin.x + basis[1] * p_other.origin.y + basis[2] * p_other.origin.z + origin.x;
        res.origin.y = basis[3] * p_other.origin.x + basis[4] * p_other.origin.y + basis[5] * p_other.origin.z + origin.y;
        res.origin.z = basis[6] * p_other.origin.x + basis[7] * p_other.origin.y + basis[8] * p_other.origin.z + origin.z;
        return res;
    }
};

}

#endif

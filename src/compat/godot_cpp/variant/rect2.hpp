#ifndef COMPAT_RECT2_HPP
#define COMPAT_RECT2_HPP

#include "vector2.hpp"

namespace godot {

struct Rect2 {
    Vector2 position;
    Vector2 size;

    Rect2() {}
    Rect2(float x, float y, float w, float h) : position(x, y), size(w, h) {}
    Rect2(Vector2 p_pos, Vector2 p_size) : position(p_pos), size(p_size) {}
};

}

#endif

#ifndef COMPAT_COLOR_HPP
#define COMPAT_COLOR_HPP

namespace godot {
class Color {
public:
    float r, g, b, a;
    Color() : r(0), g(0), b(0), a(1) {}
    Color(float p_r, float p_g, float p_b, float p_a = 1.0) : r(p_r), g(p_g), b(p_b), a(p_a) {}
    Color(const char* p_html) {}
    static Color from_hsv(float h, float s, float v, float a = 1.0) { return Color(); }
    std::string to_html(bool p_alpha = true) const { return "ffffffff"; }
};
}

#endif

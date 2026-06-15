#ifndef COMPAT_COLOR_HPP
#define COMPAT_COLOR_HPP

#include "variant.hpp"
#include <stdio.h>
#include <string>
#include <stdlib.h>

namespace godot {
class Color {
public:
    float r, g, b, a;
    Color() : r(0), g(0), b(0), a(1) {}
    Color(float p_r, float p_g, float p_b, float p_a = 1.0) : r(p_r), g(p_g), b(p_b), a(p_a) {}
    Color(const char* p_html) { _from_html(p_html); }
    Color(const String& p_html) { _from_html(p_html.c_str()); }
    static Color from_hsv(float h, float s, float v, float a = 1.0) { return Color(); }
    String to_html(bool p_alpha = true) const {
        char buf[10];
        if (p_alpha) {
            sprintf(buf, "%02x%02x%02x%02x", (int)(r * 255), (int)(g * 255), (int)(b * 255), (int)(a * 255));
        } else {
            sprintf(buf, "%02x%02x%02x", (int)(r * 255), (int)(g * 255), (int)(b * 255));
        }
        return String(buf);
    }
private:
    void _from_html(const char* p_html) {
        std::string s = p_html;
        if (s.length() >= 6) {
            if (s[0] == '#') s = s.substr(1);
            if (s.length() == 6) {
                r = strtol(s.substr(0, 2).c_str(), NULL, 16) / 255.0f;
                g = strtol(s.substr(2, 2).c_str(), NULL, 16) / 255.0f;
                b = strtol(s.substr(4, 2).c_str(), NULL, 16) / 255.0f;
                a = 1.0f;
            } else if (s.length() == 8) {
                r = strtol(s.substr(0, 2).c_str(), NULL, 16) / 255.0f;
                g = strtol(s.substr(2, 2).c_str(), NULL, 16) / 255.0f;
                b = strtol(s.substr(4, 2).c_str(), NULL, 16) / 255.0f;
                a = strtol(s.substr(6, 2).c_str(), NULL, 16) / 255.0f;
            }
        }
    }
};
}

#endif

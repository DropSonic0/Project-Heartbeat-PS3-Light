#ifndef COMPAT_UTILITY_FUNCTIONS_HPP
#define COMPAT_UTILITY_FUNCTIONS_HPP

#include <cmath>
#include <string>

namespace godot {

class UtilityFunctions {
public:
    static double inverse_lerp(double from, double to, double weight) {
        return (weight - from) / (to - from);
    }
    static double deg_to_rad(double p_deg) { return p_deg * 3.14159265358979323846 / 180.0; }
    static int randi() { return 0; } // Placeholder
};

#ifndef Math_PI
#define Math_PI 3.14159265358979323846
#endif

}

#endif

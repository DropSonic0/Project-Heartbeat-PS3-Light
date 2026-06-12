#ifndef COMPAT_UTILITY_FUNCTIONS_HPP
#define COMPAT_UTILITY_FUNCTIONS_HPP

#include <cmath>
#include <string>
#include <fstream>
#include "variant.hpp"

namespace godot {

class UtilityFunctions {
public:
    static double inverse_lerp(double from, double to, double weight) {
        return (weight - from) / (to - from);
    }
    static double deg_to_rad(double p_deg) { return p_deg * 3.14159265358979323846 / 180.0; }
    static int randi() { return 0; } // Placeholder
    
    static void print(const char* p_msg) {
        std::ofstream log_file("/dev_hdd0/game/PROJECTHB/USRDIR/log.txt", std::ios_base::app);
        if (log_file.is_open()) {
            log_file << p_msg << std::endl;
            log_file.close();
        }
    }

    static void print(const std::string& p_msg) {
        print(p_msg.c_str());
    }

    static void print(const Variant& p_v) {
        std::ofstream log_file("/dev_hdd0/game/PROJECTHB/USRDIR/log.txt", std::ios_base::app);
        if (log_file.is_open()) {
            log_file << (std::string)Variant(p_v) << std::endl;
            log_file.close();
        }
    }
};

#ifndef Math_PI
#define Math_PI 3.14159265358979323846
#endif

}

#endif

#ifndef COMPAT_TIME_HPP
#define COMPAT_TIME_HPP

#include "object.hpp"

namespace godot {

class Time : public Object {
public:
    static Time* get_singleton() {
        static Time* singleton = new Time();
        return singleton;
    }
    double get_unix_time_from_system() { return 0.0; }
};

}

#endif

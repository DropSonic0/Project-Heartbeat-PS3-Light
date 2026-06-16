#ifndef COMPAT_TIME_HPP
#define COMPAT_TIME_HPP

#include "object.hpp"
#include <time.h>
#include <stdint.h>
#ifdef __PPU__
#include <sys/sys_time.h>
#include <sys/time_util.h>
#endif

namespace godot {

class Time : public Object {
public:
    static Time* get_singleton() {
        static Time* singleton = new Time();
        return singleton;
    }
    double get_unix_time_from_system() {
        return (double)time(NULL);
    }
    uint64_t get_ticks_usec() {
#ifdef __PPU__
        return sys_time_get_system_time();
#else
        return (uint64_t)time(NULL) * 1000000;
#endif
    }
    uint64_t get_ticks_msec() {
        return get_ticks_usec() / 1000;
    }
};

}

#endif

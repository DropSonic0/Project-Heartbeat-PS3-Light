#ifndef COMPAT_OS_HPP
#define COMPAT_OS_HPP

#include "object.hpp"

namespace godot {

class OS : public Object {
public:
    static OS* get_singleton() {
        static OS* singleton = new OS();
        return singleton;
    }
};

}

#endif

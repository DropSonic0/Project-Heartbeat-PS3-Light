#ifndef COMPAT_OS_HPP
#define COMPAT_OS_HPP

#include "object.hpp"
#include "../variant/variant.hpp"

namespace godot {

class OS : public Object {
public:
    static OS* get_singleton() {
        static OS* singleton = new OS();
        return singleton;
    }

    String get_executable_path() const {
        return "/dev_hdd0/game/PROJECTHB/USRDIR/EBOOT.BIN";
    }

    String get_user_data_dir() const {
        return "/dev_hdd0/game/PROJECTHB/USRDIR/";
    }

    String get_name() const {
        return "PS3";
    }
};

}

#endif

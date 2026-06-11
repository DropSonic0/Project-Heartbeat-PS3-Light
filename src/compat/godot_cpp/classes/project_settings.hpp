#ifndef COMPAT_PROJECT_SETTINGS_HPP
#define COMPAT_PROJECT_SETTINGS_HPP

#include "object.hpp"

namespace godot {

class ProjectSettings : public Object {
public:
    static ProjectSettings* get_singleton() {
        static ProjectSettings* singleton = new ProjectSettings();
        return singleton;
    }
};

}

#endif

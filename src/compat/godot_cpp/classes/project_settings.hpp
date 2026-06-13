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

    bool load_resource_pack(const String& p_pack, bool p_replace_files = true, int p_offset = 0) {
        return true;
    }
};

}

#endif

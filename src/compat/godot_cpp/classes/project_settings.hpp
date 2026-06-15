#ifndef COMPAT_PROJECT_SETTINGS_HPP
#define COMPAT_PROJECT_SETTINGS_HPP

#include "object.hpp"
#include "../variant/variant.hpp"
#include "../variant/utility_functions.hpp"

namespace godot {

class ProjectSettings : public Object {
    std::vector<String> loaded_packs;
public:
    static ProjectSettings* get_singleton() {
        static ProjectSettings* singleton = new ProjectSettings();
        return singleton;
    }

    bool load_resource_pack(const String& p_pack, bool p_replace_files = true, int p_offset = 0) {
        UtilityFunctions::print("ProjectSettings: Loading resource pack: " + p_pack);
        loaded_packs.push_back(p_pack);
        return true;
    }

    bool is_pack_loaded(const String& p_pack) const {
        for (size_t i = 0; i < loaded_packs.size(); i++) {
            if (loaded_packs[i] == p_pack) return true;
        }
        return false;
    }
};

}

#endif

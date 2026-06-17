#ifndef COMPAT_PROJECT_SETTINGS_HPP
#define COMPAT_PROJECT_SETTINGS_HPP

#include "object.hpp"
#include "../variant/variant.hpp"
#include "../variant/utility_functions.hpp"

namespace godot {

struct PCKFileEntry {
    uint64_t offset;
    uint64_t size;
    uint32_t flags;
};

class PCKReader;

class ProjectSettings : public Object {
    std::vector<PCKReader*> loaded_packs;
public:
    static ProjectSettings* get_singleton() {
        static ProjectSettings* singleton = new ProjectSettings();
        return singleton;
    }

    bool load_resource_pack(const String& p_pack, bool p_replace_files = true, int p_offset = 0);
    bool is_pack_loaded(const String& p_pack) const;
    PCKFileEntry find_file_in_packs(const String& p_path, String& r_pck_path) const;
    PackedStringArray get_files_in_packs(const String& p_path) const;
    PackedStringArray get_directories_in_packs(const String& p_path) const;
};

}

#endif

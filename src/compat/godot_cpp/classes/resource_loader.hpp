#ifndef COMPAT_RESOURCE_LOADER_HPP
#define COMPAT_RESOURCE_LOADER_HPP

#include "object.hpp"
#include "resource.hpp"
#include "project_settings.hpp"
#include "file_access.hpp"
#include "../variant/utility_functions.hpp"

namespace godot {

class ResourceLoader : public Object {
public:
    static ResourceLoader* get_singleton() {
        static ResourceLoader* singleton = new ResourceLoader();
        return singleton;
    }

    Ref<Resource> load(const String& p_path, const String& p_type_hint = "") {
        UtilityFunctions::print("ResourceLoader: Attempting to load: " + p_path);
        
        String path = p_path;
        if (path.begins_with("res://")) {
            // Mapping res:// to USRDIR for Project Heartbeat PS3
            path = path.replace("res://", "/dev_hdd0/game/PROJECTHB/USRDIR/");
        }

        if (FileAccess::file_exists(path)) {
            UtilityFunctions::print("ResourceLoader: Found file at: " + path);
            // In a real implementation, we would instantiate the correct Resource type based on extension
            Ref<Resource> res;
            res.instantiate();
            return res;
        }

        UtilityFunctions::print("ResourceLoader: Resource not found: " + p_path);
        return Ref<Resource>(NULL);
    }
};

}

#endif

#ifndef COMPAT_RESOURCE_LOADER_HPP
#define COMPAT_RESOURCE_LOADER_HPP

#include "object.hpp"
#include "resource.hpp"
#include "image.hpp"
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
            path = path.replace("res://", "/dev_hdd0/game/PROJECTHB/USRDIR/");
        }

        if (FileAccess::file_exists(path)) {
            UtilityFunctions::print("ResourceLoader: Found file at: " + path);
            
            if (path.ends_with(".png")) {
                Ref<FileAccess> f = FileAccess::open(path, FileAccess::READ);
                if (f.is_valid()) {
                    // For Project Heartbeat PS3 port purposes, we create an Image
                    // Real Godot would use a PNG loader, here we just verify metadata
                    Ref<Image> img = Image::create(64, 64, false, 0);
                    // In a real port we would read actual pixels here
                    return (Ref<Resource>)img;
                }
            }

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

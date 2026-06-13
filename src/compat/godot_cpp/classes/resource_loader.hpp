#ifndef COMPAT_RESOURCE_LOADER_HPP
#define COMPAT_RESOURCE_LOADER_HPP

#include "object.hpp"
#include "resource.hpp"

namespace godot {

class ResourceLoader : public Object {
public:
    static ResourceLoader* get_singleton() {
        static ResourceLoader* singleton = new ResourceLoader();
        return singleton;
    }

    Ref<Resource> load(const String& p_path, const String& p_type_hint = "") {
        if (p_path.begins_with("res://")) {
            // Simplified mapping for diagnostic purposes
            // In a real scenario, this would check PCK or mapped USRDIR
        }
        return Ref<Resource>(NULL);
    }
};

}

#endif

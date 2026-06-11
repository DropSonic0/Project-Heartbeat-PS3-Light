#ifndef COMPAT_GODOT_HPP
#define COMPAT_GODOT_HPP

#include "core/defs.hpp"
#include "variant/variant.hpp"
#include "variant/callable.hpp"
#include "classes/object.hpp"
#include "classes/ref_counted.hpp"

#include "../gdextension_interface.h"

namespace godot {
    // Basic definitions to allow compilation without the full engine
    enum ModuleInitializationLevel {
        MODULE_INITIALIZATION_LEVEL_CORE = GDEXTENSION_INITIALIZATION_CORE,
        MODULE_INITIALIZATION_LEVEL_SERVERS = GDEXTENSION_INITIALIZATION_SERVERS,
        MODULE_INITIALIZATION_LEVEL_SCENE = GDEXTENSION_INITIALIZATION_SCENE,
        MODULE_INITIALIZATION_LEVEL_EDITOR = GDEXTENSION_INITIALIZATION_EDITOR,
    };

    class GDExtensionBinding {
    public:
        class InitObject {
        public:
            InitObject(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {}
            void register_initializer(void (*p_init)(ModuleInitializationLevel)) {}
            void register_terminator(void (*p_terminate)(ModuleInitializationLevel)) {}
            void set_minimum_library_initialization_level(ModuleInitializationLevel p_level) {}
            GDExtensionBool init() { return 1; }
        };
    };
}

#endif

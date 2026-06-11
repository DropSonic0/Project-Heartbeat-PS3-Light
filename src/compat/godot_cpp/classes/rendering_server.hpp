#ifndef COMPAT_RENDERING_SERVER_HPP
#define COMPAT_RENDERING_SERVER_HPP

#include "object.hpp"

namespace godot {

class RenderingServer : public Object {
public:
    static RenderingServer* get_singleton() {
        static RenderingServer* singleton = new RenderingServer();
        return singleton;
    }
};

}

#endif

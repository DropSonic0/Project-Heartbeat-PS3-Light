#ifndef COMPAT_RENDERING_SERVER_HPP
#define COMPAT_RENDERING_SERVER_HPP

#include "object.hpp"
#include "rendering_device.hpp"

namespace godot {

class RenderingServer : public Object {
public:
    static RenderingServer* get_singleton() {
        static RenderingServer* singleton = new RenderingServer();
        return singleton;
    }

    RID texture_2d_create(Ref<Object> p_image) {
        // Here we will eventually call psglCreateTexture or similar
        return RID(); 
    }
    
    void canvas_item_set_parent(RID p_item, RID p_parent) {}
    RID canvas_item_create() { return RID(); }
    void viewport_set_canvas_stack(RID p_viewport, RID p_canvas, uint32_t p_layer, uint32_t p_sublayer) {}
    
    RenderingDevice* get_rendering_device() {
        static RenderingDevice* rd = new RenderingDevice();
        return rd;
    }
};

}

#endif

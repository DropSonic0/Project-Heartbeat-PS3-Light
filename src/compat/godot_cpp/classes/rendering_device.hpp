#ifndef COMPAT_RENDERING_DEVICE_HPP
#define COMPAT_RENDERING_DEVICE_HPP

#include "object.hpp"
#include "../variant/variant.hpp"

namespace godot {

class RenderingDevice : public Object {
public:
    enum TextureUsageBits {
        TEXTURE_USAGE_SAMPLING_BIT = 1,
        TEXTURE_USAGE_COLOR_ATTACHMENT_BIT = 2,
        TEXTURE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT = 4,
        TEXTURE_USAGE_STORAGE_BIT = 8,
        TEXTURE_USAGE_CAN_COPY_FROM_BIT = 16,
        TEXTURE_USAGE_CAN_COPY_TO_BIT = 32,
        TEXTURE_USAGE_CAN_UPDATE_BIT = 64
    };

    RID texture_create(Ref<Object> p_format, Ref<Object> p_view, const Array& p_data = Array()) { return RID(); }
    void free_rid(RID p_rid) {}
};

}

#endif

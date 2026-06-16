#ifndef COMPAT_RD_TEXTURE_FORMAT_HPP
#define COMPAT_RD_TEXTURE_FORMAT_HPP

#include "object.hpp"

namespace godot {

class RDTextureFormat : public Object {
    GDCLASS(RDTextureFormat, Object);
public:
    int format = 0;
    int width = 0;
    int height = 0;
    int depth = 1;
    int array_layers = 1;
    int mipmaps = 1;
    int texture_type = 0;
    int samples = 0;
    uint32_t usage_bits = 0;

    void set_format(int p_format) { format = p_format; }
    void set_width(int p_width) { width = p_width; }
    void set_height(int p_height) { height = p_height; }
    void set_usage_bits(uint32_t p_usage) { usage_bits = p_usage; }
};

}

#endif

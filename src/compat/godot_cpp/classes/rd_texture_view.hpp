#ifndef COMPAT_RD_TEXTURE_VIEW_HPP
#define COMPAT_RD_TEXTURE_VIEW_HPP

#include "object.hpp"

namespace godot {

class RDTextureView : public Object {
    GDCLASS(RDTextureView, Object);
public:
    int format_override = 0;
    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;
};

}

#endif

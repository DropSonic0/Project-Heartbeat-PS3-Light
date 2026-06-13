#ifndef COMPAT_IMAGE_HPP
#define COMPAT_IMAGE_HPP

#include "resource.hpp"

namespace godot {

class Image : public Resource {
    GDCLASS(Image, Resource);
public:
    static Ref<Image> create(int p_width, int p_height, bool p_use_mipmaps, int p_format) {
        Ref<Image> img;
        img.instantiate();
        return img;
    }
};

}

#endif

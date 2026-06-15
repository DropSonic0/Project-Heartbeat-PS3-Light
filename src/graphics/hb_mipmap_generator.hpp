#ifndef HB_MIPMAP_GENERATOR_HPP
#define HB_MIPMAP_GENERATOR_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/object.hpp"
#include "compat/godot_cpp/classes/image.hpp"
#else
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/image.hpp>
#endif

namespace godot {

class HBMipmapGeneratorNative : public Object {
    GDCLASS(HBMipmapGeneratorNative, Object);

protected:
    static void _bind_methods();

public:
    HBMipmapGeneratorNative();
    ~HBMipmapGeneratorNative();

    void generate_mipmaps_software(Ref<Image> p_image);
};

} // namespace godot

#endif // HB_MIPMAP_GENERATOR_HPP

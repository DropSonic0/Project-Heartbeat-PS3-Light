#include "hb_mipmap_generator.hpp"

#ifdef __PPU__
#include "compat/godot_cpp/core/class_db.hpp"
#else
#include <godot_cpp/core/class_db.hpp>
#endif

using namespace godot;

void HBMipmapGeneratorNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("generate_mipmaps_software", "image"), &HBMipmapGeneratorNative::generate_mipmaps_software);
}

HBMipmapGeneratorNative::HBMipmapGeneratorNative() {}
HBMipmapGeneratorNative::~HBMipmapGeneratorNative() {}

void HBMipmapGeneratorNative::generate_mipmaps_software(Ref<Image> p_image) {
    if (p_image.is_valid()) {
        p_image->generate_mipmaps();
    }
}

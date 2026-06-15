#include "hb_diva_texture_processor.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void DIVATextureProcessorNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("process_texture", "image"), &DIVATextureProcessorNative::process_texture);
}

DIVATextureProcessorNative::DIVATextureProcessorNative() {}
DIVATextureProcessorNative::~DIVATextureProcessorNative() {}

void DIVATextureProcessorNative::process_texture(Ref<Image> p_image) {
    if (p_image.is_null()) return;
}

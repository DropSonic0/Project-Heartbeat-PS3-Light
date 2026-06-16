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

    int w = p_image->get_width();
    int h = p_image->get_height();
    if (w <= 0 || h <= 0) return;

    PackedByteArray data = p_image->get_data();
    if (data.size() < (size_t)(w * h * 4)) return;

    PackedByteArray flipped_data;
    flipped_data.resize(data.size());

    // Basic vertical flip for software processing
    for (int y = 0; y < h; y++) {
        int src_y = y;
        int dst_y = h - 1 - y;
        for (int x = 0; x < w; x++) {
            size_t src_off = (src_y * w + x) * 4;
            size_t dst_off = (dst_y * w + x) * 4;
            for (int c = 0; c < 4; c++) {
                if (src_off + c < data.size() && dst_off + c < flipped_data.size()) {
                    flipped_data[dst_off + c] = data[src_off + c];
                }
            }
        }
    }
    p_image->set_data(flipped_data);
}

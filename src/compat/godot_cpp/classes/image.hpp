#ifndef COMPAT_IMAGE_HPP
#define COMPAT_IMAGE_HPP

#include "resource.hpp"
#include "../variant/variant.hpp"
#include <algorithm>

namespace godot {

class Image : public Resource {
    GDCLASS(Image, Resource);
    int width = 0;
    int height = 0;
    int format = 0;
    PackedByteArray data;
    bool has_mipmaps = false;

public:
    static Ref<Image> create(int p_width, int p_height, bool p_use_mipmaps, int p_format) {
        Ref<Image> img;
        img.instantiate();
        img->width = p_width;
        img->height = p_height;
        img->format = p_format;
        img->has_mipmaps = p_use_mipmaps;
        if (p_width > 0 && p_height > 0) {
            img->data.resize(p_width * p_height * 4); // Assuming RGBA8
        }
        return img;
    }

    void set_data(const PackedByteArray& p_data) { data = p_data; }
    PackedByteArray get_data() const { return data; }
    int get_width() const { return width; }
    int get_height() const { return height; }
    int get_format() const { return format; }

    void generate_mipmaps() {
        if (width <= 0 || height <= 0) return;
        
        int w = width;
        int h = height;
        
        // Calculate total size for all mipmap levels
        size_t total_size = 0;
        int temp_w = w;
        int temp_h = h;
        while (true) {
            total_size += temp_w * temp_h * 4;
            if (temp_w == 1 && temp_h == 1) break;
            if (temp_w > 1) temp_w /= 2;
            if (temp_h > 1) temp_h /= 2;
        }
        
        PackedByteArray new_data;
        new_data.resize(total_size);
        
        // Copy base level
        for (size_t i = 0; i < (size_t)(w * h * 4); i++) {
            if (i < data.size()) {
                new_data[i] = data[i];
            }
        }
        
        size_t prev_level_offset = 0;
        size_t curr_level_offset = w * h * 4;
        
        int prev_w = w;
        int prev_h = h;
        
        while (w > 1 || h > 1) {
            int next_w = std::max(1, w / 2);
            int next_h = std::max(1, h / 2);
            
            for (int y = 0; y < next_h; y++) {
                for (int x = 0; x < next_w; x++) {
                    int r = 0, g = 0, b = 0, a = 0;
                    int count = 0;
                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = 0; dx < 2; dx++) {
                            int sx = x * 2 + dx;
                            int sy = y * 2 + dy;
                            if (sx < w && sy < h) {
                                size_t px_off = prev_level_offset + (sy * w + sx) * 4;
                                r += new_data[px_off + 0];
                                g += new_data[px_off + 1];
                                b += new_data[px_off + 2];
                                a += new_data[px_off + 3];
                                count++;
                            }
                        }
                    }
                    size_t dest_off = curr_level_offset + (y * next_w + x) * 4;
                    if (count > 0) {
                        new_data[dest_off + 0] = r / count;
                        new_data[dest_off + 1] = g / count;
                        new_data[dest_off + 2] = b / count;
                        new_data[dest_off + 3] = a / count;
                    }
                }
            }
            
            prev_level_offset = curr_level_offset;
            curr_level_offset += next_w * next_h * 4;
            w = next_w;
            h = next_h;
        }
        
        data = new_data;
        has_mipmaps = true;
    }
};

}

#endif

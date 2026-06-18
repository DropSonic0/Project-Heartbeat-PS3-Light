#ifndef HB_VIDEO_DRIVER_PSGL_HPP
#define HB_VIDEO_DRIVER_PSGL_HPP

#ifdef __PPU__
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include <cell/pad.h>
#endif
#include "../compat/godot_cpp/variant/color.hpp"
#include "../compat/godot_cpp/variant/rect2.hpp"
#include "../compat/godot_cpp/classes/image.hpp"

namespace godot {

class FontVariation;

class HBVideoDriverPSGL {
public:
    static bool initialize();
    static void terminate();
    static void swap_buffers();
    static void clear_buffer();
    static void update_system_callbacks();
    static bool should_exit();
    
    static void draw_rect(const Rect2& p_rect, const Color& p_color);
    static void draw_texture(const Ref<Image>& p_image, const Rect2& p_rect, const Color& p_modulate = Color(1, 1, 1, 1));
    static void draw_text(const String& p_text, const Vector2& p_pos, const Color& p_color = Color(1, 1, 1, 1), float p_scale = 1.0f, bool p_shadow = false);
    static void draw_text_with_font(const Ref<FontVariation>& p_font, const String& p_text, const Vector2& p_pos, int p_size, const Color& p_color = Color(1, 1, 1, 1), bool p_shadow = false);
    static void clear_texture_cache();
    static bool is_button_pressed(int p_button);
};

}

#endif

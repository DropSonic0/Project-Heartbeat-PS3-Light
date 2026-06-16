#ifndef HB_VIDEO_DRIVER_PSGL_HPP
#define HB_VIDEO_DRIVER_PSGL_HPP

#ifdef __PPU__
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include <cell/pad.h>
#endif
#include "../compat/godot_cpp/variant/color.hpp"
#include "../compat/godot_cpp/variant/rect2.hpp"

namespace godot {

class HBVideoDriverPSGL {
public:
    static bool initialize();
    static void terminate();
    static void swap_buffers();
    static void clear_buffer();
    static void update_system_callbacks();
    static bool should_exit();
    
    static void draw_rect(const Rect2& p_rect, const Color& p_color);
    static bool is_button_pressed(int p_button);
};

}

#endif

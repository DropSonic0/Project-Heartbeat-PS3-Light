#ifndef HB_VIDEO_DRIVER_PSGL_HPP
#define HB_VIDEO_DRIVER_PSGL_HPP

#ifdef __PPU__
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#endif

namespace godot {

class HBVideoDriverPSGL {
public:
    static bool initialize();
    static void terminate();
    static void swap_buffers();
    static void clear_buffer();
};

}

#endif

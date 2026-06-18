#ifndef HB_INPUT_NATIVE_HPP
#define HB_INPUT_NATIVE_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/object.hpp"
#else
#include <godot_cpp/classes/object.hpp>
#endif
#include "graphics/hb_video_driver_psgl.hpp"

namespace godot {

class HBInputNative : public Object {
public:
    enum Action {
        ACTION_UP,
        ACTION_DOWN,
        ACTION_LEFT,
        ACTION_RIGHT,
        ACTION_ACCEPT,
        ACTION_BACK,
        ACTION_SQUARE,
        ACTION_TRIANGLE,
        ACTION_MAX
    };

    static bool is_action_just_pressed(Action p_action);
    static void update();

private:
    static bool _last_state[ACTION_MAX];
    static bool _current_state[ACTION_MAX];
};

}

#endif

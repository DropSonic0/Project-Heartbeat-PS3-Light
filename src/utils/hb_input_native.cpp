#include "hb_input_native.hpp"

namespace godot {

bool HBInputNative::_last_state[ACTION_MAX] = {false};
bool HBInputNative::_current_state[ACTION_MAX] = {false};

void HBInputNative::update() {
    for (int i = 0; i < ACTION_MAX; i++) {
        _last_state[i] = _current_state[i];
    }

#ifdef __PPU__
    _current_state[ACTION_UP] = HBVideoDriverPSGL::is_button_pressed(CELL_PAD_CTRL_UP);
    _current_state[ACTION_DOWN] = HBVideoDriverPSGL::is_button_pressed(CELL_PAD_CTRL_DOWN);
    _current_state[ACTION_LEFT] = HBVideoDriverPSGL::is_button_pressed(CELL_PAD_CTRL_LEFT);
    _current_state[ACTION_RIGHT] = HBVideoDriverPSGL::is_button_pressed(CELL_PAD_CTRL_RIGHT);
    _current_state[ACTION_ACCEPT] = HBVideoDriverPSGL::is_button_pressed(CELL_PAD_CTRL_CROSS);
    _current_state[ACTION_BACK] = HBVideoDriverPSGL::is_button_pressed(CELL_PAD_CTRL_CIRCLE);
#endif
}

bool HBInputNative::is_action_just_pressed(Action p_action) {
    if (p_action < 0 || p_action >= ACTION_MAX) return false;
    bool pressed = _current_state[p_action] && !_last_state[p_action];
    if (pressed) {
        // UtilityFunctions::print("Input: Action just pressed: " + String::num(p_action));
    }
    return pressed;
}

}

#ifndef HB_INPUT_EVENT_HPP
#define HB_INPUT_EVENT_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/input_event_action.hpp"
#else
#include <godot_cpp/classes/input_event_action.hpp>
#endif

namespace godot {

class InputEventHBNative : public InputEventAction {
    GDCLASS(InputEventHBNative, InputEventAction);

protected:
    static void _bind_methods();

public:
    InputEventHBNative();
    ~InputEventHBNative();

    void set_event_uid(int p_uid);
    int get_event_uid() const;

private:
    int event_uid = 0;
};

} // namespace godot

#endif // HB_INPUT_EVENT_HPP

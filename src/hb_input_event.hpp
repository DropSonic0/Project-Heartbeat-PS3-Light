#ifndef HB_INPUT_EVENT_HPP
#define HB_INPUT_EVENT_HPP

#include <godot_cpp/classes/input_event_action.hpp>

namespace godot {

class InputEventHBNative : public InputEventAction {
    GDCLASS(InputEventHBNative, InputEventAction);

protected:
    static void _bind_methods();

public:
    InputEventHBNative();
    ~InputEventHBNative();
};

} // namespace godot

#endif // HB_INPUT_EVENT_HPP

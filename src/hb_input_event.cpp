#include "hb_input_event.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void InputEventHBNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_event_uid", "uid"), &InputEventHBNative::set_event_uid);
    ClassDB::bind_method(D_METHOD("get_event_uid"), &InputEventHBNative::get_event_uid);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "event_uid"), "set_event_uid", "get_event_uid");
}

InputEventHBNative::InputEventHBNative() {}
InputEventHBNative::~InputEventHBNative() {}

void InputEventHBNative::set_event_uid(int p_uid) { event_uid = p_uid; }
int InputEventHBNative::get_event_uid() const { return event_uid; }

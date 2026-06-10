#include "hb_game.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

static HBGameNative *singleton = nullptr;

void HBGameNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_demo_mode", "demo_mode"), &HBGameNative::set_demo_mode);
    ClassDB::bind_method(D_METHOD("get_demo_mode"), &HBGameNative::get_demo_mode);
    ClassDB::bind_method(D_METHOD("get_serializable_types"), &HBGameNative::get_serializable_types);
    ClassDB::bind_method(D_METHOD("register_serializable_type", "name", "type"), &HBGameNative::register_serializable_type);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "demo_mode"), "set_demo_mode", "get_demo_mode");
}

HBGameNative::HBGameNative() {
    singleton = this;
}

HBGameNative::~HBGameNative() {
    singleton = nullptr;
}

void HBGameNative::set_demo_mode(bool p_demo_mode) { demo_mode = p_demo_mode; }
bool HBGameNative::get_demo_mode() const { return demo_mode; }

Dictionary HBGameNative::get_serializable_types() const { return serializable_types; }
void HBGameNative::register_serializable_type(const String &p_name, const Variant &p_type) {
    serializable_types[p_name] = p_type;
}

HBGameNative *HBGameNative::get_singleton() {
    return singleton;
}

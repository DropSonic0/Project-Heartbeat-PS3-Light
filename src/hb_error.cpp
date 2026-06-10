#include "hb_error.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBErrorNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_error_type", "type"), &HBErrorNative::set_error_type);
    ClassDB::bind_method(D_METHOD("get_error_type"), &HBErrorNative::get_error_type);
    ClassDB::bind_method(D_METHOD("set_error_message", "message"), &HBErrorNative::set_error_message);
    ClassDB::bind_method(D_METHOD("get_error_message"), &HBErrorNative::get_error_message);
}

HBErrorNative::HBErrorNative() : error_type(0) {}
HBErrorNative::~HBErrorNative() {}

String HBErrorNative::to_string() const {
    return "HBError: " + error_message;
}

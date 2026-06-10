#include "hb_timing_point.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBTimingPointNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_time", "time"), &HBTimingPointNative::set_time);
    ClassDB::bind_method(D_METHOD("get_time"), &HBTimingPointNative::get_time);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "time"), "set_time", "get_time");
}

HBTimingPointNative::HBTimingPointNative() {
    serializable_fields.append("time");
}

HBTimingPointNative::~HBTimingPointNative() {}

void HBTimingPointNative::set_time(int64_t p_time) {
    time = p_time;
}

int64_t HBTimingPointNative::get_time() const {
    return time;
}

String HBTimingPointNative::get_serialized_type() const {
    return "TimingPoint";
}

#include "hb_timing_change.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBTimingChangeNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bpm", "bpm"), &HBTimingChangeNative::set_bpm);
    ClassDB::bind_method(D_METHOD("get_bpm"), &HBTimingChangeNative::get_bpm);
    ClassDB::bind_method(D_METHOD("set_time_signature", "time_signature"), &HBTimingChangeNative::set_time_signature);
    ClassDB::bind_method(D_METHOD("get_time_signature"), &HBTimingChangeNative::get_time_signature);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bpm"), "set_bpm", "get_bpm");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "time_signature"), "set_time_signature", "get_time_signature");
}

HBTimingChangeNative::HBTimingChangeNative() {
    serializable_fields.append("bpm");
    serializable_fields.append("time_signature");
    time_signature["numerator"] = 4;
    time_signature["denominator"] = 4;
}

HBTimingChangeNative::~HBTimingChangeNative() {}

void HBTimingChangeNative::set_bpm(double p_bpm) { bpm = p_bpm; }
double HBTimingChangeNative::get_bpm() const { return bpm; }
void HBTimingChangeNative::set_time_signature(Dictionary p_time_signature) { time_signature = p_time_signature; }
Dictionary HBTimingChangeNative::get_time_signature() const { return time_signature; }

String HBTimingChangeNative::get_serialized_type() const {
    return "TimingChange";
}

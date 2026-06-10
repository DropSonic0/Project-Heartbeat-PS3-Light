#include "hb_bpm_change.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBBPMChangeNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bpm", "bpm"), &HBBPMChangeNative::set_bpm);
    ClassDB::bind_method(D_METHOD("get_bpm"), &HBBPMChangeNative::get_bpm);
    ClassDB::bind_method(D_METHOD("set_speed_factor", "speed_factor"), &HBBPMChangeNative::set_speed_factor);
    ClassDB::bind_method(D_METHOD("get_speed_factor"), &HBBPMChangeNative::get_speed_factor);
    ClassDB::bind_method(D_METHOD("set_usage", "usage"), &HBBPMChangeNative::set_usage);
    ClassDB::bind_method(D_METHOD("get_usage"), &HBBPMChangeNative::get_usage);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bpm"), "set_bpm", "get_bpm");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed_factor"), "set_speed_factor", "get_speed_factor");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "usage", PROPERTY_HINT_ENUM, "FixedBPM,AutoBPM"), "set_usage", "get_usage");

    BIND_ENUM_CONSTANT(FIXED_BPM);
    BIND_ENUM_CONSTANT(AUTO_BPM);
}

HBBPMChangeNative::HBBPMChangeNative() {
    serializable_fields.append("bpm");
    serializable_fields.append("speed_factor");
    serializable_fields.append("usage");
}

HBBPMChangeNative::~HBBPMChangeNative() {}

void HBBPMChangeNative::set_bpm(double p_bpm) { bpm = p_bpm; }
double HBBPMChangeNative::get_bpm() const { return bpm; }
void HBBPMChangeNative::set_speed_factor(double p_speed_factor) { speed_factor = p_speed_factor; }
double HBBPMChangeNative::get_speed_factor() const { return speed_factor; }
void HBBPMChangeNative::set_usage(UsageTypes p_usage) { usage = p_usage; }
HBBPMChangeNative::UsageTypes HBBPMChangeNative::get_usage() const { return usage; }

String HBBPMChangeNative::get_serialized_type() const {
    return "BpmChange";
}

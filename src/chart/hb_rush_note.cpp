#include "hb_rush_note.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBRushNoteNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_end_time", "end_time"), &HBRushNoteNative::set_end_time);
    ClassDB::bind_method(D_METHOD("get_end_time"), &HBRushNoteNative::get_end_time);
    ClassDB::bind_method(D_METHOD("set_auto_rush_hit_cap", "auto_rush_hit_cap"), &HBRushNoteNative::set_auto_rush_hit_cap);
    ClassDB::bind_method(D_METHOD("get_auto_rush_hit_cap"), &HBRushNoteNative::get_auto_rush_hit_cap);
    ClassDB::bind_method(D_METHOD("set_custom_rush_hit_cap", "custom_rush_hit_cap"), &HBRushNoteNative::set_custom_rush_hit_cap);
    ClassDB::bind_method(D_METHOD("get_custom_rush_hit_cap"), &HBRushNoteNative::get_custom_rush_hit_cap);
    ClassDB::bind_method(D_METHOD("calculate_capped_hit_count"), &HBRushNoteNative::calculate_capped_hit_count);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "end_time"), "set_end_time", "get_end_time");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_rush_hit_cap"), "set_auto_rush_hit_cap", "get_auto_rush_hit_cap");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "custom_rush_hit_cap"), "set_custom_rush_hit_cap", "get_custom_rush_hit_cap");
}

HBRushNoteNative::HBRushNoteNative() {
    serializable_fields.append("end_time");
    serializable_fields.append("auto_rush_hit_cap");
    serializable_fields.append("custom_rush_hit_cap");
}

HBRushNoteNative::~HBRushNoteNative() {}

void HBRushNoteNative::set_end_time(int64_t p_end_time) { end_time = p_end_time; }
int64_t HBRushNoteNative::get_end_time() const { return end_time; }
void HBRushNoteNative::set_auto_rush_hit_cap(bool p_auto_rush_hit_cap) { auto_rush_hit_cap = p_auto_rush_hit_cap; }
bool HBRushNoteNative::get_auto_rush_hit_cap() const { return auto_rush_hit_cap; }
void HBRushNoteNative::set_custom_rush_hit_cap(int32_t p_custom_rush_hit_cap) { custom_rush_hit_cap = p_custom_rush_hit_cap; }
int32_t HBRushNoteNative::get_custom_rush_hit_cap() const { return custom_rush_hit_cap; }

int32_t HBRushNoteNative::calculate_capped_hit_count() const {
    if (!auto_rush_hit_cap) {
        return custom_rush_hit_cap;
    }
    int64_t duration_ms = end_time - get_time();
    return (int32_t)(duration_ms / 64);
}

String HBRushNoteNative::get_serialized_type() const {
    return "RushNote";
}

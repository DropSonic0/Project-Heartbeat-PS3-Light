#include "hb_duration_note_data.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBDurationNoteDataNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_duration", "duration"), &HBDurationNoteDataNative::set_duration);
    ClassDB::bind_method(D_METHOD("get_duration_val"), &HBDurationNoteDataNative::get_duration_val);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "duration"), "set_duration", "get_duration_val");
}

HBDurationNoteDataNative::HBDurationNoteDataNative() {
    serializable_fields.append("duration");
}

HBDurationNoteDataNative::~HBDurationNoteDataNative() {}

void HBDurationNoteDataNative::set_duration(int64_t p_duration) { duration = p_duration; }
int64_t HBDurationNoteDataNative::get_duration_val() const { return duration; }

String HBDurationNoteDataNative::get_serialized_type() const {
    return "DurationNote";
}

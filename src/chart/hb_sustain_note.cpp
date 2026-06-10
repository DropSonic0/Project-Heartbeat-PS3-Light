#include "hb_sustain_note.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBSustainNoteNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_end_time", "end_time"), &HBSustainNoteNative::set_end_time);
    ClassDB::bind_method(D_METHOD("get_end_time"), &HBSustainNoteNative::get_end_time);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "end_time"), "set_end_time", "get_end_time");
}

HBSustainNoteNative::HBSustainNoteNative() {
    serializable_fields.append("end_time");
}

HBSustainNoteNative::~HBSustainNoteNative() {}

void HBSustainNoteNative::set_end_time(int64_t p_end_time) { end_time = p_end_time; }
int64_t HBSustainNoteNative::get_end_time() const { return end_time; }

String HBSustainNoteNative::get_serialized_type() const {
    return "SustainNote";
}

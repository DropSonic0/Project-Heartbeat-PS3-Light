#include "hb_base_note.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBBaseNoteNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_position", "position"), &HBBaseNoteNative::set_position);
    ClassDB::bind_method(D_METHOD("get_position"), &HBBaseNoteNative::get_position);
    ClassDB::bind_method(D_METHOD("set_time_out", "time_out"), &HBBaseNoteNative::set_time_out);
    ClassDB::bind_method(D_METHOD("get_time_out_val"), &HBBaseNoteNative::get_time_out_val);
    ClassDB::bind_method(D_METHOD("set_auto_time_out", "auto_time_out"), &HBBaseNoteNative::set_auto_time_out);
    ClassDB::bind_method(D_METHOD("get_auto_time_out"), &HBBaseNoteNative::get_auto_time_out);
    ClassDB::bind_method(D_METHOD("set_entry_angle", "entry_angle"), &HBBaseNoteNative::set_entry_angle);
    ClassDB::bind_method(D_METHOD("get_entry_angle"), &HBBaseNoteNative::get_entry_angle);
    ClassDB::bind_method(D_METHOD("set_oscillation_amplitude", "oscillation_amplitude"), &HBBaseNoteNative::set_oscillation_amplitude);
    ClassDB::bind_method(D_METHOD("get_oscillation_amplitude"), &HBBaseNoteNative::get_oscillation_amplitude);
    ClassDB::bind_method(D_METHOD("set_oscillation_frequency", "oscillation_frequency"), &HBBaseNoteNative::set_oscillation_frequency);
    ClassDB::bind_method(D_METHOD("get_oscillation_frequency"), &HBBaseNoteNative::get_oscillation_frequency);
    ClassDB::bind_method(D_METHOD("set_distance", "distance"), &HBBaseNoteNative::set_distance);
    ClassDB::bind_method(D_METHOD("get_distance"), &HBBaseNoteNative::get_distance);
    ClassDB::bind_method(D_METHOD("set_pos_modified", "pos_modified"), &HBBaseNoteNative::set_pos_modified);
    ClassDB::bind_method(D_METHOD("get_pos_modified"), &HBBaseNoteNative::get_pos_modified);
    ClassDB::bind_method(D_METHOD("set_note_type", "note_type"), &HBBaseNoteNative::set_note_type);
    ClassDB::bind_method(D_METHOD("get_note_type"), &HBBaseNoteNative::get_note_type);
    ClassDB::bind_method(D_METHOD("get_time_out", "bpm"), &HBBaseNoteNative::get_time_out);
    ClassDB::bind_method(D_METHOD("get_score", "rating"), &HBBaseNoteNative::get_score);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "position"), "set_position", "get_position");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "time_out"), "set_time_out", "get_time_out_val");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_time_out"), "set_auto_time_out", "get_auto_time_out");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "entry_angle"), "set_entry_angle", "get_entry_angle");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "oscillation_amplitude"), "set_oscillation_amplitude", "get_oscillation_amplitude");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "oscillation_frequency"), "set_oscillation_frequency", "get_oscillation_frequency");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance"), "set_distance", "get_distance");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "pos_modified"), "set_pos_modified", "get_pos_modified");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "note_type", PROPERTY_HINT_ENUM, "Up,Left,Down,Right,SlideLeft,SlideRight,SlideChainPieceLeft,SlideChainPieceRight,Heart"), "set_note_type", "get_note_type");

    BIND_ENUM_CONSTANT(UP);
    BIND_ENUM_CONSTANT(LEFT);
    BIND_ENUM_CONSTANT(DOWN);
    BIND_ENUM_CONSTANT(RIGHT);
    BIND_ENUM_CONSTANT(SLIDE_LEFT);
    BIND_ENUM_CONSTANT(SLIDE_RIGHT);
    BIND_ENUM_CONSTANT(SLIDE_CHAIN_PIECE_LEFT);
    BIND_ENUM_CONSTANT(SLIDE_CHAIN_PIECE_RIGHT);
    BIND_ENUM_CONSTANT(HEART);
}

HBBaseNoteNative::HBBaseNoteNative() {
    serializable_fields.append("position");
    serializable_fields.append("pos_modified");
    serializable_fields.append("distance");
    serializable_fields.append("auto_time_out");
    serializable_fields.append("time_out");
    serializable_fields.append("note_type");
    serializable_fields.append("entry_angle");
    serializable_fields.append("oscillation_amplitude");
    serializable_fields.append("oscillation_frequency");
}

HBBaseNoteNative::~HBBaseNoteNative() {}

void HBBaseNoteNative::set_position(Vector2 p_position) { position = p_position; }
Vector2 HBBaseNoteNative::get_position() const { return position; }
void HBBaseNoteNative::set_time_out(int64_t p_time_out) { time_out = p_time_out; }
int64_t HBBaseNoteNative::get_time_out_val() const { return time_out; }
void HBBaseNoteNative::set_auto_time_out(bool p_auto_time_out) { auto_time_out = p_auto_time_out; }
bool HBBaseNoteNative::get_auto_time_out() const { return auto_time_out; }
void HBBaseNoteNative::set_entry_angle(float p_entry_angle) { entry_angle = p_entry_angle; }
float HBBaseNoteNative::get_entry_angle() const { return entry_angle; }
void HBBaseNoteNative::set_oscillation_amplitude(float p_oscillation_amplitude) { oscillation_amplitude = p_oscillation_amplitude; }
float HBBaseNoteNative::get_oscillation_amplitude() const { return oscillation_amplitude; }
void HBBaseNoteNative::set_oscillation_frequency(int32_t p_oscillation_frequency) { oscillation_frequency = p_oscillation_frequency; }
int32_t HBBaseNoteNative::get_oscillation_frequency() const { return oscillation_frequency; }
void HBBaseNoteNative::set_distance(float p_distance) { distance = p_distance; }
float HBBaseNoteNative::get_distance() const { return distance; }
void HBBaseNoteNative::set_pos_modified(bool p_pos_modified) { pos_modified = p_pos_modified; }
bool HBBaseNoteNative::get_pos_modified() const { return pos_modified; }
void HBBaseNoteNative::set_note_type(NoteType p_note_type) { note_type = p_note_type; }
HBBaseNoteNative::NoteType HBBaseNoteNative::get_note_type() const { return note_type; }

int64_t HBBaseNoteNative::get_time_out(double p_bpm) const {
    if (auto_time_out) {
        if (p_bpm == 0) {
            return 9223372036854775807LL;
        }
        return (int64_t)((60.0 / p_bpm * (1 + 3) * 1000.0));
    } else {
        return time_out;
    }
}

int64_t HBBaseNoteNative::get_score(int p_rating) const {
    // Corrected score mapping: 0=WORST, 1=SAD, 2=SAFE, 3=FINE, 4=COOL
    switch (p_rating) {
        case 4: return 1000; // COOL
        case 3: return 800;  // FINE
        case 2: return 500;  // SAFE
        case 1: return 100;  // SAD
        case 0: return 0;    // WORST
        default: return 0;
    }
}

String HBBaseNoteNative::get_serialized_type() const {
    return "BaseNote";
}

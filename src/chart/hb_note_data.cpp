#include "hb_note_data.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBNoteDataNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_hold", "hold"), &HBNoteDataNative::set_hold);
    ClassDB::bind_method(D_METHOD("get_hold"), &HBNoteDataNative::get_hold);
    ClassDB::bind_method(D_METHOD("is_slide_note"), &HBNoteDataNative::is_slide_note);
    ClassDB::bind_method(D_METHOD("is_slide_hold_piece"), &HBNoteDataNative::is_slide_hold_piece);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hold"), "set_hold", "get_hold");
}

HBNoteDataNative::HBNoteDataNative() {
    serializable_fields.append("hold");
}

HBNoteDataNative::~HBNoteDataNative() {}

void HBNoteDataNative::set_hold(bool p_hold) { hold = p_hold; }
bool HBNoteDataNative::get_hold() const { return hold; }

bool HBNoteDataNative::is_slide_note() const {
    NoteType type = get_note_type();
    return type == SLIDE_LEFT || type == SLIDE_RIGHT;
}

bool HBNoteDataNative::is_slide_hold_piece() const {
    NoteType type = get_note_type();
    return type == SLIDE_CHAIN_PIECE_LEFT || type == SLIDE_CHAIN_PIECE_RIGHT;
}

HBBaseNoteNative::NoteType HBNoteDataNative::get_chain_type() const {
    return get_note_type() == SLIDE_LEFT ? SLIDE_CHAIN_PIECE_LEFT : SLIDE_CHAIN_PIECE_RIGHT;
}

String HBNoteDataNative::get_serialized_type() const {
    return "Note";
}

#ifndef HB_NOTE_DATA_HPP
#define HB_NOTE_DATA_HPP

#include "hb_base_note.hpp"

namespace godot {

class HBNoteDataNative : public HBBaseNoteNative {
    GDCLASS(HBNoteDataNative, HBBaseNoteNative);

protected:
    static void _bind_methods();

private:
    bool hold = false;

public:
    HBNoteDataNative();
    ~HBNoteDataNative();

    void set_hold(bool p_hold);
    bool get_hold() const;

    bool is_slide_note() const;
    bool is_slide_hold_piece() const;
    HBBaseNoteNative::NoteType get_chain_type() const;

    virtual String get_serialized_type() const GD_OVERRIDE;
};

} // namespace godot

#endif // HB_NOTE_DATA_HPP

#ifndef HB_DOUBLE_NOTE_HPP
#define HB_DOUBLE_NOTE_HPP

#include "hb_base_note.hpp"

namespace godot {

class HBDoubleNoteNative : public HBBaseNoteNative {
    GDCLASS(HBDoubleNoteNative, HBBaseNoteNative);

protected:
    static void _bind_methods();

public:
    HBDoubleNoteNative();
    ~HBDoubleNoteNative();

    virtual String get_serialized_type() const override;
};

} // namespace godot

#endif // HB_DOUBLE_NOTE_HPP

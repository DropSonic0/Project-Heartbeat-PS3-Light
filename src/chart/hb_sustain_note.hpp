#ifndef HB_SUSTAIN_NOTE_HPP
#define HB_SUSTAIN_NOTE_HPP

#include "hb_base_note.hpp"

namespace godot {

class HBSustainNoteNative : public HBBaseNoteNative {
    GDCLASS(HBSustainNoteNative, HBBaseNoteNative);

protected:
    static void _bind_methods();

private:
    int64_t end_time = 1000;

public:
    HBSustainNoteNative();
    ~HBSustainNoteNative();

    void set_end_time(int64_t p_end_time);
    int64_t get_end_time() const;

    virtual String get_serialized_type() const override;
};

} // namespace godot

#endif // HB_SUSTAIN_NOTE_HPP

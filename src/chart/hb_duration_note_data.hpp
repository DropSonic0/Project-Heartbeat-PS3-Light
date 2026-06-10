#ifndef HB_DURATION_NOTE_DATA_HPP
#define HB_DURATION_NOTE_DATA_HPP

#include "hb_note_data.hpp"

namespace godot {

class HBDurationNoteDataNative : public HBNoteDataNative {
    GDCLASS(HBDurationNoteDataNative, HBNoteDataNative);

protected:
    static void _bind_methods();

private:
    int64_t duration = 3000;

public:
    HBDurationNoteDataNative();
    ~HBDurationNoteDataNative();

    void set_duration(int64_t p_duration);
    int64_t get_duration_val() const;

    virtual String get_serialized_type() const GD_OVERRIDE;
};

} // namespace godot

#endif // HB_DURATION_NOTE_DATA_HPP

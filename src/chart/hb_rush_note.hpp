#ifndef HB_RUSH_NOTE_HPP
#define HB_RUSH_NOTE_HPP

#include "hb_base_note.hpp"

namespace godot {

class HBRushNoteNative : public HBBaseNoteNative {
    GDCLASS(HBRushNoteNative, HBBaseNoteNative);

protected:
    static void _bind_methods();

private:
    int64_t end_time = 0;
    bool auto_rush_hit_cap = true;
    int32_t custom_rush_hit_cap = 32;

public:
    HBRushNoteNative();
    ~HBRushNoteNative();

    void set_end_time(int64_t p_end_time);
    int64_t get_end_time() const;

    void set_auto_rush_hit_cap(bool p_auto_rush_hit_cap);
    bool get_auto_rush_hit_cap() const;

    void set_custom_rush_hit_cap(int32_t p_custom_rush_hit_cap);
    int32_t get_custom_rush_hit_cap() const;

    int32_t calculate_capped_hit_count() const;

    virtual String get_serialized_type() const override;
};

} // namespace godot

#endif // HB_RUSH_NOTE_HPP

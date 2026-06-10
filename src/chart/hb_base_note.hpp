#ifndef HB_BASE_NOTE_HPP
#define HB_BASE_NOTE_HPP

#include "hb_timing_point.hpp"
#include <godot_cpp/variant/vector2.hpp>

namespace godot {

class HBBaseNoteNative : public HBTimingPointNative {
    GDCLASS(HBBaseNoteNative, HBTimingPointNative);

protected:
    static void _bind_methods();

public:
    enum NoteType {
        UP,
        LEFT,
        DOWN,
        RIGHT,
        SLIDE_LEFT,
        SLIDE_RIGHT,
        SLIDE_CHAIN_PIECE_LEFT,
        SLIDE_CHAIN_PIECE_RIGHT,
        HEART
    };

private:
    Vector2 position = Vector2(960, 540);
    int64_t time_out = 1400;
    bool auto_time_out = true;
    float entry_angle = 0.0f;
    float oscillation_amplitude = 500.0f;
    int32_t oscillation_frequency = 2;
    float distance = 1200.0f;
    bool pos_modified = false;
    NoteType note_type = RIGHT;

public:
    HBBaseNoteNative();
    ~HBBaseNoteNative();

    void set_position(Vector2 p_position);
    Vector2 get_position() const;

    void set_time_out(int64_t p_time_out);
    int64_t get_time_out_val() const;

    void set_auto_time_out(bool p_auto_time_out);
    bool get_auto_time_out() const;

    void set_entry_angle(float p_entry_angle);
    float get_entry_angle() const;

    void set_oscillation_amplitude(float p_oscillation_amplitude);
    float get_oscillation_amplitude() const;

    void set_oscillation_frequency(int32_t p_oscillation_frequency);
    int32_t get_oscillation_frequency() const;

    void set_distance(float p_distance);
    float get_distance() const;

    void set_pos_modified(bool p_pos_modified);
    bool get_pos_modified() const;

    void set_note_type(NoteType p_note_type);
    NoteType get_note_type() const;

    int64_t get_time_out(double p_bpm) const;
    int64_t get_score(int p_rating) const;

    virtual String get_serialized_type() const override;
};

} // namespace godot

VARIANT_ENUM_CAST(HBBaseNoteNative::NoteType);

#endif // HB_BASE_NOTE_HPP

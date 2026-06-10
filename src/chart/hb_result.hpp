#ifndef HB_RESULT_HPP
#define HB_RESULT_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBResultNative : public HBSerializableNative {
    GDCLASS(HBResultNative, HBSerializableNative);

protected:
    static void _bind_methods();

public:
    enum ResultRating {
        FAIL,
        CHEAP,
        STANDARD,
        GREAT,
        EXCELLENT,
        PERFECT
    };

private:
    int64_t score = 0;
    int64_t hold_bonus = 0;
    int64_t slide_bonus = 0;
    int64_t rush_bonus = 0;
    int32_t rush_hits = 0;
    int32_t max_combo = 0;
    int32_t total_notes = 0;
    int32_t notes_hit = 0;
    int64_t max_score = 1;
    bool failed = false;
    bool used_cheats = false;
    Dictionary note_ratings;
    Dictionary wrong_note_ratings;

public:
    HBResultNative();
    ~HBResultNative();

    double get_percentage() const;
    int64_t get_capped_score() const;
    int64_t get_base_score() const;
    int get_result_rating() const;

    // Getters and Setters
    void set_score(int64_t p_val) { score = p_val; }
    int64_t get_score() const { return score; }
    void set_hold_bonus(int64_t p_val) { hold_bonus = p_val; }
    int64_t get_hold_bonus() const { return hold_bonus; }
    void set_slide_bonus(int64_t p_val) { slide_bonus = p_val; }
    int64_t get_slide_bonus() const { return slide_bonus; }
    void set_rush_bonus(int64_t p_val) { rush_bonus = p_val; }
    int64_t get_rush_bonus() const { return rush_bonus; }
    void set_max_score(int64_t p_val) { max_score = p_val; }
    int64_t get_max_score() const { return max_score; }
    void set_failed(bool p_val) { failed = p_val; }
    bool get_failed() const { return failed; }
    void set_note_ratings(Dictionary p_val) { note_ratings = p_val; }
    Dictionary get_note_ratings() const { return note_ratings; }
    void set_wrong_note_ratings(Dictionary p_val) { wrong_note_ratings = p_val; }
    Dictionary get_wrong_note_ratings() const { return wrong_note_ratings; }

    virtual String get_serialized_type() const GD_OVERRIDE { return "Result"; }
};

} // namespace godot

VARIANT_ENUM_CAST(HBResultNative::ResultRating);

#endif // HB_RESULT_HPP

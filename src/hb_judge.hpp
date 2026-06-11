#ifndef HB_JUDGE_HPP
#define HB_JUDGE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class HBJudgeNative : public RefCounted {
    GDCLASS(HBJudgeNative, RefCounted);

protected:
    static void _bind_methods();

public:
    enum JudgeRatings {
        WORST,
        SAD,
        SAFE,
        FINE,
        COOL
    };

private:
    double timing_window_scale = 1.0;

public:
    HBJudgeNative();
    ~HBJudgeNative();

    int judge_note(int64_t p_hit_time_msec, int64_t p_target_note_time_msec);
    int judge_note_usec(int64_t p_hit_time_usec, int64_t p_target_note_time_usec);

    int64_t get_target_window_msec() const;
    int64_t get_target_window_usec() const;
    int64_t get_window_for_rating(int p_rating) const;

    void set_timing_window_scale(double p_scale) { timing_window_scale = p_scale; }
    double get_timing_window_scale() const { return timing_window_scale; }

    VARIANT_ENUM_CAST(JudgeRatings);
};

} // namespace godot

#endif // HB_JUDGE_HPP

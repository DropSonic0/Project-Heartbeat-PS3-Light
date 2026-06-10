#include "hb_judge.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace godot;

void HBJudgeNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("judge_note", "hit_time_msec", "target_note_time"), &HBJudgeNative::judge_note);
    ClassDB::bind_method(D_METHOD("judge_note_usec", "hit_time_usec", "target_note_time_usec"), &HBJudgeNative::judge_note_usec);
    ClassDB::bind_method(D_METHOD("get_target_window_msec"), &HBJudgeNative::get_target_window_msec);
    ClassDB::bind_method(D_METHOD("get_window_for_rating", "rating"), &HBJudgeNative::get_window_for_rating);
    ClassDB::bind_method(D_METHOD("set_timing_window_scale", "scale"), &HBJudgeNative::set_timing_window_scale);
    ClassDB::bind_method(D_METHOD("get_timing_window_scale"), &HBJudgeNative::get_timing_window_scale);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "timing_window_scale"), "set_timing_window_scale", "get_timing_window_scale");

    BIND_ENUM_CONSTANT(WORST);
    BIND_ENUM_CONSTANT(SAD);
    BIND_ENUM_CONSTANT(SAFE);
    BIND_ENUM_CONSTANT(FINE);
    BIND_ENUM_CONSTANT(COOL);
}

struct RatingWindow {
    int window;
    HBJudgeNative::JudgeRatings rating;
};

static const std::vector<RatingWindow> RATING_WINDOWS = {
    {32, HBJudgeNative::COOL},
    {64, HBJudgeNative::FINE},
    {96, HBJudgeNative::SAFE},
    {128, HBJudgeNative::SAD}
};

HBJudgeNative::HBJudgeNative() {}
HBJudgeNative::~HBJudgeNative() {}

int HBJudgeNative::judge_note(int64_t p_hit_time_msec, int64_t p_target_note_time_msec) {
    int64_t diff = p_hit_time_msec - p_target_note_time_msec;
    int64_t target_window = get_target_window_msec();

    if (diff < -target_window) {
        return -1;
    }

    if (diff >= target_window) {
        return WORST;
    }

    int64_t abs_diff = std::abs(diff);
    for (const auto& rw : RATING_WINDOWS) {
        if (abs_diff <= (int64_t)(rw.window * timing_window_scale)) {
            return rw.rating;
        }
    }

    return WORST;
}

int HBJudgeNative::judge_note_usec(int64_t p_hit_time_usec, int64_t p_target_note_time_usec) {
    int64_t diff = p_hit_time_usec - p_target_note_time_usec;
    int64_t target_window = get_target_window_usec();

    if (diff < -target_window) {
        return -1;
    }

    if (diff >= target_window) {
        return WORST;
    }

    int64_t abs_diff = std::abs(diff);
    for (const auto& rw : RATING_WINDOWS) {
        if (abs_diff <= (int64_t)(rw.window * 1000 * timing_window_scale)) {
            return rw.rating;
        }
    }

    return WORST;
}

int64_t HBJudgeNative::get_target_window_msec() const {
    return (int64_t)(128 * timing_window_scale);
}

int64_t HBJudgeNative::get_target_window_usec() const {
    return (int64_t)(128000 * timing_window_scale);
}

int64_t HBJudgeNative::get_window_for_rating(int p_rating) const {
    for (const auto& rw : RATING_WINDOWS) {
        if (rw.rating == p_rating) {
            return (int64_t)(rw.window * timing_window_scale);
        }
    }
    return 0;
}

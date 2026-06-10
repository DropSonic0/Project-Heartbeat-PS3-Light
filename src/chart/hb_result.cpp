#include "hb_result.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

using namespace godot;

void HBResultNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_percentage"), &HBResultNative::get_percentage);
    ClassDB::bind_method(D_METHOD("get_capped_score"), &HBResultNative::get_capped_score);
    ClassDB::bind_method(D_METHOD("get_base_score"), &HBResultNative::get_base_score);
    ClassDB::bind_method(D_METHOD("get_result_rating"), &HBResultNative::get_result_rating);

    ClassDB::bind_method(D_METHOD("set_score", "score"), &HBResultNative::set_score);
    ClassDB::bind_method(D_METHOD("get_score"), &HBResultNative::get_score);
    // ... add more as needed

    ADD_PROPERTY(PropertyInfo(Variant::INT, "score"), "set_score", "get_score");

    BIND_ENUM_CONSTANT(FAIL);
    BIND_ENUM_CONSTANT(CHEAP);
    BIND_ENUM_CONSTANT(STANDARD);
    BIND_ENUM_CONSTANT(GREAT);
    BIND_ENUM_CONSTANT(EXCELLENT);
    BIND_ENUM_CONSTANT(PERFECT);
}

HBResultNative::HBResultNative() {
    serializable_fields.append("score");
    serializable_fields.append("max_combo");
    serializable_fields.append("total_notes");
    serializable_fields.append("notes_hit");
    serializable_fields.append("note_ratings");
    serializable_fields.append("wrong_note_ratings");
    serializable_fields.append("failed");
    serializable_fields.append("max_score");
    serializable_fields.append("hold_bonus");
    serializable_fields.append("slide_bonus");
    serializable_fields.append("rush_bonus");
    serializable_fields.append("rush_hits");
    serializable_fields.append("used_cheats");

    for (int i=0; i<5; i++) {
        note_ratings[i] = 0;
        wrong_note_ratings[i] = 0;
    }
}

HBResultNative::~HBResultNative() {}

double HBResultNative::get_percentage() const {
    return (double)get_capped_score() / (double)max_score;
}

int64_t HBResultNative::get_capped_score() const {
    int64_t base = get_base_score();
    int64_t capped_hold = std::clamp(hold_bonus, (int64_t)0, (int64_t)(0.075 * max_score));
    int64_t capped_slide = std::clamp(slide_bonus, (int64_t)0, (int64_t)(0.075 * max_score));
    return base + capped_hold + capped_slide;
}

int64_t HBResultNative::get_base_score() const {
    return score - hold_bonus - slide_bonus - rush_bonus;
}

int HBResultNative::get_result_rating() const {
    if (failed) return FAIL;

    int64_t failures = (int64_t)note_ratings[1] + (int64_t)note_ratings[2] + (int64_t)note_ratings[0];
    Array wrong_keys = wrong_note_ratings.keys();
    for (int i=0; i<wrong_keys.size(); i++) {
        failures += (int64_t)wrong_note_ratings[wrong_keys[i]];
    }

    if (failures <= 0) return PERFECT;

    double percentage = get_percentage();
    if (percentage >= 0.95) return EXCELLENT;
    if (percentage >= 0.90) return GREAT;
    if (percentage >= 0.75) return STANDARD;
    return CHEAP;
}

#include "hb_history_entry.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

using namespace godot;

void HBHistoryEntryNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_result_better", "result"), &HBHistoryEntryNative::is_result_better);
    ClassDB::bind_method(D_METHOD("update_entry", "game_info"), &HBHistoryEntryNative::update_entry);
}

HBHistoryEntryNative::HBHistoryEntryNative() {
    highest_score_info.instantiate();
    serializable_fields.append("highest_score_info");
    serializable_fields.append("highest_score");
    serializable_fields.append("highest_percentage");
    serializable_fields.append("highest_rating");
}

HBHistoryEntryNative::~HBHistoryEntryNative() {}

bool HBHistoryEntryNative::is_result_better(Ref<HBResultNative> p_result) {
    if (p_result.is_null()) return false;
    return p_result->get_score() > highest_score ||
           p_result->get_result_rating() > highest_rating ||
           p_result->get_percentage() > highest_percentage;
}

void HBHistoryEntryNative::update_entry(Ref<HBGameInfoNative> p_game_info) {
    if (p_game_info.is_null()) return;
    Ref<HBResultNative> res = p_game_info->get_result();
    if (res.is_null()) return;

    if (res->get_score() > highest_score) {
        // Simple assignment for now, clone logic would be better
        highest_score_info = p_game_info;
        highest_score = res->get_score();
    }
    highest_percentage = std::max(highest_percentage, res->get_percentage());
    highest_rating = std::max(highest_rating, res->get_result_rating());
}

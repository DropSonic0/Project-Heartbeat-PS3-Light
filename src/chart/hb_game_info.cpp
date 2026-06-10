#include "hb_game_info.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/time.hpp>

using namespace godot;

void HBGameInfoNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_game_session_type", "type"), &HBGameInfoNative::set_game_session_type);
    ClassDB::bind_method(D_METHOD("get_game_session_type"), &HBGameInfoNative::get_game_session_type);
    ClassDB::bind_method(D_METHOD("is_leaderboard_legal"), &HBGameInfoNative::is_leaderboard_legal);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "game_session_type"), "set_game_session_type", "get_game_session_type");

    BIND_ENUM_CONSTANT(OFFLINE);
    BIND_ENUM_CONSTANT(MULTIPLAYER);
}

HBGameInfoNative::HBGameInfoNative() {
    time = Time::get_singleton()->get_unix_time_from_system();
    result.instantiate();

    serializable_fields.append("game_session_type");
    serializable_fields.append("time");
    serializable_fields.append("result");
    serializable_fields.append("modifiers");
    serializable_fields.append("used_autoplay");
    serializable_fields.append("song_id");
    serializable_fields.append("difficulty");
}

HBGameInfoNative::~HBGameInfoNative() {}

bool HBGameInfoNative::is_leaderboard_legal() const {
    return modifiers.is_empty() && result.is_valid() && result->get_result_rating() != HBResultNative::FAIL;
}

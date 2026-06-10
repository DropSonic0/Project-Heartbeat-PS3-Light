#include "hb_song_stats.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBSongStatsNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_times_played", "times_played"), &HBSongStatsNative::set_times_played);
    ClassDB::bind_method(D_METHOD("get_times_played"), &HBSongStatsNative::get_times_played);
    ClassDB::bind_method(D_METHOD("set_selected_variant", "variant"), &HBSongStatsNative::set_selected_variant);
    ClassDB::bind_method(D_METHOD("get_selected_variant"), &HBSongStatsNative::get_selected_variant);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "times_played"), "set_times_played", "get_times_played");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "selected_variant"), "set_selected_variant", "get_selected_variant");
}

HBSongStatsNative::HBSongStatsNative() {
    serializable_fields.append("times_played");
    serializable_fields.append("selected_variant");
}

HBSongStatsNative::~HBSongStatsNative() {}

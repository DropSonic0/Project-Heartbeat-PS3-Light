#include "hb_per_song_settings.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBPerSongSettingsNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_lag_compensation", "lag_compensation"), &HBPerSongSettingsNative::set_lag_compensation);
    ClassDB::bind_method(D_METHOD("get_lag_compensation"), &HBPerSongSettingsNative::get_lag_compensation);
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &HBPerSongSettingsNative::set_volume);
    ClassDB::bind_method(D_METHOD("get_volume"), &HBPerSongSettingsNative::get_volume);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "lag_compensation"), "set_lag_compensation", "get_lag_compensation");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
}

HBPerSongSettingsNative::HBPerSongSettingsNative() {
    serializable_fields.append("lag_compensation");
    serializable_fields.append("volume");
    serializable_fields.append("video_enabled");
    serializable_fields.append("use_song_skin");
    serializable_fields.append("vocals_enabled");
}

HBPerSongSettingsNative::~HBPerSongSettingsNative() {}

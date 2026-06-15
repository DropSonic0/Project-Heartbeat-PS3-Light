#include "hb_shinobu_playback_offset.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void ShinobuGodotSoundPlaybackOffsetNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &ShinobuGodotSoundPlaybackOffsetNative::set_volume);
    ClassDB::bind_method(D_METHOD("get_volume"), &ShinobuGodotSoundPlaybackOffsetNative::get_volume);
    ClassDB::bind_method(D_METHOD("set_pitch_scale", "pitch_scale"), &ShinobuGodotSoundPlaybackOffsetNative::set_pitch_scale);
    ClassDB::bind_method(D_METHOD("get_pitch_scale"), &ShinobuGodotSoundPlaybackOffsetNative::get_pitch_scale);
    ClassDB::bind_method(D_METHOD("set_offset", "offset"), &ShinobuGodotSoundPlaybackOffsetNative::set_offset);
    ClassDB::bind_method(D_METHOD("get_offset"), &ShinobuGodotSoundPlaybackOffsetNative::get_offset);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch_scale"), "set_pitch_scale", "get_pitch_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "offset"), "set_offset", "get_offset");
}

ShinobuGodotSoundPlaybackOffsetNative::ShinobuGodotSoundPlaybackOffsetNative() {}
ShinobuGodotSoundPlaybackOffsetNative::~ShinobuGodotSoundPlaybackOffsetNative() {}

void ShinobuGodotSoundPlaybackOffsetNative::set_volume(float p_volume) { volume = p_volume; }
float ShinobuGodotSoundPlaybackOffsetNative::get_volume() const { return volume; }

void ShinobuGodotSoundPlaybackOffsetNative::set_pitch_scale(float p_pitch_scale) { pitch_scale = p_pitch_scale; }
float ShinobuGodotSoundPlaybackOffsetNative::get_pitch_scale() const { return pitch_scale; }

void ShinobuGodotSoundPlaybackOffsetNative::set_offset(float p_offset) { offset = p_offset; }
float ShinobuGodotSoundPlaybackOffsetNative::get_offset() const { return offset; }

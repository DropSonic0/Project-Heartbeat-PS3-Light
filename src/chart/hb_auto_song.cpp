#include "hb_auto_song.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBAutoSongNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("generate_chart"), &HBAutoSongNative::generate_chart);
}

HBAutoSongNative::HBAutoSongNative() {
    loudness = 0.0;
    offset = 0;
}
HBAutoSongNative::~HBAutoSongNative() {}

void HBAutoSongNative::generate_chart() {
    chart.instantiate();
    // Logic for generating chart pattern...
}

String HBAutoSongNative::get_serialized_type() const {
    return "AutoSong";
}

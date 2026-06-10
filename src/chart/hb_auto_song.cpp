#include "hb_auto_song.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBAutoSongNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("generate_chart"), &HBAutoSongNative::generate_chart);
}

HBAutoSongNative::HBAutoSongNative() {}
HBAutoSongNative::~HBAutoSongNative() {}

void HBAutoSongNative::generate_chart() {
    chart.instantiate();
    // Logic for generating chart pattern...
}

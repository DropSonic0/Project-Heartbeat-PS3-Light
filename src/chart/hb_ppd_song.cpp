#include "hb_ppd_song.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBPPDSongNative::_bind_methods() {}

HBPPDSongNative::HBPPDSongNative() {
    serializable_fields.append("ppd_offset");
    serializable_fields.append("guid");
    serializable_fields.append("ppd_website_id");
    serializable_fields.append("uses_native_video");
}

HBPPDSongNative::~HBPPDSongNative() {}

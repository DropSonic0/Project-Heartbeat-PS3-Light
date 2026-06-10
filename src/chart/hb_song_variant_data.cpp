#include "hb_song_variant_data.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBSongVariantDataNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_variant_name", "variant_name"), &HBSongVariantDataNative::set_variant_name);
    ClassDB::bind_method(D_METHOD("get_variant_name"), &HBSongVariantDataNative::get_variant_name);
    ClassDB::bind_method(D_METHOD("set_variant_url", "variant_url"), &HBSongVariantDataNative::set_variant_url);
    ClassDB::bind_method(D_METHOD("get_variant_url"), &HBSongVariantDataNative::get_variant_url);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "variant_name"), "set_variant_name", "get_variant_name");
}

HBSongVariantDataNative::HBSongVariantDataNative() {
    serializable_fields.append("variant_name");
    serializable_fields.append("variant_url");
    serializable_fields.append("variant_audio");
    serializable_fields.append("variant_video");
    serializable_fields.append("variant_offset");
    serializable_fields.append("variant_normalization");
    serializable_fields.append("audio_only");
}

HBSongVariantDataNative::~HBSongVariantDataNative() {}

#include "hb_song_verification.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void HBSongVerificationNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("verify_song", "song"), &HBSongVerificationNative::verify_song);
    ClassDB::bind_method(D_METHOD("has_fatal_error", "errors", "count_ugc"), &HBSongVerificationNative::has_fatal_error, DEFVAL(false));

    BIND_ENUM_CONSTANT(CHART_OK);
    BIND_ENUM_CONSTANT(CHART_FILE_NOT_FOUND);
    BIND_ENUM_CONSTANT(CHART_FILE_INVALID_JSON);
    BIND_ENUM_CONSTANT(CHART_HOLD_CHAIN_PIECE_WITHOUT_PARENT);
    BIND_ENUM_CONSTANT(CHART_AUDIO_NOT_DOWNLOADED);

    BIND_ENUM_CONSTANT(META_YOUTUBE_URL_INVALID);
    BIND_ENUM_CONSTANT(META_MANDATORY_FIELD_MISSING);
    BIND_ENUM_CONSTANT(META_AUDIO_FIELD_MISSING);
    BIND_ENUM_CONSTANT(META_AUDIO_NOT_FOUND);
    BIND_ENUM_CONSTANT(META_VOICE_NOT_FOUND);
    BIND_ENUM_CONSTANT(META_PREVIEW_MISSING);
    BIND_ENUM_CONSTANT(META_PREVIEW_FILE_MISSING);
    BIND_ENUM_CONSTANT(META_PREVIEW_FILE_TOO_BIG);
    BIND_ENUM_CONSTANT(META_ILLEGAL_FILES);
    BIND_ENUM_CONSTANT(META_ILLEGAL_FOLDERS);
}

HBSongVerificationNative::HBSongVerificationNative() {}
HBSongVerificationNative::~HBSongVerificationNative() {}

Dictionary HBSongVerificationNative::verify_song(Ref<HBSongNative> p_song) {
    Dictionary errors;
    // Logic for verifying song...
    return errors;
}

bool HBSongVerificationNative::has_fatal_error(Dictionary p_errors, bool p_count_ugc) {
    // Logic for checking fatal errors...
    return false;
}

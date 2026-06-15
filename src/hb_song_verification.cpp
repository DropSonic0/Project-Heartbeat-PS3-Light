#include "hb_song_verification.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/file_access.hpp>

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
    if (p_song.is_null()) return errors;

    Array meta_errors;
    static const char* mandatory_fields[] = {"title", "artist", "creator", "preview_image"};
    for (int i = 0; i < 4; i++) {
        String field = mandatory_fields[i];
        String val = p_song->get(field);
        if (val.strip_edges().is_empty()) {
            Dictionary err;
            err["type"] = META_MANDATORY_FIELD_MISSING;
            err["string"] = "The song is missing the " + field + " field";
            err["fatal"] = false;
            err["warning"] = true;
            err["fatal_ugc"] = true;
            meta_errors.append(err);
        }
    }
    errors["meta"] = meta_errors;
    errors["audio"] = Array();

    Dictionary charts = p_song->get_charts();
    Array chart_names = charts.keys();
    for (int i = 0; i < chart_names.size(); i++) {
        String chart_name = chart_names[i];
        Array chart_errors;
        
        String chart_path = p_song->get_path() + "/" + (String)((Dictionary)charts[chart_name])["file"];
        if (!FileAccess::file_exists(chart_path)) {
            Dictionary err;
            err["type"] = CHART_FILE_NOT_FOUND;
            err["string"] = "Couldn't find this chart's file";
            err["fatal"] = true;
            err["warning"] = false;
            err["fatal_ugc"] = false;
            chart_errors.append(err);
        }
        
        errors["chart_" + chart_name] = chart_errors;
    }

    return errors;
}

bool HBSongVerificationNative::has_fatal_error(Dictionary p_errors, bool p_count_ugc) {
    Array keys = p_errors.keys();
    for (int i = 0; i < keys.size(); i++) {
        Array error_list = p_errors[keys[i]];
        for (int j = 0; j < error_list.size(); j++) {
            Dictionary err = error_list[j];
            if ((bool)err["fatal"] || (p_count_ugc && (bool)err["fatal_ugc"])) {
                return true;
            }
        }
    }
    return false;
}

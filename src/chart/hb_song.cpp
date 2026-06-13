#include "hb_song.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void HBSongNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_title", "title"), &HBSongNative::set_title);
    ClassDB::bind_method(D_METHOD("get_title"), &HBSongNative::get_title);
    ClassDB::bind_method(D_METHOD("set_romanized_title", "romanized_title"), &HBSongNative::set_romanized_title);
    ClassDB::bind_method(D_METHOD("get_romanized_title"), &HBSongNative::get_romanized_title);
    ClassDB::bind_method(D_METHOD("set_artist", "artist"), &HBSongNative::set_artist);
    ClassDB::bind_method(D_METHOD("get_artist"), &HBSongNative::get_artist);
    ClassDB::bind_method(D_METHOD("set_artist_alias", "artist_alias"), &HBSongNative::set_artist_alias);
    ClassDB::bind_method(D_METHOD("get_artist_alias"), &HBSongNative::get_artist_alias);
    ClassDB::bind_method(D_METHOD("set_composers", "composers"), &HBSongNative::set_composers);
    ClassDB::bind_method(D_METHOD("get_composers"), &HBSongNative::get_composers);
    ClassDB::bind_method(D_METHOD("set_writers", "writers"), &HBSongNative::set_writers);
    ClassDB::bind_method(D_METHOD("get_writers"), &HBSongNative::get_writers);
    ClassDB::bind_method(D_METHOD("set_vocals", "vocals"), &HBSongNative::set_vocals);
    ClassDB::bind_method(D_METHOD("get_vocals"), &HBSongNative::get_vocals);
    ClassDB::bind_method(D_METHOD("set_audio", "audio"), &HBSongNative::set_audio);
    ClassDB::bind_method(D_METHOD("get_audio"), &HBSongNative::get_audio);
    ClassDB::bind_method(D_METHOD("set_video", "video"), &HBSongNative::set_video);
    ClassDB::bind_method(D_METHOD("get_video"), &HBSongNative::get_video);
    ClassDB::bind_method(D_METHOD("set_voice", "voice"), &HBSongNative::set_voice);
    ClassDB::bind_method(D_METHOD("get_voice"), &HBSongNative::get_voice);
    ClassDB::bind_method(D_METHOD("set_creator", "creator"), &HBSongNative::set_creator);
    ClassDB::bind_method(D_METHOD("get_creator"), &HBSongNative::get_creator);
    ClassDB::bind_method(D_METHOD("set_original_title", "original_title"), &HBSongNative::set_original_title);
    ClassDB::bind_method(D_METHOD("get_original_title"), &HBSongNative::get_original_title);
    ClassDB::bind_method(D_METHOD("set_bpm", "bpm"), &HBSongNative::set_bpm);
    ClassDB::bind_method(D_METHOD("get_bpm"), &HBSongNative::get_bpm);
    ClassDB::bind_method(D_METHOD("set_bpm_string", "bpm_string"), &HBSongNative::set_bpm_string);
    ClassDB::bind_method(D_METHOD("get_bpm_string"), &HBSongNative::get_bpm_string);
    ClassDB::bind_method(D_METHOD("set_preview_start", "preview_start"), &HBSongNative::set_preview_start);
    ClassDB::bind_method(D_METHOD("get_preview_start"), &HBSongNative::get_preview_start);
    ClassDB::bind_method(D_METHOD("set_preview_end", "preview_end"), &HBSongNative::set_preview_end);
    ClassDB::bind_method(D_METHOD("get_preview_end"), &HBSongNative::get_preview_end);
    ClassDB::bind_method(D_METHOD("set_charts", "charts"), &HBSongNative::set_charts);
    ClassDB::bind_method(D_METHOD("get_charts"), &HBSongNative::get_charts);
    ClassDB::bind_method(D_METHOD("set_preview_image", "preview_image"), &HBSongNative::set_preview_image);
    ClassDB::bind_method(D_METHOD("get_preview_image"), &HBSongNative::get_preview_image);
    ClassDB::bind_method(D_METHOD("set_background_image", "background_image"), &HBSongNative::set_background_image);
    ClassDB::bind_method(D_METHOD("get_background_image"), &HBSongNative::get_background_image);
    ClassDB::bind_method(D_METHOD("set_circle_image", "circle_image"), &HBSongNative::set_circle_image);
    ClassDB::bind_method(D_METHOD("get_circle_image"), &HBSongNative::get_circle_image);
    ClassDB::bind_method(D_METHOD("set_circle_logo", "circle_logo"), &HBSongNative::set_circle_logo);
    ClassDB::bind_method(D_METHOD("get_circle_logo"), &HBSongNative::get_circle_logo);
    ClassDB::bind_method(D_METHOD("set_youtube_url", "youtube_url"), &HBSongNative::set_youtube_url);
    ClassDB::bind_method(D_METHOD("get_youtube_url"), &HBSongNative::get_youtube_url);
    ClassDB::bind_method(D_METHOD("set_youtube_preview_url", "youtube_preview_url"), &HBSongNative::set_youtube_preview_url);
    ClassDB::bind_method(D_METHOD("get_youtube_preview_url"), &HBSongNative::get_youtube_preview_url);
    ClassDB::bind_method(D_METHOD("set_use_youtube_for_audio", "use_youtube_for_audio"), &HBSongNative::set_use_youtube_for_audio);
    ClassDB::bind_method(D_METHOD("get_use_youtube_for_audio"), &HBSongNative::get_use_youtube_for_audio);
    ClassDB::bind_method(D_METHOD("set_use_youtube_for_video", "use_youtube_for_video"), &HBSongNative::set_use_youtube_for_video);
    ClassDB::bind_method(D_METHOD("get_use_youtube_for_video"), &HBSongNative::get_use_youtube_for_video);
    ClassDB::bind_method(D_METHOD("set_ugc_service_name", "ugc_service_name"), &HBSongNative::set_ugc_service_name);
    ClassDB::bind_method(D_METHOD("get_ugc_service_name"), &HBSongNative::get_ugc_service_name);
    ClassDB::bind_method(D_METHOD("set_ugc_id", "ugc_id"), &HBSongNative::set_ugc_id);
    ClassDB::bind_method(D_METHOD("get_ugc_id"), &HBSongNative::get_ugc_id);
    ClassDB::bind_method(D_METHOD("set_allows_intro_skip", "allows_intro_skip"), &HBSongNative::set_allows_intro_skip);
    ClassDB::bind_method(D_METHOD("get_allows_intro_skip"), &HBSongNative::get_allows_intro_skip);
    ClassDB::bind_method(D_METHOD("set_intro_skip_min_time", "intro_skip_min_time"), &HBSongNative::set_intro_skip_min_time);
    ClassDB::bind_method(D_METHOD("get_intro_skip_min_time"), &HBSongNative::get_intro_skip_min_time);
    ClassDB::bind_method(D_METHOD("set_start_time", "start_time"), &HBSongNative::set_start_time);
    ClassDB::bind_method(D_METHOD("get_start_time"), &HBSongNative::get_start_time);
    ClassDB::bind_method(D_METHOD("set_end_time", "end_time"), &HBSongNative::set_end_time);
    ClassDB::bind_method(D_METHOD("get_end_time"), &HBSongNative::get_end_time);
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &HBSongNative::set_volume);
    ClassDB::bind_method(D_METHOD("get_volume"), &HBSongNative::get_volume);
    ClassDB::bind_method(D_METHOD("set_hide_artist_name", "hide_artist_name"), &HBSongNative::set_hide_artist_name);
    ClassDB::bind_method(D_METHOD("get_hide_artist_name"), &HBSongNative::get_hide_artist_name);
    ClassDB::bind_method(D_METHOD("set_lyrics", "lyrics"), &HBSongNative::set_lyrics);
    ClassDB::bind_method(D_METHOD("get_lyrics"), &HBSongNative::get_lyrics);
    ClassDB::bind_method(D_METHOD("set_show_epilepsy_warning", "show_epilepsy_warning"), &HBSongNative::set_show_epilepsy_warning);
    ClassDB::bind_method(D_METHOD("get_show_epilepsy_warning"), &HBSongNative::get_show_epilepsy_warning);
    ClassDB::bind_method(D_METHOD("set_has_audio_loudness", "has_audio_loudness"), &HBSongNative::set_has_audio_loudness);
    ClassDB::bind_method(D_METHOD("get_has_audio_loudness"), &HBSongNative::get_has_audio_loudness);
    ClassDB::bind_method(D_METHOD("set_audio_loudness", "audio_loudness"), &HBSongNative::set_audio_loudness);
    ClassDB::bind_method(D_METHOD("get_audio_loudness"), &HBSongNative::get_audio_loudness);
    ClassDB::bind_method(D_METHOD("set_song_variants", "song_variants"), &HBSongNative::set_song_variants);
    ClassDB::bind_method(D_METHOD("get_song_variants"), &HBSongNative::get_song_variants);
    ClassDB::bind_method(D_METHOD("set_sections", "sections"), &HBSongNative::set_sections);
    ClassDB::bind_method(D_METHOD("get_sections"), &HBSongNative::get_sections);
    ClassDB::bind_method(D_METHOD("set_skin_ugc_id", "skin_ugc_id"), &HBSongNative::set_skin_ugc_id);
    ClassDB::bind_method(D_METHOD("get_skin_ugc_id"), &HBSongNative::get_skin_ugc_id);
    ClassDB::bind_method(D_METHOD("set_timing_changes", "timing_changes"), &HBSongNative::set_timing_changes);
    ClassDB::bind_method(D_METHOD("get_timing_changes"), &HBSongNative::get_timing_changes);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "title"), "set_title", "get_title");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "romanized_title"), "set_romanized_title", "get_romanized_title");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "artist"), "set_artist", "get_artist");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "artist_alias"), "set_artist_alias", "get_artist_alias");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "composers"), "set_composers", "get_composers");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "vocals"), "set_vocals", "get_vocals");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "writers"), "set_writers", "get_writers");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "audio"), "set_audio", "get_audio");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "creator"), "set_creator", "get_creator");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "original_title"), "set_original_title", "get_original_title");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bpm"), "set_bpm", "get_bpm");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "bpm_string"), "set_bpm_string", "get_bpm_string");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "preview_start"), "set_preview_start", "get_preview_start");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "preview_end"), "set_preview_end", "get_preview_end");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "charts"), "set_charts", "get_charts");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "preview_image"), "set_preview_image", "get_preview_image");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "background_image"), "set_background_image", "get_background_image");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "voice"), "set_voice", "get_voice");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "circle_image"), "set_circle_image", "get_circle_image");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "circle_logo"), "set_circle_logo", "get_circle_logo");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "youtube_url"), "set_youtube_url", "get_youtube_url");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "youtube_preview_url"), "set_youtube_preview_url", "get_youtube_preview_url");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_youtube_for_video"), "set_use_youtube_for_video", "get_use_youtube_for_video");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_youtube_for_audio"), "set_use_youtube_for_audio", "get_use_youtube_for_audio");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "video"), "set_video", "get_video");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "ugc_service_name"), "set_ugc_service_name", "get_ugc_service_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ugc_id"), "set_ugc_id", "get_ugc_id");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "allows_intro_skip"), "set_allows_intro_skip", "get_allows_intro_skip");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "intro_skip_min_time"), "set_intro_skip_min_time", "get_intro_skip_min_time");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "start_time"), "set_start_time", "get_start_time");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "end_time"), "set_end_time", "get_end_time");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hide_artist_name"), "set_hide_artist_name", "get_hide_artist_name");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "lyrics"), "set_lyrics", "get_lyrics");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_epilepsy_warning"), "set_show_epilepsy_warning", "get_show_epilepsy_warning");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_audio_loudness"), "set_has_audio_loudness", "get_has_audio_loudness");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "audio_loudness"), "set_audio_loudness", "get_audio_loudness");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "song_variants"), "set_song_variants", "get_song_variants");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "sections"), "set_sections", "get_sections");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "skin_ugc_id"), "set_skin_ugc_id", "get_skin_ugc_id");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "timing_changes"), "set_timing_changes", "get_timing_changes");
}

HBSongNative::HBSongNative() {
    set_bpm(150.0);
    set_bpm_string("150");
    preview_start = 0;
    preview_end = -1;
    use_youtube_for_audio = true;
    use_youtube_for_video = true;
    ugc_id = 0;
    allows_intro_skip = false;
    intro_skip_min_time = 30.0;
    start_time = 0;
    end_time = -1;
    volume = 1.0;
    hide_artist_name = false;
    show_epilepsy_warning = false;
    has_audio_loudness = false;
    audio_loudness = 0.0;
    skin_ugc_id = 0;

    serializable_fields.append("title");
    serializable_fields.append("romanized_title");
    serializable_fields.append("artist");
    serializable_fields.append("artist_alias");
    serializable_fields.append("composers");
    serializable_fields.append("vocals");
    serializable_fields.append("writers");
    serializable_fields.append("audio");
    serializable_fields.append("creator");
    serializable_fields.append("original_title");
    serializable_fields.append("bpm");
    serializable_fields.append("bpm_string");
    serializable_fields.append("preview_start");
    serializable_fields.append("preview_end");
    serializable_fields.append("charts");
    serializable_fields.append("preview_image");
    serializable_fields.append("background_image");
    serializable_fields.append("voice");
    serializable_fields.append("circle_image");
    serializable_fields.append("circle_logo");
    serializable_fields.append("youtube_url");
    serializable_fields.append("youtube_preview_url");
    serializable_fields.append("use_youtube_for_video");
    serializable_fields.append("use_youtube_for_audio");
    serializable_fields.append("video");
    serializable_fields.append("ugc_service_name");
    serializable_fields.append("ugc_id");
    serializable_fields.append("allows_intro_skip");
    serializable_fields.append("intro_skip_min_time");
    serializable_fields.append("start_time");
    serializable_fields.append("end_time");
    serializable_fields.append("volume");
    serializable_fields.append("hide_artist_name");
    serializable_fields.append("lyrics");
    serializable_fields.append("show_epilepsy_warning");
    serializable_fields.append("has_audio_loudness");
    serializable_fields.append("audio_loudness");
    serializable_fields.append("song_variants");
    serializable_fields.append("sections");
    serializable_fields.append("skin_ugc_id");
    serializable_fields.append("timing_changes");
}

HBSongNative::~HBSongNative() {}

void HBSongNative::update_bpm_string() {
    double min_bpm = 1000000.0;
    double max_bpm = -1000000.0;
    for (int i = 0; i < timing_changes.size(); i++) {
        Variant tc = timing_changes[i];
        double tc_bpm = tc.get("bpm");
        min_bpm = std::min(min_bpm, tc_bpm);
        max_bpm = std::max(max_bpm, tc_bpm);
    }

    if (timing_changes.is_empty()) {
        bpm_string = String::num(bpm);
    } else {
        if (min_bpm != max_bpm) {
            bpm_string = String::num(min_bpm) + "-" + String::num(max_bpm);
        } else {
            bpm_string = String::num(min_bpm);
        }
    }
}

String HBSongNative::get_serialized_type() const {
    return "Song";
}

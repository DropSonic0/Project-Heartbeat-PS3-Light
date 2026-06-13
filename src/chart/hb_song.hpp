#ifndef HB_SONG_HPP
#define HB_SONG_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBSongNative : public HBSerializableNative {
    GDCLASS(HBSongNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    String title;
    String romanized_title;
    String artist;
    String artist_alias;
    Array composers;
    Array writers;
    Array vocals;
    String audio;
    String video;
    String voice;
    String creator;
    String original_title;
    double bpm;
    String bpm_string;
    int64_t preview_start;
    int64_t preview_end;
    Dictionary charts;
    String preview_image;
    String background_image;
    String circle_image;
    String circle_logo;
    String youtube_url;
    String youtube_preview_url;
    bool use_youtube_for_audio;
    bool use_youtube_for_video;
    String ugc_service_name;
    int64_t ugc_id;
    bool allows_intro_skip;
    double intro_skip_min_time;
    int64_t start_time;
    int64_t end_time;
    double volume;
    bool hide_artist_name;
    Array lyrics;
    bool show_epilepsy_warning;
    bool has_audio_loudness;
    double audio_loudness;
    Array song_variants;
    Array sections;
    int64_t skin_ugc_id;
    Array timing_changes;

    // Non-serialized
    String path;
    String id;

public:
    HBSongNative();
    ~HBSongNative();

    void set_title(String p_title) { title = p_title; set("title", p_title); }
    String get_title() const { return get("title"); }
    void set_romanized_title(String p_val) { romanized_title = p_val; }
    String get_romanized_title() const { return romanized_title; }
    void set_artist(String p_val) { artist = p_val; }
    String get_artist() const { return artist; }
    void set_artist_alias(String p_val) { artist_alias = p_val; }
    String get_artist_alias() const { return artist_alias; }
    void set_composers(Array p_val) { composers = p_val; }
    Array get_composers() const { return composers; }
    void set_writers(Array p_val) { writers = p_val; }
    Array get_writers() const { return writers; }
    void set_vocals(Array p_val) { vocals = p_val; }
    Array get_vocals() const { return vocals; }
    void set_audio(String p_val) { audio = p_val; }
    String get_audio() const { return audio; }
    void set_video(String p_val) { video = p_val; }
    String get_video() const { return video; }
    void set_voice(String p_val) { voice = p_val; }
    String get_voice() const { return voice; }
    void set_creator(String p_val) { creator = p_val; }
    String get_creator() const { return creator; }
    void set_original_title(String p_val) { original_title = p_val; }
    String get_original_title() const { return original_title; }
    void set_bpm(double p_val) { bpm = p_val; set("bpm", p_val); }
    double get_bpm() const { return get("bpm"); }
    void set_bpm_string(String p_val) { bpm_string = p_val; set("bpm_string", p_val); }
    String get_bpm_string() const { return bpm_string; }
    void set_preview_start(int64_t p_val) { preview_start = p_val; }
    int64_t get_preview_start() const { return preview_start; }
    void set_preview_end(int64_t p_val) { preview_end = p_val; }
    int64_t get_preview_end() const { return preview_end; }
    void set_charts(Dictionary p_val) { charts = p_val; }
    Dictionary get_charts() const { return charts; }
    void set_preview_image(String p_val) { preview_image = p_val; }
    String get_preview_image() const { return preview_image; }
    void set_background_image(String p_val) { background_image = p_val; }
    String get_background_image() const { return background_image; }
    void set_circle_image(String p_val) { circle_image = p_val; }
    String get_circle_image() const { return circle_image; }
    void set_circle_logo(String p_val) { circle_logo = p_val; }
    String get_circle_logo() const { return circle_logo; }
    void set_youtube_url(String p_val) { youtube_url = p_val; }
    String get_youtube_url() const { return youtube_url; }
    void set_youtube_preview_url(String p_val) { youtube_preview_url = p_val; }
    String get_youtube_preview_url() const { return youtube_preview_url; }
    void set_use_youtube_for_audio(bool p_val) { use_youtube_for_audio = p_val; }
    bool get_use_youtube_for_audio() const { return use_youtube_for_audio; }
    void set_use_youtube_for_video(bool p_val) { use_youtube_for_video = p_val; }
    bool get_use_youtube_for_video() const { return use_youtube_for_video; }
    void set_ugc_service_name(String p_val) { ugc_service_name = p_val; }
    String get_ugc_service_name() const { return ugc_service_name; }
    void set_ugc_id(int64_t p_val) { ugc_id = p_val; }
    int64_t get_ugc_id() const { return ugc_id; }
    void set_allows_intro_skip(bool p_val) { allows_intro_skip = p_val; }
    bool get_allows_intro_skip() const { return allows_intro_skip; }
    void set_intro_skip_min_time(double p_val) { intro_skip_min_time = p_val; }
    double get_intro_skip_min_time() const { return intro_skip_min_time; }
    void set_start_time(int64_t p_val) { start_time = p_val; }
    int64_t get_start_time() const { return start_time; }
    void set_end_time(int64_t p_val) { end_time = p_val; }
    int64_t get_end_time() const { return end_time; }
    void set_volume(double p_val) { volume = p_val; }
    double get_volume() const { return volume; }
    void set_hide_artist_name(bool p_val) { hide_artist_name = p_val; }
    bool get_hide_artist_name() const { return hide_artist_name; }
    void set_lyrics(Array p_val) { lyrics = p_val; }
    Array get_lyrics() const { return lyrics; }
    void set_show_epilepsy_warning(bool p_val) { show_epilepsy_warning = p_val; }
    bool get_show_epilepsy_warning() const { return show_epilepsy_warning; }
    void set_has_audio_loudness(bool p_val) { has_audio_loudness = p_val; }
    bool get_has_audio_loudness() const { return has_audio_loudness; }
    void set_audio_loudness(double p_val) { audio_loudness = p_val; }
    double get_audio_loudness() const { return audio_loudness; }
    void set_song_variants(Array p_val) { song_variants = p_val; }
    Array get_song_variants() const { return song_variants; }
    void set_sections(Array p_val) { sections = p_val; }
    Array get_sections() const { return sections; }
    void set_skin_ugc_id(int64_t p_val) { skin_ugc_id = p_val; }
    int64_t get_skin_ugc_id() const { return skin_ugc_id; }
    void set_timing_changes(Array p_val) { timing_changes = p_val; }
    Array get_timing_changes() const { return timing_changes; }

    void update_bpm_string();

    virtual String get_serialized_type() const;
};

} // namespace godot

#endif // HB_SONG_HPP

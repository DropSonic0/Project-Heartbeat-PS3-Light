#ifndef HB_SONG_VARIANT_DATA_HPP
#define HB_SONG_VARIANT_DATA_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBSongVariantDataNative : public HBSerializableNative {
    GDCLASS(HBSongVariantDataNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    String variant_name;
    String variant_url;
    String variant_audio;
    String variant_video;
    int64_t variant_offset = 0;
    bool has_audio_normalization = false;
    double variant_normalization = 0.0;
    bool audio_only = false;

public:
    HBSongVariantDataNative();
    ~HBSongVariantDataNative();

    void set_variant_name(String p_val) { variant_name = p_val; }
    String get_variant_name() const { return variant_name; }
    void set_variant_url(String p_val) { variant_url = p_val; }
    String get_variant_url() const { return variant_url; }
    void set_variant_audio(String p_val) { variant_audio = p_val; }
    String get_variant_audio() const { return variant_audio; }
    void set_variant_video(String p_val) { variant_video = p_val; }
    String get_variant_video() const { return variant_video; }
    void set_variant_offset(int64_t p_val) { variant_offset = p_val; }
    int64_t get_variant_offset() const { return variant_offset; }
    void set_variant_normalization(double p_val) { variant_normalization = p_val; }
    double get_variant_normalization() const { return variant_normalization; }
    void set_audio_only(bool p_val) { audio_only = p_val; }
    bool get_audio_only() const { return audio_only; }

    virtual String get_serialized_type() const override { return "HBSongVariantData"; }
};

} // namespace godot

#endif // HB_SONG_VARIANT_DATA_HPP

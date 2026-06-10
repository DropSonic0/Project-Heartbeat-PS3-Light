#ifndef HB_PER_SONG_SETTINGS_HPP
#define HB_PER_SONG_SETTINGS_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBPerSongSettingsNative : public HBSerializableNative {
    GDCLASS(HBPerSongSettingsNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    int32_t lag_compensation = 0;
    double volume = 1.0;
    bool video_enabled = true;
    bool use_song_skin = true;
    bool vocals_enabled = true;

public:
    HBPerSongSettingsNative();
    ~HBPerSongSettingsNative();

    void set_lag_compensation(int32_t p_val) { lag_compensation = p_val; }
    int32_t get_lag_compensation() const { return lag_compensation; }
    void set_volume(double p_val) { volume = p_val; }
    double get_volume() const { return volume; }
    void set_video_enabled(bool p_val) { video_enabled = p_val; }
    bool get_video_enabled() const { return video_enabled; }

    virtual String get_serialized_type() const override { return "PerSongSettings"; }
};

} // namespace godot

#endif // HB_PER_SONG_SETTINGS_HPP

#ifndef HB_SHINOBU_PLAYBACK_OFFSET_HPP
#define HB_SHINOBU_PLAYBACK_OFFSET_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/node.hpp"
#else
#include <godot_cpp/classes/node.hpp>
#endif

namespace godot {

class ShinobuGodotSoundPlaybackOffsetNative : public Node {
    GDCLASS(ShinobuGodotSoundPlaybackOffsetNative, Node);

protected:
    static void _bind_methods();

public:
    ShinobuGodotSoundPlaybackOffsetNative();
    ~ShinobuGodotSoundPlaybackOffsetNative();

    void set_volume(float p_volume);
    float get_volume() const;

    void set_pitch_scale(float p_pitch_scale);
    float get_pitch_scale() const;

    void set_offset(float p_offset);
    float get_offset() const;

    void start();
    void stop();
    bool is_playing() const;
    void seek(int64_t p_msec);
    int64_t get_playback_position_msec() const;

private:
    float volume = 1.0f;
    float pitch_scale = 1.0f;
    float offset = 0.0f;
    bool playing = false;
    int64_t last_start_time = 0;
    int64_t position_at_stop = 0;
};

} // namespace godot

#endif // HB_SHINOBU_PLAYBACK_OFFSET_HPP

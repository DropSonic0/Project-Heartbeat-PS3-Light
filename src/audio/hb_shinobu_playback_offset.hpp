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

private:
    float volume = 1.0f;
    float pitch_scale = 1.0f;
    float offset = 0.0f;
};

} // namespace godot

#endif // HB_SHINOBU_PLAYBACK_OFFSET_HPP

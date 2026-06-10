#ifndef HB_SHINOBU_PLAYBACK_OFFSET_HPP
#define HB_SHINOBU_PLAYBACK_OFFSET_HPP

#include <godot_cpp/classes/node.hpp>

namespace godot {

class ShinobuGodotSoundPlaybackOffsetNative : public Node {
    GDCLASS(ShinobuGodotSoundPlaybackOffsetNative, Node);

protected:
    static void _bind_methods();

public:
    ShinobuGodotSoundPlaybackOffsetNative();
    ~ShinobuGodotSoundPlaybackOffsetNative();
};

} // namespace godot

#endif // HB_SHINOBU_PLAYBACK_OFFSET_HPP

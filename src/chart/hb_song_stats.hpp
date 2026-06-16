#ifndef HB_SONG_STATS_HPP
#define HB_SONG_STATS_HPP

#include "hb_serializable.hpp"

namespace godot {

class HBSongStatsNative : public HBSerializableNative {
    GDCLASS(HBSongStatsNative, HBSerializableNative);

protected:
    static void _bind_methods();

private:
    int32_t times_played = 0;
    int32_t selected_variant = -1;

public:
    HBSongStatsNative();
    ~HBSongStatsNative();

    void set_times_played(int32_t p_val) { times_played = p_val; }
    int32_t get_times_played() const { return times_played; }
    void set_selected_variant(int32_t p_val) { selected_variant = p_val; }
    int32_t get_selected_variant() const { return selected_variant; }

    virtual String get_serialized_type() const GD_OVERRIDE { return "SongStats"; }
};

} // namespace godot

#endif // HB_SONG_STATS_HPP

#ifndef HB_AUTO_SONG_HPP
#define HB_AUTO_SONG_HPP

#include "hb_song.hpp"
#include "hb_chart.hpp"

namespace godot {

class HBAutoSongNative : public HBSongNative {
    GDCLASS(HBAutoSongNative, HBSongNative);

protected:
    static void _bind_methods();

private:
    Dictionary pattern;
    double loudness;
    int64_t offset;
    String audio_path;
    Ref<HBChartNative> chart;

public:
    HBAutoSongNative();
    ~HBAutoSongNative();

    void generate_chart();

    virtual String get_serialized_type() const;
};

} // namespace godot

#endif // HB_AUTO_SONG_HPP

#ifndef HB_PPD_SONG_HPP
#define HB_PPD_SONG_HPP

#include "hb_song.hpp"

namespace godot {

class HBPPDSongNative : public HBSongNative {
    GDCLASS(HBPPDSongNative, HBSongNative);

protected:
    static void _bind_methods();

private:
    int64_t ppd_offset = 0;
    String guid;
    String ppd_website_id;
    bool uses_native_video = false;

public:
    HBPPDSongNative();
    ~HBPPDSongNative();

    virtual String get_serialized_type() const GD_OVERRIDE { return "PPDSong"; }
};

} // namespace godot

#endif // HB_PPD_SONG_HPP

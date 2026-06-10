#ifndef HB_SONG_VERIFICATION_HPP
#define HB_SONG_VERIFICATION_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include "chart/hb_song.hpp"

namespace godot {

class HBSongVerificationNative : public Node {
    GDCLASS(HBSongVerificationNative, Node);

protected:
    static void _bind_methods();

public:
    enum ChartError {
        CHART_OK,
        CHART_FILE_NOT_FOUND,
        CHART_FILE_INVALID_JSON,
        CHART_HOLD_CHAIN_PIECE_WITHOUT_PARENT,
        CHART_AUDIO_NOT_DOWNLOADED
    };

    enum MetaError {
        META_YOUTUBE_URL_INVALID,
        META_MANDATORY_FIELD_MISSING,
        META_AUDIO_FIELD_MISSING,
        META_AUDIO_NOT_FOUND,
        META_VOICE_NOT_FOUND,
        META_PREVIEW_MISSING,
        META_PREVIEW_FILE_MISSING,
        META_PREVIEW_FILE_TOO_BIG,
        META_ILLEGAL_FILES,
        META_ILLEGAL_FOLDERS
    };

    HBSongVerificationNative();
    ~HBSongVerificationNative();

    Dictionary verify_song(Ref<HBSongNative> p_song);
    bool has_fatal_error(Dictionary p_errors, bool p_count_ugc = false);
};

} // namespace godot

VARIANT_ENUM_CAST(HBSongVerificationNative::ChartError);
VARIANT_ENUM_CAST(HBSongVerificationNative::MetaError);

#endif // HB_SONG_VERIFICATION_HPP

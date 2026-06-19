#ifndef HB_SONG_LIST_NATIVE_HPP
#define HB_SONG_LIST_NATIVE_HPP

#include "hb_song_loader.hpp"
#include "compat/godot_cpp/classes/node.hpp"
#include "compat/godot_cpp/classes/image.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include "compat/godot_cpp/variant/vector2.hpp"
#include <vector>

namespace godot {

class HBSongListNative : public Node {
    struct SongEntry {
        Ref<HBSongNative> song;
    };

    std::vector<SongEntry> songs;
    int selected_index = -1;
    int scroll_offset = 0;
    
    Ref<FontVariation> font;
    Ref<FontVariation> font_bold;
    Ref<Image> background_tex;
    Ref<Image> default_preview;
    Ref<Image> current_preview;
    int loaded_preview_idx = -1;

public:
    HBSongListNative();
    void update();
    void draw();
};

}

#endif

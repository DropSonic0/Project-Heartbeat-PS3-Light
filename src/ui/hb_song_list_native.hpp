#ifndef HB_SONG_LIST_NATIVE_HPP
#define HB_SONG_LIST_NATIVE_HPP

#include "hb_song_loader.hpp"
#include "compat/godot_cpp/classes/node.hpp"
#include "compat/godot_cpp/classes/image.hpp"
#include "compat/godot_cpp/classes/font_variation.hpp"
#include "compat/godot_cpp/variant/vector2.hpp"
#include "compat/godot_cpp/variant/transform3d.hpp"
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
    Ref<Image> default_avatar;
    Ref<Image> current_preview;
    int loaded_preview_idx = -1;

    Ref<Image> arcade_icon;
    Ref<Image> console_icon;
    Ref<Image> heart_icon;
    Ref<Image> folder_icon;
    Ref<Image> star_icon;
    Ref<Image> filter_icon;
    
    Ref<Image> prompt_cross;
    Ref<Image> prompt_circle;
    Ref<Image> prompt_square;
    Ref<Image> prompt_triangle;

    Transform3D tilt_list;
    Transform3D tilt_info;

    bool selecting_difficulty = false;
    int selected_difficulty_idx = 0;
    Array available_difficulties;

    Color get_difficulty_color(String p_difficulty);
    Array get_sorted_difficulties(Ref<HBSongNative> p_song);

public:
    HBSongListNative();
    virtual ~HBSongListNative() {}
    void update();
    void draw();
};

}

#endif

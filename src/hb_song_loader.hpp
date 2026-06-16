#ifndef HB_SONG_LOADER_HPP
#define HB_SONG_LOADER_HPP

#ifdef __PPU__
#include "compat/godot_cpp/classes/node.hpp"
#else
#include <godot_cpp/classes/node.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/dictionary.hpp"
#else
#include <godot_cpp/variant/dictionary.hpp>
#endif
#ifdef __PPU__
#include "compat/godot_cpp/variant/array.hpp"
#else
#include <godot_cpp/variant/array.hpp>
#endif
#include "chart/hb_song.hpp"

namespace godot {

class HBSongLoaderNative : public Node {
    GDCLASS(HBSongLoaderNative, Node);

protected:
    static void _bind_methods();

private:
    Dictionary songs;
    bool initial_load_done = false;
    Array base_difficulty_order;

public:
    HBSongLoaderNative();
    ~HBSongLoaderNative();

    void add_song(Ref<HBSongNative> p_song);
    Ref<HBSongNative> load_song_meta(const String &p_path, const String &p_id);
    void scan_songs_recursive(const String& p_path);

    Dictionary get_songs() const { return songs; }
    bool is_initial_load_done() const { return initial_load_done; }

    bool difficulty_sort(const String &p_a, const String &p_b);

    static HBSongLoaderNative *get_singleton();
};

} // namespace godot

#endif // HB_SONG_LOADER_HPP

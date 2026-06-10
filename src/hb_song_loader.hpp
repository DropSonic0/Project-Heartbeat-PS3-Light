#ifndef HB_SONG_LOADER_HPP
#define HB_SONG_LOADER_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
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

    Dictionary get_songs() const { return songs; }
    bool is_initial_load_done() const { return initial_load_done; }

    bool difficulty_sort(const String &p_a, const String &p_b);

    static HBSongLoaderNative *get_singleton();
};

} // namespace godot

#endif // HB_SONG_LOADER_HPP

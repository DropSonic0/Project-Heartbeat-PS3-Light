#include "hb_song_loader.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>

using namespace godot;

static HBSongLoaderNative *singleton = nullptr;

void HBSongLoaderNative::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_song", "song"), &HBSongLoaderNative::add_song);
    ClassDB::bind_method(D_METHOD("load_song_meta", "path", "id"), &HBSongLoaderNative::load_song_meta);
    ClassDB::bind_method(D_METHOD("get_songs"), &HBSongLoaderNative::get_songs);
    ClassDB::bind_method(D_METHOD("difficulty_sort", "a", "b"), &HBSongLoaderNative::difficulty_sort);
}

HBSongLoaderNative::HBSongLoaderNative() {
    singleton = this;
    base_difficulty_order.append("easy");
    base_difficulty_order.append("normal");
    base_difficulty_order.append("hard");
    base_difficulty_order.append("extreme");
    base_difficulty_order.append("extra extreme");
}

HBSongLoaderNative::~HBSongLoaderNative() {
    singleton = nullptr;
}

void HBSongLoaderNative::add_song(Ref<HBSongNative> p_song) {
    if (p_song.is_valid()) {
        songs[p_song->get_title()] = p_song; // This is a simplification, should use ID
    }
}

Ref<HBSongNative> HBSongLoaderNative::load_song_meta(const String &p_path, const String &p_id) {
    Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
    if (file.is_null()) {
        return nullptr;
    }

    String content = file->get_as_text();
    Ref<JSON> json;
    json.instantiate();
    if (json->parse(content) != OK) {
        return nullptr;
    }

    Variant data = json->get_data();
    if (data.get_type() != Variant::DICTIONARY) {
        return nullptr;
    }

    Ref<HBSongNative> song;
    song.instantiate();

    Dictionary d = data;
    Array keys = d.keys();
    for (int i = 0; i < keys.size(); i++) {
        String key = keys[i];
        song->set(key, d[key]);
    }

    return song;
}

bool HBSongLoaderNative::difficulty_sort(const String &p_a, const String &p_b) {
    int a_i = base_difficulty_order.find(p_a.to_lower());
    int b_i = base_difficulty_order.find(p_b.to_lower());
    return a_i > b_i;
}

HBSongLoaderNative *HBSongLoaderNative::get_singleton() {
    return singleton;
}

#include "hb_song_list_native.hpp"
#include "hb_game.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"

namespace godot {

HBSongListNative::HBSongListNative() {
    font = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Regular.ttf");
    font_bold = ResourceLoader::get_singleton()->load("res://fonts/orbitron/Orbitron-Black.ttf");
    background_tex = ResourceLoader::get_singleton()->load("res://graphics/predarkenedbg.png");
    default_preview = ResourceLoader::get_singleton()->load("res://icon.png");

    Dictionary loaded_songs = HBSongLoaderNative::get_singleton()->get_songs();
    Array keys = loaded_songs.keys();
    for (int i = 0; i < keys.size(); i++) {
        SongEntry entry;
        entry.song = loaded_songs[keys[i]];
        songs.push_back(entry);
    }
    if (!songs.empty()) {
        selected_index = 0;
    }
}

void HBSongListNative::update() {
    HBInputNative::update();

    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_DOWN)) {
        if (!songs.empty()) {
            selected_index = (selected_index + 1) % songs.size();
        }
    }
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_UP)) {
        if (!songs.empty()) {
            selected_index = (selected_index - 1 + songs.size()) % songs.size();
        }
    }
    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_BACK)) {
        HBGameNative::get_singleton()->change_to_menu("main_menu");
        return;
    }

    if (HBInputNative::is_action_just_pressed(HBInputNative::ACTION_ACCEPT)) {
        if (selected_index >= 0 && selected_index < (int)songs.size()) {
            UtilityFunctions::print("Starting song: " + songs[selected_index].song->get_title());
            // TODO: Actually start the song when the game logic is ready.
            // For now, we just show a print to verify the button works.
        }
    }
}

void HBSongListNative::draw() {
    Vector2 window_size = HBVideoDriverPSGL::get_window_size();
    float scale_x = window_size.x / 1920.0f;
    float scale_y = window_size.y / 1080.0f;

    // Draw background
    if (background_tex.is_valid()) {
        HBVideoDriverPSGL::draw_texture(background_tex, Rect2(0, 0, window_size.x, window_size.y));
    } else {
        HBVideoDriverPSGL::draw_rect(Rect2(0, 0, window_size.x, window_size.y), Color(0.129, 0.071, 0.259, 1.0));
    }

    HBVideoDriverPSGL::draw_text_with_font(font_bold, "Juego Libre", Vector2(100 * scale_x, 80 * scale_y), (int)(50 * scale_y), Color(1, 1, 1, 1));

    float item_height = 80.0f * scale_y;
    float start_y = 150.0f * scale_y;
    int visible_items = 10;

    if (selected_index >= scroll_offset + visible_items) {
        scroll_offset = selected_index - visible_items + 1;
    }
    if (selected_index < scroll_offset) {
        scroll_offset = selected_index;
    }

    for (int i = 0; i < visible_items && (scroll_offset + i) < (int)songs.size(); i++) {
        int idx = scroll_offset + i;
        float item_y = start_y + i * item_height;
        Color item_color = (idx == selected_index) ? Color(0.93f, 0.22f, 0.8f, 0.5f) : Color(0.19f, 0.07f, 0.3f, 0.5f);
        
        HBVideoDriverPSGL::draw_parallelogram(Rect2(100 * scale_x, item_y, 800 * scale_x, item_height - 5 * scale_y), -40.0f * scale_x, item_color);
        HBVideoDriverPSGL::draw_text_with_font(font, songs[idx].song->get_title(), Vector2(150 * scale_x, item_y + 15 * scale_y), (int)(30 * scale_y), Color(1, 1, 1, 1));
    }
    
    // Draw "preview" for selected song
    if (selected_index >= 0 && selected_index < (int)songs.size()) {
        if (loaded_preview_idx != selected_index) {
            String preview_path = songs[selected_index].song->get_song_preview_res_path();
            if (!preview_path.is_empty()) {
                current_preview = ResourceLoader::get_singleton()->load(preview_path);
            } else {
                current_preview.unref();
            }
            loaded_preview_idx = selected_index;
        }
        
        Ref<Image> preview = current_preview;
        if (preview.is_null()) {
            preview = default_preview;
        }
        HBVideoDriverPSGL::draw_texture(preview, Rect2(1000 * scale_x, 200 * scale_y, 600 * scale_x, 600 * scale_y));
        HBVideoDriverPSGL::draw_text_with_font(font_bold, songs[selected_index].song->get_title(), Vector2(1000 * scale_x, 850 * scale_y), (int)(40 * scale_y), Color(1, 1, 1, 1));
        HBVideoDriverPSGL::draw_text_with_font(font, songs[selected_index].song->get_artist(), Vector2(1000 * scale_x, 900 * scale_y), (int)(30 * scale_y), Color(1, 1, 1, 0.7f));
    }
}

}

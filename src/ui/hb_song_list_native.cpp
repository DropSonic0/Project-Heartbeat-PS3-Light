#include "hb_song_list_native.hpp"
#include "hb_game.hpp"
#include "utils/hb_input_native.hpp"
#include "graphics/hb_video_driver_psgl.hpp"
#include "compat/godot_cpp/variant/utility_functions.hpp"
#include "compat/godot_cpp/classes/resource_loader.hpp"

namespace godot {

HBSongListNative::HBSongListNative() {
    // Using paths confirmed to be in the PS3 PCK
    font = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/default_skin/skin_resources/orbitron_medium.otf");
    font_bold = ResourceLoader::get_singleton()->load("res://graphics/resource_packs/default_skin/skin_resources/orbitron_black.otf");
    background_tex = ResourceLoader::get_singleton()->load("res://graphics/predarkenedbg.png");
    default_preview = ResourceLoader::get_singleton()->load("res://graphics/no_preview_texture.png");
    default_avatar = ResourceLoader::get_singleton()->load("res://graphics/default_avatar.png");

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

    // Initialize 3D tilts
    tilt_list = Transform3D::rotated(Vector3(0, 1, 0), 0.15f); // ~8.5 degrees
    tilt_list = tilt_list.translated(Vector3(0, 0, -0.45f));

    tilt_info = Transform3D::rotated(Vector3(0, 1, 0), -0.15f); // ~-8.5 degrees
    tilt_info = tilt_info.translated(Vector3(0, 0, -0.45f));
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
            HBGameNative::get_singleton()->change_to_menu("rhythm_game", songs[selected_index].song);
            return;
        }
    }
}

Color HBSongListNative::get_difficulty_color(String p_difficulty) {
    String diff = p_difficulty.to_lower();
    if (diff == "easy") return Color::from_hsv(190.0f / 360.0f, 0.7f, 0.75f);
    if (diff == "normal") return Color::from_hsv(111.0f / 360.0f, 0.7f, 0.75f);
    if (diff == "hard") return Color::from_hsv(46.0f / 360.0f, 0.7f, 0.75f);
    if (diff == "extreme") return Color::from_hsv(343.0f / 360.0f, 0.7f, 0.75f);
    return Color(0.5, 0.5, 0.5);
}

static String array_to_string(Array p_array) {
    String res = "";
    for (size_t i = 0; i < p_array.size(); i++) {
        if (i > 0) res += ", ";
        res += (String)p_array[i];
    }
    return res;
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

    HBVideoDriverPSGL::draw_text_with_font(font_bold, "Juego Libre", Vector2(80 * scale_x, 80 * scale_y), (int)(55 * scale_y), Color(1, 1, 1, 1), true);

    float item_height = 125.0f * scale_y;
    float start_y = 230.0f * scale_y;
    int visible_items = 6;

    if (selected_index >= scroll_offset + visible_items) {
        scroll_offset = selected_index - visible_items + 1;
    }
    if (selected_index < scroll_offset) {
        scroll_offset = selected_index;
    }

    for (int i = 0; i < visible_items && (scroll_offset + i) < (int)songs.size(); i++) {
        int idx = scroll_offset + i;
        float item_y = (start_y + i * item_height) / scale_y;
        
        bool is_selected = (idx == selected_index);
        float current_scale = is_selected ? 1.0f : 0.85f;
        
        Color item_color = is_selected ? Color(0.93f, 0.22f, 0.8f, 0.6f) : Color(0.19f, 0.07f, 0.3f, 0.75f);
        Color highlight_color = Color(0.93f, 0.22f, 0.8f, 1.0f);
        
        float base_h = 115.0f;
        float base_w = 1000.0f;
        float draw_h = base_h * current_scale;
        float offset_y = (125.0f - draw_h) / 2.0f;
        float offset_x = 50.0f;
        
        Rect2 item_rect(offset_x, item_y + offset_y, base_w, base_h);
        float slant = -40.0f * current_scale;

        // Selected Glow (3D)
        if (is_selected) {
            for (int j = 1; j <= 3; j++) {
                float g = j * 3.0f;
                Color gc = highlight_color; gc.a = 0.2f / j;
                HBVideoDriverPSGL::draw_rect_3d(Rect2(offset_x - g, item_y + offset_y - g, base_w + g * 2, base_h + g * 2), tilt_list, gc, slant);
            }
        }

        // Main item body (3D)
        HBVideoDriverPSGL::draw_rect_3d(item_rect, tilt_list, item_color, slant);
        
        // Album Art (3D)
        float art_size = base_h - 12.0f;
        HBVideoDriverPSGL::draw_texture_3d(default_preview, Rect2(offset_x + 10.0f, item_y + offset_y + 6.0f, art_size, art_size), tilt_list);

        // Text content
        float text_x = offset_x + art_size + 40.0f;
        
        // Heart Icon
        if (heart_icon.is_valid()) {
            HBVideoDriverPSGL::draw_texture_3d(heart_icon, Rect2(text_x - 35.0f, item_y + offset_y + 15.0f, 25.0f, 25.0f), tilt_list);
        }

        // Title
        HBVideoDriverPSGL::draw_text_with_font_3d(font_bold, songs[idx].song->get_title(), Vector2(text_x, item_y + offset_y + 10.0f), (int)(32 * current_scale), tilt_list, Color(1, 1, 1, 1), true);
        
        // Artist & Creator
        String artist_creator = songs[idx].song->get_artist();
        if (!songs[idx].song->get_creator().is_empty()) {
            artist_creator += "  " + songs[idx].song->get_creator();
        }
        HBVideoDriverPSGL::draw_text_with_font_3d(font, artist_creator, Vector2(text_x + 350.0f * current_scale, item_y + offset_y + 84.0f), (int)(22 * current_scale), tilt_list, Color(0.9, 0.9, 0.9, 1.0));

        // Draw Difficulty Tags (3D)
        Dictionary charts = songs[idx].song->get_charts();
        Array chart_keys = charts.keys();
        // Simple sorting for common difficulties
        Array sorted_keys;
        const char* diff_order[] = {"Easy", "Normal", "Hard", "Extreme"};
        for (int d = 0; d < 4; d++) {
            if (charts.has(diff_order[d])) {
                sorted_keys.push_back(diff_order[d]);
            }
        }

        float tag_x = text_x;
        float tag_y = item_y + offset_y + 65.0f * current_scale;
        float tag_h = 35.0f * current_scale;
        int tag_font_size = (int)(20 * current_scale);

        for (int k = 0; k < sorted_keys.size(); k++) {
            String diff_name = sorted_keys[k];
            Dictionary chart_data = charts[diff_name];
            float stars = 0.0f;
            if (chart_data.has("stars")) {
                stars = (float)chart_data["stars"];
            }

            Color diff_color = get_difficulty_color(diff_name);
            String star_text = String::num(stars);
            if (stars == (int)stars) {
                star_text = String::num_int64((long long)stars);
            }
            
            // Draw star/number part (3D)
            float star_w = 40.0f * current_scale;
            float tag_slant = -8.0f * current_scale;
            HBVideoDriverPSGL::draw_rect_3d(Rect2(tag_x, tag_y, star_w, tag_h), tilt_list, Color(0.1, 0.05, 0.15, 0.9), tag_slant);
            HBVideoDriverPSGL::draw_text_with_font_3d(font, star_text, Vector2(tag_x + 5.0f, tag_y + 6.0f), tag_font_size, tilt_list, Color(1, 1, 1, 1));
            
            // Draw difficulty name part (3D)
            float name_w = 100.0f * current_scale;
            HBVideoDriverPSGL::draw_rect_3d(Rect2(tag_x + star_w, tag_y, name_w, tag_h), tilt_list, diff_color, tag_slant);
            HBVideoDriverPSGL::draw_text_with_font_3d(font_bold, diff_name.to_upper(), Vector2(tag_x + star_w + 8.0f, tag_y + 6.0f), tag_font_size, tilt_list, Color(1, 1, 1, 1));
            
            tag_x += (star_w + name_w + 12.0f) * current_scale;
        }

        // Draw Note Usage Icons (3D)
        float icon_size = 35.0f * current_scale;
        float icon_x = text_x - 45.0f * current_scale;
        
        if (arcade_icon.is_valid()) {
            HBVideoDriverPSGL::draw_texture_3d(arcade_icon, Rect2(icon_x, item_y + offset_y + 10.0f, icon_size, icon_size), tilt_list);
            icon_x += 40.0f * current_scale;
        }
        if (console_icon.is_valid()) {
            HBVideoDriverPSGL::draw_texture_3d(console_icon, Rect2(icon_x, item_y + offset_y + 10.0f, icon_size, icon_size), tilt_list);
        }
    }
    
    // Scrollbar
    float scrollbar_h = visible_items * item_height;
    HBVideoDriverPSGL::draw_rect(Rect2(1060 * scale_x, start_y, 10 * scale_x, scrollbar_h), Color(1, 1, 1, 0.1));
    if (!songs.empty()) {
        float handle_h = scrollbar_h * (visible_items / (float)songs.size());
        if (handle_h > scrollbar_h) handle_h = scrollbar_h;
        float handle_y = start_y + (scroll_offset / (float)songs.size()) * scrollbar_h;
        HBVideoDriverPSGL::draw_rect(Rect2(1060 * scale_x, handle_y, 10 * scale_x, handle_h), Color(1, 1, 1, 0.8));
    }

    // Song Counter
    String counter_text = String::num_int64(selected_index + 1) + "/" + String::num_int64(songs.size());
    HBVideoDriverPSGL::draw_text_with_font(font, counter_text, Vector2(70 * scale_x, 970 * scale_y), (int)(32 * scale_y), Color(1, 1, 1, 1));


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
        
        float preview_x = 1350.0f;
        float preview_y = 350.0f;
        float preview_w = 480.0f;
        float preview_h = 480.0f;
        
        // Preview Background and Art (3D)
        HBVideoDriverPSGL::draw_rect_3d(Rect2(preview_x - 10.0f, preview_y - 10.0f, preview_w + 20.0f, preview_h + 20.0f), tilt_info, Color(0.1, 0.1, 0.1, 0.5), -5.0f);
        HBVideoDriverPSGL::draw_texture_3d(preview, Rect2(preview_x, preview_y, preview_w, preview_h), tilt_info, Color(1, 1, 1, 1), -5.0f);
        
        // Song Info (3D)
        float info_x = 1350.0f;
        float info_y = 150.0f;
        
        HBVideoDriverPSGL::draw_text_with_font_3d(font_bold, songs[selected_index].song->get_title(), Vector2(info_x + 240.0f, info_y), (int)(38.0f), tilt_info, Color(1, 1, 1, 1), true, true);
        HBVideoDriverPSGL::draw_text_with_font_3d(font, "por", Vector2(info_x + 240.0f, info_y + 45.0f), (int)(18.0f), tilt_info, Color(1, 1, 1, 0.8), false, true);
        HBVideoDriverPSGL::draw_text_with_font_3d(font_bold, songs[selected_index].song->get_artist(), Vector2(info_x + 240.0f, info_y + 70.0f), (int)(26.0f), tilt_info, Color(1, 1, 1, 1), false, true);
        
        // BPM and other details (3D)
        float detail_y = 750.0f;
        HBVideoDriverPSGL::draw_rect_3d(Rect2(info_x - 40.0f, detail_y, 550.0f, 220.0f), tilt_info, Color(0.1, 0.05, 0.2, 0.6), -5.0f);
        
        String bpm_text = String::num(songs[selected_index].song->get_bpm()) + " PPM"; // PPM in screenshot
        HBVideoDriverPSGL::draw_text_with_font_3d(font, bpm_text, Vector2(info_x + 240.0f, detail_y + 25.0f), (int)(22.0f), tilt_info, Color(1, 1, 1, 1), false, true);
        
        String writers_str = array_to_string(songs[selected_index].song->get_writers());
        if (writers_str.is_empty()) writers_str = "-";
        HBVideoDriverPSGL::draw_text_with_font_3d(font, "Escrito por: " + writers_str, Vector2(info_x + 240.0f, detail_y + 60.0f), (int)(18.0f), tilt_info, Color(1, 1, 1, 0.8), false, true);
        
        String vocals_str = array_to_string(songs[selected_index].song->get_vocals());
        if (vocals_str.is_empty()) vocals_str = "-";
        HBVideoDriverPSGL::draw_text_with_font_3d(font, "Vocales por: " + vocals_str, Vector2(info_x + 240.0f, detail_y + 90.0f), (int)(18.0f), tilt_info, Color(1, 1, 1, 0.8), false, true);

        String composers_str = array_to_string(songs[selected_index].song->get_composers());
        if (composers_str.is_empty()) composers_str = "-";
        HBVideoDriverPSGL::draw_text_with_font_3d(font, "Compuesto por: " + composers_str, Vector2(info_x + 240.0f, detail_y + 120.0f), (int)(18.0f), tilt_info, Color(1, 1, 1, 0.8), false, true);

        if (!songs[selected_index].song->get_creator().is_empty()) {
            HBVideoDriverPSGL::draw_text_with_font_3d(font, "Mapa por: " + songs[selected_index].song->get_creator(), Vector2(info_x + 240.0f, detail_y + 150.0f), (int)(18.0f), tilt_info, Color(1, 1, 1, 0.8), false, true);
        }

    }
}

}
